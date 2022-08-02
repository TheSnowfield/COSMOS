#!/usr/bin/python3

import io
import sys

def main():

  # Check for correct number of arguments
  if len(sys.argv) != 3:
    print("Usage: hexembed.py <pathtofile> <output>")
    sys.exit(1)

  # Read the file
  file = readfile(sys.argv[1])
  output = open(sys.argv[2], "wt")

  fname = filename(sys.argv[1])
  cfname = fname.replace(".", "_")
  cmacro = cfname.upper()

  # Write the file
  output.write("#ifndef _EMBEDDED_%s_H\n" % cmacro)
  output.write("#define _EMBEDDED_%s_H\n\n" % cmacro)
  output.write("/* Embedded file: %s */\n" % fname)
  output.write("const unsigned char %s[] = {\n" % cfname)
  for i in range(0, len(file), 16):
    output.write("\t")
    for j in range(0, 16):
      if i + j < len(file):
        output.write("0x%02x, " % ord(file[i + j]))
      else:
        output.write("0x00, ")
    output.write("\n")
  output.write("};\n\n")
  output.write("#endif\n")
  output.close()

def readfile(file):
  fp = open(file, "r")
  fp.seek(io.SEEK_SET, 0)

  bin = fp.read()
  fp.close()
  return bin

def filename(file: str):
  split = file.replace("\\", "/").split("/")
  return split[len(split) - 1]

if __name__ == "__main__":
  main()
