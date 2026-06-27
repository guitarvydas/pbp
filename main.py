import sys
import kernel0d as zd

# perform all custom imports
# import <customize this>

[palette, env] = zd.initialize_from_files (sys.argv[3:])
# perform all custom installs
# <customize this>.install (palette)
top = zd.start_bare (part_name=sys.argv[2], palette=palette, env=env)
# customize the "" 2nd arg as the first injected message into your project
zd.inject (top, "", sys.argv[1])
zd.finalize (top)

