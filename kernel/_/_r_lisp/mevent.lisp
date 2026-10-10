#|line 1|#
(defclass Datum ()                                          #|line 2|#
  (
    (v :accessor v :initarg :v :initform  nil)              #|line 3|#
    (clone :accessor clone :initarg :clone :initform  nil)  #|line 4|#
    (reclaim :accessor reclaim :initarg :reclaim :initform  nil)  #|line 5|#)) #|line 6|#

                                                            #|line 7|# #|line 8|# #|  Mevent passed to a leaf component. |# #|line 9|# #|  |# #|line 10|# #|  `port` refers to the name of the incoming or outgoing port of this component. |# #|line 11|# #|  `payload` is the data attached to this mevent. |# #|line 12|#
(defclass Mevent ()                                         #|line 13|#
  (
    (port :accessor port :initarg :port :initform  nil)     #|line 14|#
    (payload :accessor payload :initarg :payload :initform  nil)  #|line 15|#)) #|line 16|#

                                                            #|line 17|#
(defun clone_port (&optional  s)
  (declare (ignorable  s))                                  #|line 18|#
  (return-from clone_port (funcall (quote clone_string)   s  #|line 19|#)) #|line 20|#
  ) #|  Utility for making a `Mevent`. Used to safely "seed“ mevents |# #|line 22|# #|  entering the very top of a network. |# #|line 23|#
(defun make_mevent (&optional  port  datum)
  (declare (ignorable  port  datum))                        #|line 24|#
  (let ((p (funcall (quote clone_string)   port             #|line 25|#)))
    (declare (ignorable p))
    (let (( m  (make-instance 'Mevent)                      #|line 26|#))
      (declare (ignorable  m))
      (setf (slot-value  m 'port)  p)                       #|line 27|#
      (setf (slot-value  m 'payload) (funcall (slot-value  datum 'clone) )) #|line 28|#
      (return-from make_mevent  m)                          #|line 29|#)) #|line 30|#
  ) #|  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. |# #|line 32|#
(defun mevent_clone (&optional  mev)
  (declare (ignorable  mev))                                #|line 33|#
  (let (( m  (make-instance 'Mevent)                        #|line 34|#))
    (declare (ignorable  m))
    (setf (slot-value  m 'port) (funcall (quote clone_port)  (slot-value  mev 'port)  #|line 35|#))
    (setf (slot-value  m 'payload) (funcall (slot-value (slot-value  mev 'payload) 'clone) )) #|line 36|#
    (return-from mevent_clone  m)                           #|line 37|#) #|line 38|#
  ) #|  Frees a mevent. |#                                  #|line 40|#
(defun destroy_mevent (&optional  mev)
  (declare (ignorable  mev))                                #|line 41|#
  #|  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents |# #|line 42|#
  #| pass |#                                                #|line 43|# #|line 44|#
  )
(defun destroy_datum (&optional  mev)
  (declare (ignorable  mev))                                #|line 46|#
  #| pass |#                                                #|line 47|# #|line 48|#
  )
(defun destroy_port (&optional  mev)
  (declare (ignorable  mev))                                #|line 50|#
  #| pass |#                                                #|line 51|# #|line 52|#
  ) #|  |#                                                  #|line 54|#
(defun format_mevent (&optional  m)
  (declare (ignorable  m))                                  #|line 55|#
  (cond
    (( equal    m  nil)                                     #|line 56|#
      (return-from format_mevent  "{}")                     #|line 57|#
      )
    (t                                                      #|line 58|#
      (return-from format_mevent  (concatenate 'string  "{%5C”"  (concatenate 'string (slot-value  m 'port)  (concatenate 'string  "%5C”:%5C”"  (concatenate 'string (slot-value (slot-value  m 'payload) 'v)  "%5C”}")))) #|line 59|#) #|line 60|#
      ))                                                    #|line 61|#
  )
(defun format_mevent_raw (&optional  m)
  (declare (ignorable  m))                                  #|line 62|#
  (cond
    (( equal    m  nil)                                     #|line 63|#
      (return-from format_mevent_raw  "")                   #|line 64|#
      )
    (t                                                      #|line 65|#
      (return-from format_mevent_raw (slot-value (slot-value  m 'payload) 'v)) #|line 66|# #|line 67|#
      ))                                                    #|line 68|#
  )
