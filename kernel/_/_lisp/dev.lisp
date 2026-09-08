(defclass Datum ()                                          #|line 1|#
  (
    (v :accessor v :initarg :v :initform  nil)              #|line 2|#
    (clone :accessor clone :initarg :clone :initform  nil)  #|line 3|#
    (reclaim :accessor reclaim :initarg :reclaim :initform  nil)  #|line 4|#
    (other :accessor other :initarg :other :initform  nil)  #|  reserved for use on per-project basis  |# #|line 5|#)) #|line 6|#
