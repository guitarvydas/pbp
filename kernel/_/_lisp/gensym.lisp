(defparameter  digits (list                                 #|line 1|#  "₀"  "₁"  "₂"  "₃"  "₄"  "₅"  "₆"  "₇"  "₈"  "₉"  "₁₀"  "₁₁"  "₁₂"  "₁₃"  "₁₄"  "₁₅"  "₁₆"  "₁₇"  "₁₈"  "₁₉"  "₂₀"  "₂₁"  "₂₂"  "₂₃"  "₂₄"  "₂₅"  "₂₆"  "₂₇"  "₂₈"  "₂₉" )) #|line 7|# #|line 8|# #|line 9|#
(defun subscripted_digit (&optional  n)
  (declare (ignorable  n))                                  #|line 10|# #|line 11|#
  (cond
    (( and  ( >=   n  0) ( <=   n  29))                     #|line 12|#
      (return-from subscripted_digit (nth  n  digits))      #|line 13|#
      )
    (t                                                      #|line 14|#
      (return-from subscripted_digit  (concatenate 'string  "₊" (format nil "~a"  n)) #|line 15|#) #|line 16|#
      ))                                                    #|line 17|#
  )
(defun gensymbol (&optional  s)
  (declare (ignorable  s))                                  #|line 19|# #|line 20|#
  (let ((name_with_id  (concatenate 'string  s (funcall (quote subscripted_digit)   counter )) #|line 21|#))
    (declare (ignorable name_with_id))
    (setf  counter (+  counter  1))                         #|line 22|#
    (return-from gensymbol  name_with_id)                   #|line 23|#) #|line 24|#
  )
