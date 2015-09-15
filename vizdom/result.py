#!/usr/bin/python
import json

class Result():
    def __init__(self, task, complete, error):
        self.task = task
        self.complete = complete
        self.error = error
        self.timestamp = 0

    def to_json(self):
        raise NotImplementedError()

class ClassifyResult(Result):
    def __init__(self, task, complete, error, classifiers, stats):
        Result.__init__(self, task, complete, error)
        self.classifiers = classifiers
        self.stats = stats

    def to_json(self):
        return json.dumps(self.stats)

class TransformResult(Result):
    def __init__(self, task, complete, error, data):
        Result.__init__(self, task, complete, error)
        self.data = data

    def to_json(self):
        return self.data.to_json()
