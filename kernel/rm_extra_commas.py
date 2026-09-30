import sys
sys.stdout.write(sys.stdin.read().replace(',}', '}').replace(',\n+]', '\n]').replace(',]', '\n]'))
