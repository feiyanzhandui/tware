#!/usr/bin/python
from multiprocessing import Manager

class Cache():
    def __init__(self):
        self.mgr = Manager()
        #self.cache = self.mgr.dict()
        self.cache = {}

    def __getitem__(self, key):
        return self.cache[key]

    def __setitem__(self, key, val):
        self.cache[key] = val

    def __delitem__(self, key):
        del self.cache[key]
