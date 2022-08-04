#!/usr/bin/python3

import sys
from PIL import Image
from distutils.log import error
from lib.cwriter import cwriter

def main():

  # Check for correct number of arguments
  if len(sys.argv) != 3:
    print("Usage: mktexture.py <bmpfile> <output>\n")
    print("Please note this script only supported 8 bit bmp file.")
    sys.exit(1)

  # Read the image
  image = Image.open(sys.argv[1])
  width, height = image.size
  print("Image size: %d x %d" % (width, height))

  if width > 32 or height > 128:
    error("Image is too large, limit is 32(w) x 128(h)")
    sys.exit(1)

  # Create writer
  writer = cwriter()
  writer.open(sys.argv[1], sys.argv[2])

  # Read image pixels
  pixels = image.convert("L").load()
  for page in range(int(width / 8)):
    for column in range(height):

      # Read each column
      bits = 0
      for row in range(8):
        bits <<= 1
        if(pixels[page * 8 + row, column] == 255): bits |= 1
    
      # Write 8 pixels to file
      writer.append(bytes.fromhex("%02X" % bits))

  writer.writeall()
  writer.close()

if __name__ == "__main__":
  main()
