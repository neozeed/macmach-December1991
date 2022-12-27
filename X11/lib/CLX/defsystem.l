;;; -*- Mode: LISP; Syntax: Common-lisp; Base: 10; Lowercase: Yes;  -*-

;;;
;;;			 TEXAS INSTRUMENTS INCORPORATED
;;;				  P.O. BOX 2909
;;;			       AUSTIN, TEXAS 78769
;;;
;;; Copyright (C) 1987 Texas Instruments Incorporated.
;;;
;;; Permission is granted to any individual or institution to use, copy, modify,
;;; and distribute this software, provided that this complete copyright and
;;; permission notice is maintained, intact, in all copies and supporting
;;; documentation.
;;;
;;; Texas Instruments Incorporated provides this software "as is" without
;;; express or implied warranty.
;;;

(lisp:in-package 'xlib :use '(lisp))

#-lispm
(export '(
	  compile-clx
	  load-clx))

;;; #+ features used in this file
;;;   lispm
;;;   genera
;;;   CLX-Genera-New-Scheduler
;;;   lucid
;;;   lcl3.0
;;;   apollo
;;;   kcl
;;;   excl

#+excl (error "Use excldefsys")


;;;; Lisp Machines

;;; Lisp machines have their own defsystems, so we use them to define
;;; the CLX load.

#+(and lispm (not genera))
(global:defsystem CLX
  (:pathname-default "clx:clx;")
  (:patchable "clx:patch;" clx-ti)
  (:initial-status :experimental)

  (:module depdefs "depdefs")
  (:module clx "clx")
  (:module dependent "dependent")
  (:module macros "macros")
  (:module bufmac "bufmac")
  (:module buffer "buffer")
  (:module display "display")
  (:module gcontext "gcontext")
  (:module requests "requests")
  (:module input "input")
  (:module fonts "fonts")
  (:module graphics "graphics")
  (:module text "text")
  (:module attributes "attributes")
  (:module translate "translate")
  (:module keysyms "keysyms")
  (:module manager "manager")
  (:module image "image")
  (:module resource "resource")
  (:module doc "doc")

  (:compile-load depdefs)
  (:compile-load clx
   (:fasload depdefs))
  (:compile-load dependent
   (:fasload depdefs clx))
  ;; Macros only needed for compilation
  (:skip :compile-load macros
   (:fasload depdefs clx dependent))
  ;; Bufmac only needed for compilation
  (:skip :compile-load bufmac
   (:fasload depdefs clx dependent macros))
  (:compile-load buffer
   (:fasload depdefs clx dependent macros bufmac))
  (:compile-load display
   (:fasload depdefs clx dependent macros bufmac buffer))
  (:compile-load gcontext
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:compile-load input
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:compile-load requests
   (:fasload depdefs clx dependent macros bufmac buffer display input))
  (:compile-load fonts
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:compile-load graphics
   (:fasload depdefs clx dependent macros fonts bufmac buffer display fonts))
  (:compile-load text
   (:fasload depdefs clx dependent macros fonts bufmac buffer display gcontext fonts))
  (:compile-load-init attributes
   (dependent)					;<- There may be other modules needed here.
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:compile-load translate
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:compile-load keysyms
   (:fasload depdefs clx dependent macros bufmac buffer display translate))
  (:compile-load manager
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:compile-load image
   (:fasload depdefs clx dependent macros bufmac buffer display))
  (:compile-load resource)
  (:auxiliary doc)
  )


