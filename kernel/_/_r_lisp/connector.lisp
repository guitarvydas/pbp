#|line 1|#
(defparameter  enumDown  0)
(defparameter  enumAcross  1)
(defparameter  enumUp  2)
(defparameter  enumThrough  3)                              #|line 6|# #|line 7|# #|  Routing connection for a container component. The `direction` field has |# #|line 8|# #|  no affect on the default mevent routing system _ it is there for debugging |# #|line 9|# #|  purposes, or for reading by other tools. |# #|line 10|# #|line 11|#
(defclass Connector ()                                      #|line 12|#
  (
    (direction :accessor direction :initarg :direction :initform  nil)  #|  down, across, up, through |# #|line 13|#
    (sender :accessor sender :initarg :sender :initform  nil)  #|line 14|#
    (receiver :accessor receiver :initarg :receiver :initform  nil)  #|line 15|#)) #|line 16|#

                                                            #|line 17|# #|  `Sender` is used to "pattern match“ which `Receiver` a mevent should go to, |# #|line 18|# #|  based on component ID (pointer) and port name. |# #|line 19|# #|line 20|#
(defclass Sender ()                                         #|line 21|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 22|#
    (component :accessor component :initarg :component :initform  nil)  #|line 23|#
    (port :accessor port :initarg :port :initform  nil)     #|line 24|#)) #|line 25|#

                                                            #|line 26|# #|line 27|# #|line 28|# #|  `Receiver` is a handle to a destination queue, and a `port` name to assign |# #|line 29|# #|  to incoming mevents to this queue. |# #|line 30|# #|line 31|#
(defclass Receiver ()                                       #|line 32|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 33|#
    (queue :accessor queue :initarg :queue :initform  nil)  #|line 34|#
    (port :accessor port :initarg :port :initform  nil)     #|line 35|#
    (component :accessor component :initarg :component :initform  nil)  #|line 36|#)) #|line 37|#

                                                            #|line 38|#
(defun mkSender (&optional  name  component  port)
  (declare (ignorable  name  component  port))              #|line 39|#
  (let (( s  (make-instance 'Sender)                        #|line 40|#))
    (declare (ignorable  s))
    (setf (slot-value  s 'name)  name)                      #|line 41|#
    (setf (slot-value  s 'component)  component)            #|line 42|#
    (setf (slot-value  s 'port)  port)                      #|line 43|#
    (return-from mkSender  s)                               #|line 44|#) #|line 45|#
  )
(defun mkReceiver (&optional  name  component  port  q)
  (declare (ignorable  name  component  port  q))           #|line 47|#
  (let (( r  (make-instance 'Receiver)                      #|line 48|#))
    (declare (ignorable  r))
    (setf (slot-value  r 'name)  name)                      #|line 49|#
    (setf (slot-value  r 'component)  component)            #|line 50|#
    (setf (slot-value  r 'port)  port)                      #|line 51|#
    #|  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. |# #|line 52|#
    (setf (slot-value  r 'queue)  q)                        #|line 53|#
    (return-from mkReceiver  r)                             #|line 54|#) #|line 55|#
  )
