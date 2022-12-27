;******************************************************************************
;* MENU.LSP -- Database / Menu Demonstration
;******************************************************************************

; create name list from result of db-name
(defun name (name-result)
  (cond
    ((null name-result) nil)
    (t (cons (car (cdr (car name-result))) (name (cdr name-result))))
  )
)

; process a data item
(defun process (index)
  (prog (update)
    (princ "INDEX = ") (princ index) (terpri)
    (princ "NAME = ") (princ (name (db-name index))) (terpri)
    (princ "DATA = ") (princ (db-data index)) (terpri)
    (terpri) (princ "Enter update or NIL: ")
    (if (setq update (read)) (db-update index update))
    (return t)
  )
)

; demonstrate use of db-menu
(defun menu (header footer process)
  (let (result (group 0))
    (loop
      (if (not (setq result (db-menu group header footer)))
        (return t)
        (process (cadr result))
      )
      (setq group (if (null result) 0 (car result)))
    )
  )
)

; open database "data.bin" or load from "data.txt"
(if (not (db-open "data.bin")) (if (not (db-open (db-load "data.txt"))) (exit)))

; apply menu function
(menu "Database / Menu Demonstration" "Copyright (c) 1990 FWR Group" 'process)

; all done
(exit)
