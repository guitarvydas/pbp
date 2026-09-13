"""
UTF-8 string pool with 16-bit descriptor indices.

Layout:
  - `block`: one contiguous bytearray holding all UTF-8-encoded string
    bytes, back to back, addressed by a 32-bit byte offset.
  - descriptors: parallel arrays of (length: uint16, offset: uint32),
    addressed by a 16-bit index (0..65535). Descriptor i describes the
    substring block[offsets[i] : offsets[i] + lengths[i]].

Strings are interned: encoding the same str twice returns the same index.
"""

import struct
from array import array


class Utf8StringPool:
    MAX_DESCRIPTORS = 1 << 16            # valid indices: 0..65535
    MAX_STRING_BYTES = (1 << 16) - 1     # length field is uint16
    MAX_BLOCK_BYTES = 1 << 32            # offset field is uint32

    def __init__(self):
        if array('H').itemsize != 2 or array('I').itemsize != 4:
            # True on every mainstream CPython platform (Linux/macOS/Windows,
            # 32- or 64-bit). Guarded here because the array module only
            # guarantees a *minimum* width, not an exact one.
            raise RuntimeError("platform array typecodes aren't 16/32-bit")

        self.block = bytearray()
        self.lengths = array('H')   # uint16 byte-lengths, parallel to offsets
        self.offsets = array('I')   # uint32 byte offsets into block
        self._index: dict[str, int] = {}   # str -> descriptor index

    def __len__(self):
        return len(self.lengths)

    def intern(self, s: str) -> int:
        idx = self._index.get(s)
        if idx is not None:
            return idx

        if len(self.lengths) >= self.MAX_DESCRIPTORS:
            raise OverflowError(f"descriptor table full (max {self.MAX_DESCRIPTORS})")

        data = s.encode('utf-8')
        if len(data) > self.MAX_STRING_BYTES:
            raise ValueError(f"string is {len(data)} bytes, max is {self.MAX_STRING_BYTES}")

        offset = len(self.block)
        if offset + len(data) > self.MAX_BLOCK_BYTES:
            raise OverflowError("block exceeds 32-bit addressable range")

        idx = len(self.lengths)
        self.block.extend(data)
        self.lengths.append(len(data))
        self.offsets.append(offset)
        self._index[s] = idx
        return idx

    def get(self, idx: int) -> str:
        return self.get_bytes(idx).decode('utf-8')

    def get_bytes(self, idx: int) -> bytes:
        offset = self.offsets[idx]
        length = self.lengths[idx]
        return bytes(self.block[offset:offset + length])

    def __contains__(self, s: str) -> bool:
        return s in self._index

    # ---- optional: serialize to a single flat binary blob ----

    def to_bytes(self) -> bytes:
        """
        [uint32 descriptor_count]
        [descriptor_count * (uint16 length, uint32 offset)]
        [uint32 block_length][block bytes]
        All fields little-endian.
        """
        header = struct.pack('<I', len(self.lengths))
        descriptors = b''.join(
            struct.pack('<HI', self.lengths[i], self.offsets[i])
            for i in range(len(self.lengths))
        )
        block_part = struct.pack('<I', len(self.block)) + bytes(self.block)
        return header + descriptors + block_part

    @classmethod
    def from_bytes(cls, buf: bytes) -> "Utf8StringPool":
        pool = cls()
        (count,) = struct.unpack_from('<I', buf, 0)
        offset = 4
        descs = []
        for _ in range(count):
            length, block_off = struct.unpack_from('<HI', buf, offset)
            descs.append((length, block_off))
            offset += 6
        (block_len,) = struct.unpack_from('<I', buf, offset)
        offset += 4
        pool.block = bytearray(buf[offset:offset + block_len])
        pool.lengths = array('H', (d[0] for d in descs))
        pool.offsets = array('I', (d[1] for d in descs))
        pool._index = {pool.get(i): i for i in range(len(pool.lengths))}
        return pool


if __name__ == "__main__":
    pool = Utf8StringPool()
    i1 = pool.intern("héllo")
    i2 = pool.intern("wörld")
    i3 = pool.intern("héllo")  # re-interned, same index as i1

    assert i1 == i3
    print(f"'héllo' -> index {i1}, offset {pool.offsets[i1]}, "
          f"length {pool.lengths[i1]} bytes")
    print(f"'wörld' -> index {i2}")
    print("decoded back:", pool.get(i1), pool.get(i2))

    blob = pool.to_bytes()
    restored = Utf8StringPool.from_bytes(blob)
    assert restored.get(i1) == "héllo"
    print(f"round-tripped through {len(blob)} bytes OK")
