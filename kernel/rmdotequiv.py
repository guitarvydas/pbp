import sys

for line in sys.stdin:
    content = line.rstrip('\n')
    idx = content.find('≡')
    if idx != -1 and '.' in content[:idx]:
        # line qualifies: delete up to (not including) ⤶
        outdent_idx = content.find('⤶')
        if outdent_idx != -1:
            print(content[outdent_idx:])
        # else: no ⤶ -> print nothing (line is dropped entirely)
    else:
        print(content)
