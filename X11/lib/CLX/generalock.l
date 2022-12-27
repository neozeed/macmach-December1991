;;; -*- Mode: LISP; Syntax: Common-lisp; Package: PROCESS; Base: 10; Lowercase: Yes -*-

(defflavor xlib::clx-lock () (simple-recursive-normal-lock)
  (:init-keywords :flavor))

(defwhopper (lock-internal xlib::clx-lock) (lock-argument)
  (catch 'timeout
    (continue-whopper lock-argument)))

(defmethod (lock-block-internal xlib::clx-lock) (lock-argument)
  (declare (dbg:locking-function describe-process-lock-for-debugger self))
  (when (null waiter-queue)
    (setf waiter-queue (make-scheduler-queue :name name))
    (setf timer (create-timer-call #'lock-timer-expired `(,self) :name name)))
  (let ((process (lock-argument-process lock-argument)))
    (unwind-protect
	(progn
	  (lock-map-over-conflicting-owners
	    self lock-argument
	    #'(lambda (other-lock-arg)
		(add-promotion process lock-argument
			       (lock-argument-process other-lock-arg) other-lock-arg)))
	  (unless (timer-pending-p timer)
	    (when (and (safe-to-use-timers %real-current-process)
		       (not dbg:*debugger-might-have-system-problems*))
	      (reset-timer-relative-timer-units timer *lock-timer-interval*)))
	  (assert (store-conditional (locf latch) process nil))
	  (sys:with-aborts-enabled (lock-latch)
	    (let ((timeout (lock-argument-getf lock-argument :timeout nil)))
	      (cond ((null timeout)
		     (promotion-block waiter-queue name #'lock-lockable self lock-argument))
		    ((and (plusp timeout)
			  (using-resource (timer process-block-timers)
			    ;; Yeah, we know about the internal representation
			    ;; of timers here.
			    (setf (car (timer-args timer)) %real-current-process)
			    (with-scheduler-locked
			      (reset-timer-relative timer timeout)
			      (promotion-block
				waiter-queue name
				#'(lambda (timer lock lock-argument)
				    (or (not (timer-pending-p timer))
					(lock-lockable lock lock-argument)))
				timer self lock-argument)
			      (lock-lockable self lock-argument)))))
		    (t (throw 'timeout nil))))))
      (unless (store-conditional (locf latch) nil process)
	(lock-latch-wait-internal self))
      (remove-promotions process lock-argument))))

(compile-flavor-methods xlib::clx-lock)
