#!/usr/bin/python
import json

def parse_task(uuid, task_msg):
    task_type = task_msg['type']
    if task_type == 'classify':
        classifier = task_msg['classifier']
        params = task_msg['params']
        label = task_msg['label']
        features = task_msg['features']
        return ClassifyTask(uuid, classifier, params, label, features)
    elif task_type == 'correlate':
        source = task_msg['source']
        return CorrelateTask(uuid, source)
    elif task_type == 'difference':
        left = task_msg['left']
        right = task_msg['right']
        return DifferenceTask(uuid, left, right)
    elif task_type == 'feature_select':
        label = task_msg['label']
        features = task_msg['features']
        percentile = int(task_msg['percentile'])
        return FeatureSelectTask(uuid, label, features, percentile)
    elif task_type == 'frequent_itemsets':
        source = task_msg['source']
        support = float(task_msg['support'])
        return FrequentItemsetsTask(uuid, source, support)
    elif task_type == 'intersect':
        sources = task_msg['sources']
        return IntersectTask(uuid, sources)
    elif task_type == 'load':
        name = task_msg['name']
        schema_file = None
        if 'schema_file' in task_msg:
            schema_file = task_msg['schema_file']
        data_file = task_msg['data_file']
        return LoadTask(uuid, name, schema_file, data_file)
    elif task_type == 'merge':
        sources = task_msg['sources']
        return MergeTask(uuid, sources)
    elif task_type == 'project':
        source = task_msg['source']
        attributes = task_msg['attributes']
        return ProjectTask(uuid, source, attributes)
    elif task_type == 'select':
        source = task_msg['source']
        predicate = task_msg['predicate']
        return SelectTask(uuid, source, predicate)
    elif task_type == 'union':
        sources = task_msg['sources']
        return UnionTask(uuid, sources)
    return None

class Task():
    def __init__(self, uuid):
        self.uuid = uuid

class ClassifyTask(Task):
    def __init__(self, uuid, classifier, params, label, features):
        Task.__init__(self, uuid)
        self.classifier = classifier
        self.params = params
        self.label = label
        self.features = features

class CorrelateTask(Task):
    def __init__(self, uuid, source):
        Task.__init__(self, uuid)
        self.source = source

class DifferenceTask(Task):
    def __init__(self, uuid, left, right):
        Task.__init__(self, uuid)
        self.left = left
        self.right = right

class FeatureSelectTask(Task):
    def __init__(self, uuid, label, features, percentile):
        Task.__init__(self, uuid)
        self.label = label
        self.features = features
        self.percentile = percentile

class FrequentItemsetsTask(Task):
    def __init__(self, uuid, source, support):
        Task.__init__(self, uuid)
        self.source = source
        self.support = support

class IntersectTask(Task):
    def __init__(self, uuid, sources):
        Task.__init__(self, uuid)
        self.sources = sources

class LoadTask(Task):
    def __init__(self, uuid, name, schema_file, data_file):
        Task.__init__(self, uuid)
        self.name = name
        self.schema_file = schema_file
        self.data_file = data_file

class MergeTask(Task):
    def __init__(self, uuid, sources):
        Task.__init__(self, uuid)
        self.sources = sources

class ProjectTask(Task):
    def __init__(self, uuid, source, attributes):
        Task.__init__(self, uuid)
        self.source = source
        self.attributes = attributes

class SelectTask(Task):
    def __init__(self, result_id, source, predicate):
        Task.__init__(self, result_id)
        self.source = source
        self.predicate = predicate

class UnionTask(Task):
    def __init__(self, result_id, sources):
        Task.__init__(self, result_id)
        self.sources = sources
