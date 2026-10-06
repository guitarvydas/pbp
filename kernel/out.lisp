#|  Data for an asyncronous component _ effectively, a function with input |# #|line 1|# #|  and output queues of mevents. |# #|line 2|# #|  |# #|line 3|# #|  Components can either be a user_supplied function ("leaf“), or a “container“ |# #|line 4|# #|  that routes mevents to child components according to a list of connections |# #|line 5|# #|  that serve as a mevent routing table. |# #|line 6|# #|  |# #|line 7|# #|  Child components themselves can be leaves or other containers. |# #|line 8|# #|  |# #|line 9|# #|  `handler` invokes the code that is attached to this component. |# #|line 10|# #|  |# #|line 11|# #|  `instance_data` is a pointer to instance data that the `leaf_handler` |# #|line 12|# #|  function may want whenever it is invoked again. |# #|line 13|# #|line 14|# #|  Eh_States :: enum { idle, active } |# #|line 15|#
(defclass Eh ()                                             #|line 16|#
  (
    (name :accessor name :initarg :name :initform  "")      #|line 17|#
    (inq :accessor inq :initarg :inq :initform  (make-instance 'Queue) #|line 18|#)
    (outq :accessor outq :initarg :outq :initform  (make-instance 'Queue) #|line 19|#)
    (owner :accessor owner :initarg :owner :initform  nil)  #|line 20|#
    (children :accessor children :initarg :children :initform  nil)  #|line 21|#
    (visit_ordering :accessor visit_ordering :initarg :visit_ordering :initform  (make-instance 'Queue) #|line 22|#)
    (connections :accessor connections :initarg :connections :initform  nil)  #|line 23|#
    (handler :accessor handler :initarg :handler :initform  nil)  #|line 24|#
    (finject :accessor finject :initarg :finject :initform  nil)  #|line 25|#
    (reset :accessor reset :initarg :reset :initform  nil)  #|line 26|#
    (instance_data :accessor instance_data :initarg :instance_data :initform  nil)  #|line 27|# #|  arg needed for probe support  |# #|line 28|#
    (arg :accessor arg :initarg :arg :initform  "")         #|line 29|#
    (state :accessor state :initarg :state :initform  "idle")  #|line 30|#
    (special :accessor special :initarg :special :initform  nil)  #|line 31|#)) #|line 32|#

                                                            #|line 33|#
(defun injector (&optional  eh  mevent)
  (declare (ignorable  eh  mevent))                         #|line 34|#
  (funcall (slot-value  eh 'handler)   eh  mevent           #|line 35|#) #|line 36|#
  )
