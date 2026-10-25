#|  Directions of wires. This section is the only place that knows how a direction is represented |# #|line 1|# #|  (here: strings). To use integers, change the four constants and direction_label; nothing else changes. |# #|line 2|#
(defparameter directionDown  "down")                        #|line 3|#
(defparameter directionAcross  "across")                    #|line 4|#
(defparameter directionUp  "up")                            #|line 5|#
(defparameter directionThrough  "through")                  #|line 6|# #|line 7|# #|  d is the number used for a direction in the diagram JSON: 0 down, 1 across, 2 up, 3 through. |# #|line 8|# #|  Returns ϕ for any other number. |# #|line 9|#
(defun direction_of_json (&optional  d)
  (declare (ignorable  d))                                  #|line 10|# #|line 11|# #|line 12|# #|line 13|# #|line 14|#
  (cond
    (( equal    d  0)                                       #|line 15|#
      (return-from direction_of_json  directionDown)        #|line 16|#
      )
    (( equal    d  1)                                       #|line 17|#
      (return-from direction_of_json  directionAcross)      #|line 18|#
      )
    (( equal    d  2)                                       #|line 19|#
      (return-from direction_of_json  directionUp)          #|line 20|#
      )
    (( equal    d  3)                                       #|line 21|#
      (return-from direction_of_json  directionThrough)     #|line 22|#
      )
    (t                                                      #|line 23|#
      (return-from direction_of_json  nil)                  #|line 24|# #|line 25|#
      ))                                                    #|line 26|#
  ) #|  direction   sender                   receiver |#    #|line 28|# #|  down        the container            a child |# #|line 29|# #|  across      a child                  a child |# #|line 30|# #|  up          a child                  the container |# #|line 31|# #|  through     the container            the container |# #|line 32|#
(defun sender_is_container (&optional  direction)
  (declare (ignorable  direction))                          #|line 33|# #|line 34|# #|line 35|#
  (return-from sender_is_container ( or  ( equal    direction  directionDown) ( equal    direction  directionThrough))) #|line 36|# #|line 37|#
  )
(defun receiver_is_container (&optional  direction)
  (declare (ignorable  direction))                          #|line 39|# #|line 40|# #|line 41|#
  (return-from receiver_is_container ( or  ( equal    direction  directionUp) ( equal    direction  directionThrough))) #|line 42|# #|line 43|#
  ) #|  a name for messages and debugging |#                #|line 45|#
(defun direction_label (&optional  direction)
  (declare (ignorable  direction))                          #|line 46|#
  (return-from direction_label  direction)                  #|line 47|# #|line 48|#
  ) #|  Routing connection (wire) for a container. The `direction` field has no effect |# #|line 50|# #|  on routing; it is there for debugging, or for reading by other tools. |# #|line 51|#
