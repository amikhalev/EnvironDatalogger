#!/usr/bin/env python3

import datetime, sys, struct, os

time = datetime.datetime.now()

time_bin = struct.pack("<HBBBBB", time.year, time.month,
                       time.day, time.hour, time.minute, time.second)

write_bin = sys.stdout.buffer.write
write_bin(b"t")
write_bin(time_bin)
write_bin(b"\n")
