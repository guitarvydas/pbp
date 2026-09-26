#|line 1|#
(defclass Datum ()                                          #|line 2|#
  (
    (v :accessor v :initarg :v :initform  nil)              #|line 3|#
    (clone :accessor clone :initarg :clone :initform  nil)  #|line 4|#
    (reclaim :accessor reclaim :initarg :reclaim :initform  nil)  #|line 5|#
    (other :accessor other :initarg :other :initform  nil)  #|  reserved for use on per-project basis  |# #|line 6|#)) #|line 7|#

                                                            #|line 8|# #|line 9|# #|  Mevent passed to a leaf component. |# #|line 10|# #|  |# #|line 11|# #|  `port` refers to the name of the incoming or outgoing port of this component. |# #|line 12|# #|  `payload` is the data attached to this mevent. |# #|line 13|#
(defclass Mevent ()                                         #|line 14|#
  (
    (port :accessor port :initarg :port :initform  nil)     #|line 15|#
    (payload :accessor payload :initarg :payload :initform  nil)  #|line 16|#)) #|line 17|#

                                                            #|line 18|#
(defun clone_port (&optional  s)
  (declare (ignorable  s))                                  #|line 19|#
  (return-from clone_port (funcall (quote clone_string)   s  #|line 20|#)) #|line 21|#
  ) #|  Utility for making a `Mevent`. Used to safely "seed“ mevents |# #|line 23|# #|  entering the very top of a network. |# #|line 24|#
(defun make_mevent (&optional  port  datum)
  (declare (ignorable  port  datum))                        #|line 25|#
  (let ((p (funcall (quote clone_string)   port             #|line 26|#)))
    (declare (ignorable p))
    (let (( m  (make-instance 'Mevent)                      #|line 27|#))
      (declare (ignorable  m))
      (setf (slot-value  m 'port)  p)                       #|line 28|#
      (setf (slot-value  m 'payload) (funcall (slot-value  datum 'clone) )) #|line 29|#
      (return-from make_mevent  m)                          #|line 30|#)) #|line 31|#
  ) #|  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. |# #|line 33|#
(defun mevent_clone (&optional  mev)
  (declare (ignorable  mev))                                #|line 34|#
  (let (( m  (make-instance 'Mevent)                        #|line 35|#))
    (declare (ignorable  m))
    (setf (slot-value  m 'port) (funcall (quote clone_port)  (slot-value  mev 'port)  #|line 36|#))
    (setf (slot-value  m 'payload) (funcall (slot-value (slot-value  mev 'payload) 'clone) )) #|line 37|#
    (return-from mevent_clone  m)                           #|line 38|#) #|line 39|#
  ) #|  Frees a mevent. |#                                  #|line 41|#
(defun destroy_mevent (&optional  mev)
  (declare (ignorable  mev))                                #|line 42|#
  #|  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents |# #|line 43|#
  #| pass |#                                                #|line 44|# #|line 45|#
  )
(defun destroy_datum (&optional  mev)
  (declare (ignorable  mev))                                #|line 47|#
  #| pass |#                                                #|line 48|# #|line 49|#
  )
(defun destroy_port (&optional  mev)
  (declare (ignorable  mev))                                #|line 51|#
  #| pass |#                                                #|line 52|# #|line 53|#
  ) #|  |#                                                  #|line 55|#
(defun format_mevent (&optional  m)
  (declare (ignorable  m))                                  #|line 56|#
  (cond
    (( equal    m  nil)                                     #|line 57|#
      (return-from format_mevent  "{}")                     #|line 58|#
      )
    (t                                                      #|line 59|#
      (return-from format_mevent  (concatenate 'string  "{%5C”"  (concatenate 'string (slot-value  m 'port)  (concatenate 'string  "%5C”:%5C”"  (concatenate 'string (slot-value (slot-value  m 'payload) 'v)  "%5C”}")))) #|line 60|#) #|line 61|#
      ))                                                    #|line 62|#
  )
(defun format_mevent_raw (&optional  m)
  (declare (ignorable  m))                                  #|line 63|#
  (cond
    (( equal    m  nil)                                     #|line 64|#
      (return-from format_mevent_raw  "")                   #|line 65|#
      )
    (t                                                      #|line 66|#
      (return-from format_mevent_raw (slot-value (slot-value  m 'payload) 'v)) #|line 67|# #|line 68|#
      ))                                                    #|line 69|#
  )
