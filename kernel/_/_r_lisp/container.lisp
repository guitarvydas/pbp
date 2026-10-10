(defun create_down_connector (&optional  container  proto_conn  connectors  children_by_id)
  (declare (ignorable  container  proto_conn  connectors  children_by_id)) #|line 1|#
  #|  JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''}, |# #|line 2|#
  (let (( connector  (make-instance 'Connector)             #|line 3|#))
    (declare (ignorable  connector))
    (setf (slot-value  connector 'direction)  "down")       #|line 4|#
    (setf (slot-value  connector 'sender) (funcall (quote mkSender)  (slot-value  container 'name)  container (gethash undefined "source_port")  #|line 5|#))
    (let ((target_proto (gethash undefined "target")))
      (declare (ignorable target_proto))                    #|line 6|#
      (let ((id_proto (gethash undefined "id")))
        (declare (ignorable id_proto))                      #|line 7|#
        (let ((target_component (gethash undefined id_proto)))
          (declare (ignorable target_component))            #|line 8|#
          (cond
            (( equal    target_component  nil)              #|line 9|#
              (funcall (quote load_error)   (concatenate 'string  "internal error: .Down connection target internal error " (gethash undefined "name")) ) #|line 10|#
              )
            (t                                              #|line 11|#
              (setf (slot-value  connector 'receiver) (funcall (quote mkReceiver)  (slot-value  target_component 'name)  target_component (gethash undefined "target_port") (slot-value  target_component 'inq)  #|line 12|#)) #|line 13|#
              ))
          (return-from create_down_connector  connector)    #|line 14|#)))) #|line 15|#
  )
(defun create_across_connector (&optional  container  proto_conn  connectors  children_by_id)
  (declare (ignorable  container  proto_conn  connectors  children_by_id)) #|line 17|#
  (let (( connector  (make-instance 'Connector)             #|line 18|#))
    (declare (ignorable  connector))
    (setf (slot-value  connector 'direction)  "across")     #|line 19|#
    (let ((sid (gethash undefined "id")))
      (declare (ignorable sid))                             #|line 20|#
      (let ((source_component (gethash undefined sid)))
        (declare (ignorable source_component))              #|line 21|#
        (let ((tid (gethash undefined "id")))
          (declare (ignorable tid))                         #|line 22|#
          (let ((target_component (gethash undefined tid)))
            (declare (ignorable target_component))          #|line 23|#
            (cond
              (( equal    source_component  nil)            #|line 24|#
                (funcall (quote load_error)   (concatenate 'string  "internal error: .Across connection source not ok " (gethash undefined "name"))  #|line 25|#)
                )
              (t                                            #|line 26|#
                (setf (slot-value  connector 'sender) (funcall (quote mkSender)  (slot-value  source_component 'name)  source_component (gethash undefined "source_port")  #|line 27|#))
                (cond
                  (( equal    target_component  nil)        #|line 28|#
                    (funcall (quote load_error)   (concatenate 'string  "internal error: .Across connection target not ok " (gethash undefined "name"))  #|line 29|#)
                    )
                  (t                                        #|line 30|#
                    (setf (slot-value  connector 'receiver) (funcall (quote mkReceiver)  (slot-value  target_component 'name)  target_component (gethash undefined "target_port") (slot-value  target_component 'inq)  #|line 31|#)) #|line 32|#
                    ))                                      #|line 33|#
                ))
            (return-from create_across_connector  connector) #|line 34|#))))) #|line 35|#
  )
(defun create_up_connector (&optional  container  proto_conn  connectors  children_by_id)
  (declare (ignorable  container  proto_conn  connectors  children_by_id)) #|line 37|#
  (let (( connector  (make-instance 'Connector)             #|line 38|#))
    (declare (ignorable  connector))
    (setf (slot-value  connector 'direction)  "up")         #|line 39|#
    (let ((sid (gethash undefined "id")))
      (declare (ignorable sid))                             #|line 40|#
      (let ((source_component (gethash undefined sid)))
        (declare (ignorable source_component))              #|line 41|#
        (cond
          (( equal    source_component  nil)                #|line 42|#
            (funcall (quote load_error)   (concatenate 'string  "internal error: .Up connection source not ok " (gethash undefined "name")) ) #|line 43|#
            )
          (t                                                #|line 44|#
            (setf (slot-value  connector 'sender) (funcall (quote mkSender)  (slot-value  source_component 'name)  source_component (gethash undefined "source_port")  #|line 45|#))
            (setf (slot-value  connector 'receiver) (funcall (quote mkReceiver)  (slot-value  container 'name)  container (gethash undefined "target_port") (slot-value  container 'outq)  #|line 46|#)) #|line 47|#
            ))
        (return-from create_up_connector  connector)        #|line 48|#))) #|line 49|#
  )
(defun create_through_connector (&optional  container  proto_conn  connectors  children_by_id)
  (declare (ignorable  container  proto_conn  connectors  children_by_id)) #|line 51|#
  (let (( connector  (make-instance 'Connector)             #|line 52|#))
    (declare (ignorable  connector))
    (setf (slot-value  connector 'direction)  "through")    #|line 53|#
    (setf (slot-value  connector 'sender) (funcall (quote mkSender)  (slot-value  container 'name)  container (gethash undefined "source_port")  #|line 54|#))
    (setf (slot-value  connector 'receiver) (funcall (quote mkReceiver)  (slot-value  container 'name)  container (gethash undefined "target_port") (slot-value  container 'outq)  #|line 55|#))
    (return-from create_through_connector  connector)       #|line 56|#) #|line 57|#
  )                                                         #|line 59|#
(defun container_instantiator (&optional  reg  owner  container_name  desc  arg)
  (declare (ignorable  reg  owner  container_name  desc  arg)) #|line 60|# #|line 61|# #|line 62|# #|line 63|# #|line 64|#
  (let ((container (funcall (quote make_container)   container_name  owner  #|line 65|#)))
    (declare (ignorable container))
    (let ((children  nil))
      (declare (ignorable children))                        #|line 66|#
      (let ((children_by_id  (dict-fresh)))
        (declare (ignorable children_by_id))
        #|  not strictly necessary, but, we can remove 1 runtime lookup by "compiling it out“ here |# #|line 67|#
        #|  collect children |#                             #|line 68|#
        (loop for child_desc in (gethash undefined "children")
          do
            (progn
              child_desc                                    #|line 69|#
              (let ((child_instance (funcall (quote get_component_instance)   reg (gethash undefined "name")  container  #|line 70|#)))
                (declare (ignorable child_instance))
                (setf  children (append  children (list  child_instance))) #|line 71|#
                (let ((id (gethash undefined "id")))
                  (declare (ignorable id))                  #|line 72|#
                  (setf (gethash undefined id)  child_instance) #|line 73|# #|line 74|#)) #|line 75|#
              ))
        (setf (slot-value  container 'children)  children)  #|line 76|# #|line 77|#
        (let ((connectors  nil))
          (declare (ignorable connectors))                  #|line 78|#
          (loop for proto_conn in (gethash undefined "connections")
            do
              (progn
                proto_conn                                  #|line 79|#
                (let (( connector  (make-instance 'Connector) #|line 80|#))
                  (declare (ignorable  connector))
                  (cond
                    (( equal   (gethash undefined "dir")  enumDown) #|line 81|#
                      (setf  connectors (append  connectors (list (funcall (quote create_down_connector)   container  proto_conn  connectors  children_by_id )))) #|line 82|#
                      )
                    (( equal   (gethash undefined "dir")  enumAcross) #|line 83|#
                      (setf  connectors (append  connectors (list (funcall (quote create_across_connector)   container  proto_conn  connectors  children_by_id )))) #|line 84|#
                      )
                    (( equal   (gethash undefined "dir")  enumUp) #|line 85|#
                      (setf  connectors (append  connectors (list (funcall (quote create_up_connector)   container  proto_conn  connectors  children_by_id )))) #|line 86|#
                      )
                    (( equal   (gethash undefined "dir")  enumThrough) #|line 87|#
                      (setf  connectors (append  connectors (list (funcall (quote create_through_connector)   container  proto_conn  connectors  children_by_id )))) #|line 88|# #|line 89|#
                      )))                                   #|line 90|#
                ))
          (setf (slot-value  container 'connections)  connectors) #|line 91|#
          (return-from container_instantiator  container)   #|line 92|#)))) #|line 93|#
  ) #|  The default handler for container components. |#    #|line 95|#
(defun container_handler (&optional  container  mevent)
  (declare (ignorable  container  mevent))                  #|line 96|#
  (funcall (quote route)   container  #|  from=  |# container  mevent )
  #|  references to 'self' are replaced by the container during instantiation |# #|line 97|#
  (loop while (funcall (quote any_child_ready)   container )
    do
      (progn                                                #|line 98|#
        (funcall (quote step_children)   container  mevent ) #|line 99|#
        ))                                                  #|line 100|#
  ) #|  Stop all children. Reset to a known state. Hit the big red button.  |# #|line 102|#
(defun container_reset (&optional  container)
  (declare (ignorable  container))                          #|line 103|#
  (loop for child in (slot-value  container 'children)
    do
      (progn
        child                                               #|line 104|#
        (funcall (slot-value  child 'reset)   child         #|line 105|#) #|line 106|#
        ))

  (setf (slot-value  container 'visit_ordering) (make-instance 'Queue)) #|line 107|#

  (setf (slot-value  container 'inq) (make-instance 'Queue)) #|line 108|#

  (setf (slot-value  container 'outq) (make-instance 'Queue)) #|line 109|#
  (setf (slot-value  container 'state)  "idle")             #|line 110|# #|line 111|#
  ) #|  Frees the given container and associated data. |#   #|line 113|#
(defun destroy_container (&optional  eh)
  (declare (ignorable  eh))                                 #|line 114|#
  #| pass |#                                                #|line 115|# #|line 116|#
  ) #|  Checks if two senders match, by pointer equality and port name matching. |# #|line 117|#
(defun sender_eq (&optional  s1  s2)
  (declare (ignorable  s1  s2))                             #|line 118|#
  (let ((same_components ( equal   (slot-value  s1 'component) (slot-value  s2 'component))))
    (declare (ignorable same_components))                   #|line 119|#
    (let ((same_ports ( equal   (slot-value  s1 'port) (slot-value  s2 'port))))
      (declare (ignorable same_ports))                      #|line 120|#
      (return-from sender_eq ( and   same_components  same_ports)) #|line 121|#)) #|line 122|#
  ) #|  Delivers the given mevent to the receiver of this connector. |# #|line 124|# #|line 125|#
(defun deposit (&optional  parent  conn  mevent)
  (declare (ignorable  parent  conn  mevent))               #|line 126|#
  (let ((new_mevent (funcall (quote make_mevent)  (slot-value (slot-value  conn 'receiver) 'port) (slot-value  mevent 'payload)  #|line 127|#)))
    (declare (ignorable new_mevent))
    (funcall (quote push_mevent)   parent (slot-value (slot-value  conn 'receiver) 'component) (slot-value (slot-value  conn 'receiver) 'queue)  new_mevent  #|line 128|#)) #|line 129|#
  )
(defun force_tick (&optional  parent  eh)
  (declare (ignorable  parent  eh))                         #|line 131|#
  (let ((tick_mev (funcall (quote make_mevent)   "." (funcall (quote new_datum_bang) )  #|line 132|#)))
    (declare (ignorable tick_mev))
    (funcall (quote push_mevent)   parent  eh (slot-value  eh 'inq)  tick_mev  #|line 133|#)
    (return-from force_tick  tick_mev)                      #|line 134|#) #|line 135|#
  )
(defun push_mevent (&optional  parent  receiver  inq  m)
  (declare (ignorable  parent  receiver  inq  m))           #|line 137|#
  (enqueue  inq  m)                                         #|line 138|#
  (cond
    ((slot-value  receiver 'special)                        #|line 139|#
      (prequeue (slot-value  parent 'visit_ordering)  receiver) #|line 140|#
      )
    (t                                                      #|line 141|#
      (enqueue (slot-value  parent 'visit_ordering)  receiver) #|line 142|# #|line 143|#
      ))                                                    #|line 144|# #|line 145|#
  )
(defun is_self (&optional  child  container)
  (declare (ignorable  child  container))                   #|line 147|#
  #|  in an earlier version “self“ was denoted as ϕ |#      #|line 148|#
  (return-from is_self ( equal    child  container))        #|line 149|# #|line 150|#
  )
(defun step_child_once (&optional  child  mev)
  (declare (ignorable  child  mev))                         #|line 152|#
  (cond
    ( (not (null (uiop:getenv "PBPSTEPPING")))              #|line 153|#
      (format *error-output* "~a~%"  (concatenate 'string  "-- stepping ❮"  (concatenate 'string (slot-value  child 'name)  "❯"))) #|line 154|#
      (format *error-output* "
      ")                                                    #|line 155|# #|line 156|#
      ))
  (funcall (slot-value  child 'handler)   child  mev        #|line 157|#) #|line 158|#
  )
(defun step_children (&optional  container  causingMevent)
  (declare (ignorable  container  causingMevent))           #|line 160|#
  (setf (slot-value  container 'state)  "idle")             #|line 161|# #|line 162|#
  #|  phase 1 - loop through children and process inputs or children that not "idle"  |# #|line 163|#
  (loop for child in (queue2list (slot-value  container 'visit_ordering))
    do
      (progn
        child                                               #|line 164|#
        #|  child = container represents self, skip it |#   #|line 165|#
        (cond
          ((not (funcall (quote is_self)   child  container )) #|line 166|#
            (cond
              ((not (empty? (slot-value  child 'inq)))      #|line 167|#
                (let ((mev (dequeue (slot-value  child 'inq)) #|line 168|#))
                  (declare (ignorable mev))
                  (funcall (quote step_child_once)   child  mev  #|line 169|#) #|line 170|#
                  (funcall (quote destroy_mevent)   mev     #|line 171|#))
                )
              (t                                            #|line 172|#
                (cond
                  (( equal   (slot-value  child 'state)  "idle") #|line 173|#
                    #| pass |#                              #|line 174|#
                    )
                  (t                                        #|line 175|#
                    (let ((mev (funcall (quote force_tick)   container  child  #|line 176|#)))
                      (declare (ignorable mev))
                      (funcall (quote step_child_once)   child  mev  #|line 177|#)
                      (funcall (quote destroy_mevent)   mev  #|line 178|#)) #|line 179|#
                    ))                                      #|line 180|#
                ))                                          #|line 181|#
            ))                                              #|line 182|#
        ))

  (setf (slot-value  container 'visit_ordering) (make-instance 'Queue)) #|line 183|# #|line 184|#
  #|  phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections  |# #|line 185|#
  (loop for child in (slot-value  container 'children)
    do
      (progn
        child                                               #|line 186|#
        (cond
          (( equal   (slot-value  child 'state)  "active")  #|line 187|#
            #|  if child remains active, then the container must remain active and must propagate “ticks“ to child |# #|line 188|#
            (setf (slot-value  container 'state)  "active") #|line 189|# #|line 190|#
            ))                                              #|line 191|#
        (loop while (not (empty? (slot-value  child 'outq)))
          do
            (progn                                          #|line 192|#
              (let ((mev (dequeue (slot-value  child 'outq)) #|line 193|#))
                (declare (ignorable mev))
                (funcall (quote route)   container  child  mev  #|line 194|#)
                (funcall (quote destroy_mevent)   mev       #|line 195|#)) #|line 196|#
              ))                                            #|line 197|#
        ))                                                  #|line 198|#
  )
(defun attempt_tick (&optional  parent  eh)
  (declare (ignorable  parent  eh))                         #|line 200|#
  (cond
    ((not (equal  (slot-value  eh 'state)  "idle"))         #|line 201|#
      (funcall (quote force_tick)   parent  eh              #|line 202|#) #|line 203|#
      ))                                                    #|line 204|#
  )
(defun is_tick (&optional  mev)
  (declare (ignorable  mev))                                #|line 206|#
  (return-from is_tick ( equal    "." (slot-value  mev 'port))
    #|  assume that any mevent that is sent to port "." is a tick  |# #|line 207|#) #|line 208|#
  ) #|  Routes a single mevent to all matching destinations, according to |# #|line 210|# #|  the container's connection network. |# #|line 211|# #|line 212|#
(defun route (&optional  container  from_component  mevent)
  (declare (ignorable  container  from_component  mevent))  #|line 213|#
  (let (( was_sent  nil))
    (declare (ignorable  was_sent))
    #|  for checking that output went somewhere (at least during bootstrap) |# #|line 214|#
    (let (( fromname  ""))
      (declare (ignorable  fromname))                       #|line 215|# #|line 216|#
      (setf  ticktime (+  ticktime  1))                     #|line 217|#
      (cond
        ((funcall (quote is_tick)   mevent )                #|line 218|#
          (loop for child in (slot-value  container 'children)
            do
              (progn
                child                                       #|line 219|#
                (funcall (quote attempt_tick)   container  child ) #|line 220|#
                ))
          (setf  was_sent  t)                               #|line 221|#
          )
        (t                                                  #|line 222|#
          (cond
            ((not (funcall (quote is_self)   from_component  container )) #|line 223|#
              (setf  fromname (slot-value  from_component 'name)) #|line 224|# #|line 225|#
              ))
          (let ((from_sender (funcall (quote mkSender)   fromname  from_component (slot-value  mevent 'port)  #|line 226|#)))
            (declare (ignorable from_sender))               #|line 227|#
            (loop for connector in (slot-value  container 'connections)
              do
                (progn
                  connector                                 #|line 228|#
                  (cond
                    ((funcall (quote sender_eq)   from_sender (slot-value  connector 'sender) ) #|line 229|#
                      (funcall (quote deposit)   container  connector  mevent  #|line 230|#)
                      (setf  was_sent  t)                   #|line 231|# #|line 232|#
                      ))                                    #|line 233|#
                  )))                                       #|line 234|#
          ))
      (cond
        ((not  was_sent)                                    #|line 235|#
          (live_update  "internal error"  (concatenate 'string (slot-value  container 'name)  (concatenate 'string  ": mevent on port '"  (concatenate 'string (slot-value  mevent 'port)  (concatenate 'string  "' from "  (concatenate 'string  fromname  " dropped on floor...")))))) #|line 236|# #|line 237|#
          ))))                                              #|line 238|#
  )
(defun any_child_ready (&optional  container)
  (declare (ignorable  container))                          #|line 240|#
  (loop for child in (slot-value  container 'children)
    do
      (progn
        child                                               #|line 241|#
        (cond
          ((funcall (quote child_is_ready)   child )        #|line 242|#
            (return-from any_child_ready  t)                #|line 243|# #|line 244|#
            ))                                              #|line 245|#
        ))
  (return-from any_child_ready  nil)                        #|line 246|# #|line 247|#
  )
(defun child_is_ready (&optional  eh)
  (declare (ignorable  eh))                                 #|line 249|#
  (return-from child_is_ready ( or  ( or  ( or  (not (empty? (slot-value  eh 'outq))) (not (empty? (slot-value  eh 'inq)))) (not (equal  (slot-value  eh 'state)  "idle"))) (funcall (quote any_child_ready)   eh ))) #|line 250|# #|line 251|#
  )                                                         #|line 253|# #|  Creates a component that acts as a container. It is the same as a `Eh` instance |# #|line 254|# #|  whose handler function is `container_handler`. |# #|line 255|#
(defun make_container (&optional  name  owner)
  (declare (ignorable  name  owner))                        #|line 256|#
  (let (( eh  (make-instance 'Eh)                           #|line 257|#))
    (declare (ignorable  eh))
    (setf (slot-value  eh 'name)  name)                     #|line 258|#
    (setf (slot-value  eh 'owner)  owner)                   #|line 259|#
    (setf (slot-value  eh 'handler)  #'container_handler)   #|line 260|#
    (setf (slot-value  eh 'finject)  #'injector)            #|line 261|#
    (setf (slot-value  eh 'reset)  #'container_reset)       #|line 262|#
    (setf (slot-value  eh 'state)  "idle")                  #|line 263|#
    (setf (slot-value  eh 'kind)  "container")              #|line 264|#
    (return-from make_container  eh)                        #|line 265|#) #|line 266|#
  ) #|  Sends a mevent on the given `port` with `data`, placing it on the output |# #|line 268|# #|  of the given component. |# #|line 269|# #|line 270|#
(defun send (&optional  eh  port  obj  causingMevent)
  (declare (ignorable  eh  port  obj  causingMevent))       #|line 271|#
  (let (( d  (make-instance 'Datum)                         #|line 272|#))
    (declare (ignorable  d))
    (setf (slot-value  d 'v)  obj)                          #|line 273|#
    (setf (slot-value  d 'clone)  #'(lambda (&optional )(funcall (quote obj_clone)   d  #|line 274|#)))
    (setf (slot-value  d 'reclaim)  nil)                    #|line 275|#
    (let ((mev (funcall (quote make_mevent)   port  d       #|line 276|#)))
      (declare (ignorable mev))
      (funcall (quote put_output)   eh  mev                 #|line 277|#))) #|line 278|#
  )
(defun forward (&optional  eh  port  mev)
  (declare (ignorable  eh  port  mev))                      #|line 280|#
  (let ((fwdmev (funcall (quote make_mevent)   port (slot-value  mev 'payload)  #|line 281|#)))
    (declare (ignorable fwdmev))
    (funcall (quote put_output)   eh  fwdmev                #|line 282|#)) #|line 283|#
  )
(defun inject_mevent (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 285|#
  (funcall (slot-value  eh 'finject)   eh  mev              #|line 286|#) #|line 287|#
  )
(defun set_active (&optional  eh)
  (declare (ignorable  eh))                                 #|line 289|#
  (setf (slot-value  eh 'state)  "active")                  #|line 290|# #|line 291|#
  )
(defun set_idle (&optional  eh)
  (declare (ignorable  eh))                                 #|line 293|#
  (setf (slot-value  eh 'state)  "idle")                    #|line 294|# #|line 295|#
  )
(defun put_output (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 297|#
  (enqueue (slot-value  eh 'outq)  mev)                     #|line 298|# #|line 299|#
  )
(defun obj_clone (&optional  obj)
  (declare (ignorable  obj))                                #|line 301|#
  (return-from obj_clone  obj)                              #|line 302|# #|line 303|#
  )
