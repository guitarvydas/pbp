#|  Runtime data of a Part. |#                              #|line 1|# #|  Parts are either leaves (a user-supplied handler) or containers (a handler that routes mevents to children). |# #|line 2|# #|  `handler` is called as handler (eh, mev). |# #|line 3|# #|  `instance_data` is private to the leaf; the handler finds it again each time it is invoked. |# #|line 4|# #|  `visit_queue` (containers only) holds the children that have been sent a mevent, once for each mevent. |# #|line 5|# #|  `tick` is a function (eh) that continues the processing of an active part. make_container sets it to container_tick; the programmer supplies it to make_leaf. |# #|line 6|# #|  `reset` is a function (eh) that puts the part back into a known state. make_container sets it to container_reset; the programmer supplies it to make_leaf. |# #|line 7|# #|  Eh_States :: enum { idle, active } |# #|line 8|#
(defclass Eh ()                                             #|line 9|#
  (
    (name :accessor name :initarg :name :initform  "")      #|line 10|#
    (inq :accessor inq :initarg :inq :initform  (make-instance 'Queue) #|line 11|#)
    (outq :accessor outq :initarg :outq :initform  (make-instance 'Queue) #|line 12|#)
    (owner :accessor owner :initarg :owner :initform  nil)  #|line 13|#
    (children :accessor children :initarg :children :initform  nil)  #|line 14|#
    (visit_queue :accessor visit_queue :initarg :visit_queue :initform  (make-instance 'Queue) #|line 15|#)
    (connections :accessor connections :initarg :connections :initform  nil)  #|line 16|#
    (handler :accessor handler :initarg :handler :initform  nil)  #|line 17|#
    (tick :accessor tick :initarg :tick :initform  nil)     #|line 18|#
    (reset :accessor reset :initarg :reset :initform  nil)  #|line 19|#
    (instance_data :accessor instance_data :initarg :instance_data :initform  nil)  #|line 20|#
    (arg :accessor arg :initarg :arg :initform  "")         #|line 21|#
    (state :accessor state :initarg :state :initform  "idle")  #|line 22|#
    (vip :accessor vip :initarg :vip :initform  nil)        #|line 23|#)) #|line 24|#
