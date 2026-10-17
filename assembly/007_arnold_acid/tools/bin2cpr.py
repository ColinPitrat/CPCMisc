#!/usr/bin/env python3
"""Convert a raw binary file into an Amstrad Plus .cpr RIFF cartridge image."""
import struct
import sys


def build_cpr(bin_path, cpr_path):
    with open(bin_path, "rb") as f:
        data = f.read()
    if len(data) == 0:
        raise ValueError(f"Empty input binary: {bin_path}")
    chunks = []
    page_size = 16384
    num_pages = (len(data) + page_size - 1) // page_size
    for i in range(num_pages):
        page_data = data[i * page_size : (i + 1) * page_size]
        if len(page_data) < page_size:
            page_data = page_data + b"\x00" * (page_size - len(page_data))
        chunk_id = f"cb{i:02d}".encode("ascii")
        chunks.append(chunk_id + struct.pack("<I", page_size) + page_data)
    body = b"AMS!" + b"".join(chunks)
    riff = b"RIFF" + struct.pack("<I", len(body)) + body
    with open(cpr_path, "wb") as f:
        f.write(riff)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit(f"Usage: {sys.argv[0]} <input.bin> <output.cpr>")
    build_cpr(sys.argv[1], sys.argv[2])
