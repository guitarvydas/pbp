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
    (setf (slot-value  d 'clone)  #'(lambda (&optional )(funcall (quote obj_clone)   d  #|line 271|#)))
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
