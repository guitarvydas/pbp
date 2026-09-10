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
(defparameter  ticktime  0)                                 #|line 4|# #|line 5|# #|line 6|#
(defclass Component_Registry ()                             #|line 7|#
  (
    (templates :accessor templates :initarg :templates :initform  (dict-fresh))  #|line 8|#)) #|line 9|#

                                                            #|line 10|#
(defclass Template ()                                       #|line 11|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 12|#
    (container :accessor container :initarg :container :initform  nil)  #|line 13|#
    (instantiator :accessor instantiator :initarg :instantiator :initform  nil)  #|line 14|#)) #|line 15|#

                                                            #|line 16|#
(defun mkTemplate (&optional  name  template_data  instantiator)
  (declare (ignorable  name  template_data  instantiator))  #|line 17|#
  (let (( templ  (make-instance 'Template)                  #|line 18|#))
    (declare (ignorable  templ))
    (setf (slot-value  templ 'name)  name)                  #|line 19|#
    (setf (slot-value  templ 'template_data)  template_data) #|line 20|#
    (setf (slot-value  templ 'instantiator)  instantiator)  #|line 21|#
    (return-from mkTemplate  templ)                         #|line 22|#) #|line 23|#
  )
(defun make_component_registry (&optional )
  (declare (ignorable ))                                    #|line 25|#
  (return-from make_component_registry  (make-instance 'Component_Registry) #|line 26|#) #|line 27|#
  ) #|  Data for an asyncronous component _ effectively, a function with input |# #|line 29|# #|  and output queues of mevents. |# #|line 30|# #|  |# #|line 31|# #|  Components can either be a user_supplied function ("leaf“), or a “container“ |# #|line 32|# #|  that routes mevents to child components according to a list of connections |# #|line 33|# #|  that serve as a mevent routing table. |# #|line 34|# #|  |# #|line 35|# #|  Child components themselves can be leaves or other containers. |# #|line 36|# #|  |# #|line 37|# #|  `handler` invokes the code that is attached to this component. |# #|line 38|# #|  |# #|line 39|# #|  `instance_data` is a pointer to instance data that the `leaf_handler` |# #|line 40|# #|  function may want whenever it is invoked again. |# #|line 41|# #|line 42|# #|  TODO: what is .routings for? (is it a historical artefact that can be removed?)  |# #|line 43|# #|line 44|# #|  Eh_States :: enum { idle, active } |# #|line 45|#
(defclass Eh ()                                             #|line 46|#
  (
    (name :accessor name :initarg :name :initform  "")      #|line 47|#
    (inq :accessor inq :initarg :inq :initform  (make-instance 'Queue) #|line 48|#)
    (outq :accessor outq :initarg :outq :initform  (make-instance 'Queue) #|line 49|#)
    (owner :accessor owner :initarg :owner :initform  nil)  #|line 50|#
    (children :accessor children :initarg :children :initform  nil)  #|line 51|#
    (visit_ordering :accessor visit_ordering :initarg :visit_ordering :initform  (make-instance 'Queue) #|line 52|#)
    (connections :accessor connections :initarg :connections :initform  nil)  #|line 53|#
    (routings :accessor routings :initarg :routings :initform  (make-instance 'Queue) #|line 54|#)
    (handler :accessor handler :initarg :handler :initform  nil)  #|line 55|#
    (reset_instance_data :accessor reset_instance_data :initarg :reset_instance_data :initform  nil)  #|line 56|#
    (finject :accessor finject :initarg :finject :initform  nil)  #|line 57|#
    (stop :accessor stop :initarg :stop :initform  nil)     #|line 58|#
    (instance_data :accessor instance_data :initarg :instance_data :initform  nil)  #|line 59|# #|  arg needed for probe support  |# #|line 60|#
    (arg :accessor arg :initarg :arg :initform  "")         #|line 61|#
    (state :accessor state :initarg :state :initform  "idle")  #|line 62|#
    (special :accessor special :initarg :special :initform  nil)  #|line 63|# #|  bootstrap debugging |# #|line 64|#
    (kind :accessor kind :initarg :kind :initform  nil)  #|  enum { container, leaf, } |# #|line 65|#)) #|line 66|#

                                                            #|line 67|#
(defparameter  load_errors  nil)                            #|line 68|#
(defparameter  runtime_errors  nil)                         #|line 69|# #|line 70|#
(defun clone_string (&optional  s)
  (declare (ignorable  s))                                  #|line 71|#
  (return-from clone_string  s)                             #|line 72|# #|line 73|#
  )
(defun injector (&optional  eh  mevent)
  (declare (ignorable  eh  mevent))                         #|line 75|#
  (funcall (slot-value  eh 'handler)   eh  mevent           #|line 76|#) #|line 77|#
  )
