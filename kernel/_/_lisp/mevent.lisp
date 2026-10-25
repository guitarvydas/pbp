#|  A mevent has a port (the name of the incoming or outgoing port) and a payload. The payload is always a string. |# #|line 1|#
(defclass Mevent ()                                         #|line 2|#
  (
    (port :accessor port :initarg :port :initform  nil)     #|line 3|#
    (payload :accessor payload :initarg :payload :initform  nil)  #|line 4|#)) #|line 5|#

                                                            #|line 6|#
(defun make_mevent (&optional  port  payload)
  (declare (ignorable  port  payload))                      #|line 7|#
  (let ((m  (make-instance 'Mevent)                         #|line 8|#))
    (declare (ignorable m))
    (setf (slot-value  m 'port)  port)                      #|line 9|#
    (setf (slot-value  m 'payload)  payload)                #|line 10|#
    (return-from make_mevent  m)                            #|line 11|#) #|line 12|#
  )
