import sys

for line in sys.stdin:
    idx = line.find('≡')
    if idx != -1 and '.' in line[:idx]:
        continue  # skip: has a '.' before '≡'
    print(line, end='')
