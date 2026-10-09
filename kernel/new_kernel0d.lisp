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
                                                            #|line 1|#
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
(defparameter digits (list                                  #|line 1|#  "₀"  "₁"  "₂"  "₃"  "₄"  "₅"  "₆"  "₇"  "₈"  "₉"  "₁₀"  "₁₁"  "₁₂"  "₁₃"  "₁₄"  "₁₅"  "₁₆"  "₁₇"  "₁₈"  "₁₉"  "₂₀"  "₂₁"  "₂₂"  "₂₃"  "₂₄"  "₂₅"  "₂₆"  "₂₇"  "₂₈"  "₂₉" )) #|line 7|# #|line 8|# #|line 9|#
(defun subscripted_digit (&optional  n)
  (declare (ignorable  n))                                  #|line 10|# #|line 11|#
  (cond
    (( and  ( >=   n  0) ( <=   n  29))                     #|line 12|#
      (return-from subscripted_digit (nth  n  digits))      #|line 13|#
      )
    (t                                                      #|line 14|#
      (return-from subscripted_digit  (concatenate 'string  "₊" (format nil "~a"  n)) #|line 15|#) #|line 16|#
      ))                                                    #|line 17|#
  )
(defparameter counter  0)                                   #|line 19|# #|line 20|#
(defun gensymbol (&optional  s)
  (declare (ignorable  s))                                  #|line 21|# #|line 22|#
  (let ((name_with_id  (concatenate 'string  s (funcall (quote subscripted_digit)   counter )) #|line 23|#))
    (declare (ignorable name_with_id))
    (setf  counter (+  counter  1))                         #|line 24|#
    (return-from gensymbol  name_with_id)                   #|line 25|#) #|line 26|#
  )
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
#|line 1|#
(defparameter enumDown  0)                                  #|line 2|#
(defparameter enumAcross  1)                                #|line 3|#
(defparameter enumUp  2)                                    #|line 4|#
(defparameter enumThrough  3)                               #|line 5|# #|line 6|# #|line 7|# #|  Routing connection for a container component. The `direction` field has |# #|line 8|# #|  no affect on the default mevent routing system _ it is there for debugging |# #|line 9|# #|  purposes, or for reading by other tools. |# #|line 10|# #|line 11|#
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
(defclass Component_Registry ()                             #|line 1|#
  (
    (templates :accessor templates :initarg :templates :initform  (dict-fresh))  #|line 2|#)) #|line 3|#

                                                            #|line 4|#
(defclass Template ()                                       #|line 5|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 6|#
    (container :accessor container :initarg :container :initform  nil)  #|line 7|#
    (instantiator :accessor instantiator :initarg :instantiator :initform  nil)  #|line 8|#)) #|line 9|#

                                                            #|line 10|#
(defun mkTemplate (&optional  name  template_data  instantiator)
  (declare (ignorable  name  template_data  instantiator))  #|line 11|#
  (let (( templ  (make-instance 'Template)                  #|line 12|#))
    (declare (ignorable  templ))
    (setf (slot-value  templ 'name)  name)                  #|line 13|#
    (setf (slot-value  templ 'template_data)  template_data) #|line 14|#
    (setf (slot-value  templ 'instantiator)  instantiator)  #|line 15|#
    (return-from mkTemplate  templ)                         #|line 16|#) #|line 17|#
  )                                                         #|line 19|# #|  convert a little-network to internal form (an object data structure created by json parser) ...  |# #|line 20|# #|  the actual data structure depends on the json parser library used by the target language  |# #|line 21|# #|  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  |# #|line 22|# #|line 23|# #|  ... by reading the little-net from an external file  |# #|line 24|#
(defun lnet2internal_from_file (&optional  container_xml)
  (declare (ignorable  container_xml))                      #|line 25|#
  (let ((pathname (uiop:getenv "PBPWD")                     #|line 26|#))
    (declare (ignorable pathname))
    (let ((filename  container_xml                          #|line 27|#))
      (declare (ignorable filename))

      ;; read json from a named file and convert it into internal form (a list of Container alists)
      (json2dict (merge-pathnames pathname filename))
                                                            #|line 28|#)) #|line 29|#
  ) #|  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  |# #|line 31|#
(defun lnet2internal_from_string (&optional  lnet)
  (declare (ignorable  lnet))                               #|line 32|#

  (internalize-lnet-from-JSON *lnet*)
                                                            #|line 33|# #|line 34|#
  )
(defun make_component_registry (&optional )
  (declare (ignorable ))                                    #|line 36|#
  (return-from make_component_registry  (make-instance 'Component_Registry) #|line 37|#) #|line 38|#
  )
(defun register_component (&optional  reg  template)
  (declare (ignorable  reg  template))
  (return-from register_component (funcall (quote abstracted_register_component)   reg  template  nil )) #|line 40|#
  )
(defun register_component_allow_overwriting (&optional  reg  template)
  (declare (ignorable  reg  template))
  (return-from register_component_allow_overwriting (funcall (quote abstracted_register_component)   reg  template  t )) #|line 41|#
  )
(defun abstracted_register_component (&optional  reg  template  ok_to_overwrite)
  (declare (ignorable  reg  template  ok_to_overwrite))     #|line 43|#
  (let ((name (funcall (quote mangle_name)  (slot-value  template 'name)  #|line 44|#)))
    (declare (ignorable name))
    (cond
      (( and  ( dict-in?  ( and  (not (equal   reg  nil))  name) (slot-value  reg 'templates)) (not  ok_to_overwrite)) #|line 45|#
        (funcall (quote load_error)   (concatenate 'string  "Component /"  (concatenate 'string (slot-value  template 'name)  "/ already declared"))  #|line 46|#)
        (return-from abstracted_register_component  reg)    #|line 47|#
        )
      (t                                                    #|line 48|#
        (setf (gethash name (slot-value  reg 'templates))  template) #|line 49|#
        (return-from abstracted_register_component  reg)    #|line 50|# #|line 51|#
        )))                                                 #|line 52|#
  )
(defun get_component_instance (&optional  reg  full_name  owner)
  (declare (ignorable  reg  full_name  owner))              #|line 54|#
  #|  If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it.  |# #|line 55|#
  #|  ":?<string>" is a probe part that is tagged with <string>  |# #|line 56|#
  #|  ":$ <command>" is a shell-out part that sends <command> to the operating system shell  |# #|line 57|#
  #|  ":<string>" else, it's just treated as a string part that produces <string> on its output  |# #|line 58|#
  (let ((template_name (funcall (quote mangle_name)   full_name  #|line 59|#)))
    (declare (ignorable template_name))
    (cond
      (( equal    ":"  (string (char  full_name 0)))        #|line 60|#
        (let ((instance_name (funcall (quote generate_instance_name)   owner  template_name  #|line 61|#)))
          (declare (ignorable instance_name))
          (let ((instance (funcall (quote jit_instantiate)   reg  owner  instance_name  full_name  #|line 62|#)))
            (declare (ignorable instance))
            (return-from get_component_instance  instance)  #|line 63|#))
        )
      (t                                                    #|line 64|#
        (cond
          (( dict-in?   template_name (slot-value  reg 'templates)) #|line 65|#
            (let ((template (gethash template_name (slot-value  reg 'templates))))
              (declare (ignorable template))                #|line 66|#
              (cond
                (( equal    template  nil)                  #|line 67|#
                  (funcall (quote load_error)   (concatenate 'string  "Registry Error (A): Can't find component /"  (concatenate 'string  template_name  "/"))  #|line 68|#)
                  (return-from get_component_instance  nil) #|line 69|#
                  )
                (t                                          #|line 70|#
                  (let ((instance_name (funcall (quote generate_instance_name)   owner  template_name  #|line 71|#)))
                    (declare (ignorable instance_name))
                    (let ((instance (funcall (slot-value  template 'instantiator)   reg  owner  instance_name (slot-value  template 'template_data)  ""  #|line 72|#)))
                      (declare (ignorable instance))
                      (return-from get_component_instance  instance) #|line 73|#)) #|line 74|#
                  )))
            )
          (t                                                #|line 75|#
            (funcall (quote load_error)   (concatenate 'string  "Registry Error (B): Can't find component /"  (concatenate 'string  template_name  "/"))  #|line 76|#)
            (return-from get_component_instance  nil)       #|line 77|# #|line 78|#
            ))                                              #|line 79|#
        )))                                                 #|line 80|#
  )
(defun generate_instance_name (&optional  owner  template_name)
  (declare (ignorable  owner  template_name))               #|line 82|#
  (let ((owner_name  ""))
    (declare (ignorable owner_name))                        #|line 83|#
    (let ((instance_name  template_name))
      (declare (ignorable instance_name))                   #|line 84|#
      (cond
        ((not (equal   nil  owner))                         #|line 85|#
          (setf  owner_name (slot-value  owner 'name))      #|line 86|#
          (setf  instance_name  (concatenate 'string  owner_name  (concatenate 'string  "▹"  template_name)) #|line 87|#)
          )
        (t                                                  #|line 88|#
          (setf  instance_name  template_name)              #|line 89|# #|line 90|#
          ))
      (return-from generate_instance_name  instance_name)   #|line 91|#)) #|line 92|#
  )
(defun mangle_name (&optional  s)
  (declare (ignorable  s))                                  #|line 94|#
  #|  trim name to remove code from Container component names _ deferred until later (or never) |# #|line 95|#
  (return-from mangle_name  s)                              #|line 96|# #|line 97|#
  )
(defun create_down_connector (&optional  container  proto_conn  connectors  children_by_id)
  (declare (ignorable  container  proto_conn  connectors  children_by_id)) #|line 1|#
  #|  JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''}, |# #|line 2|#
  (let (( connector  (make-instance 'Connector)             #|line 3|#))
    (declare (ignorable  connector))
    (setf (slot-value  connector 'direction)  "down")       #|line 4|#
    (setf (slot-value  connector 'sender) (funcall (quote mkSender)  (slot-value  container 'name)  container (gethash  "source_port"  proto_conn)  #|line 5|#))
    (let ((target_proto (gethash  "target"  proto_conn)))
      (declare (ignorable target_proto))                    #|line 6|#
      (let ((id_proto (gethash  "id"  target_proto)))
        (declare (ignorable id_proto))                      #|line 7|#
        (let ((target_component (gethash id_proto  children_by_id)))
          (declare (ignorable target_component))            #|line 8|#
          (cond
            (( equal    target_component  nil)              #|line 9|#
              (funcall (quote load_error)   (concatenate 'string  "internal error: .Down connection target internal error " (gethash  "name" (gethash  "target"  proto_conn))) ) #|line 10|#
              )
            (t                                              #|line 11|#
              (setf (slot-value  connector 'receiver) (funcall (quote mkReceiver)  (slot-value  target_component 'name)  target_component (gethash  "target_port"  proto_conn) (slot-value  target_component 'inq)  #|line 12|#)) #|line 13|#
              ))
          (return-from create_down_connector  connector)    #|line 14|#)))) #|line 15|#
  )
(defun create_across_connector (&optional  container  proto_conn  connectors  children_by_id)
  (declare (ignorable  container  proto_conn  connectors  children_by_id)) #|line 17|#
  (let (( connector  (make-instance 'Connector)             #|line 18|#))
    (declare (ignorable  connector))
    (setf (slot-value  connector 'direction)  "across")     #|line 19|#
    (let ((source_component (gethash (gethash  "id" (gethash  "source"  proto_conn))  children_by_id)))
      (declare (ignorable source_component))                #|line 20|#
      (let ((target_component (gethash (gethash  "id" (gethash  "target"  proto_conn))  children_by_id)))
        (declare (ignorable target_component))              #|line 21|#
        (cond
          (( equal    source_component  nil)                #|line 22|#
            (funcall (quote load_error)   (concatenate 'string  "internal error: .Across connection source not ok " (gethash  "name" (gethash  "source"  proto_conn)))  #|line 23|#)
            )
          (t                                                #|line 24|#
            (setf (slot-value  connector 'sender) (funcall (quote mkSender)  (slot-value  source_component 'name)  source_component (gethash  "source_port"  proto_conn)  #|line 25|#))
            (cond
              (( equal    target_component  nil)            #|line 26|#
                (funcall (quote load_error)   (concatenate 'string  "internal error: .Across connection target not ok " (gethash  "name" (gethash  "target"  proto_conn)))  #|line 27|#)
                )
              (t                                            #|line 28|#
                (setf (slot-value  connector 'receiver) (funcall (quote mkReceiver)  (slot-value  target_component 'name)  target_component (gethash  "target_port"  proto_conn) (slot-value  target_component 'inq)  #|line 29|#)) #|line 30|#
                ))                                          #|line 31|#
            ))
        (return-from create_across_connector  connector)    #|line 32|#))) #|line 33|#
  )
(defun create_up_connector (&optional  container  proto_conn  connectors  children_by_id)
  (declare (ignorable  container  proto_conn  connectors  children_by_id)) #|line 35|#
  (let (( connector  (make-instance 'Connector)             #|line 36|#))
    (declare (ignorable  connector))
    (setf (slot-value  connector 'direction)  "up")         #|line 37|#
    (let ((source_component (gethash (gethash  "id" (gethash  "source"  proto_conn))  children_by_id)))
      (declare (ignorable source_component))                #|line 38|#
      (cond
        (( equal    source_component  nil)                  #|line 39|#
          (funcall (quote load_error)   (concatenate 'string  "internal error: .Up connection source not ok " (gethash  "name" (gethash  "source"  proto_conn))) ) #|line 40|#
          )
        (t                                                  #|line 41|#
          (setf (slot-value  connector 'sender) (funcall (quote mkSender)  (slot-value  source_component 'name)  source_component (gethash  "source_port"  proto_conn)  #|line 42|#))
          (setf (slot-value  connector 'receiver) (funcall (quote mkReceiver)  (slot-value  container 'name)  container (gethash  "target_port"  proto_conn) (slot-value  container 'outq)  #|line 43|#)) #|line 44|#
          ))
      (return-from create_up_connector  connector)          #|line 45|#)) #|line 46|#
  )
(defun create_through_connector (&optional  container  proto_conn  connectors  children_by_id)
  (declare (ignorable  container  proto_conn  connectors  children_by_id)) #|line 48|#
  (let (( connector  (make-instance 'Connector)             #|line 49|#))
    (declare (ignorable  connector))
    (setf (slot-value  connector 'direction)  "through")    #|line 50|#
    (setf (slot-value  connector 'sender) (funcall (quote mkSender)  (slot-value  container 'name)  container (gethash  "source_port"  proto_conn)  #|line 51|#))
    (setf (slot-value  connector 'receiver) (funcall (quote mkReceiver)  (slot-value  container 'name)  container (gethash  "target_port"  proto_conn) (slot-value  container 'outq)  #|line 52|#))
    (return-from create_through_connector  connector)       #|line 53|#) #|line 54|#
  )                                                         #|line 56|#
(defun container_instantiator (&optional  reg  owner  container_name  desc  arg)
  (declare (ignorable  reg  owner  container_name  desc  arg)) #|line 57|# #|line 58|# #|line 59|# #|line 60|# #|line 61|#
  (let ((container (funcall (quote make_container)   container_name  owner  #|line 62|#)))
    (declare (ignorable container))
    (let ((children  nil))
      (declare (ignorable children))                        #|line 63|#
      (let ((children_by_id  (dict-fresh)))
        (declare (ignorable children_by_id))
        #|  not strictly necessary, but, we can remove 1 runtime lookup by "compiling it out“ here |# #|line 64|#
        #|  collect children |#                             #|line 65|#
        (loop for child_desc in (gethash  "children"  desc)
          do
            (progn
              child_desc                                    #|line 66|#
              (let ((child_instance (funcall (quote get_component_instance)   reg (gethash  "name"  child_desc)  container  #|line 67|#)))
                (declare (ignorable child_instance))
                (setf  children (append  children (list  child_instance))) #|line 68|#
                (let ((id (gethash  "id"  child_desc)))
                  (declare (ignorable id))                  #|line 69|#
                  (setf (gethash id  children_by_id)  child_instance) #|line 70|# #|line 71|#)) #|line 72|#
              ))
        (setf (slot-value  container 'children)  children)  #|line 73|# #|line 74|#
        (let ((connectors  nil))
          (declare (ignorable connectors))                  #|line 75|#
          (loop for proto_conn in (gethash  "connections"  desc)
            do
              (progn
                proto_conn                                  #|line 76|#
                (let (( connector  (make-instance 'Connector) #|line 77|#))
                  (declare (ignorable  connector))
                  (cond
                    (( equal   (gethash  "dir"  proto_conn)  enumDown) #|line 78|#
                      (setf  connectors (append  connectors (list (funcall (quote create_down_connector)   container  proto_conn  connectors  children_by_id )))) #|line 79|#
                      )
                    (( equal   (gethash  "dir"  proto_conn)  enumAcross) #|line 80|#
                      (setf  connectors (append  connectors (list (funcall (quote create_across_connector)   container  proto_conn  connectors  children_by_id )))) #|line 81|#
                      )
                    (( equal   (gethash  "dir"  proto_conn)  enumUp) #|line 82|#
                      (setf  connectors (append  connectors (list (funcall (quote create_up_connector)   container  proto_conn  connectors  children_by_id )))) #|line 83|#
                      )
                    (( equal   (gethash  "dir"  proto_conn)  enumThrough) #|line 84|#
                      (setf  connectors (append  connectors (list (funcall (quote create_through_connector)   container  proto_conn  connectors  children_by_id )))) #|line 85|# #|line 86|#
                      )))                                   #|line 87|#
                ))
          (setf (slot-value  container 'connections)  connectors) #|line 88|#
          (return-from container_instantiator  container)   #|line 89|#)))) #|line 90|#
  ) #|  The default handler for container components. |#    #|line 92|#
(defun container_handler (&optional  container  mevent)
  (declare (ignorable  container  mevent))                  #|line 93|#
  (funcall (quote route)   container  #|  from=  |# container  mevent )
  #|  references to 'self' are replaced by the container during instantiation |# #|line 94|#
  (loop while (funcall (quote any_child_ready)   container )
    do
      (progn                                                #|line 95|#
        (funcall (quote step_children)   container  mevent ) #|line 96|#
        ))                                                  #|line 97|#
  ) #|  Stop all children. Reset to a known state. Hit the big red button.  |# #|line 99|#
(defun container_reset (&optional  container)
  (declare (ignorable  container))                          #|line 100|#
  (loop for child in (slot-value  container 'children)
    do
      (progn
        child                                               #|line 101|#
        (funcall (slot-value  child 'reset)   child         #|line 102|#) #|line 103|#
        ))

  (setf (slot-value  container 'visit_ordering) (make-instance 'Queue)) #|line 104|#

  (setf (slot-value  container 'inq) (make-instance 'Queue)) #|line 105|#

  (setf (slot-value  container 'outq) (make-instance 'Queue)) #|line 106|#
  (setf (slot-value  container 'state)  "idle")             #|line 107|# #|line 108|#
  ) #|  Frees the given container and associated data. |#   #|line 110|#
(defun destroy_container (&optional  eh)
  (declare (ignorable  eh))                                 #|line 111|#
  #| pass |#                                                #|line 112|# #|line 113|#
  ) #|  Checks if two senders match, by pointer equality and port name matching. |# #|line 114|#
(defun sender_eq (&optional  s1  s2)
  (declare (ignorable  s1  s2))                             #|line 115|#
  (let ((same_components ( equal   (slot-value  s1 'component) (slot-value  s2 'component))))
    (declare (ignorable same_components))                   #|line 116|#
    (let ((same_ports ( equal   (slot-value  s1 'port) (slot-value  s2 'port))))
      (declare (ignorable same_ports))                      #|line 117|#
      (return-from sender_eq ( and   same_components  same_ports)) #|line 118|#)) #|line 119|#
  ) #|  Delivers the given mevent to the receiver of this connector. |# #|line 121|# #|line 122|#
(defun deposit (&optional  parent  conn  mevent)
  (declare (ignorable  parent  conn  mevent))               #|line 123|#
  (let ((new_mevent (funcall (quote make_mevent)  (slot-value (slot-value  conn 'receiver) 'port) (slot-value  mevent 'payload)  #|line 124|#)))
    (declare (ignorable new_mevent))
    (funcall (quote push_mevent)   parent (slot-value (slot-value  conn 'receiver) 'component) (slot-value (slot-value  conn 'receiver) 'queue)  new_mevent  #|line 125|#)) #|line 126|#
  )
(defun force_tick (&optional  parent  eh)
  (declare (ignorable  parent  eh))                         #|line 128|#
  (let ((tick_mev (funcall (quote make_mevent)   "." (funcall (quote new_datum_bang) )  #|line 129|#)))
    (declare (ignorable tick_mev))
    (funcall (quote push_mevent)   parent  eh (slot-value  eh 'inq)  tick_mev  #|line 130|#)
    (return-from force_tick  tick_mev)                      #|line 131|#) #|line 132|#
  )
(defun push_mevent (&optional  parent  receiver  inq  m)
  (declare (ignorable  parent  receiver  inq  m))           #|line 134|#
  (enqueue  inq  m)                                         #|line 135|#
  (cond
    ((slot-value  receiver 'special)                        #|line 136|#
      (prequeue (slot-value  parent 'visit_ordering)  receiver) #|line 137|#
      )
    (t                                                      #|line 138|#
      (enqueue (slot-value  parent 'visit_ordering)  receiver) #|line 139|# #|line 140|#
      ))                                                    #|line 141|# #|line 142|#
  )
(defun is_self (&optional  child  container)
  (declare (ignorable  child  container))                   #|line 144|#
  #|  in an earlier version “self“ was denoted as ϕ |#      #|line 145|#
  (return-from is_self ( equal    child  container))        #|line 146|# #|line 147|#
  )
(defun step_child_once (&optional  child  mev)
  (declare (ignorable  child  mev))                         #|line 149|#
  (cond
    ( (not (null (uiop:getenv "PBPSTEPPING")))              #|line 150|#
      (format *error-output* "~a~%"  (concatenate 'string  "-- stepping ❮"  (concatenate 'string (slot-value  child 'name)  "❯"))) #|line 151|#
      (format *error-output* "
      ")                                                    #|line 152|# #|line 153|#
      ))
  (funcall (slot-value  child 'handler)   child  mev        #|line 154|#) #|line 155|#
  )
(defun step_children (&optional  container  causingMevent)
  (declare (ignorable  container  causingMevent))           #|line 157|#
  (setf (slot-value  container 'state)  "idle")             #|line 158|# #|line 159|#
  #|  phase 1 - loop through children and process inputs or children that not "idle"  |# #|line 160|#
  (loop for child in (queue2list (slot-value  container 'visit_ordering))
    do
      (progn
        child                                               #|line 161|#
        #|  child = container represents self, skip it |#   #|line 162|#
        (cond
          ((not (funcall (quote is_self)   child  container )) #|line 163|#
            (cond
              ((not (empty? (slot-value  child 'inq)))      #|line 164|#
                (let ((mev (dequeue (slot-value  child 'inq)) #|line 165|#))
                  (declare (ignorable mev))
                  (funcall (quote step_child_once)   child  mev  #|line 166|#) #|line 167|#
                  (funcall (quote destroy_mevent)   mev     #|line 168|#))
                )
              (t                                            #|line 169|#
                (cond
                  (( equal   (slot-value  child 'state)  "idle") #|line 170|#
                    #| pass |#                              #|line 171|#
                    )
                  (t                                        #|line 172|#
                    (let ((mev (funcall (quote force_tick)   container  child  #|line 173|#)))
                      (declare (ignorable mev))
                      (funcall (quote step_child_once)   child  mev  #|line 174|#)
                      (funcall (quote destroy_mevent)   mev  #|line 175|#)) #|line 176|#
                    ))                                      #|line 177|#
                ))                                          #|line 178|#
            ))                                              #|line 179|#
        ))

  (setf (slot-value  container 'visit_ordering) (make-instance 'Queue)) #|line 180|# #|line 181|#
  #|  phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections  |# #|line 182|#
  (loop for child in (slot-value  container 'children)
    do
      (progn
        child                                               #|line 183|#
        (cond
          (( equal   (slot-value  child 'state)  "active")  #|line 184|#
            #|  if child remains active, then the container must remain active and must propagate “ticks“ to child |# #|line 185|#
            (setf (slot-value  container 'state)  "active") #|line 186|# #|line 187|#
            ))                                              #|line 188|#
        (loop while (not (empty? (slot-value  child 'outq)))
          do
            (progn                                          #|line 189|#
              (let ((mev (dequeue (slot-value  child 'outq)) #|line 190|#))
                (declare (ignorable mev))
                (funcall (quote route)   container  child  mev  #|line 191|#)
                (funcall (quote destroy_mevent)   mev       #|line 192|#)) #|line 193|#
              ))                                            #|line 194|#
        ))                                                  #|line 195|#
  )
(defun attempt_tick (&optional  parent  eh)
  (declare (ignorable  parent  eh))                         #|line 197|#
  (cond
    ((not (equal  (slot-value  eh 'state)  "idle"))         #|line 198|#
      (funcall (quote force_tick)   parent  eh              #|line 199|#) #|line 200|#
      ))                                                    #|line 201|#
  )
(defun is_tick (&optional  mev)
  (declare (ignorable  mev))                                #|line 203|#
  (return-from is_tick ( equal    "." (slot-value  mev 'port))
    #|  assume that any mevent that is sent to port "." is a tick  |# #|line 204|#) #|line 205|#
  ) #|  Routes a single mevent to all matching destinations, according to |# #|line 207|# #|  the container's connection network. |# #|line 208|# #|line 209|#
(defun route (&optional  container  from_component  mevent)
  (declare (ignorable  container  from_component  mevent))  #|line 210|#
  (let (( was_sent  nil))
    (declare (ignorable  was_sent))
    #|  for checking that output went somewhere (at least during bootstrap) |# #|line 211|#
    (let (( fromname  ""))
      (declare (ignorable  fromname))                       #|line 212|# #|line 213|#
      (setf  ticktime (+  ticktime  1))                     #|line 214|#
      (cond
        ((funcall (quote is_tick)   mevent )                #|line 215|#
          (loop for child in (slot-value  container 'children)
            do
              (progn
                child                                       #|line 216|#
                (funcall (quote attempt_tick)   container  child ) #|line 217|#
                ))
          (setf  was_sent  t)                               #|line 218|#
          )
        (t                                                  #|line 219|#
          (cond
            ((not (funcall (quote is_self)   from_component  container )) #|line 220|#
              (setf  fromname (slot-value  from_component 'name)) #|line 221|# #|line 222|#
              ))
          (let ((from_sender (funcall (quote mkSender)   fromname  from_component (slot-value  mevent 'port)  #|line 223|#)))
            (declare (ignorable from_sender))               #|line 224|#
            (loop for connector in (slot-value  container 'connections)
              do
                (progn
                  connector                                 #|line 225|#
                  (cond
                    ((funcall (quote sender_eq)   from_sender (slot-value  connector 'sender) ) #|line 226|#
                      (funcall (quote deposit)   container  connector  mevent  #|line 227|#)
                      (setf  was_sent  t)                   #|line 228|# #|line 229|#
                      ))                                    #|line 230|#
                  )))                                       #|line 231|#
          ))
      (cond
        ((not  was_sent)                                    #|line 232|#
          (live_update  "internal error"  (concatenate 'string (slot-value  container 'name)  (concatenate 'string  ": mevent on port '"  (concatenate 'string (slot-value  mevent 'port)  (concatenate 'string  "' from "  (concatenate 'string  fromname  " dropped on floor...")))))) #|line 233|# #|line 234|#
          ))))                                              #|line 235|#
  )
(defun any_child_ready (&optional  container)
  (declare (ignorable  container))                          #|line 237|#
  (loop for child in (slot-value  container 'children)
    do
      (progn
        child                                               #|line 238|#
        (cond
          ((funcall (quote child_is_ready)   child )        #|line 239|#
            (return-from any_child_ready  t)                #|line 240|# #|line 241|#
            ))                                              #|line 242|#
        ))
  (return-from any_child_ready  nil)                        #|line 243|# #|line 244|#
  )
(defun child_is_ready (&optional  eh)
  (declare (ignorable  eh))                                 #|line 246|#
  (return-from child_is_ready ( or  ( or  ( or  (not (empty? (slot-value  eh 'outq))) (not (empty? (slot-value  eh 'inq)))) (not (equal  (slot-value  eh 'state)  "idle"))) (funcall (quote any_child_ready)   eh ))) #|line 247|# #|line 248|#
  )                                                         #|line 250|# #|  Creates a component that acts as a container. It is the same as a `Eh` instance |# #|line 251|# #|  whose handler function is `container_handler`. |# #|line 252|#
(defun make_container (&optional  name  owner)
  (declare (ignorable  name  owner))                        #|line 253|#
  (let (( eh  (make-instance 'Eh)                           #|line 254|#))
    (declare (ignorable  eh))
    (setf (slot-value  eh 'name)  name)                     #|line 255|#
    (setf (slot-value  eh 'owner)  owner)                   #|line 256|#
    (setf (slot-value  eh 'handler)  #'container_handler)   #|line 257|#
    (setf (slot-value  eh 'finject)  #'injector)            #|line 258|#
    (setf (slot-value  eh 'reset)  #'container_reset)       #|line 259|#
    (setf (slot-value  eh 'state)  "idle")                  #|line 260|#
    (setf (slot-value  eh 'kind)  "container")              #|line 261|#
    (return-from make_container  eh)                        #|line 262|#) #|line 263|#
  ) #|  Sends a mevent on the given `port` with `data`, placing it on the output |# #|line 265|# #|  of the given component. |# #|line 266|# #|line 267|#
(defun send (&optional  eh  port  obj  causingMevent)
  (declare (ignorable  eh  port  obj  causingMevent))       #|line 268|#
  (let (( d  (make-instance 'Datum)                         #|line 269|#))
    (declare (ignorable  d))
    (setf (slot-value  d 'v)  obj)                          #|line 270|#
    (setf (slot-value  d 'clone)  #'obj_clone)              #|line 271|#
    (setf (slot-value  d 'reclaim)  nil)                    #|line 272|#
    (let ((mev (funcall (quote make_mevent)   port  d       #|line 273|#)))
      (declare (ignorable mev))
      (funcall (quote put_output)   eh  mev                 #|line 274|#))) #|line 275|#
  )
(defun forward (&optional  eh  port  mev)
  (declare (ignorable  eh  port  mev))                      #|line 277|#
  (let ((fwdmev (funcall (quote make_mevent)   port (slot-value  mev 'payload)  #|line 278|#)))
    (declare (ignorable fwdmev))
    (funcall (quote put_output)   eh  fwdmev                #|line 279|#)) #|line 280|#
  )
(defun inject_mevent (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 282|#
  (funcall (slot-value  eh 'finject)   eh  mev              #|line 283|#) #|line 284|#
  )
(defun set_active (&optional  eh)
  (declare (ignorable  eh))                                 #|line 286|#
  (setf (slot-value  eh 'state)  "active")                  #|line 287|# #|line 288|#
  )
(defun set_idle (&optional  eh)
  (declare (ignorable  eh))                                 #|line 290|#
  (setf (slot-value  eh 'state)  "idle")                    #|line 291|# #|line 292|#
  )
(defun put_output (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 294|#
  (enqueue (slot-value  eh 'outq)  mev)                     #|line 295|# #|line 296|#
  )
(defun obj_clone (&optional  obj)
  (declare (ignorable  obj))                                #|line 298|#
  (return-from obj_clone  obj)                              #|line 299|# #|line 300|#
  )
#|  Creates a new leaf component out of a handler function, and a data parameter |# #|line 1|# #|  that will be passed back to your handler when called. |# #|line 2|# #|line 3|#
(defun make_leaf (&optional  name  owner  instance_data  arg  handler  reset_handler)
  (declare (ignorable  name  owner  instance_data  arg  handler  reset_handler)) #|line 4|#
  (let (( eh  (make-instance 'Eh)                           #|line 5|#))
    (declare (ignorable  eh))
    (let (( nm  ""))
      (declare (ignorable  nm))                             #|line 6|#
      (cond
        ((not (equal   nil  owner))                         #|line 7|#
          (setf  nm (slot-value  owner 'name))              #|line 8|# #|line 9|#
          ))
      (setf (slot-value  eh 'name)  (concatenate 'string  nm  (concatenate 'string  "▹"  name)) #|line 10|#)
      (setf (slot-value  eh 'owner)  owner)                 #|line 11|#
      (setf (slot-value  eh 'handler)  handler)             #|line 12|#
      (setf (slot-value  eh 'reset_handler)  reset_handler) #|line 13|#
      (setf (slot-value  eh 'finject)  #'injector)          #|line 14|#
      (setf (slot-value  eh 'reset)  #'leaf_reset)          #|line 15|#
      (setf (slot-value  eh 'instance_data)  instance_data) #|line 16|#
      (setf (slot-value  eh 'arg)  arg)                     #|line 17|#
      (setf (slot-value  eh 'state)  "idle")                #|line 18|#
      (return-from make_leaf  eh)                           #|line 19|#)) #|line 20|#
  ) #|  Reset Leaf part to a known, idle state. Hit the big red button.  |# #|line 22|#
(defun leaf_reset (&optional  part)
  (declare (ignorable  part))                               #|line 23|#

  (setf (slot-value  part 'inq) (make-instance 'Queue))     #|line 24|#

  (setf (slot-value  part 'outq) (make-instance 'Queue))    #|line 25|#
  (cond
    ((not (equal  (slot-value  part 'reset_handler)  nil))  #|line 26|#
      (funcall (slot-value  part 'reset_handler)   part     #|line 27|#) #|line 28|#
      ))
  (setf (slot-value  part 'state)  "idle")                  #|line 29|# #|line 30|#
  )
#|  (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way.  |# #|line 1|# #|line 2|#
(defun jit_instantiate (&optional  reg  owner  name  arg)
  (declare (ignorable  reg  owner  name  arg))              #|line 3|#
  (let ((name_with_id (funcall (quote gensymbol)   name     #|line 4|#)))
    (declare (ignorable name_with_id))
    (let (( inst (funcall (quote make_leaf)   name_with_id  owner  nil  arg  #'handle_jit  nil  #|line 5|#)))
      (declare (ignorable  inst))
      (let (( firstc (nth  1  name)))
        (declare (ignorable  firstc))                       #|line 6|#
        (cond
          ((not (equal   firstc  "$"))                      #|line 7|#
            #|  probes get to go to the front of the line  |# #|line 8|#
            (setf (slot-value  inst 'special)  t)           #|line 9|# #|line 10|#
            ))
        (return-from jit_instantiate  inst)                 #|line 11|#))) #|line 12|#
  )
(defun handle_jit (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 14|#
  (let ((s (slot-value  eh 'arg)))
    (declare (ignorable s))                                 #|line 15|#
    (let (( firstc (nth  1  s)))
      (declare (ignorable  firstc))                         #|line 16|#
      (cond
        (( equal    firstc  "$")                            #|line 17|#
          (funcall (quote shell_out_handler)   eh  (subseq  (subseq  (subseq  s 1) 1) 1)  mev  #|line 18|#)
          )
        (( equal    firstc  "?")                            #|line 19|#
          (funcall (quote probe_handler)   eh  (subseq  s 1)  mev  #|line 20|#)
          )
        (t                                                  #|line 21|#
          #|  just a string, send it out  |#                #|line 22|#
          (funcall (quote send)   eh  ""  (subseq  s 1)  mev  #|line 23|#) #|line 24|#
          ))))                                              #|line 25|#
  )
(defun probe_handler (&optional  eh  tag  mev)
  (declare (ignorable  eh  tag  mev))                       #|line 27|# #|line 28|#
  (let ((s (slot-value (slot-value  mev 'payload) 'v)))
    (declare (ignorable s))                                 #|line 29|#
    (live_update  "Info"  (concatenate 'string  "  @"  (concatenate 'string (format nil "~a"  ticktime)  (concatenate 'string  "  "  (concatenate 'string  "probe "  (concatenate 'string (slot-value  eh 'name)  (concatenate 'string  ": " (format nil "~a"  s)))))))) #|line 37|#) #|line 38|#
  )
(defun shell_out_handler (&optional  eh  cmd  mev)
  (declare (ignorable  eh  cmd  mev))                       #|line 40|#
  (let ((s (slot-value (slot-value  mev 'payload) 'v)))
    (declare (ignorable s))                                 #|line 41|#
    (let (( ret  nil))
      (declare (ignorable  ret))                            #|line 42|#
      (let (( rc  nil))
        (declare (ignorable  rc))                           #|line 43|#
        (let (( stdout  nil))
          (declare (ignorable  stdout))                     #|line 44|#
          (let (( stderr  nil))
            (declare (ignorable  stderr))                   #|line 45|#
            (let (( command  cmd))
              (declare (ignorable  command))                #|line 46|#
              (let (( pbpRoot (uiop:getenv "PBP")           #|line 47|#))
                (declare (ignorable  pbpRoot))
                (cond
                  ((not (equal   pbpRoot  ""))              #|line 48|#
                    (setf  command (substitute  "_/"  (concatenate 'string  pbpRoot  "/")  command) #|line 51|#) #|line 52|#
                    ))
                (cond
                  ( (not (null (uiop:getenv "PBPSHELLOUT")))  #|line 53|#
                    (format *error-output* "~a~%"  (concatenate 'string  "- --- shell-out: "  command)) #|line 54|#
                    (format *error-output* "
                    ")                                      #|line 55|# #|line 56|#
                    ))
                (multiple-value-setq (stdout stderr rc) (uiop::run-program (concatenate 'string  command " "  s) :output :string :error :string)) #|line 57|#
                (cond
                  (( equal    rc  0)                        #|line 58|#
                    (funcall (quote send)   eh  ""  (concatenate 'string  stdout  stderr)  mev  #|line 59|#)
                    )
                  (t                                        #|line 60|#
                    (funcall (quote send)   eh  "✗"  (concatenate 'string  stdout  stderr)  mev  #|line 61|#) #|line 62|#
                    )))))))))                               #|line 63|#
  )
(defun clone_string (&optional  s)
  (declare (ignorable  s))                                  #|line 1|#
  (return-from clone_string  s)                             #|line 2|# #|line 3|#
  )                                                         #|line 5|#
(defun trash_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 6|#
  (let ((name_with_id (funcall (quote gensymbol)   "trash"  #|line 7|#)))
    (declare (ignorable name_with_id))
    (return-from trash_instantiate (funcall (quote make_leaf)   name_with_id  owner  nil  ""  #'trash_handler  nil  #|line 8|#))) #|line 9|#
  )
(defun trash_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 11|#
  #|  to appease dumped_on_floor checker |#                 #|line 12|#
  #| pass |#                                                #|line 13|# #|line 14|#
  )
(defclass TwoMevents ()                                     #|line 15|#
  (
    (firstmev :accessor firstmev :initarg :firstmev :initform  nil)  #|line 16|#
    (secondmev :accessor secondmev :initarg :secondmev :initform  nil)  #|line 17|#)) #|line 18|#

                                                            #|line 19|# #|  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } |# #|line 20|#
(defclass Deracer_Instance_Data ()                          #|line 21|#
  (
    (state :accessor state :initarg :state :initform  nil)  #|line 22|#
    (buffer :accessor buffer :initarg :buffer :initform  nil)  #|line 23|#)) #|line 24|#

                                                            #|line 25|#
(defun reclaim_Buffers_from_heap (&optional  inst)
  (declare (ignorable  inst))                               #|line 26|#
  #| pass |#                                                #|line 27|# #|line 28|#
  )
(defun deracer_reset_handler (&optional  eh)
  (declare (ignorable  eh))                                 #|line 30|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 31|#
    (setf (slot-value  inst 'state)  "idle")                #|line 32|#
    (setf (slot-value  inst 'buffer)  (make-instance 'TwoMevents) #|line 33|#)) #|line 34|#
  )
(defun deracer_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 36|#
  (let ((name_with_id (funcall (quote gensymbol)   "deracer"  #|line 37|#)))
    (declare (ignorable name_with_id))
    (let (( inst  (make-instance 'Deracer_Instance_Data)    #|line 38|#))
      (declare (ignorable  inst))
      (setf (slot-value  inst 'state)  "idle")              #|line 39|#
      (setf (slot-value  inst 'buffer)  (make-instance 'TwoMevents) #|line 40|#)
      (let ((eh (funcall (quote make_leaf)   name_with_id  owner  inst  ""  #'deracer_handler  #'deracer_reset_handler  #|line 41|#)))
        (declare (ignorable eh))
        (return-from deracer_instantiate  eh)               #|line 42|#))) #|line 43|#
  )
(defun send_firstmev_then_secondmev (&optional  eh  inst)
  (declare (ignorable  eh  inst))                           #|line 45|#
  (funcall (quote forward)   eh  "1" (slot-value (slot-value  inst 'buffer) 'firstmev)  #|line 46|#)
  (funcall (quote forward)   eh  "2" (slot-value (slot-value  inst 'buffer) 'secondmev)  #|line 47|#)
  (funcall (quote reclaim_Buffers_from_heap)   inst         #|line 48|#) #|line 49|#
  )
(defun deracer_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 51|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 52|#
    (cond
      (( equal   (slot-value  inst 'state)  "idle")         #|line 53|#
        (cond
          (( equal    "1" (slot-value  mev 'port))          #|line 54|#
            (setf (slot-value (slot-value  inst 'buffer) 'firstmev)  mev) #|line 55|#
            (setf (slot-value  inst 'state)  "waitingForSecondmev") #|line 56|#
            )
          (( equal    "2" (slot-value  mev 'port))          #|line 57|#
            (setf (slot-value (slot-value  inst 'buffer) 'secondmev)  mev) #|line 58|#
            (setf (slot-value  inst 'state)  "waitingForFirstmev") #|line 59|#
            )
          (t                                                #|line 60|#
            (funcall (quote runtime_error)   (concatenate 'string  "bad mev.port (case A) for deracer " (slot-value  mev 'port))  #|line 61|#) #|line 62|#
            ))
        )
      (( equal   (slot-value  inst 'state)  "waitingForFirstmev") #|line 63|#
        (cond
          (( equal    "1" (slot-value  mev 'port))          #|line 64|#
            (setf (slot-value (slot-value  inst 'buffer) 'firstmev)  mev) #|line 65|#
            (funcall (quote send_firstmev_then_secondmev)   eh  inst  #|line 66|#)
            (setf (slot-value  inst 'state)  "idle")        #|line 67|#
            )
          (t                                                #|line 68|#
            (funcall (quote runtime_error)   (concatenate 'string  "deracer: waiting for 1 but got ["  (concatenate 'string (slot-value  mev 'port)  "] (case B)"))  #|line 69|#) #|line 70|#
            ))
        )
      (( equal   (slot-value  inst 'state)  "waitingForSecondmev") #|line 71|#
        (cond
          (( equal    "2" (slot-value  mev 'port))          #|line 72|#
            (setf (slot-value (slot-value  inst 'buffer) 'secondmev)  mev) #|line 73|#
            (funcall (quote send_firstmev_then_secondmev)   eh  inst  #|line 74|#)
            (setf (slot-value  inst 'state)  "idle")        #|line 75|#
            )
          (t                                                #|line 76|#
            (funcall (quote runtime_error)   (concatenate 'string  "deracer: waiting for 2 but got ["  (concatenate 'string (slot-value  mev 'port)  "] (case C)"))  #|line 77|#) #|line 78|#
            ))
        )
      (t                                                    #|line 79|#
        (funcall (quote runtime_error)   "bad state for deracer {eh.state}"  #|line 80|#) #|line 81|#
        )))                                                 #|line 82|#
  )
(defun low_level_read_text_file_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 84|#
  (let ((name_with_id (funcall (quote gensymbol)   "Low Level Read Text File"  #|line 85|#)))
    (declare (ignorable name_with_id))
    (return-from low_level_read_text_file_instantiate (funcall (quote make_leaf)   name_with_id  owner  nil  ""  #'low_level_read_text_file_handler  nil  #|line 86|#))) #|line 87|#
  )
(defun low_level_read_text_file_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 89|#
  (let ((fname (slot-value (slot-value  mev 'payload) 'v)))
    (declare (ignorable fname))                             #|line 90|#

    ;; read text from a named file fname, send the text out on port "" else send error info on port "✗"
    ;; given eh and mev if needed
    (handler-bind ((error #'(lambda (condition) (send_string eh "✗" (format nil "~&~A~&" condition)))))
      (with-open-file (stream fname)
        (let ((contents (make-string (file-length stream))))
          (read-sequence contents stream)
          (send_string eh "" contents))))
                                                            #|line 91|#) #|line 92|#
  )
(defun ensure_string_datum_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 94|#
  (let ((name_with_id (funcall (quote gensymbol)   "Ensure String Datum"  #|line 95|#)))
    (declare (ignorable name_with_id))
    (return-from ensure_string_datum_instantiate (funcall (quote make_leaf)   name_with_id  owner  nil  ""  #'ensure_string_datum_handler  nil  #|line 96|#))) #|line 97|#
  )
(defun ensure_string_datum_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 99|#
  (cond
    (( equal    "string" (funcall (slot-value (slot-value  mev 'payload) 'kind) )) #|line 100|#
      (funcall (quote forward)   eh  ""  mev                #|line 101|#)
      )
    (t                                                      #|line 102|#
      (let ((emev  (concatenate 'string  "*** ensure: type error (expected a string payload) but got " (slot-value  mev 'payload)) #|line 103|#))
        (declare (ignorable emev))
        (funcall (quote send)   eh  "✗"  emev  mev          #|line 104|#)) #|line 105|#
      ))                                                    #|line 106|#
  )
(defclass Syncfilewrite_Data ()                             #|line 108|#
  (
    (filename :accessor filename :initarg :filename :initform  "")  #|line 109|#)) #|line 110|#

                                                            #|line 111|#
(defun syncfilewrite_reset_handler (&optional  eh)
  (declare (ignorable  eh))                                 #|line 112|#
  (setf (slot-value  eh 'instance_data)  (make-instance 'Syncfilewrite_Data) #|line 113|#) #|line 114|#
  ) #|  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) |# #|line 116|#
(defun syncfilewrite_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 117|#
  (let ((name_with_id (funcall (quote gensymbol)   "syncfilewrite"  #|line 118|#)))
    (declare (ignorable name_with_id))
    (let ((inst  (make-instance 'Syncfilewrite_Data)        #|line 119|#))
      (declare (ignorable inst))
      (return-from syncfilewrite_instantiate (funcall (quote make_leaf)   name_with_id  owner  inst  ""  #'syncfilewrite_handler  #'syncfilewrite_reset_handler  #|line 120|#)))) #|line 121|#
  )
(defun syncfilewrite_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 123|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 124|#
    (cond
      (( equal    "filename" (slot-value  mev 'port))       #|line 125|#
        (setf (slot-value  inst 'filename) (slot-value (slot-value  mev 'payload) 'v)) #|line 126|#
        )
      (( equal    "input" (slot-value  mev 'port))          #|line 127|#
        (let ((contents (slot-value (slot-value  mev 'payload) 'v)))
          (declare (ignorable contents))                    #|line 128|#
          (let (( f (funcall (quote open)  (slot-value  inst 'filename)  "w"  #|line 129|#)))
            (declare (ignorable  f))
            (cond
              ((not (equal   f  nil))                       #|line 130|#
                (funcall (slot-value  f 'write)  (slot-value (slot-value  mev 'payload) 'v)  #|line 131|#)
                (funcall (slot-value  f 'close) )           #|line 132|#
                (funcall (quote send)   eh  "done" (funcall (quote new_datum_bang) )  mev  #|line 133|#)
                )
              (t                                            #|line 134|#
                (funcall (quote send)   eh  "✗"  (concatenate 'string  "open error on file " (slot-value  inst 'filename))  mev  #|line 135|#) #|line 136|#
                ))))                                        #|line 137|#
        )))                                                 #|line 138|#
  )
(defclass StringConcat_Instance_Data ()                     #|line 140|#
  (
    (buffer1 :accessor buffer1 :initarg :buffer1 :initform  nil)  #|line 141|#
    (buffer2 :accessor buffer2 :initarg :buffer2 :initform  nil)  #|line 142|#)) #|line 143|#

                                                            #|line 144|#
(defun stringconcat_reset_handler (&optional  eh)
  (declare (ignorable  eh))                                 #|line 145|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 146|#
    (setf (slot-value  inst 'buffer1)  nil)                 #|line 147|#
    (setf (slot-value  inst 'buffer2)  nil)                 #|line 148|#) #|line 149|#
  )
(defun stringconcat_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 151|#
  (let ((name_with_id (funcall (quote gensymbol)   "stringconcat"  #|line 152|#)))
    (declare (ignorable name_with_id))
    (let ((instp  (make-instance 'StringConcat_Instance_Data) #|line 153|#))
      (declare (ignorable instp))
      (return-from stringconcat_instantiate (funcall (quote make_leaf)   name_with_id  owner  instp  ""  #'stringconcat_handler  #'stringconcat_reset_handler  #|line 154|#)))) #|line 155|#
  )
(defun stringconcat_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 157|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 158|#
    (cond
      (( equal    "1" (slot-value  mev 'port))              #|line 159|#
        (setf (slot-value  inst 'buffer1) (funcall (quote clone_string)  (slot-value (slot-value  mev 'payload) 'v)  #|line 160|#))
        (funcall (quote maybe_stringconcat)   eh  inst  mev  #|line 161|#)
        )
      (( equal    "2" (slot-value  mev 'port))              #|line 162|#
        (setf (slot-value  inst 'buffer2) (funcall (quote clone_string)  (slot-value (slot-value  mev 'payload) 'v)  #|line 163|#))
        (funcall (quote maybe_stringconcat)   eh  inst  mev  #|line 164|#)
        )
      (( equal    "reset" (slot-value  mev 'port))          #|line 165|#
        (setf (slot-value  inst 'buffer1)  nil)             #|line 166|#
        (setf (slot-value  inst 'buffer2)  nil)             #|line 167|#
        )
      (t                                                    #|line 168|#
        (funcall (quote runtime_error)   (concatenate 'string  "bad mev.port for stringconcat: " (slot-value  mev 'port))  #|line 169|#) #|line 170|#
        )))                                                 #|line 171|#
  )
(defun maybe_stringconcat (&optional  eh  inst  mev)
  (declare (ignorable  eh  inst  mev))                      #|line 173|#
  (cond
    (( and  (not (equal  (slot-value  inst 'buffer1)  nil)) (not (equal  (slot-value  inst 'buffer2)  nil))) #|line 174|#
      (let (( concatenated_string  ""))
        (declare (ignorable  concatenated_string))          #|line 175|#
        (cond
          (( equal    0 (length (slot-value  inst 'buffer1))) #|line 176|#
            (setf  concatenated_string (slot-value  inst 'buffer2)) #|line 177|#
            )
          (( equal    0 (length (slot-value  inst 'buffer2))) #|line 178|#
            (setf  concatenated_string (slot-value  inst 'buffer1)) #|line 179|#
            )
          (t                                                #|line 180|#
            (setf  concatenated_string (+ (slot-value  inst 'buffer1) (slot-value  inst 'buffer2))) #|line 181|# #|line 182|#
            ))
        (funcall (quote send)   eh  ""  concatenated_string  mev  #|line 183|#)
        (setf (slot-value  inst 'buffer1)  nil)             #|line 184|#
        (setf (slot-value  inst 'buffer2)  nil)             #|line 185|#) #|line 186|#
      ))                                                    #|line 187|#
  ) #|  |#                                                  #|line 189|# #|line 190|#
(defparameter projectRoot  ".")                             #|line 191|# #|line 192|#
(defun string_constant_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 193|# #|line 194|#
  (let ((name_with_id (funcall (quote gensymbol)   "strconst"  #|line 195|#)))
    (declare (ignorable name_with_id))
    (let (( s  template_data))
      (declare (ignorable  s))                              #|line 196|#
      (cond
        ((not (equal   projectRoot  ""))                    #|line 197|#
          (setf  s (substitute  "_00_"  projectRoot  s)     #|line 198|#) #|line 199|#
          ))
      (return-from string_constant_instantiate (funcall (quote make_leaf)   name_with_id  owner  s  ""  #'string_constant_handler  nil  #|line 200|#)))) #|line 201|#
  )
(defun string_constant_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 203|#
  (let ((s (slot-value  eh 'instance_data)))
    (declare (ignorable s))                                 #|line 204|#
    (funcall (quote send)   eh  ""  s  mev                  #|line 205|#)) #|line 206|#
  )
(defun fakepipename_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 208|#
  (let ((instance_name (funcall (quote gensymbol)   "fakepipe"  #|line 209|#)))
    (declare (ignorable instance_name))
    (return-from fakepipename_instantiate (funcall (quote make_leaf)   instance_name  owner  nil  ""  #'fakepipename_handler  nil  #|line 210|#))) #|line 211|#
  )
(defparameter rand  0)                                      #|line 213|# #|line 214|#
(defun fakepipename_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 215|# #|line 216|#
  (setf  rand (+  rand  1))
  #|  not very random, but good enough _ ;rand' must be unique within a single run |# #|line 217|#
  (funcall (quote send)   eh  ""  (concatenate 'string  "/tmp/fakepipe"  rand)  mev  #|line 218|#) #|line 219|#
  )                                                         #|line 221|#
(defclass Switch1star_Instance_Data ()                      #|line 222|#
  (
    (state :accessor state :initarg :state :initform  "1")  #|line 223|#)) #|line 224|#

                                                            #|line 225|#
(defun switch1star_reset_handler (&optional  eh)
  (declare (ignorable  eh))                                 #|line 226|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 227|#
    (setf  inst  (make-instance 'Switch1star_Instance_Data) #|line 228|#)) #|line 229|#
  )
(defun switch1star_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 231|#
  (let ((name_with_id (funcall (quote gensymbol)   "switch1*"  #|line 232|#)))
    (declare (ignorable name_with_id))
    (let ((instp  (make-instance 'Switch1star_Instance_Data) #|line 233|#))
      (declare (ignorable instp))
      (return-from switch1star_instantiate (funcall (quote make_leaf)   name_with_id  owner  instp  ""  #'switch1star_handler  #'switch1star_reset_handler  #|line 234|#)))) #|line 235|#
  )
(defun switch1star_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 237|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 238|#
    (let ((whichOutput (slot-value  inst 'state)))
      (declare (ignorable whichOutput))                     #|line 239|#
      (cond
        (( equal    "" (slot-value  mev 'port))             #|line 240|#
          (cond
            (( equal    "1"  whichOutput)                   #|line 241|#
              (funcall (quote forward)   eh  "1"  mev       #|line 242|#)
              (setf (slot-value  inst 'state)  "*")         #|line 243|#
              )
            (( equal    "*"  whichOutput)                   #|line 244|#
              (funcall (quote forward)   eh  "*"  mev       #|line 245|#)
              )
            (t                                              #|line 246|#
              (funcall (quote send)   eh  "✗"  "internal error bad state in switch1*"  mev  #|line 247|#) #|line 248|#
              ))
          )
        (( equal    "reset" (slot-value  mev 'port))        #|line 249|#
          (setf (slot-value  inst 'state)  "1")             #|line 250|#
          )
        (t                                                  #|line 251|#
          (funcall (quote send)   eh  "✗"  "internal error bad mevent for switch1*"  mev  #|line 252|#) #|line 253|#
          ))))                                              #|line 254|#
  )
(defclass StringAccumulator ()                              #|line 256|#
  (
    (s :accessor s :initarg :s :initform  "")               #|line 257|#)) #|line 258|#

                                                            #|line 259|#
(defun strcatstar_reset_handler (&optional  eh)
  (declare (ignorable  eh))                                 #|line 260|#
  (setf (slot-value  eh 'instance_data)  (make-instance 'StringAccumulator) #|line 261|#) #|line 262|#
  )
(defun strcatstar_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 264|#
  (let ((name_with_id (funcall (quote gensymbol)   "String Concat *"  #|line 265|#)))
    (declare (ignorable name_with_id))
    (let ((instp  (make-instance 'StringAccumulator)        #|line 266|#))
      (declare (ignorable instp))
      (return-from strcatstar_instantiate (funcall (quote make_leaf)   name_with_id  owner  instp  ""  #'strcatstar_handler  #'strcatstar_reset_handler  #|line 267|#)))) #|line 268|#
  )
(defun strcatstar_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 270|#
  (let (( accum (slot-value  eh 'instance_data)))
    (declare (ignorable  accum))                            #|line 271|#
    (cond
      (( equal    "" (slot-value  mev 'port))               #|line 272|#
        (setf (slot-value  accum 's)  (concatenate 'string (slot-value  accum 's) (slot-value (slot-value  mev 'payload) 'v)) #|line 273|#)
        )
      (( equal    "fini" (slot-value  mev 'port))           #|line 274|#
        (funcall (quote send)   eh  "" (slot-value  accum 's)  mev  #|line 275|#)
        )
      (t                                                    #|line 276|#
        (funcall (quote send)   eh  "✗"  "internal error bad mevent for String Concat *"  mev  #|line 277|#) #|line 278|#
        )))                                                 #|line 279|#
  )
(defun stop_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 281|#
  (let ((name_with_id (funcall (quote gensymbol)   "Stop"   #|line 282|#)))
    (declare (ignorable name_with_id))
    (let ((inst  nil))
      (declare (ignorable inst))                            #|line 283|#
      (return-from stop_instantiate (funcall (quote make_leaf)   name_with_id  owner  inst  ""  #'stop_handler  nil  #|line 284|#)))) #|line 285|#
  )
(defun stop_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 287|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 288|#
    (let (( parent (slot-value  eh 'owner)))
      (declare (ignorable  parent))                         #|line 289|#
      (let (( s  (concatenate 'string  "   !!! stopping: '"  (concatenate 'string (slot-value  parent 'name)  "'")) #|line 290|#))
        (declare (ignorable  s))
        (format *error-output* "~a~%"  s)                   #|line 291|#
        (format *error-output* "
        ")                                                  #|line 292|#
        (funcall (slot-value  parent 'reset)   parent       #|line 293|#)
        (funcall (quote send)   eh  "" (slot-value (slot-value  mev 'payload) 'v)  mev  #|line 294|#)))) #|line 295|#
  ) #|  all of the the built_in leaves are listed here |#   #|line 297|# #|  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project |# #|line 298|# #|line 299|#
(defun initialize_stock_components (&optional  reg)
  (declare (ignorable  reg))                                #|line 300|#
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "1then2"  nil  #'deracer_instantiate )  #|line 301|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "1→2"  nil  #'deracer_instantiate )  #|line 302|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "trash"  nil  #'trash_instantiate )  #|line 303|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "🗑️"  nil  #'trash_instantiate )  #|line 304|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "🚫"  nil  #'stop_instantiate )  #|line 305|#) #|line 306|# #|line 307|#
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "Read Text File"  nil  #'low_level_read_text_file_instantiate )  #|line 308|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "Ensure String Datum"  nil  #'ensure_string_datum_instantiate )  #|line 309|#) #|line 310|#
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "syncfilewrite"  nil  #'syncfilewrite_instantiate )  #|line 311|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "String Concat"  nil  #'stringconcat_instantiate )  #|line 312|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "switch1*"  nil  #'switch1star_instantiate )  #|line 313|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "String Concat *"  nil  #'strcatstar_instantiate )  #|line 314|#)
  #|  for fakepipe |#                                       #|line 315|#
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "fakepipename"  nil  #'fakepipename_instantiate )  #|line 316|#) #|line 317|#
  )
(defparameter load_errors  nil)                             #|line 1|#
(defparameter runtime_errors  nil)                          #|line 2|#
(defparameter ticktime  0)                                  #|line 3|# #|line 4|#
(defun load_error (&optional  s)
  (declare (ignorable  s))                                  #|line 5|# #|line 6|#
  (format *error-output* "~a~%"  s)                         #|line 7|#
  (format *error-output* "
  ")                                                        #|line 8|#
  (setf  load_errors  t)                                    #|line 9|# #|line 10|#
  )
(defun runtime_error (&optional  s)
  (declare (ignorable  s))                                  #|line 12|# #|line 13|#
  (format *error-output* "~a~%"  s)                         #|line 14|#
  (break)                                                   #|line 15|#
  (setf  runtime_errors  t)                                 #|line 16|# #|line 17|#
  )                                                         #|line 19|#
(defun initialize_component_palette_from_files (&optional  diagram_source_files)
  (declare (ignorable  diagram_source_files))               #|line 20|#
  (let (( reg (funcall (quote make_component_registry) )))
    (declare (ignorable  reg))                              #|line 21|#
    (loop for diagram_source in  diagram_source_files
      do
        (progn
          diagram_source                                    #|line 22|#
          (let ((all_containers_within_single_file (funcall (quote lnet2internal_from_file)   diagram_source  #|line 23|#)))
            (declare (ignorable all_containers_within_single_file))
            (loop for container in  all_containers_within_single_file
              do
                (progn
                  container                                 #|line 24|#
                  (funcall (quote register_component)   reg (funcall (quote mkTemplate)  (gethash  "name"  container)  #| container= |# container  #| instantiator= |# #'container_instantiator )  #|line 25|#) #|line 26|#
                  )))                                       #|line 27|#
          ))
    (funcall (quote initialize_stock_components)   reg      #|line 28|#)
    (return-from initialize_component_palette_from_files  reg) #|line 29|#) #|line 30|#
  )
(defun initialize_component_palette_from_string (&optional  lnet)
  (declare (ignorable  lnet))                               #|line 32|#
  (let (( reg (funcall (quote make_component_registry) )))
    (declare (ignorable  reg))                              #|line 33|#
    (let ((all_containers (funcall (quote lnet2internal_from_string)   lnet  #|line 34|#)))
      (declare (ignorable all_containers))
      (loop for container in  all_containers
        do
          (progn
            container                                       #|line 35|#
            (funcall (quote register_component)   reg (funcall (quote mkTemplate)  (gethash  "name"  container)  #| container= |# container  #| instantiator= |# #'container_instantiator )  #|line 36|#) #|line 37|#
            ))
      (funcall (quote initialize_stock_components)   reg    #|line 38|#)
      (return-from initialize_component_palette_from_string  reg) #|line 39|#)) #|line 40|#
  )
(defun initialize_from_files (&optional  diagram_names)
  (declare (ignorable  diagram_names))                      #|line 41|#
  (let ((arg  nil))
    (declare (ignorable arg))                               #|line 42|#
    (let ((palette (funcall (quote initialize_component_palette_from_files)   diagram_names  #|line 43|#)))
      (declare (ignorable palette))
      (return-from initialize_from_files (values  palette (list   diagram_names  arg ))) #|line 44|#)) #|line 45|#
  )
(defun initialize_from_string (&optional )
  (declare (ignorable ))                                    #|line 47|#
  (let ((arg  nil))
    (declare (ignorable arg))                               #|line 48|#
    (let ((palette (funcall (quote initialize_component_palette_from_string) )))
      (declare (ignorable palette))                         #|line 49|#
      (return-from initialize_from_string (values  palette (list   nil  arg ))) #|line 50|#)) #|line 51|#
  )
(defun start (&optional  arg  part_name  palette  env)
  (declare (ignorable  arg  part_name  palette  env))       #|line 53|#
  (let ((part (funcall (quote start_bare)   part_name  palette  env  #|line 54|#)))
    (declare (ignorable part))
    (funcall (quote inject)   part  ""  arg                 #|line 55|#)
    (funcall (quote finalize)   part                        #|line 56|#)) #|line 57|#
  )
(defun start_bare (&optional  part_name  palette  env)
  (declare (ignorable  part_name  palette  env))            #|line 59|#
  (let ((diagram_names (nth  0  env)))
    (declare (ignorable diagram_names))                     #|line 60|#
    #|  get entrypoint container |#                         #|line 61|#
    (let (( part (funcall (quote get_component_instance)   palette  part_name  nil  #|line 62|#)))
      (declare (ignorable  part))
      (cond
        (( equal    nil  part)                              #|line 63|#
          (funcall (quote load_error)   (concatenate 'string  "Couldn;t find container with page name /"  (concatenate 'string  part_name  (concatenate 'string  "/ in files "  (concatenate 'string (format nil "~a"  diagram_names)  " (check tab names, or disable compression?)"))))  #|line 67|#) #|line 68|#
          ))
      (return-from start_bare  part)                        #|line 69|#)) #|line 70|#
  )
(defun inject (&optional  part  port  payload)
  (declare (ignorable  part  port  payload))                #|line 72|# #|line 73|#
  (cond
    ((not  load_errors)                                     #|line 74|#
      (let (( d  (make-instance 'Datum)                     #|line 75|#))
        (declare (ignorable  d))
        (setf (slot-value  d 'v)  payload)                  #|line 76|#
        (setf (slot-value  d 'clone)  #'(lambda (&optional )(funcall (quote obj_clone)   d  #|line 77|#)))
        (setf (slot-value  d 'reclaim)  nil)                #|line 78|#
        (let (( mev (funcall (quote make_mevent)   port  d  #|line 79|#)))
          (declare (ignorable  mev))
          (funcall (quote inject_mevent)   part  mev        #|line 80|#)))
      )
    (t                                                      #|line 81|#
      (break)                                               #|line 82|# #|line 83|#
      ))                                                    #|line 84|#
  )
(defun finalize (&optional  part)
  (declare (ignorable  part))                               #|line 86|#
  (queue-as-json-to-stdout (slot-value  part 'outq))        #|line 87|# #|line 88|#
  )
(defun new_datum_bang (&optional )
  (declare (ignorable ))                                    #|line 90|#
  (let (( d  (make-instance 'Datum)                         #|line 91|#))
    (declare (ignorable  d))
    (setf (slot-value  d 'v)  "!")                          #|line 92|#
    (setf (slot-value  d 'clone)  #'(lambda (&optional )(funcall (quote obj_clone)   d  #|line 93|#)))
    (setf (slot-value  d 'reclaim)  nil)                    #|line 94|#
    (return-from new_datum_bang  d                          #|line 95|# #|line 96|#))
  )
