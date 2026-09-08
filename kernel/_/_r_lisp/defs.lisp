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
(defparameter  enumThrough  3)                              #|line 104|# #|line 105|#
(defclass Component_Registry ()                             #|line 106|#
  (
    (templates :accessor templates :initarg :templates :initform  (dict-fresh))  #|line 107|#)) #|line 108|#

                                                            #|line 109|# #|  Routing connection for a container component. The `direction` field has |# #|line 110|# #|  no affect on the default mevent routing system _ it is there for debugging |# #|line 111|# #|  purposes, or for reading by other tools. |# #|line 112|# #|line 113|#
(defclass Connector ()                                      #|line 114|#
  (
    (direction :accessor direction :initarg :direction :initform  nil)  #|  down, across, up, through |# #|line 115|#
    (sender :accessor sender :initarg :sender :initform  nil)  #|line 116|#
    (receiver :accessor receiver :initarg :receiver :initform  nil)  #|line 117|#)) #|line 118|#

                                                            #|line 119|# #|  `Sender` is used to “pattern match“ which `Receiver` a mevent should go to, |# #|line 120|# #|  based on component ID (pointer) and port name. |# #|line 121|# #|line 122|#
(defclass Sender ()                                         #|line 123|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 124|#
    (component :accessor component :initarg :component :initform  nil)  #|line 125|#
    (port :accessor port :initarg :port :initform  nil)     #|line 126|#)) #|line 127|#

                                                            #|line 128|# #|line 129|# #|line 130|# #|  `Receiver` is a handle to a destination queue, and a `port` name to assign |# #|line 131|# #|  to incoming mevents to this queue. |# #|line 132|# #|line 133|#
(defclass Receiver ()                                       #|line 134|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 135|#
    (queue :accessor queue :initarg :queue :initform  nil)  #|line 136|#
    (port :accessor port :initarg :port :initform  nil)     #|line 137|#
    (component :accessor component :initarg :component :initform  nil)  #|line 138|#)) #|line 139|#

                                                            #|line 140|#
(defun mkSender (&optional  name  component  port)
  (declare (ignorable  name  component  port))              #|line 141|#
  (let (( s  (make-instance 'Sender)                        #|line 142|#))
    (declare (ignorable  s))
    (setf (slot-value  s 'name)  name)                      #|line 143|#
    (setf (slot-value  s 'component)  component)            #|line 144|#
    (setf (slot-value  s 'port)  port)                      #|line 145|#
    (return-from mkSender  s)                               #|line 146|#) #|line 147|#
  )
(defun mkReceiver (&optional  name  component  port  q)
  (declare (ignorable  name  component  port  q))           #|line 149|#
  (let (( r  (make-instance 'Receiver)                      #|line 150|#))
    (declare (ignorable  r))
    (setf (slot-value  r 'name)  name)                      #|line 151|#
    (setf (slot-value  r 'component)  component)            #|line 152|#
    (setf (slot-value  r 'port)  port)                      #|line 153|#
    #|  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. |# #|line 154|#
    (setf (slot-value  r 'queue)  q)                        #|line 155|#
    (return-from mkReceiver  r)                             #|line 156|#) #|line 157|#
  )                                                         #|line 159|#
(defclass Component_Registry ()                             #|line 160|#
  (
    (templates :accessor templates :initarg :templates :initform  (dict-fresh))  #|line 161|#)) #|line 162|#

                                                            #|line 163|#
(defclass Template ()                                       #|line 164|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 165|#
    (container :accessor container :initarg :container :initform  nil)  #|line 166|#
    (instantiator :accessor instantiator :initarg :instantiator :initform  nil)  #|line 167|#)) #|line 168|#

                                                            #|line 169|#
(defun mkTemplate (&optional  name  template_data  instantiator)
  (declare (ignorable  name  template_data  instantiator))  #|line 170|#
  (let (( templ  (make-instance 'Template)                  #|line 171|#))
    (declare (ignorable  templ))
    (setf (slot-value  templ 'name)  name)                  #|line 172|#
    (setf (slot-value  templ 'template_data)  template_data) #|line 173|#
    (setf (slot-value  templ 'instantiator)  instantiator)  #|line 174|#
    (return-from mkTemplate  templ)                         #|line 175|#) #|line 176|#
  )
(defun make_component_registry (&optional )
  (declare (ignorable ))                                    #|line 178|#
  (return-from make_component_registry  (make-instance 'Component_Registry) #|line 179|#) #|line 180|#
  ) #|  Data for an asyncronous component _ effectively, a function with input |# #|line 182|# #|  and output queues of mevents. |# #|line 183|# #|  |# #|line 184|# #|  Components can either be a user_supplied function (“leaf“), or a “container“ |# #|line 185|# #|  that routes mevents to child components according to a list of connections |# #|line 186|# #|  that serve as a mevent routing table. |# #|line 187|# #|  |# #|line 188|# #|  Child components themselves can be leaves or other containers. |# #|line 189|# #|  |# #|line 190|# #|  `handler` invokes the code that is attached to this component. |# #|line 191|# #|  |# #|line 192|# #|  `instance_data` is a pointer to instance data that the `leaf_handler` |# #|line 193|# #|  function may want whenever it is invoked again. |# #|line 194|# #|line 195|# #|  TODO: what is .routings for? (is it a historical artefact that can be removed?)  |# #|line 196|# #|line 197|# #|  Eh_States :: enum { idle, active } |# #|line 198|#
(defclass Eh ()                                             #|line 199|#
  (
    (name :accessor name :initarg :name :initform  "")      #|line 200|#
    (inq :accessor inq :initarg :inq :initform  (make-instance 'Queue) #|line 201|#)
    (outq :accessor outq :initarg :outq :initform  (make-instance 'Queue) #|line 202|#)
    (owner :accessor owner :initarg :owner :initform  nil)  #|line 203|#
    (children :accessor children :initarg :children :initform  nil)  #|line 204|#
    (visit_ordering :accessor visit_ordering :initarg :visit_ordering :initform  (make-instance 'Queue) #|line 205|#)
    (connections :accessor connections :initarg :connections :initform  nil)  #|line 206|#
    (routings :accessor routings :initarg :routings :initform  (make-instance 'Queue) #|line 207|#)
    (handler :accessor handler :initarg :handler :initform  nil)  #|line 208|#
    (reset_instance_data :accessor reset_instance_data :initarg :reset_instance_data :initform  nil)  #|line 209|#
    (finject :accessor finject :initarg :finject :initform  nil)  #|line 210|#
    (stop :accessor stop :initarg :stop :initform  nil)     #|line 211|#
    (instance_data :accessor instance_data :initarg :instance_data :initform  nil)  #|line 212|# #|  arg needed for probe support  |# #|line 213|#
    (arg :accessor arg :initarg :arg :initform  "")         #|line 214|#
    (state :accessor state :initarg :state :initform  "idle")  #|line 215|#
    (special :accessor special :initarg :special :initform  nil)  #|line 216|# #|  bootstrap debugging |# #|line 217|#
    (kind :accessor kind :initarg :kind :initform  nil)  #|  enum { container, leaf, } |# #|line 218|#)) #|line 219|#

                                                            #|line 220|#
(defparameter  load_errors  nil)                            #|line 221|#
(defparameter  runtime_errors  nil)                         #|line 222|# #|line 223|#
(defun clone_string (&optional  s)
  (declare (ignorable  s))                                  #|line 224|#
  (return-from clone_string  s)                             #|line 225|# #|line 226|#
  )
(defun injector (&optional  eh  mevent)
  (declare (ignorable  eh  mevent))                         #|line 228|#
  (funcall (slot-value  eh 'handler)   eh  mevent           #|line 229|#) #|line 230|#
  )
