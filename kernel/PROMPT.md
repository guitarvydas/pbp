change this so that it deletes everything up to, but not including, any outdent character `⤶`, If there is no outdent character, delete up to the end of the line
```
import sys

for line in sys.stdin:
    idx = line.find('≡')
    if idx != -1 and '.' in line[:idx]:
        continue  # skip: has a '.' before '≡'
    print(line, end='')
```
