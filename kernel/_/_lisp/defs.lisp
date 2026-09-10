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
(defclass Datum ()                                          #|line 6|#
  (
    (v :accessor v :initarg :v :initform  nil)              #|line 7|#
    (clone :accessor clone :initarg :clone :initform  nil)  #|line 8|#
    (reclaim :accessor reclaim :initarg :reclaim :initform  nil)  #|line 9|#
    (other :accessor other :initarg :other :initform  nil)  #|  reserved for use on per-project basis  |# #|line 10|#)) #|line 11|#

                                                            #|line 12|# #|line 13|# #|  Mevent passed to a leaf component. |# #|line 14|# #|  |# #|line 15|# #|  `port` refers to the name of the incoming or outgoing port of this component. |# #|line 16|# #|  `payload` is the data attached to this mevent. |# #|line 17|#
(defclass Mevent ()                                         #|line 18|#
  (
    (port :accessor port :initarg :port :initform  nil)     #|line 19|#
    (payload :accessor payload :initarg :payload :initform  nil)  #|line 20|#)) #|line 21|#

                                                            #|line 22|#
(defun clone_port (&optional  s)
  (declare (ignorable  s))                                  #|line 23|#
  (return-from clone_port (funcall (quote clone_string)   s  #|line 24|#)) #|line 25|#
  ) #|  Utility for making a `Mevent`. Used to safely "seed“ mevents |# #|line 27|# #|  entering the very top of a network. |# #|line 28|#
(defun make_mevent (&optional  port  datum)
  (declare (ignorable  port  datum))                        #|line 29|#
  (let ((p (funcall (quote clone_string)   port             #|line 30|#)))
    (declare (ignorable p))
    (let (( m  (make-instance 'Mevent)                      #|line 31|#))
      (declare (ignorable  m))
      (setf (slot-value  m 'port)  p)                       #|line 32|#
      (setf (slot-value  m 'payload) (funcall (slot-value  datum 'clone) )) #|line 33|#
      (return-from make_mevent  m)                          #|line 34|#)) #|line 35|#
  ) #|  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. |# #|line 37|#
(defun mevent_clone (&optional  mev)
  (declare (ignorable  mev))                                #|line 38|#
  (let (( m  (make-instance 'Mevent)                        #|line 39|#))
    (declare (ignorable  m))
    (setf (slot-value  m 'port) (funcall (quote clone_port)  (slot-value  mev 'port)  #|line 40|#))
    (setf (slot-value  m 'payload) (funcall (slot-value (slot-value  mev 'payload) 'clone) )) #|line 41|#
    (return-from mevent_clone  m)                           #|line 42|#) #|line 43|#
  ) #|  Frees a mevent. |#                                  #|line 45|#
(defun destroy_mevent (&optional  mev)
  (declare (ignorable  mev))                                #|line 46|#
  #|  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents |# #|line 47|#
  #| pass |#                                                #|line 48|# #|line 49|#
  )
(defun destroy_datum (&optional  mev)
  (declare (ignorable  mev))                                #|line 51|#
  #| pass |#                                                #|line 52|# #|line 53|#
  )
(defun destroy_port (&optional  mev)
  (declare (ignorable  mev))                                #|line 55|#
  #| pass |#                                                #|line 56|# #|line 57|#
  ) #|  |#                                                  #|line 59|#
(defun format_mevent (&optional  m)
  (declare (ignorable  m))                                  #|line 60|#
  (cond
    (( equal    m  nil)                                     #|line 61|#
      (return-from format_mevent  "{}")                     #|line 62|#
      )
    (t                                                      #|line 63|#
      (return-from format_mevent  (concatenate 'string  "{%5C”"  (concatenate 'string (slot-value  m 'port)  (concatenate 'string  "%5C”:%5C”"  (concatenate 'string (slot-value (slot-value  m 'payload) 'v)  "%5C”}")))) #|line 64|#) #|line 65|#
      ))                                                    #|line 66|#
  )
(defun format_mevent_raw (&optional  m)
  (declare (ignorable  m))                                  #|line 67|#
  (cond
    (( equal    m  nil)                                     #|line 68|#
      (return-from format_mevent_raw  "")                   #|line 69|#
      )
    (t                                                      #|line 70|#
      (return-from format_mevent_raw (slot-value (slot-value  m 'payload) 'v)) #|line 71|# #|line 72|#
      ))                                                    #|line 73|#
  )
(defparameter  enumDown  0)
(defparameter  enumAcross  1)
(defparameter  enumUp  2)
(defparameter  enumThrough  3)                              #|line 79|# #|line 80|# #|  Routing connection for a container component. The `direction` field has |# #|line 81|# #|  no affect on the default mevent routing system _ it is there for debugging |# #|line 82|# #|  purposes, or for reading by other tools. |# #|line 83|# #|line 84|#
(defclass Connector ()                                      #|line 85|#
  (
    (direction :accessor direction :initarg :direction :initform  nil)  #|  down, across, up, through |# #|line 86|#
    (sender :accessor sender :initarg :sender :initform  nil)  #|line 87|#
    (receiver :accessor receiver :initarg :receiver :initform  nil)  #|line 88|#)) #|line 89|#

                                                            #|line 90|# #|  `Sender` is used to “pattern match“ which `Receiver` a mevent should go to, |# #|line 91|# #|  based on component ID (pointer) and port name. |# #|line 92|# #|line 93|#
(defclass Sender ()                                         #|line 94|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 95|#
    (component :accessor component :initarg :component :initform  nil)  #|line 96|#
    (port :accessor port :initarg :port :initform  nil)     #|line 97|#)) #|line 98|#

                                                            #|line 99|# #|line 100|# #|line 101|# #|  `Receiver` is a handle to a destination queue, and a `port` name to assign |# #|line 102|# #|  to incoming mevents to this queue. |# #|line 103|# #|line 104|#
(defclass Receiver ()                                       #|line 105|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 106|#
    (queue :accessor queue :initarg :queue :initform  nil)  #|line 107|#
    (port :accessor port :initarg :port :initform  nil)     #|line 108|#
    (component :accessor component :initarg :component :initform  nil)  #|line 109|#)) #|line 110|#

                                                            #|line 111|#
(defun mkSender (&optional  name  component  port)
  (declare (ignorable  name  component  port))              #|line 112|#
  (let (( s  (make-instance 'Sender)                        #|line 113|#))
    (declare (ignorable  s))
    (setf (slot-value  s 'name)  name)                      #|line 114|#
    (setf (slot-value  s 'component)  component)            #|line 115|#
    (setf (slot-value  s 'port)  port)                      #|line 116|#
    (return-from mkSender  s)                               #|line 117|#) #|line 118|#
  )
(defun mkReceiver (&optional  name  component  port  q)
  (declare (ignorable  name  component  port  q))           #|line 120|#
  (let (( r  (make-instance 'Receiver)                      #|line 121|#))
    (declare (ignorable  r))
    (setf (slot-value  r 'name)  name)                      #|line 122|#
    (setf (slot-value  r 'component)  component)            #|line 123|#
    (setf (slot-value  r 'port)  port)                      #|line 124|#
    #|  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. |# #|line 125|#
    (setf (slot-value  r 'queue)  q)                        #|line 126|#
    (return-from mkReceiver  r)                             #|line 127|#) #|line 128|#
  )                                                         #|line 130|#
(defclass Component_Registry ()                             #|line 131|#
  (
    (templates :accessor templates :initarg :templates :initform  (dict-fresh))  #|line 132|#)) #|line 133|#

                                                            #|line 134|#
(defclass Template ()                                       #|line 135|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 136|#
    (container :accessor container :initarg :container :initform  nil)  #|line 137|#
    (instantiator :accessor instantiator :initarg :instantiator :initform  nil)  #|line 138|#)) #|line 139|#

                                                            #|line 140|#
(defun mkTemplate (&optional  name  template_data  instantiator)
  (declare (ignorable  name  template_data  instantiator))  #|line 141|#
  (let (( templ  (make-instance 'Template)                  #|line 142|#))
    (declare (ignorable  templ))
    (setf (slot-value  templ 'name)  name)                  #|line 143|#
    (setf (slot-value  templ 'template_data)  template_data) #|line 144|#
    (setf (slot-value  templ 'instantiator)  instantiator)  #|line 145|#
    (return-from mkTemplate  templ)                         #|line 146|#) #|line 147|#
  )
(defun make_component_registry (&optional )
  (declare (ignorable ))                                    #|line 149|#
  (return-from make_component_registry  (make-instance 'Component_Registry) #|line 150|#) #|line 151|#
  ) #|  Data for an asyncronous component _ effectively, a function with input |# #|line 153|# #|  and output queues of mevents. |# #|line 154|# #|  |# #|line 155|# #|  Components can either be a user_supplied function (“leaf“), or a “container“ |# #|line 156|# #|  that routes mevents to child components according to a list of connections |# #|line 157|# #|  that serve as a mevent routing table. |# #|line 158|# #|  |# #|line 159|# #|  Child components themselves can be leaves or other containers. |# #|line 160|# #|  |# #|line 161|# #|  `handler` invokes the code that is attached to this component. |# #|line 162|# #|  |# #|line 163|# #|  `instance_data` is a pointer to instance data that the `leaf_handler` |# #|line 164|# #|  function may want whenever it is invoked again. |# #|line 165|# #|line 166|# #|  TODO: what is .routings for? (is it a historical artefact that can be removed?)  |# #|line 167|# #|line 168|# #|  Eh_States :: enum { idle, active } |# #|line 169|#
(defclass Eh ()                                             #|line 170|#
  (
    (name :accessor name :initarg :name :initform  "")      #|line 171|#
    (inq :accessor inq :initarg :inq :initform  (make-instance 'Queue) #|line 172|#)
    (outq :accessor outq :initarg :outq :initform  (make-instance 'Queue) #|line 173|#)
    (owner :accessor owner :initarg :owner :initform  nil)  #|line 174|#
    (children :accessor children :initarg :children :initform  nil)  #|line 175|#
    (visit_ordering :accessor visit_ordering :initarg :visit_ordering :initform  (make-instance 'Queue) #|line 176|#)
    (connections :accessor connections :initarg :connections :initform  nil)  #|line 177|#
    (routings :accessor routings :initarg :routings :initform  (make-instance 'Queue) #|line 178|#)
    (handler :accessor handler :initarg :handler :initform  nil)  #|line 179|#
    (reset_instance_data :accessor reset_instance_data :initarg :reset_instance_data :initform  nil)  #|line 180|#
    (finject :accessor finject :initarg :finject :initform  nil)  #|line 181|#
    (stop :accessor stop :initarg :stop :initform  nil)     #|line 182|#
    (instance_data :accessor instance_data :initarg :instance_data :initform  nil)  #|line 183|# #|  arg needed for probe support  |# #|line 184|#
    (arg :accessor arg :initarg :arg :initform  "")         #|line 185|#
    (state :accessor state :initarg :state :initform  "idle")  #|line 186|#
    (special :accessor special :initarg :special :initform  nil)  #|line 187|# #|  bootstrap debugging |# #|line 188|#
    (kind :accessor kind :initarg :kind :initform  nil)  #|  enum { container, leaf, } |# #|line 189|#)) #|line 190|#

                                                            #|line 191|#
(defparameter  load_errors  nil)                            #|line 192|#
(defparameter  runtime_errors  nil)                         #|line 193|# #|line 194|#
(defun clone_string (&optional  s)
  (declare (ignorable  s))                                  #|line 195|#
  (return-from clone_string  s)                             #|line 196|# #|line 197|#
  )
(defun injector (&optional  eh  mevent)
  (declare (ignorable  eh  mevent))                         #|line 199|#
  (funcall (slot-value  eh 'handler)   eh  mevent           #|line 200|#) #|line 201|#
  )
