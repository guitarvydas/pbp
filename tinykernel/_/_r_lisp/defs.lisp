(load "~/quicklisp/setup.lisp")
(proclaim '(optimize (debug 3) (safety 3) (speed 0)))
(ql:quickload :uiop)
(ql:quickload :cl-json)

(defun getwd (s)
#+lispworks (merge-pathnames s (get-working-directory))
#-lispworks s
)

(defun dict-fresh () (make-hash-table :test 'equal))

(defun dict-in? (name table)
(when (and table name)
(multiple-value-bind (dont-care found)
(gethash name table)
dont-care ;; quell warnings that dont-care is unused
found)))

(defun jparse (filename)
(let ((s (uiop:read-file-string filename)))
(internalize-lnet-from-JSON s)))

(defun internalize-lnet-from-JSON (s)
(let ((s (uiop:read-file-string filename)))
(let ((cl-json:*json-identifier-name-to-lisp* 'identity)) ;; preserves case
(with-input-from-string (strm s)
(cl-json:decode-json strm)))))

(defun json2dict (filename)
(let ((j (jparse filename)))
(make-dict nil j)))


(defun make-dict (dict x)
(assert (or (not (null dict)) (not (null x))))
(cond

;; done
((null x) dict)

;; bottom
((atom x) x)

;; key/value pair - put it in dict
((kv? x)
(let ((v (make-dict dict (val x))))
(setf (gethash (key x) dict) v)
dict))

;; begin new dict
((kv? (car x))
(let ((new-dict (make-hash-table :test 'equal)))
(mapc #'(lambda (y)
(make-dict new-dict y))
x)
new-dict))

;; list of dicts (json array)
((not (kv? (car x)))
;; list of kvs (json array)
(mapcar #'(lambda (y)
(make-dict nil y))
x))))

(defun key (kv)
(symbol-name (car kv)))

(defun val (kv)
(cdr kv))

(defun kv? (x)
(and (listp x)
(atom (car x))))

;;;;
;(load "~/quicklisp/setup.lisp")
(ql:quickload '(:websocket-driver-client :cl-json :uiop))

(defun live_update (key value)
(let* ((client (wsd:make-client "ws://localhost:8966"))
(json-data (json:encode-json-to-string
(list (cons key value)))))
(wsd:start-connection client)
(wsd:send client json-data)
(sleep 0.1)  ; Add small delay to ensure message is sent
(wsd:close-connection client)))


;;;;

(defclass Queue ()
((contents :accessor contents :initform nil)))

(defmethod enqueue ((self Queue) v)
(setf (contents self) (append (contents self) (list v))))

(defmethod prequeue ((self Queue) v)
(push v (contents self)))

(defmethod dequeue ((self Queue))
(pop (contents self)))

(defmethod empty? ((self Queue))
(null (contents self)))

(defmethod queue2list ((self Queue))
(contents self))
                                                            #|line 1|# #|line 2|#
(defparameter  counter  0)                                  #|line 3|#
(defparameter  ticktime  0)                                 #|line 4|# #|line 5|#
(defparameter  enumDown  0)
(defparameter  enumAcross  1)
(defparameter  enumUp  2)
(defparameter  enumThrough  3)                              #|line 10|# #|line 11|# #|  Routing connection for a container component. The `direction` field has |# #|line 12|# #|  no affect on the default mevent routing system _ it is there for debugging |# #|line 13|# #|  purposes, or for reading by other tools. |# #|line 14|# #|line 15|#
(defclass Connector ()                                      #|line 16|#
  (
    (direction :accessor direction :initarg :direction :initform  nil)  #|  down, across, up, through |# #|line 17|#
    (sender :accessor sender :initarg :sender :initform  nil)  #|line 18|#
    (receiver :accessor receiver :initarg :receiver :initform  nil)  #|line 19|#)) #|line 20|#

                                                            #|line 21|# #|  `Sender` is used to "pattern match“ which `Receiver` a mevent should go to, |# #|line 22|# #|  based on component ID (pointer) and port name. |# #|line 23|# #|line 24|#
(defclass Sender ()                                         #|line 25|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 26|#
    (component :accessor component :initarg :component :initform  nil)  #|line 27|#
    (port :accessor port :initarg :port :initform  nil)     #|line 28|#)) #|line 29|#

                                                            #|line 30|# #|line 31|# #|line 32|# #|  `Receiver` is a handle to a destination queue, and a `port` name to assign |# #|line 33|# #|  to incoming mevents to this queue. |# #|line 34|# #|line 35|#
(defclass Receiver ()                                       #|line 36|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 37|#
    (queue :accessor queue :initarg :queue :initform  nil)  #|line 38|#
    (port :accessor port :initarg :port :initform  nil)     #|line 39|#
    (component :accessor component :initarg :component :initform  nil)  #|line 40|#)) #|line 41|#

                                                            #|line 42|#
(defun mkSender (&optional  name  component  port)
  (declare (ignorable  name  component  port))              #|line 43|#
  (let (( s  (make-instance 'Sender)                        #|line 44|#))
    (declare (ignorable  s))
    (setf (slot-value  s 'name)  name)                      #|line 45|#
    (setf (slot-value  s 'component)  component)            #|line 46|#
    (setf (slot-value  s 'port)  port)                      #|line 47|#
    (return-from mkSender  s)                               #|line 48|#) #|line 49|#
  )
(defun mkReceiver (&optional  name  component  port  q)
  (declare (ignorable  name  component  port  q))           #|line 51|#
  (let (( r  (make-instance 'Receiver)                      #|line 52|#))
    (declare (ignorable  r))
    (setf (slot-value  r 'name)  name)                      #|line 53|#
    (setf (slot-value  r 'component)  component)            #|line 54|#
    (setf (slot-value  r 'port)  port)                      #|line 55|#
    #|  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. |# #|line 56|#
    (setf (slot-value  r 'queue)  q)                        #|line 57|#
    (return-from mkReceiver  r)                             #|line 58|#) #|line 59|#
  )                                                         #|line 61|#
(defclass Component_Registry ()                             #|line 62|#
  (
    (templates :accessor templates :initarg :templates :initform  (dict-fresh))  #|line 63|#)) #|line 64|#

                                                            #|line 65|#
(defclass Template ()                                       #|line 66|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 67|#
    (container :accessor container :initarg :container :initform  nil)  #|line 68|#
    (instantiator :accessor instantiator :initarg :instantiator :initform  nil)  #|line 69|#)) #|line 70|#

                                                            #|line 71|#
(defun mkTemplate (&optional  name  template_data  instantiator)
  (declare (ignorable  name  template_data  instantiator))  #|line 72|#
  (let (( templ  (make-instance 'Template)                  #|line 73|#))
    (declare (ignorable  templ))
    (setf (slot-value  templ 'name)  name)                  #|line 74|#
    (setf (slot-value  templ 'template_data)  template_data) #|line 75|#
    (setf (slot-value  templ 'instantiator)  instantiator)  #|line 76|#
    (return-from mkTemplate  templ)                         #|line 77|#) #|line 78|#
  )
(defun make_component_registry (&optional )
  (declare (ignorable ))                                    #|line 80|#
  (return-from make_component_registry  (make-instance 'Component_Registry) #|line 81|#) #|line 82|#
  ) #|  Data for an asyncronous component _ effectively, a function with input |# #|line 84|# #|  and output queues of mevents. |# #|line 85|# #|  |# #|line 86|# #|  Components can either be a user_supplied function (“leaf“), or a “container“ |# #|line 87|# #|  that routes mevents to child components according to a list of connections |# #|line 88|# #|  that serve as a mevent routing table. |# #|line 89|# #|  |# #|line 90|# #|  Child components themselves can be leaves or other containers. |# #|line 91|# #|  |# #|line 92|# #|  `handler` invokes the code that is attached to this component. |# #|line 93|# #|  |# #|line 94|# #|  `instance_data` is a pointer to instance data that the `leaf_handler` |# #|line 95|# #|  function may want whenever it is invoked again. |# #|line 96|# #|line 97|# #|  TODO: what is .routings for? (is it a historical artefact that can be removed?)  |# #|line 98|# #|line 99|# #|  Eh_States :: enum { idle, active } |# #|line 100|#
(defclass Eh ()                                             #|line 101|#
  (
    (name :accessor name :initarg :name :initform  "")      #|line 102|#
    (inq :accessor inq :initarg :inq :initform  (make-instance 'Queue) #|line 103|#)
    (outq :accessor outq :initarg :outq :initform  (make-instance 'Queue) #|line 104|#)
    (owner :accessor owner :initarg :owner :initform  nil)  #|line 105|#
    (children :accessor children :initarg :children :initform  nil)  #|line 106|#
    (visit_ordering :accessor visit_ordering :initarg :visit_ordering :initform  (make-instance 'Queue) #|line 107|#)
    (connections :accessor connections :initarg :connections :initform  nil)  #|line 108|#
    (routings :accessor routings :initarg :routings :initform  (make-instance 'Queue) #|line 109|#)
    (handler :accessor handler :initarg :handler :initform  nil)  #|line 110|#
    (reset_instance_data :accessor reset_instance_data :initarg :reset_instance_data :initform  nil)  #|line 111|#
    (finject :accessor finject :initarg :finject :initform  nil)  #|line 112|#
    (stop :accessor stop :initarg :stop :initform  nil)     #|line 113|#
    (instance_data :accessor instance_data :initarg :instance_data :initform  nil)  #|line 114|# #|  arg needed for probe support  |# #|line 115|#
    (arg :accessor arg :initarg :arg :initform  "")         #|line 116|#
    (state :accessor state :initarg :state :initform  "idle")  #|line 117|#
    (special :accessor special :initarg :special :initform  nil)  #|line 118|# #|  bootstrap debugging |# #|line 119|#
    (kind :accessor kind :initarg :kind :initform  nil)  #|  enum { container, leaf, } |# #|line 120|#)) #|line 121|#

                                                            #|line 122|#
(defparameter  load_errors  nil)                            #|line 123|#
(defparameter  runtime_errors  nil)                         #|line 124|# #|line 125|#
(defun clone_string (&optional  s)
  (declare (ignorable  s))                                  #|line 126|#
  (return-from clone_string  s)                             #|line 127|# #|line 128|#
  )
(defun injector (&optional  eh  mevent)
  (declare (ignorable  eh  mevent))                         #|line 130|#
  (funcall (slot-value  eh 'handler)   eh  mevent           #|line 131|#) #|line 132|#
  )
