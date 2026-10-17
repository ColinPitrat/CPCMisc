#!/usr/bin/env python3
"""Prepend a 128-byte AMSDOS header to a raw binary file."""
import os
import struct
import sys


def add_amsdos_header(in_path, out_path, load_addr=0xC000, exec_addr=0x0000, file_type=2):
    with open(in_path, "rb") as f:
        data = f.read()
    length = len(data)
    header = bytearray(128)
    base = os.path.splitext(os.path.basename(in_path))[0].upper()[:8].ljust(8)
    ext = os.path.splitext(os.path.basename(in_path))[1].lstrip(".").upper()[:3].ljust(3)
    header[0] = 0
    header[1:9] = base.encode("ascii")
    header[9:12] = ext.encode("ascii")
    header[18] = file_type
    struct.pack_into("<H", header, 21, load_addr)
    header[23] = 0xFF
    struct.pack_into("<H", header, 24, length)
    struct.pack_into("<H", header, 26, exec_addr)
    struct.pack_into("<H", header, 64, length & 0xFFFF)
    header[66] = (length >> 16) & 0xFF
    checksum = sum(header[:67]) & 0xFFFF
    struct.pack_into("<H", header, 67, checksum)
    with open(out_path, "wb") as f:
        f.write(header)
        f.write(data)


if __name__ == "__main__":
    args = sys.argv[1:]
    load_addr = 0xC000
    if len(args) >= 2 and args[0] == "-s":
        load_addr = int(args[1], 0)
        args = args[2:]
    if len(args) != 2:
        sys.exit("Usage: addhead.py [-s <start_addr>] <input.bin> <output.bin>")
    add_amsdos_header(args[0], args[1], load_addr=load_addr)
