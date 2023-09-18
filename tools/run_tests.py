import multiprocessing
import argparse
import os
import subprocess
from sys import argv

if __name__ == '__main__':
  parser = argparse.ArgumentParser(
    prog = argv[0],
    description = 'Helper for qcc unit test suite',
  )
  parser.add_argument('path', type=str)
  args = parser.parse_args()
  tests = [path for path in os.listdir(args.path) if path.startswith('unit_')]
  print(tests)

