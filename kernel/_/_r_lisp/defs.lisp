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
(defparameter  digits (list                                 #|line 6|#  "₀"  "₁"  "₂"  "₃"  "₄"  "₅"  "₆"  "₇"  "₈"  "₉"  "₁₀"  "₁₁"  "₁₂"  "₁₃"  "₁₄"  "₁₅"  "₁₆"  "₁₇"  "₁₈"  "₁₉"  "₂₀"  "₂₁"  "₂₂"  "₂₃"  "₂₄"  "₂₅"  "₂₆"  "₂₇"  "₂₈"  "₂₉" )) #|line 12|# #|line 13|# #|line 14|#
(defun gensymbol (&optional  s)
  (declare (ignorable  s))                                  #|line 15|# #|line 16|#
  (let ((name_with_id  (concatenate 'string  s (funcall (quote subscripted_digit)   counter )) #|line 17|#))
    (declare (ignorable name_with_id))
    (setf  counter (+  counter  1))                         #|line 18|#
    (return-from gensymbol  name_with_id)                   #|line 19|#) #|line 20|#
  )
(defun subscripted_digit (&optional  n)
  (declare (ignorable  n))                                  #|line 22|# #|line 23|#
  (cond
    (( and  ( >=   n  0) ( <=   n  29))                     #|line 24|#
      (return-from subscripted_digit (nth  n  digits))      #|line 25|#
      )
    (t                                                      #|line 26|#
      (return-from subscripted_digit  (concatenate 'string  "₊" (format nil "~a"  n)) #|line 27|#) #|line 28|#
      ))                                                    #|line 29|#
  )
(defclass Datum ()                                          #|line 31|#
  (
    (v :accessor v :initarg :v :initform  nil)              #|line 32|#
    (clone :accessor clone :initarg :clone :initform  nil)  #|line 33|#
    (reclaim :accessor reclaim :initarg :reclaim :initform  nil)  #|line 34|#
    (other :accessor other :initarg :other :initform  nil)  #|  reserved for use on per-project basis  |# #|line 35|#)) #|line 36|#

                                                            #|line 37|# #|line 38|# #|  Mevent passed to a leaf component. |# #|line 39|# #|  |# #|line 40|# #|  `port` refers to the name of the incoming or outgoing port of this component. |# #|line 41|# #|  `payload` is the data attached to this mevent. |# #|line 42|#
(defclass Mevent ()                                         #|line 43|#
  (
    (port :accessor port :initarg :port :initform  nil)     #|line 44|#
    (payload :accessor payload :initarg :payload :initform  nil)  #|line 45|#)) #|line 46|#

                                                            #|line 47|#
(defun clone_port (&optional  s)
  (declare (ignorable  s))                                  #|line 48|#
  (return-from clone_port (funcall (quote clone_string)   s  #|line 49|#)) #|line 50|#
  ) #|  Utility for making a `Mevent`. Used to safely "seed“ mevents |# #|line 52|# #|  entering the very top of a network. |# #|line 53|#
(defun make_mevent (&optional  port  datum)
  (declare (ignorable  port  datum))                        #|line 54|#
  (let ((p (funcall (quote clone_string)   port             #|line 55|#)))
    (declare (ignorable p))
    (let (( m  (make-instance 'Mevent)                      #|line 56|#))
      (declare (ignorable  m))
      (setf (slot-value  m 'port)  p)                       #|line 57|#
      (setf (slot-value  m 'payload) (funcall (slot-value  datum 'clone) )) #|line 58|#
      (return-from make_mevent  m)                          #|line 59|#)) #|line 60|#
  ) #|  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. |# #|line 62|#
(defun mevent_clone (&optional  mev)
  (declare (ignorable  mev))                                #|line 63|#
  (let (( m  (make-instance 'Mevent)                        #|line 64|#))
    (declare (ignorable  m))
    (setf (slot-value  m 'port) (funcall (quote clone_port)  (slot-value  mev 'port)  #|line 65|#))
    (setf (slot-value  m 'payload) (funcall (slot-value (slot-value  mev 'payload) 'clone) )) #|line 66|#
    (return-from mevent_clone  m)                           #|line 67|#) #|line 68|#
  ) #|  Frees a mevent. |#                                  #|line 70|#
(defun destroy_mevent (&optional  mev)
  (declare (ignorable  mev))                                #|line 71|#
  #|  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents |# #|line 72|#
  #| pass |#                                                #|line 73|# #|line 74|#
  )
(defun destroy_datum (&optional  mev)
  (declare (ignorable  mev))                                #|line 76|#
  #| pass |#                                                #|line 77|# #|line 78|#
  )
(defun destroy_port (&optional  mev)
  (declare (ignorable  mev))                                #|line 80|#
  #| pass |#                                                #|line 81|# #|line 82|#
  ) #|  |#                                                  #|line 84|#
(defun format_mevent (&optional  m)
  (declare (ignorable  m))                                  #|line 85|#
  (cond
    (( equal    m  nil)                                     #|line 86|#
      (return-from format_mevent  "{}")                     #|line 87|#
      )
    (t                                                      #|line 88|#
      (return-from format_mevent  (concatenate 'string  "{%5C”"  (concatenate 'string (slot-value  m 'port)  (concatenate 'string  "%5C”:%5C”"  (concatenate 'string (slot-value (slot-value  m 'payload) 'v)  "%5C”}")))) #|line 89|#) #|line 90|#
      ))                                                    #|line 91|#
  )
(defun format_mevent_raw (&optional  m)
  (declare (ignorable  m))                                  #|line 92|#
  (cond
    (( equal    m  nil)                                     #|line 93|#
      (return-from format_mevent_raw  "")                   #|line 94|#
      )
    (t                                                      #|line 95|#
      (return-from format_mevent_raw (slot-value (slot-value  m 'payload) 'v)) #|line 96|# #|line 97|#
      ))                                                    #|line 98|#
  )
(defparameter  enumDown  0)
(defparameter  enumAcross  1)
(defparameter  enumUp  2)
(defparameter  enumThrough  3)                              #|line 104|# #|line 105|# #|  Routing connection for a container component. The `direction` field has |# #|line 106|# #|  no affect on the default mevent routing system _ it is there for debugging |# #|line 107|# #|  purposes, or for reading by other tools. |# #|line 108|# #|line 109|#
(defclass Connector ()                                      #|line 110|#
  (
    (direction :accessor direction :initarg :direction :initform  nil)  #|  down, across, up, through |# #|line 111|#
    (sender :accessor sender :initarg :sender :initform  nil)  #|line 112|#
    (receiver :accessor receiver :initarg :receiver :initform  nil)  #|line 113|#)) #|line 114|#

                                                            #|line 115|# #|  `Sender` is used to “pattern match“ which `Receiver` a mevent should go to, |# #|line 116|# #|  based on component ID (pointer) and port name. |# #|line 117|# #|line 118|#
(defclass Sender ()                                         #|line 119|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 120|#
    (component :accessor component :initarg :component :initform  nil)  #|line 121|#
    (port :accessor port :initarg :port :initform  nil)     #|line 122|#)) #|line 123|#

                                                            #|line 124|# #|line 125|# #|line 126|# #|  `Receiver` is a handle to a destination queue, and a `port` name to assign |# #|line 127|# #|  to incoming mevents to this queue. |# #|line 128|# #|line 129|#
(defclass Receiver ()                                       #|line 130|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 131|#
    (queue :accessor queue :initarg :queue :initform  nil)  #|line 132|#
    (port :accessor port :initarg :port :initform  nil)     #|line 133|#
    (component :accessor component :initarg :component :initform  nil)  #|line 134|#)) #|line 135|#

                                                            #|line 136|#
(defun mkSender (&optional  name  component  port)
  (declare (ignorable  name  component  port))              #|line 137|#
  (let (( s  (make-instance 'Sender)                        #|line 138|#))
    (declare (ignorable  s))
    (setf (slot-value  s 'name)  name)                      #|line 139|#
    (setf (slot-value  s 'component)  component)            #|line 140|#
    (setf (slot-value  s 'port)  port)                      #|line 141|#
    (return-from mkSender  s)                               #|line 142|#) #|line 143|#
  )
(defun mkReceiver (&optional  name  component  port  q)
  (declare (ignorable  name  component  port  q))           #|line 145|#
  (let (( r  (make-instance 'Receiver)                      #|line 146|#))
    (declare (ignorable  r))
    (setf (slot-value  r 'name)  name)                      #|line 147|#
    (setf (slot-value  r 'component)  component)            #|line 148|#
    (setf (slot-value  r 'port)  port)                      #|line 149|#
    #|  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. |# #|line 150|#
    (setf (slot-value  r 'queue)  q)                        #|line 151|#
    (return-from mkReceiver  r)                             #|line 152|#) #|line 153|#
  )                                                         #|line 155|#
(defclass Component_Registry ()                             #|line 156|#
  (
    (templates :accessor templates :initarg :templates :initform  (dict-fresh))  #|line 157|#)) #|line 158|#

                                                            #|line 159|#
(defclass Template ()                                       #|line 160|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 161|#
    (container :accessor container :initarg :container :initform  nil)  #|line 162|#
    (instantiator :accessor instantiator :initarg :instantiator :initform  nil)  #|line 163|#)) #|line 164|#

                                                            #|line 165|#
(defun mkTemplate (&optional  name  template_data  instantiator)
  (declare (ignorable  name  template_data  instantiator))  #|line 166|#
  (let (( templ  (make-instance 'Template)                  #|line 167|#))
    (declare (ignorable  templ))
    (setf (slot-value  templ 'name)  name)                  #|line 168|#
    (setf (slot-value  templ 'template_data)  template_data) #|line 169|#
    (setf (slot-value  templ 'instantiator)  instantiator)  #|line 170|#
    (return-from mkTemplate  templ)                         #|line 171|#) #|line 172|#
  )
(defun make_component_registry (&optional )
  (declare (ignorable ))                                    #|line 174|#
  (return-from make_component_registry  (make-instance 'Component_Registry) #|line 175|#) #|line 176|#
  ) #|  Data for an asyncronous component _ effectively, a function with input |# #|line 178|# #|  and output queues of mevents. |# #|line 179|# #|  |# #|line 180|# #|  Components can either be a user_supplied function (“leaf“), or a “container“ |# #|line 181|# #|  that routes mevents to child components according to a list of connections |# #|line 182|# #|  that serve as a mevent routing table. |# #|line 183|# #|  |# #|line 184|# #|  Child components themselves can be leaves or other containers. |# #|line 185|# #|  |# #|line 186|# #|  `handler` invokes the code that is attached to this component. |# #|line 187|# #|  |# #|line 188|# #|  `instance_data` is a pointer to instance data that the `leaf_handler` |# #|line 189|# #|  function may want whenever it is invoked again. |# #|line 190|# #|line 191|# #|  TODO: what is .routings for? (is it a historical artefact that can be removed?)  |# #|line 192|# #|line 193|# #|  Eh_States :: enum { idle, active } |# #|line 194|#
(defclass Eh ()                                             #|line 195|#
  (
    (name :accessor name :initarg :name :initform  "")      #|line 196|#
    (inq :accessor inq :initarg :inq :initform  (make-instance 'Queue) #|line 197|#)
    (outq :accessor outq :initarg :outq :initform  (make-instance 'Queue) #|line 198|#)
    (owner :accessor owner :initarg :owner :initform  nil)  #|line 199|#
    (children :accessor children :initarg :children :initform  nil)  #|line 200|#
    (visit_ordering :accessor visit_ordering :initarg :visit_ordering :initform  (make-instance 'Queue) #|line 201|#)
    (connections :accessor connections :initarg :connections :initform  nil)  #|line 202|#
    (routings :accessor routings :initarg :routings :initform  (make-instance 'Queue) #|line 203|#)
    (handler :accessor handler :initarg :handler :initform  nil)  #|line 204|#
    (reset_instance_data :accessor reset_instance_data :initarg :reset_instance_data :initform  nil)  #|line 205|#
    (finject :accessor finject :initarg :finject :initform  nil)  #|line 206|#
    (stop :accessor stop :initarg :stop :initform  nil)     #|line 207|#
    (instance_data :accessor instance_data :initarg :instance_data :initform  nil)  #|line 208|# #|  arg needed for probe support  |# #|line 209|#
    (arg :accessor arg :initarg :arg :initform  "")         #|line 210|#
    (state :accessor state :initarg :state :initform  "idle")  #|line 211|#
    (special :accessor special :initarg :special :initform  nil)  #|line 212|# #|  bootstrap debugging |# #|line 213|#
    (kind :accessor kind :initarg :kind :initform  nil)  #|  enum { container, leaf, } |# #|line 214|#)) #|line 215|#

                                                            #|line 216|#
(defparameter  load_errors  nil)                            #|line 217|#
(defparameter  runtime_errors  nil)                         #|line 218|# #|line 219|#
(defun clone_string (&optional  s)
  (declare (ignorable  s))                                  #|line 220|#
  (return-from clone_string  s)                             #|line 221|# #|line 222|#
  )
(defun injector (&optional  eh  mevent)
  (declare (ignorable  eh  mevent))                         #|line 224|#
  (funcall (slot-value  eh 'handler)   eh  mevent           #|line 225|#) #|line 226|#
  )
