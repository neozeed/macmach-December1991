;;; -*- Mode: LISP; Syntax: Common-lisp; Base: 10; Lowercase: Yes;  -*-

;;;; Module definition for CLX

;;; This file is a Common Lisp Module description, but you will have to edit
;;; it to meet the needs of your site.

;;; Ideally, this file (or a file that loads this file) should be
;;; located in the system directory that REQUIRE searches.  Thus a user
;;; would say
;;;			(REQUIRE 'CLX)
;;; to load CLX.  If there is no such registry, then the user must
;;; put in a site specific
;;;			(REQUIRE 'CLX <pathname-of-this-file>)
;;;
;;; The file CLX.LISP does the PROVIDE.
;;;

(in-package 'xlib :use '(lisp))

;;; Load the defsystem file from the source directory.  You may
;;; want to include an explicit extension (such as ".l" or ".lisp").
;;;
(load "/usr/src/local/clx/defsystem")

;;; The binary files for a particular lisp implementation and architecture.
;;;
(load-clx
  #+(and lucid sun      sparc ) "/usr/src/local/clx/lucid.sparc/"
  #+(and lucid sun (not sparc)) "/usr/src/local/clx/lucid.sun3/"
  #+(and lucid hp)              "/usr/src/local/clx/lucid.hp9000/"
  #+(and kcl            sparc ) "/usr/src/local/clx/akcl.sparc/"
  #+(and kcl       (not sparc)) "/usr/src/local/clx/akcl.sun3/")