;;; Symbolics Genera has two different schedulers, one in 7.2 and the other in
;;; 7.3 and 7.4.  Define a feature so we can use different code for them.
#+Genera
(eval-when (compile load eval)
  (multiple-value-bind (ignore status)
      (find-symbol "WITH-LOCK" (find-package 'process))
    (when (eq status :external)
      (pushnew :clx-genera-new-scheduler *features*))))

#+Genera
(scl:defsystem CLX
    (:default-pathname "SYS:X11;CLX;"
     :default-package "XLIB"
     :pretty-name "CLX"
     :maintaining-sites (:scrc)
     :distribute-sources t
     :distribute-binaries t
     :source-category :basic)
  (:module doc ("doc")
	   (:type :lisp-example))
  (:module depdefs ("depdefs" #+CLX-Genera-New-Scheduler "generalock"))
  (:module clx ("clx")
	   (:uses-definitions-from depdefs))
  (:module dependent ("dependent")
	   (:uses-definitions-from clx))
  (:module macros ("macros")
	   (:uses-definitions-from dependent))
  (:module bufmac ("bufmac")
	   (:uses-definitions-from dependent macros))
  (:module buffer ("buffer")
	   (:uses-definitions-from dependent macros bufmac))
  (:module display ("display")
	   (:uses-definitions-from dependent macros bufmac buffer))
  (:module gcontext ("gcontext")
	   (:uses-definitions-from dependent macros bufmac display))
  (:module input ("input")
	   (:uses-definitions-from dependent macros bufmac display))
  (:module requests ("requests")
	   (:uses-definitions-from dependent macros bufmac display input))
  (:module fonts ("fonts")
	   (:uses-definitions-from dependent macros bufmac display))
  (:module graphics ("graphics")
	   (:uses-definitions-from dependent macros bufmac fonts))
  (:module text ("text")
	   (:uses-definitions-from dependent macros bufmac gcontext fonts))
  (:module attributes ("attributes")
	   (:uses-definitions-from dependent macros bufmac display))
  (:module translate ("translate")
	   (:uses-definitions-from dependent macros bufmac display))
  (:module keysyms ("keysyms")
	   (:uses-definitions-from translate))
  (:module manager ("manager")
	   (:uses-definitions-from dependent macros bufmac display))
  (:module image ("image")
	   (:uses-definitions-from dependent macros bufmac display))
  (:module resource ("resource"))
  )


;;;; Non Lisp Machines

#+lucid
(defvar *foreign-libraries* '("-lc")) ; '("-lresolv" "-lc") for some sites

#+lucid
(defun clx-foreign-files ()

  ;; apply a patch to 2.0 systems
  #-(or lcl3.0 vax) (load "make-sequence-patch")

  ;; Link lisp to the C function connect_to_server
  #+(and apollo (not lcl3.0))
  (lucid::define-foreign-function '(xlib::connect-to-server "connect_to_server")
      '((:val host    :string)
	(:val display :integer32))
    :integer32)
  #+(and (not apollo) (not lcl3.0))
  (lucid::define-c-function xlib::connect-to-server
      (host display)
    :result-type :integer)
  #+lcl3.0
  (lucid::def-foreign-function (xlib::connect-to-server 
				 (:language :c)
				 (:return-type :signed-32bit))
      (host :simple-string) (display :signed-32bit))
  (unintern 'display)

  ;; Load the definition of connect_to_server
  #+apollo
  (lucid::load-foreign-file "socket" :preserve-pathname t)
  #-apollo
  (lucid::load-foreign-files '("socket.o") *foreign-libraries*))


;; socket interface for kcl
;;   defines the function (open-socket-stream host display)
;;
;; You must first compile file socket.c
#+kcl
(defun kcl-socket-init (binary-path)
  (let ((sockcl (namestring (merge-pathnames "sockcl.o" binary-path)))
	(socket (namestring (merge-pathnames "socket.o" binary-path))))
    (si:faslink sockcl (format nil "~a -lc" socket))
    ))


;;;; Compile CLX

;;; COMPILE-CLX compiles the lisp source files and loads the binaries.
;;; It goes to some trouble to let the source files be in one directory
;;; and the binary files in another.  Thus the same set of sources can
;;; be used for different machines and/or lisp systems.  It also allows
;;; you to supply explicit extensions, so source files do not have to
;;; be renamed to fit into the naming conventions of an implementation.

;;; For example,
;;;     (compile-clx "*.lisp" "machine/")
;;; compiles source files from the connected directory and puts them
;;; into the "machine" subdirectory.  You can then load CLX out of the
;;; machine directory.

;;; The code has no knowledge of the source file types (eg, ".l" or
;;; ".lisp") or of the binary file types (eg, ".b" or ".sbin").  Calling
;;; compile-file and load with a file type of NIL usually sorts things
;;; out correctly, but you may have to explicitly give the source and
;;; binary file types.

;;; An attempt at compiling the C language sources is also made,
;;; but you may have to set different compiler switches
;;; should be.  If it doesn't do the right thing, then do
;;;     (compile-clx "" "" :compile-c NIL)
;;; to prevent the compilation.

;;; compilation notes
;;;   lucid2.0/hp9000
;;;     load foreign files uses connected directory instead of 
;;;       *default-pathname-defaults* so cd to binary directory and
;;;       (compile-clx "../*.lisp" "./") 
;;;     must uudecode the file

#-lispm
(defun compile-clx (&optional
		    (source-pathname-defaults "")
		    (binary-pathname-defaults "")
		    &key
		    (compile-c t))

  ;; The pathname-defaults above might only be strings, so coerce them
  ;; to pathnames.  Build a default binary path with every component
  ;; of the source except the file type.  This should prevent
  ;; (compile-clx "*.lisp") from destroying source files.
  (let* ((source-path (pathname source-pathname-defaults))
	 (path        (make-pathname
			:host      (pathname-host      source-path)
			:device    (pathname-device    source-path)
			:directory (pathname-directory source-path)
			:name      (pathname-name      source-path)
			:type      NIL
			:version   (pathname-version   source-path)))
	 (binary-path (merge-pathnames binary-pathname-defaults
				       path)))
				       
    ;; Make sure source-path and binary-path file types are distinct so
    ;; we don't accidently overwrite the source files.  NIL should be an
    ;; ok type, but anything else spells trouble.
    (if (and (equal (pathname-type source-path)
		    (pathname-type binary-path))
	     (not (null (pathname-type binary-path))))
	(error "Source and binary pathname defaults have same type ~s ~s"
	       source-path binary-path))

    (format t ";;; Default paths: ~s ~s~%" source-path binary-path)

    ;; Make sure we're using the lucid production compiler 
    #+(and lcl3.0 (not clx-debugging))
    (proclaim '(optimize (speed 3) (compilation-speed 0)))

    (flet ((compile-and-load (filename)
	     (let ((source (merge-pathnames filename source-path))
		   (binary (merge-pathnames filename binary-path)))
	       ;; If the source and binary pathnames are the same,
	       ;; then don't supply an output file just to be sure
	       ;; compile-file defaults correctly.
	       (if (equal source binary)
		   (compile-file source)
		   (compile-file source :output-file binary))
	       (load binary))))

      ;; Now compile and load all the files.  For lucid 3.0, make the
      ;; compiler warnings apply to the files as a whole.
      (#+lcl3.0 lucid::with-deferred-warnings #-lcl3.0 progn

       #+lucid
       (progn 
	 (when compile-c			; compile the C files
	   #-(or lcl3.0 vax)
	   (progn				; sequence patch
	     (format t "You may need to uudecode ms-patch.uu and copy~%")
	     (format t "the result to the binary directory.~%")
	     (format t "You also must rename the file to have the canonical~%")
	     (format t "binary file type in order for lisp to realize it's a~%")
	     (format t "binary file.~%"))
	   ;; compile socket.c
	   (let* ((src  (merge-pathnames "socket.c" source-path))
		  (obj  (merge-pathnames "socket.o" binary-path))
		  (args (list "-c" (namestring src)
			      "-o" (namestring obj)
			      "-DUNIXCONN")))
	     (format t ";;; cc~{ ~a~}~%" args)
	     (multiple-value-bind (iostream estream exitstatus pid)
		 ;; in 2.0, run-program is exported from system:
		 ;; in 3.0, run-program is exported from lcl:
		 ;; system inheirits lcl
		 (system::run-program "cc" :arguments args)
	       (declare (ignore iostream estream pid))
	       (if (/= 0 exitstatus)
		   (error "Exit status of socket.c compile is ~d" exitstatus)))))
	 (let ((*default-pathname-defaults* binary-path))
	   (declare (special *default-pathname-defaults*))
	   (format t ";;; Loading foreign files~%")
	   (clx-foreign-files)))

       #+kcl
       (progn
	 (when compile-c			; compile the C files
	   (let* ((src (merge-pathnames "socket.c" source-path))
		  (obj (merge-pathnames "socket.o" binary-path))
		  (arg (format nil "cc -c ~a -o ~a -DUNIXCONN"
			       (namestring src)
			       (namestring obj))))
	     (format t ";;; ~a~%" arg)
	     (if (/= 0 (system arg))
		 (error "bad exit status for ~s" src))))
	 ;; compile the lisp interface to the c code
	 (let ((src (merge-pathnames "sockcl"   source-path))
	       (obj (merge-pathnames "sockcl.o" binary-path)))
	   (compile-file src :output-file obj))
	 (kcl-socket-init binary-path))

       (compile-and-load "depdefs")
       (compile-and-load "clx")
       (compile-and-load "dependent")
       (compile-and-load "macros")		; these are just macros
       (compile-and-load "bufmac")		; these are just macros
       (compile-and-load "buffer")
       (compile-and-load "display")
       (compile-and-load "gcontext")
       (compile-and-load "input")
       (compile-and-load "requests")
       (compile-and-load "fonts")
       (compile-and-load "graphics")
       (compile-and-load "text")
       (compile-and-load "attributes")
       (compile-and-load "translate")
       (compile-and-load "keysyms")
       (compile-and-load "manager")
       (compile-and-load "image")
       (compile-and-load "resource")
       ))))


;;;; Load CLX

;;; This procedure loads the binaries for CLX.  All of the binaries
;;; should be in the same directory, so setting the default pathname
;;; should point load to the right place.

;;; You should have a module definition somewhere so the require/provide
;;; mechanism can avoid reloading CLX.  In an ideal world, somebody would
;;; just put
;;;		(REQUIRE 'CLX)
;;; in their file (some implementations don't have a central registry for
;;; modules, so a pathname needs to be supplied).

;;; The REQUIRE should find a file that does
;;;		(IN-PACKAGE 'XLIB :USE '(LISP))
;;;		(PROVIDE 'CLX)
;;;		(LOAD <clx-defsystem-file>)
;;;		(LOAD-CLX <binary-specific-clx-directory>)

#-lispm
(defun load-clx (&optional (pathname-defaults "")
		 &key (macros-p nil))
  ;; Try to keep *default-pathname-defaults* as an absolute pathname.
  (let ((*default-pathname-defaults* (merge-pathnames pathname-defaults)))
    (declare (special *default-pathname-defaults*))

    #+lucid
    (clx-foreign-files)

    #+kcl
    (kcl-socket-init pathname-defaults)

    (load "depdefs")
    (load "clx")
    (load "dependent")
    (when macros-p
      (load "macros")
      (load "bufmac"))
    (load "buffer")
    (load "display")
    (load "gcontext")
    (load "input")
    (load "requests")
    (load "fonts")
    (load "graphics")
    (load "text")
    (load "attributes")
    (load "translate")
    (load "keysyms")
    (load "manager")
    (load "image")
    (load "resource")
    ))
