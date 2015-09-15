#!/usr/bin/python
import json
import pandas as pd

class Result():
    def __init__(self, task, complete):
        self.task = task
        self.complete = complete
        self.timestamp = 0

    def to_json(self):
        raise NotImplementedError()

class ClassifyResult(Result):
    def __init__(self, task, complete=0.0, classifier=None, stats={}):
        Result.__init__(self, task, complete)
        self.classifier = classifier
        self.stats = stats

    def get_classifier(self):
        return self.classifier

    def to_json(self):
        return json.dumps(self.stats)

class CorrelateResult(Result):
    def __init__(self, task):
        Result.__init__(self, task, 0.0)

    def to_json(self):
        return ''

class DifferenceResult(Result):
    def __init__(self, task):
        Result.__init__(self, task, 0.0)

    def to_json(self):
        return ''

class EmptyResult(Result):
    def __init__(self, task):
        Result.__init__(self, task, 0.0)

    def to_json(self):
        return json.dumps({'empty':True})

class ErrorResult(Result):
    def __init__(self, task, error):
        Result.__init__(self, task, 1.0)
        self.error = error

    def get_error(self):
        return self.error

    def to_json(self):
        return json.dumps({'error': self.error})

class FeatureSelectResult(Result):
    def __init__(self, task):
        Result.__init__(self, task, 0.0)

    def to_json(self):
        return ''

class IntersectResult(Result):
    def __init__(self, task):
        Result.__init__(self, task, 0.0)

    def to_json(self):
        return ''

class LoadResult(Result):
    def __init__(self, task):
        Result.__init__(self, task, 0.0)

    def to_json(self):
        return ''

class MergeResult(Result):
    def __init__(self, task):
        Result.__init__(self, task, 0.0)

    def to_json(self):
        return ''

class ProjectResult(Result):
    def __init__(self, task, complete, data=[]):
        Result.__init__(self, task, complete)
        self.data = data

    def get_data(self):
        return self.data

    def to_json(self):
        return self.data.to_json()

class SelectResult(Result):
    def __init__(self, task, complete, data):
        Result.__init__(self, task, complete)
        self.data = data

    def get_data(self):
        return self.data

    def to_json(self):
        return self.data.to_json()

class TransformResult(Result):
    def __init__(self, task, complete, data):
        Result.__init__(self, task, complete)
        self.data = data

    def get_data(self):
        return self.data

    def to_json(self):
        return self.data.to_json()

class UnionResult(Result):
    def __init__(self, task):
        Result.__init__(self, task, 0.0)

    def to_json(self):
        return ''
