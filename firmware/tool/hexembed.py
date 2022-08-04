#!/usr/bin/python3

import io
import sys
from lib.cwriter import cwriter

def main():

  # Check for correct number of arguments
  if len(sys.argv) != 3:
    print("Usage: hexembed.py <pathtofile> <output>")
    sys.exit(1)

  # Read the file
  fp = open(sys.argv[1], "rb")
  fp.seek(io.SEEK_SET, 0)
  file = fp.read()
  fp.close()

  # Write the file
  writer = cwriter()
  writer.open(sys.argv[1], sys.argv[2])
  writer.append(file)
  writer.writeall()
  writer.close()

if __name__ == "__main__":
  main()
