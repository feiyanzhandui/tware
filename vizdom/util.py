#!/usr/bin/python
import time

class Timer():
    def __init__(self):
        self.total_time = 0.0
        self.start_time = 0.0

    def time(self):
        return self.total_time

    def start(self):
        self.start_time = time.time()

    def stop(self):
        stop_time = time.time() if self.start_time > 0.0 else 0.0
        self.total_time += stop_time - self.start_time
        self.start_time = 0.0

    def reset():
        self.total_time = 0.0
        self.start_time = 0.0
