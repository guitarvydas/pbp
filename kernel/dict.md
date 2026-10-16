{ key:... } dict
key must be a string
... can be one of: (commas are whitespace)
- a string
- a number
- #true
- #false
- a dict { ... }
- a list of dicts [ ... ]


drawio file:
[ diagram+ ]

diagram:
{ 
  "name" : string
  "children" : [ child_id+ ]
  "connections" : [ wire+ ]
  "file" : string
}

child_id:
{
  "name" : string
  "id" : int
}

<!-- wire: -->
<!-- { -->
<!--   "dir": int -->
<!--   "source_port" : string -->
<!--   "target_port" : string -->
<!--   "source" : child_id            optional -->
<!--   "target" : child_id            optional -->
<!-- } -->

wire = down_wire | up_wire | across_wire | through_wire

down_wire:
{
  "dir": 0
  "source_port" : string
  "target_port" : string
  "target" : child_id
}

up_wire:
{
  "dir": 2
  "source_port" : string
  "target_port" : string
  "source" : child_id
}

across_wire:
{
  "dir": 1
  "source_port" : string
  "target_port" : string
  "source" : child_id
  "target" : child_id
}

through_wire:
{
  "dir": 3
  "source_port" : string
  "target_port" : string
}

