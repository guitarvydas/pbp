import sys
import kernel0d as zd

import counter

[palette, env] = zd.initialize_from_files (sys.argv[3:])
top = zd.start_bare (part_name=sys.argv[2], palette=palette, env=env)
zd.inject (top, "", sys.argv[1])
zd.finalize (top)