(defclass Connector ()                                      #|line 52|#
  (
    (direction :accessor direction :initarg :direction :initform  nil)  #|  down, across, up, through |# #|line 53|#
    (sender :accessor sender :initarg :sender :initform  nil)  #|line 54|#
    (receiver :accessor receiver :initarg :receiver :initform  nil)  #|line 55|#)) #|line 56|#

                                                            #|line 57|# #|  `Sender` is matched on the part (identity) and the port name. |# #|line 58|#
(defclass Sender ()                                         #|line 59|#
  (
    (component :accessor component :initarg :component :initform  nil)  #|line 60|#
    (port :accessor port :initarg :port :initform  nil)     #|line 61|#)) #|line 62|#

                                                            #|line 63|# #|  `Receiver` is the part and port that a mevent is delivered to. |# #|line 64|# #|  When the receiver is the container itself (up and through wires) the port is an output gate. |# #|line 65|#
(defclass Receiver ()                                       #|line 66|#
  (
    (component :accessor component :initarg :component :initform  nil)  #|line 67|#
    (port :accessor port :initarg :port :initform  nil)     #|line 68|#)) #|line 69|#

                                                            #|line 70|#
(defun mkSender (&optional  component  port)
  (declare (ignorable  component  port))                    #|line 71|#
  (let ((s  (make-instance 'Sender)                         #|line 72|#))
    (declare (ignorable s))
    (setf (slot-value  s 'component)  component)            #|line 73|#
    (setf (slot-value  s 'port)  port)                      #|line 74|#
    (return-from mkSender  s)                               #|line 75|#) #|line 76|#
  )
(defun mkReceiver (&optional  component  port)
  (declare (ignorable  component  port))                    #|line 78|#
  (let ((r  (make-instance 'Receiver)                       #|line 79|#))
    (declare (ignorable r))
    (setf (slot-value  r 'component)  component)            #|line 80|#
    (setf (slot-value  r 'port)  port)                      #|line 81|#
    (return-from mkReceiver  r)                             #|line 82|#) #|line 83|#
  )
(defun mkConnector (&optional  direction  sender  receiver)
  (declare (ignorable  direction  sender  receiver))        #|line 85|#
  (let ((c  (make-instance 'Connector)                      #|line 86|#))
    (declare (ignorable c))
    (setf (slot-value  c 'direction)  direction)            #|line 87|#
    (setf (slot-value  c 'sender)  sender)                  #|line 88|#
    (setf (slot-value  c 'receiver)  receiver)              #|line 89|#
    (return-from mkConnector  c)                            #|line 90|#) #|line 91|#
  ) #|  JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''}, |# #|line 93|# #|  The sender port is proto_conn@"source_port" and the receiver port is proto_conn@"target_port". |# #|line 94|# #|  A child sender is proto_conn@"source", a child receiver is proto_conn@"target". |# #|line 95|# #|  Returns ϕ if the wire can not be made. |# #|line 96|#
(defun create_connector (&optional  container  proto_conn  children_by_id)
  (declare (ignorable  container  proto_conn  children_by_id)) #|line 97|#
  (let ((direction (funcall (quote direction_of_json)  (gethash "dir"  proto_conn)  #|line 98|#)))
    (declare (ignorable direction))
    (cond
      (( equal    direction  nil)                           #|line 99|#
        (funcall (quote load_error)   (concatenate 'string  "unknown wire direction " (format nil "~a" (gethash "dir"  proto_conn)))  #|line 100|#)
        (return-from create_connector  nil)                 #|line 101|# #|line 102|#
        ))
    (let ((sender_component  container))
      (declare (ignorable sender_component))                #|line 103|#
      (cond
        ((not (funcall (quote sender_is_container)   direction )) #|line 104|#
          (let ((sid (gethash "id" (gethash "source"  proto_conn))))
            (declare (ignorable sid))                       #|line 105|#
            (setf  sender_component (gethash sid  children_by_id)) #|line 106|#) #|line 107|#
          ))
      (let ((receiver_component  container))
        (declare (ignorable receiver_component))            #|line 108|#
        (cond
          ((not (funcall (quote receiver_is_container)   direction )) #|line 109|#
            (let ((tid (gethash "id" (gethash "target"  proto_conn))))
              (declare (ignorable tid))                     #|line 110|#
              (setf  receiver_component (gethash tid  children_by_id)) #|line 111|#) #|line 112|#
            ))
        (cond
          (( or  ( equal    sender_component  nil) ( equal    receiver_component  nil)) #|line 113|#
            (funcall (quote load_error)   (concatenate 'string (funcall (quote direction_label)   direction )  " wire: no such source or target")  #|line 114|#)
            (return-from create_connector  nil)             #|line 115|# #|line 116|#
            ))
        (return-from create_connector (funcall (quote mkConnector)   direction  #|line 117|#(funcall (quote mkSender)   sender_component (gethash "source_port"  proto_conn) )  #|line 118|#(funcall (quote mkReceiver)   receiver_component (gethash "target_port"  proto_conn) )  #|line 119|#))))) #|line 120|#
  )
