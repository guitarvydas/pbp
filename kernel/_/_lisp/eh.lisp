#|  Data for an asyncronous component _ effectively, a function with input |# #|line 1|# #|  and output queues of mevents. |# #|line 2|# #|  |# #|line 3|# #|  Components can either be a user_supplied function ("leaf“), or a “container“ |# #|line 4|# #|  that routes mevents to child components according to a list of connections |# #|line 5|# #|  that serve as a mevent routing table. |# #|line 6|# #|  |# #|line 7|# #|  Child components themselves can be leaves or other containers. |# #|line 8|# #|  |# #|line 9|# #|  `handler` invokes the code that is attached to this component. |# #|line 10|# #|  |# #|line 11|# #|  `instance_data` is a pointer to instance data that the `leaf_handler` |# #|line 12|# #|  function may want whenever it is invoked again. |# #|line 13|# #|line 14|# #|  TODO: what is .routings for? (is it a historical artefact that can be removed?)  |# #|line 15|# #|line 16|# #|  Eh_States :: enum { idle, active } |# #|line 17|#
(defclass Eh ()                                             #|line 18|#
  (
    (name :accessor name :initarg :name :initform  "")      #|line 19|#
    (inq :accessor inq :initarg :inq :initform  (make-instance 'Queue) #|line 20|#)
    (outq :accessor outq :initarg :outq :initform  (make-instance 'Queue) #|line 21|#)
    (owner :accessor owner :initarg :owner :initform  nil)  #|line 22|#
    (children :accessor children :initarg :children :initform  nil)  #|line 23|#
    (visit_ordering :accessor visit_ordering :initarg :visit_ordering :initform  (make-instance 'Queue) #|line 24|#)
    (connections :accessor connections :initarg :connections :initform  nil)  #|line 25|#
    (handler :accessor handler :initarg :handler :initform  nil)  #|line 26|#
    (reset_instance_data :accessor reset_instance_data :initarg :reset_instance_data :initform  nil)  #|line 27|#
    (finject :accessor finject :initarg :finject :initform  nil)  #|line 28|#
    (stop :accessor stop :initarg :stop :initform  nil)     #|line 29|#
    (instance_data :accessor instance_data :initarg :instance_data :initform  nil)  #|line 30|# #|  arg needed for probe support  |# #|line 31|#
    (arg :accessor arg :initarg :arg :initform  "")         #|line 32|#
    (state :accessor state :initarg :state :initform  "idle")  #|line 33|#
    (special :accessor special :initarg :special :initform  nil)  #|line 34|# #|  bootstrap debugging |# #|line 35|#
    (kind :accessor kind :initarg :kind :initform  nil)  #|  enum { container, leaf, } |# #|line 36|#)) #|line 37|#

                                                            #|line 38|#
(defun injector (&optional  eh  mevent)
  (declare (ignorable  eh  mevent))                         #|line 39|#
  (funcall (slot-value  eh 'handler)   eh  mevent           #|line 40|#) #|line 41|#
  )
