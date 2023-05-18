import io
import os

class binwriter:
  def __init__(self) -> None:
    pass

  def open(self, ifile: str, ofile: str) -> None:
    self.fp = open(ofile, "wb")
    self.fp.seek(io.SEEK_SET, 0)
    self.data = bytearray()

  def writeall(self) -> None:
    self.fp.write(self.data)

  def append(self, data: bytes) -> None:
    self.data += bytearray(data)

  def close(self) -> None:
    self.fp.close()
    self.fp = None
