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
  (declare (ignorable  reg  owner  container_name  desc  arg)) #|line 57|# #|line 58|#
  (let ((container (funcall (quote make_container)   container_name  owner  #|line 59|#)))
    (declare (ignorable container))
    (let ((children  nil))
      (declare (ignorable children))                        #|line 60|#
      (let ((children_by_id  (dict-fresh)))
        (declare (ignorable children_by_id))
        #|  not strictly necessary, but, we can remove 1 runtime lookup by "compiling it out“ here |# #|line 61|#
        #|  collect children |#                             #|line 62|#
        (loop for child_desc in (gethash  "children"  desc)
          do
            (progn
              child_desc                                    #|line 63|#
              (let ((child_instance (funcall (quote get_component_instance)   reg (gethash  "name"  child_desc)  container  #|line 64|#)))
                (declare (ignorable child_instance))
                (setf  children (append  children (list  child_instance))) #|line 65|#
                (let ((id (gethash  "id"  child_desc)))
                  (declare (ignorable id))                  #|line 66|#
                  (setf (gethash id  children_by_id)  child_instance) #|line 67|# #|line 68|#)) #|line 69|#
              ))
        (setf (slot-value  container 'children)  children)  #|line 70|# #|line 71|#
        (let ((connectors  nil))
          (declare (ignorable connectors))                  #|line 72|#
          (loop for proto_conn in (gethash  "connections"  desc)
            do
              (progn
                proto_conn                                  #|line 73|#
                (let (( connector  (make-instance 'Connector) #|line 74|#))
                  (declare (ignorable  connector))
                  (cond
                    (( equal   (gethash  "dir"  proto_conn)  enumDown) #|line 75|#
                      (setf  connectors (append  connectors (list (funcall (quote create_down_connector)   container  proto_conn  connectors  children_by_id )))) #|line 76|#
                      )
                    (( equal   (gethash  "dir"  proto_conn)  enumAcross) #|line 77|#
                      (setf  connectors (append  connectors (list (funcall (quote create_across_connector)   container  proto_conn  connectors  children_by_id )))) #|line 78|#
                      )
                    (( equal   (gethash  "dir"  proto_conn)  enumUp) #|line 79|#
                      (setf  connectors (append  connectors (list (funcall (quote create_up_connector)   container  proto_conn  connectors  children_by_id )))) #|line 80|#
                      )
                    (( equal   (gethash  "dir"  proto_conn)  enumThrough) #|line 81|#
                      (setf  connectors (append  connectors (list (funcall (quote create_through_connector)   container  proto_conn  connectors  children_by_id )))) #|line 82|# #|line 83|#
                      )))                                   #|line 84|#
                ))
          (setf (slot-value  container 'connections)  connectors) #|line 85|#
          (return-from container_instantiator  container)   #|line 86|#)))) #|line 87|#
  ) #|  The default handler for container components. |#    #|line 89|#
(defun container_handler (&optional  container  mevent)
  (declare (ignorable  container  mevent))                  #|line 90|#
  (funcall (quote route)   container  #|  from=  |# container  mevent )
  #|  references to 'self' are replaced by the container during instantiation |# #|line 91|#
  (loop while (funcall (quote any_child_ready)   container )
    do
      (progn                                                #|line 92|#
        (funcall (quote step_children)   container  mevent ) #|line 93|#
        ))                                                  #|line 94|#
  ) #|  Stop all children. Reset to a known state. Hit the big red button.  |# #|line 96|#
(defun container_reset_children (&optional  container)
  (declare (ignorable  container))                          #|line 97|#
  (loop for child in (slot-value  container 'children)
    do
      (progn
        child                                               #|line 98|#
        (funcall (slot-value  child 'stop)   child          #|line 99|#) #|line 100|#
        ))

  (setf (slot-value  container 'visit_ordering) (make-instance 'Queue)) #|line 101|#

  (setf (slot-value  container 'routings) (make-instance 'Queue)) #|line 102|#

  (setf (slot-value  container 'inq) (make-instance 'Queue)) #|line 103|#

  (setf (slot-value  container 'outq) (make-instance 'Queue)) #|line 104|#
  (setf (slot-value  container 'state)  "idle")             #|line 105|# #|line 106|#
  ) #|  Frees the given container and associated data. |#   #|line 108|#
(defun destroy_container (&optional  eh)
  (declare (ignorable  eh))                                 #|line 109|#
  #| pass |#                                                #|line 110|# #|line 111|#
  ) #|  Checks if two senders match, by pointer equality and port name matching. |# #|line 112|#
(defun sender_eq (&optional  s1  s2)
  (declare (ignorable  s1  s2))                             #|line 113|#
  (let ((same_components ( equal   (slot-value  s1 'component) (slot-value  s2 'component))))
    (declare (ignorable same_components))                   #|line 114|#
    (let ((same_ports ( equal   (slot-value  s1 'port) (slot-value  s2 'port))))
      (declare (ignorable same_ports))                      #|line 115|#
      (return-from sender_eq ( and   same_components  same_ports)) #|line 116|#)) #|line 117|#
  ) #|  Delivers the given mevent to the receiver of this connector. |# #|line 119|# #|line 120|#
(defun deposit (&optional  parent  conn  mevent)
  (declare (ignorable  parent  conn  mevent))               #|line 121|#
  (let ((new_mevent (funcall (quote make_mevent)  (slot-value (slot-value  conn 'receiver) 'port) (slot-value  mevent 'payload)  #|line 122|#)))
    (declare (ignorable new_mevent))
    (funcall (quote push_mevent)   parent (slot-value (slot-value  conn 'receiver) 'component) (slot-value (slot-value  conn 'receiver) 'queue)  new_mevent  #|line 123|#)) #|line 124|#
  )
(defun force_tick (&optional  parent  eh)
  (declare (ignorable  parent  eh))                         #|line 126|#
  (let ((tick_mev (funcall (quote make_mevent)   "." (funcall (quote new_datum_bang) )  #|line 127|#)))
    (declare (ignorable tick_mev))
    (funcall (quote push_mevent)   parent  eh (slot-value  eh 'inq)  tick_mev  #|line 128|#)
    (return-from force_tick  tick_mev)                      #|line 129|#) #|line 130|#
  )
(defun push_mevent (&optional  parent  receiver  inq  m)
  (declare (ignorable  parent  receiver  inq  m))           #|line 132|#
  (enqueue  inq  m)                                         #|line 133|#
  (cond
    ((slot-value  receiver 'special)                        #|line 134|#
      (prequeue (slot-value  parent 'visit_ordering)  receiver) #|line 135|#
      )
    (t                                                      #|line 136|#
      (enqueue (slot-value  parent 'visit_ordering)  receiver) #|line 137|# #|line 138|#
      ))                                                    #|line 139|# #|line 140|#
  )
(defun is_self (&optional  child  container)
  (declare (ignorable  child  container))                   #|line 142|#
  #|  in an earlier version “self“ was denoted as ϕ |#      #|line 143|#
  (return-from is_self ( equal    child  container))        #|line 144|# #|line 145|#
  )
(defun step_child_once (&optional  child  mev)
  (declare (ignorable  child  mev))                         #|line 147|#
  (cond
    ( (not (null (uiop:getenv "PBPSTEPPING")))              #|line 148|#
      (format *error-output* "~a~%"  (concatenate 'string  "-- stepping ❮"  (concatenate 'string (slot-value  child 'name)  "❯"))) #|line 149|#
      (format *error-output* "
      ")                                                    #|line 150|# #|line 151|#
      ))
  (funcall (slot-value  child 'handler)   child  mev        #|line 152|#) #|line 153|#
  )
(defun step_children (&optional  container  causingMevent)
  (declare (ignorable  container  causingMevent))           #|line 155|#
  (setf (slot-value  container 'state)  "idle")             #|line 156|# #|line 157|#
  #|  phase 1 - loop through children and process inputs or children that not "idle"  |# #|line 158|#
  (loop for child in (queue2list (slot-value  container 'visit_ordering))
    do
      (progn
        child                                               #|line 159|#
        #|  child = container represents self, skip it |#   #|line 160|#
        (cond
          ((not (funcall (quote is_self)   child  container )) #|line 161|#
            (cond
              ((not (empty? (slot-value  child 'inq)))      #|line 162|#
                (let ((mev (dequeue (slot-value  child 'inq)) #|line 163|#))
                  (declare (ignorable mev))
                  (funcall (quote step_child_once)   child  mev  #|line 164|#) #|line 165|#
                  (funcall (quote destroy_mevent)   mev     #|line 166|#))
                )
              (t                                            #|line 167|#
                (cond
                  (( equal   (slot-value  child 'state)  "idle") #|line 168|#
                    #| pass |#                              #|line 169|#
                    )
                  (t                                        #|line 170|#
                    (let ((mev (funcall (quote force_tick)   container  child  #|line 171|#)))
                      (declare (ignorable mev))
                      (funcall (quote step_child_once)   child  mev  #|line 172|#)
                      (funcall (quote destroy_mevent)   mev  #|line 173|#)) #|line 174|#
                    ))                                      #|line 175|#
                ))                                          #|line 176|#
            ))                                              #|line 177|#
        ))

  (setf (slot-value  container 'visit_ordering) (make-instance 'Queue)) #|line 178|# #|line 179|#
  #|  phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections  |# #|line 180|#
  (loop for child in (slot-value  container 'children)
    do
      (progn
        child                                               #|line 181|#
        (cond
          (( equal   (slot-value  child 'state)  "active")  #|line 182|#
            #|  if child remains active, then the container must remain active and must propagate “ticks“ to child |# #|line 183|#
            (setf (slot-value  container 'state)  "active") #|line 184|# #|line 185|#
            ))                                              #|line 186|#
        (loop while (not (empty? (slot-value  child 'outq)))
          do
            (progn                                          #|line 187|#
              (let ((mev (dequeue (slot-value  child 'outq)) #|line 188|#))
                (declare (ignorable mev))
                (funcall (quote route)   container  child  mev  #|line 189|#)
                (funcall (quote destroy_mevent)   mev       #|line 190|#)) #|line 191|#
              ))                                            #|line 192|#
        ))                                                  #|line 193|#
  )
(defun attempt_tick (&optional  parent  eh)
  (declare (ignorable  parent  eh))                         #|line 195|#
  (cond
    ((not (equal  (slot-value  eh 'state)  "idle"))         #|line 196|#
      (funcall (quote force_tick)   parent  eh              #|line 197|#) #|line 198|#
      ))                                                    #|line 199|#
  )
(defun is_tick (&optional  mev)
  (declare (ignorable  mev))                                #|line 201|#
  (return-from is_tick ( equal    "." (slot-value  mev 'port))
    #|  assume that any mevent that is sent to port "." is a tick  |# #|line 202|#) #|line 203|#
  ) #|  Routes a single mevent to all matching destinations, according to |# #|line 205|# #|  the container's connection network. |# #|line 206|# #|line 207|#
(defun route (&optional  container  from_component  mevent)
  (declare (ignorable  container  from_component  mevent))  #|line 208|#
  (let (( was_sent  nil))
    (declare (ignorable  was_sent))
    #|  for checking that output went somewhere (at least during bootstrap) |# #|line 209|#
    (let (( fromname  ""))
      (declare (ignorable  fromname))                       #|line 210|# #|line 211|#
      (setf  ticktime (+  ticktime  1))                     #|line 212|#
      (cond
        ((funcall (quote is_tick)   mevent )                #|line 213|#
          (loop for child in (slot-value  container 'children)
            do
              (progn
                child                                       #|line 214|#
                (funcall (quote attempt_tick)   container  child ) #|line 215|#
                ))
          (setf  was_sent  t)                               #|line 216|#
          )
        (t                                                  #|line 217|#
          (cond
            ((not (funcall (quote is_self)   from_component  container )) #|line 218|#
              (setf  fromname (slot-value  from_component 'name)) #|line 219|# #|line 220|#
              ))
          (let ((from_sender (funcall (quote mkSender)   fromname  from_component (slot-value  mevent 'port)  #|line 221|#)))
            (declare (ignorable from_sender))               #|line 222|#
            (loop for connector in (slot-value  container 'connections)
              do
                (progn
                  connector                                 #|line 223|#
                  (cond
                    ((funcall (quote sender_eq)   from_sender (slot-value  connector 'sender) ) #|line 224|#
                      (funcall (quote deposit)   container  connector  mevent  #|line 225|#)
                      (setf  was_sent  t)                   #|line 226|# #|line 227|#
                      ))                                    #|line 228|#
                  )))                                       #|line 229|#
          ))
      (cond
        ((not  was_sent)                                    #|line 230|#
          (live_update  "internal error"  (concatenate 'string (slot-value  container 'name)  (concatenate 'string  ": mevent on port '"  (concatenate 'string (slot-value  mevent 'port)  (concatenate 'string  "' from "  (concatenate 'string  fromname  " dropped on floor...")))))) #|line 231|# #|line 232|#
          ))))                                              #|line 233|#
  )
(defun any_child_ready (&optional  container)
  (declare (ignorable  container))                          #|line 235|#
  (loop for child in (slot-value  container 'children)
    do
      (progn
        child                                               #|line 236|#
        (cond
          ((funcall (quote child_is_ready)   child )        #|line 237|#
            (return-from any_child_ready  t)                #|line 238|# #|line 239|#
            ))                                              #|line 240|#
        ))
  (return-from any_child_ready  nil)                        #|line 241|# #|line 242|#
  )
(defun child_is_ready (&optional  eh)
  (declare (ignorable  eh))                                 #|line 244|#
  (return-from child_is_ready ( or  ( or  ( or  (not (empty? (slot-value  eh 'outq))) (not (empty? (slot-value  eh 'inq)))) (not (equal  (slot-value  eh 'state)  "idle"))) (funcall (quote any_child_ready)   eh ))) #|line 245|# #|line 246|#
  )
(defun append_routing_descriptor (&optional  container  desc)
  (declare (ignorable  container  desc))                    #|line 248|#
  (enqueue (slot-value  container 'routings)  desc)         #|line 249|# #|line 250|#
  )                                                         #|line 252|# #|  Creates a component that acts as a container. It is the same as a `Eh` instance |# #|line 253|# #|  whose handler function is `container_handler`. |# #|line 254|#
(defun make_container (&optional  name  owner)
  (declare (ignorable  name  owner))                        #|line 255|#
  (let (( eh  (make-instance 'Eh)                           #|line 256|#))
    (declare (ignorable  eh))
    (setf (slot-value  eh 'name)  name)                     #|line 257|#
    (setf (slot-value  eh 'owner)  owner)                   #|line 258|#
    (setf (slot-value  eh 'handler)  #'container_handler)   #|line 259|#
    (setf (slot-value  eh 'finject)  #'injector)            #|line 260|#
    (setf (slot-value  eh 'stop)  #'container_reset_children) #|line 261|#
    (setf (slot-value  eh 'state)  "idle")                  #|line 262|#
    (setf (slot-value  eh 'kind)  "container")              #|line 263|#
    (return-from make_container  eh)                        #|line 264|#) #|line 265|#
  ) #|  Sends a mevent on the given `port` with `data`, placing it on the output |# #|line 267|# #|  of the given component. |# #|line 268|# #|line 269|#
(defun send (&optional  eh  port  obj  causingMevent)
  (declare (ignorable  eh  port  obj  causingMevent))       #|line 270|#
  (let (( d  (make-instance 'Datum)                         #|line 271|#))
    (declare (ignorable  d))
    (setf (slot-value  d 'v)  obj)                          #|line 272|#
    (setf (slot-value  d 'clone)  #'(lambda (&optional )(funcall (quote obj_clone)   d  #|line 273|#)))
    (setf (slot-value  d 'reclaim)  nil)                    #|line 274|#
    (let ((mev (funcall (quote make_mevent)   port  d       #|line 275|#)))
      (declare (ignorable mev))
      (funcall (quote put_output)   eh  mev                 #|line 276|#))) #|line 277|#
  )
(defun forward (&optional  eh  port  mev)
  (declare (ignorable  eh  port  mev))                      #|line 279|#
  (let ((fwdmev (funcall (quote make_mevent)   port (slot-value  mev 'payload)  #|line 280|#)))
    (declare (ignorable fwdmev))
    (funcall (quote put_output)   eh  fwdmev                #|line 281|#)) #|line 282|#
  )
(defun inject_mevent (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 284|#
  (funcall (slot-value  eh 'finject)   eh  mev              #|line 285|#) #|line 286|#
  )
(defun set_active (&optional  eh)
  (declare (ignorable  eh))                                 #|line 288|#
  (setf (slot-value  eh 'state)  "active")                  #|line 289|# #|line 290|#
  )
(defun set_idle (&optional  eh)
  (declare (ignorable  eh))                                 #|line 292|#
  (setf (slot-value  eh 'state)  "idle")                    #|line 293|# #|line 294|#
  )
(defun put_output (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 296|#
  (enqueue (slot-value  eh 'outq)  mev)                     #|line 297|# #|line 298|#
  )
(defun obj_clone (&optional  obj)
  (declare (ignorable  obj))                                #|line 300|#
  (return-from obj_clone  obj)                              #|line 301|# #|line 302|#
  )
