xxx.drawio.json:

- JSON array of diagrams

	- a diagram is
		- "name" : "...",
		- "children" : [ child_id+ ]
		- "connections" : [ wire+ ]
		- "file" : "xxx.drawio"


		- child_id is
			{ 
				"name" : "..." 
				"id" : int 
			}
			
		- a wire is
			{ 
				"dir": 0|1|2|3 
				"source_port": "..." 
				"target_port": "..." 
				"source" : child_id        <-- missing if source = self (dir = 0)
				"target" : child_id        <-- missing if target = self (dir = 2)
			}
			
				
				

interned strings:
"name"
"children"
"connections"
"file"
"id"
"dir"
"source_port"
"target_port"
"source"
"target"
