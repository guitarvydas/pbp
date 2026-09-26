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
      (setf (slot-value  eh 'stop)  #'leaf_reset)           #|line 15|#
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
