;;; -*- Mode:Lisp; Package:XLIB; Syntax:COMMON-LISP; Base:10; Lowercase:T -*-

;;; Tests image code by randomly reading, copying and then writing images to
;;; the exact same place on the screen.  If everything works, just the borders
;;; of the image windows appear.  If one of these image windows is garbled,
;;; then somewhere something is broken.  Entry point is the function
;;; IMAGE-TEST

(in-package 'xlib :use '(lisp))

(export '(image-test))

(defvar *image-test-host* "unix")

(defvar *image-test-get-image-result-type-choices*
  '(image-x image-x image-xy image-z))

(defvar *image-test-get-image-image-x-format-choices*
  '(:xy-pixmap :z-pixmap))

(defun image-test-choose (list)
  (nth (random (length list)) list))

(defun image-test-get-image (window)
  (let* ((x (random (floor (drawable-width window) 3)))
	 (y (random (floor (drawable-height window) 3)))
	 (hw (floor (- (drawable-width window) x) 3))
	 (hh (floor (- (drawable-height window) y) 3))
	 (width (+ hw hw (random hw)))
	 (height (+ hh hh (random hh)))
	 (result-type (image-test-choose
			*image-test-get-image-result-type-choices*))
	 (format
	   (ecase result-type
	     (image-x (image-test-choose
			*image-test-get-image-image-x-format-choices*))
	     (image-xy :xy-pixmap)
	     (image-z :z-pixmap)))
	 (image (get-image window :x x :y y :width width :height height
			   :format format :result-type result-type)))
    (setf (image-x-hot image) (- x))
    (setf (image-y-hot image) (- y))
    image))

(defun test-image-subimage-parameters (image)
  (let* ((x (random (floor (image-width image) 3)))
	 (y (random (floor (image-height image) 3)))
	 (hw (floor (- (image-width image) x) 3))
	 (hh (floor (- (image-height image) y) 3))
	 (width (+ hw hw (random hw)))
	 (height (+ hh hh (random hh))))
    (values x y width height)))

(defun image-test-copy-image (image)
  (let ((result-type
	  (if (zerop (random 2))
	      (type-of image)
	    (etypecase image
	      (image-x (ecase (image-x-format image)
			 (:xy-pixmap 'image-xy)
			 (:z-pixmap 'image-z)))
	      ((or image-xy image-z) 'image-x)))))
    (multiple-value-bind (x y width height)
	(test-image-subimage-parameters image)
      (copy-image image :x x :y y :width width :height height
		  :result-type result-type))))

(defun image-test-put-image (screen gcontext image info)
  (multiple-value-bind (src-x src-y width height)
      (test-image-subimage-parameters image)
    (let* ((border-width 1)
	   (x (- src-x (image-x-hot image) border-width))
	   (y (- src-y (image-y-hot image) border-width)))
      (unless (or (zerop width) (zerop height))
	(let ((window
		(create-window
		  :parent (screen-root screen) :x x :y y
		  :width width :height height
		  :border-width border-width
		  :background (screen-white-pixel screen)
		  :override-redirect :on)))
	  (map-window window)
	  (display-finish-output (drawable-display window))
	  (put-image window gcontext image
		     :x 0 :y 0 :src-x src-x :src-y src-y
		     :width width :height height)
	  (draw-image-glyphs window gcontext 0 (1- height) info)
	  (display-finish-output (drawable-display window))
	  window)))))

(defun image-info (image)
  (etypecase image
    (image-x (ecase (image-x-format image)
	       (:xy-pixmap "XXY")
	       (:z-pixmap  "XZ ")))
    (image-xy "XY ")
    (image-z  "Z  ")))

(defun image-test (&optional (host *image-test-host*))
  (let* ((display (open-display host))
	 (abort t))
    (unwind-protect
	(let* ((screen (display-default-screen display))
	       (window (screen-root screen))
	       (gcontext (create-gcontext
			   :drawable window
			   :font (open-font display "fixed"))))
	  (dotimes (i 30)
	    (let* ((image (image-test-get-image window))
		   (copy (image-test-copy-image image))
		   (info (concatenate 'string (image-info image)
				      (image-info copy))))
	      (image-test-put-image screen gcontext copy info)
	      ))
	  (read-char)
	  (setq abort nil))
      (close-display display :abort abort))))

