#!/usr/bin/env python2

import datetime
import os
import sys
import struct
import subprocess
import time
import threading

import serial

class SetTime:
    def __init__(self, port, baud = 115200):
        self.ser = serial.Serial(port, baud)
        self.reader_running = None
        self.reader_thread = None

    def start_reader(self):
        self.reader_running = True
        self.reader_thread = threading.Thread(target=self.reader, name="reader")
        self.reader_thread.daemon = True
        self.reader_thread.start()

    def reader(self):
        while self.reader_running:
            try:
                data = self.ser.read(1)
                sys.stdout.write(data)
            except serial.SerialException:
                self.reader_running = False
                raise

    def set_time(self):
        try:
            self.start_reader()
            time.sleep(5)

            t = datetime.datetime.now()
            time_bin = struct.pack("<HBBBBB", t.year, t.month, t.day, t.hour, t.minute, t.second)

            print("==> setting time to {}".format(t))
            write_bin = self.ser.write
            write_bin(b"t")
            write_bin(time_bin)
            write_bin(b"\0\0\0\0")
            self.ser.flush()

            while True:
                time.sleep(1)
        except KeyboardInterrupt:
            print("interrupt")
            self.reader_running = False
            self.ser.cancel_read()

if __name__ == "__main__":
    port = subprocess.check_output(["bash", os.path.dirname(__file__) + "/find_port.sh"]).splitlines()[0]
    SetTime(port).set_time()
