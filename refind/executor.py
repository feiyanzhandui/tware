#!/usr/bin/python
import json
import numpy as np
import pandas as pd
import time
from multiprocessing import Process
from itertools import combinations
from sklearn.cross_validation import train_test_split
from sklearn.feature_selection import f_classif
from sklearn.feature_selection import SelectPercentile
from sklearn.metrics import auc
from sklearn.metrics import confusion_matrix
from sklearn.metrics import roc_curve
from sklearn.utils import compute_class_weight

from task import ClassifyTask
from task import CorrelateTask
from task import DifferenceTask
from task import FeatureSelectTask
from task import FrequentItemsetsTask
from task import IntersectTask
from task import LoadTask
from task import MergeTask
from task import ProjectTask
from task import SelectTask
from task import UnionTask
from result import ClassifyResult
from result import ErrorResult
from result import TransformResult
from util import classify_stats
from util import get_classifier
from util import Timer

TILE_SIZE = 1000
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import classification_report

class Executor(Process):
    def __init__(self, catalog, results, task):
        Process.__init__(self)
        self.catalog = catalog
        self.results = results
        self.task = task
        self.timer = Timer()

    def get_result(self, uuid):
        result = self.results[uuid]
        while result.complete == 0.0:
            time.sleep(0.0005)
            result = self.results[uuid]
        return result

    def wait(self, uuid):
        while self.results[uuid].complete == 0.0:
            time.sleep(0.0005)

    def run(self):
        self.timer.start()
        try:
            if isinstance(self.task, ClassifyTask):
                self.classify()
            elif isinstance(self.task, CorrelateTask):
                self.correlate()
            elif isinstance(self.task, DifferenceTask):
                self.difference()
            elif isinstance(self.task, FeatureSelectTask):
                self.feature_select()
            elif isinstance(self.task, FrequentItemsetsTask):
                self.frequent_itemsets()
            elif isinstance(self.task, IntersectTask):
                self.intersect()
            elif isinstance(self.task, LoadTask):
                self.load()
            elif isinstance(self.task, MergeTask):
                self.merge()
            elif isinstance(self.task, ProjectTask):
                self.project()
            elif isinstance(self.task, SelectTask):
                self.select()
            elif isinstance(self.task, UnionTask):
                self.union()
            else:
                raise NotImplementedError()
        except Exception as e:
            print str(e)
            result = ErrorResult(self.task, str(e))
            self.results[self.task.uuid] = result
        self.timer.stop()
        print 'task' + str(self.task.uuid) + ': ' + str(self.timer.time()) + 's'

    def classify(self):
        raise NotImplementedError()

    def correlate(self):
        raise NotImplementedError()

    def difference(self):
        raise NotImplementedError()

    def feature_select(self):
        raise NotImplementedError()

    def frequent_itemsets(self):
        raise NotImplementedError()

    def intersect(self):
        raise NotImplementedError()

    def load(self):
        raise NotImplementedError()

    def merge(self):
        raise NotImplementedError()

    def project(self):
        raise NotImplementedError()

    def select(self):
        raise NotImplementedError()

    def union(self):
        raise NotImplementedError()

class BasicExecutor(Executor):
    def __init__(self, catalog, results, task):
        Executor.__init__(self, catalog, results, task)

    def classify(self):
        y_data = self.get_result(self.task.label)
        X_data = self.get_result(self.task.features)

        y = np.array(y_data.data).ravel()
        X = np.array(pd.get_dummies(X_data.data))
        #X = MinMaxScaler().fit_transform(X)

        X_train = X[:-TILE_SIZE]
        y_train = y[:-TILE_SIZE]
        X_test = X[-TILE_SIZE:]
        y_test = y[-TILE_SIZE:]

        cw = compute_class_weight('auto', np.array([0,1]), y)
        cw = {0:cw[0],1:cw[1]}

        b = get_classifier(self.task.classifier, cw)
        b.partial_fit(X_train, y_train, classes=np.array([0,1]))

        y_prob = None
        y_pred = None
        if self.task.classifier in ['perceptron','svm']:
            y_pred = b.predict(X_test)
            y_prob = np.array([[0,y] for y in y_pred])
        else:
            y_prob = b.predict_proba(X_test)
            y_pred = [1 if t[0] >= 0.5 else 0 for t in y_prob]

        cm = confusion_matrix(y_test, y_pred)
        stats = classify_stats(cm, y_test, y_prob, TILE_SIZE)

        result = ClassifyResult(self.task, 1.0, b, stats)
        self.results[self.task.uuid] = result

    def correlate(self):
        source = self.results[self.task.source]
        #wait(source)
        result = TransformResult(self.task, 1.0, source.data.corr())
        self.results[self.task.uuid] = result

    def difference(self):
        left = self.results[self.task.left]
        #wait(left)
        ldata = left.data
        right = self.results[self.task.right]
        #wait(right)
        rdata = right.data
        data = ldata[~ldata.isin(rdata)]
        result = TransformResult(self.task, 1.0, data)
        self.results[self.task.uuid] = result

    def feature_select(self):
        b = SelectPercentile(f_classif, percentile=task.percentile)
        y = np.array(self.results[self.task.label].data)
        X = np.array(self.results[self.task.features].data)
        data = pd.DataFrame(b.fit_transform(X, y))
        result = TransformResult(self.task, 1.0, data)
        self.results[self.task.uuid] = result

    def frequent_itemsets(self):
        source = self.get_result(self.task.source)
        data = source.data
        size = float(len(data.index))
        print 'size',size
        itemsets = []
        C = set(data.columns)
        k = 1
        while len(C) > k:
            C_next = set()
            for c in combinations(C, k):
                support = pd.DataFrame(data[list(c)] != 0).product(1).sum(0) / size
                if support > self.task.support:
                    itemsets.append((str(c),support))
                    for x in c:
                        C_next.add(x)
            C = C_next
            k += 1
        itemsets = pd.DataFrame(itemsets, columns=['items','support'])
        print 'len:', len(itemsets.index)
        print 'final:', itemsets
        result = TransformResult(self.task, 1.0, itemsets)
        self.results[self.task.uuid] = result

    def intersect(self):
        sources = [self.results[s] for s in self.task.sources]
        #wait(sources[0])
        data = sources[0].data
        for s in sources[1:]:
            #wait(s)
            data = data[data.isin(s.data)].dropna(how='all')
        result = TransformResult(self.task, 1.0, data)
        self.results[self.task.uuid] = result

    def load(self):
        name = self.task.name
        schema_file = self.task.schema_file
        data_file = self.task.data_file
        data = pd.read_csv(data_file)

        schema = None
        if schema_file is None:
            attrs = data.columns
            dtypes = data.dtypes
            schema = [(attr,str(dtypes[attr])) for attr in attrs]
        else:
            with open(self.task.schema_file) as f:
                schema = json.load(f)
        self.catalog[name] = {'uuid': self.task.uuid, 'schema': schema}

        result = TransformResult(self.task, 1.0, data)
        self.results[self.task.uuid] = result

    def merge(self):
        sources = pd.DataFrame(self.get_result(self.task.sources[0]).y_pred)
        for source in self.task.sources[1:]:
            s = pd.DataFrame(self.get_result(source).y_pred)
            sources = pd.merge(sources, s, left_index=True, right_index=True, how='outer')
        result = TransformResult(self.task, 1.0, sources)
        self.results[self.task.uuid] = result

    def project(self):
        self.wait(self.task.source)
        source = self.results[self.task.source]
        data = pd.DataFrame(source.data[self.task.attributes])
        result = TransformResult(self.task, 1.0, data)
        self.results[self.task.uuid] = result

    def select(self):
        pred = self.task.predicate
        pred = pred.replace(' and ', ' & ')
        pred = pred.replace(' or ', ' | ')
        pred = pred.replace(' not ', ' ~')
        source = self.results[self.task.source]
        #wait(source)
        data = source.data.query(pred)
        result = TransformResult(self.task, 1.0, data)
        self.results[self.task.uuid] = result

    def union(self):
        data = pd.concat([self.results[s].data
                       for s in self.task.sources]).drop_duplicates()
        result = TransformResult(self.task, 1.0, data)
        self.results[self.task.uuid] = result

class IncrementalExecutor(Executor):
    def __init__(self, catalog, results, task):
        Executor.__init__(self, catalog, results, task)
        pos = 0

    def classify(self):
        y_data = self.results[self.task.label].data
        X_data = self.results[self.task.features].data
        y = np.array(y_data)
        X = np.array(X_data)

        cw = compute_class_weight('auto', np.array([0,1]), y)
        cw = {0:cw[0],1:cw[1]}
        print cw

        b = get_classifier(self.task.classifier, cw)

        tile_size = 1000
        num_tiles = y.size / tile_size
        for i in range(num_tiles):
            pos = i * tile_size
            X_sub = X[pos : pos + tile_size]
            y_sub = y[pos : pos + tile_size]

            y_prob = None
            y_pred = None
            if self.task.classifier == 'svm':
                y_pred = b.predict(X_sub)
                y_prob = np.array([[0,y] for y in y_pred])
            else:
                y_prob = b.predict_proba(X_sub)
                y_pred = [1 if y[1] >= 0.5 else 0 for y in y_prob]

            cm = confusion_matrix(y_sub, y_pred)
            stats = classify_stats(cm, y_test, y_prob)

            y_pred = pd.DataFrame(y_pred, columns=y_data.columns)
            result = ClassifyResult(self.task, 1.0, b, stats)
            self.results[self.task.uuid] = result

            b.partial_fit(X_sub, y_sub)

    def correlate(self):
        raise NotImplementedError()

    def difference(self):
        raise NotImplementedError()

    def feature_select(self):
        raise NotImplementedError()

    def intersect(self):
        raise NotImplementedError()

    def load(self):
        schema_file = self.task.schema_file
        data_file = self.task.data_file
        data = pd.read_csv(data_file, nrows=0)

        schema = None
        if schema_file is None:
            attrs = data.columns
            dtypes = data.dtypes
            schema = [(attr,str(dtypes[attr])) for attr in attrs]
        else:
            with open(self.task.schema_file) as f:
                schema = json.load(f)
        self.catalog[data_file] = {'uuid': self.task.uuid, 'schema': schema}

        for chunk in pd.read_csv(data_file, chunksize=1000):
            data.append(chunk, ignore_index=True)
            result = TransformResult(self.task, 1.0, data) #actual size
            self.results[self.task.uuid] = result

    def project(self):
        raise NotImplementedError()

    def select(self):
        raise NotImplementedError()

    def union(self):
        raise NotImplementedError()

class SparkExecutor(Executor):
    def __init__(self, catalog, results, task):
        Executor.__init__(self, catalog, results, task)

    def classify(self):
        raise NotImplementedError()

    def correlate(self):
        raise NotImplementedError()

    def difference(self):
        raise NotImplementedError()

    def feature_select(self):
        raise NotImplementedError()

    def intersect(self):
        raise NotImplementedError()

    def load(self):
        raise NotImplementedError()

    def merge(self):
        raise NotImplementedError()

    def project(self):
        raise NotImplementedError()

    def select(self):
        raise NotImplementedError()

    def union(self):
        raise NotImplementedError()

class TuplewareExecutor(Executor):
    def __init__(self, catalog, results, task):
        Executor.__init__(self, catalog, results, task)

    def classify(self):
        raise NotImplementedError()

    def correlate(self):
        raise NotImplementedError()

    def difference(self):
        raise NotImplementedError()

    def feature_select(self):
        raise NotImplementedError()

    def intersect(self):
        raise NotImplementedError()

    def load(self):
        raise NotImplementedError()

    def merge(self):
        raise NotImplementedError()

    def project(self):
        raise NotImplementedError()

    def select(self):
        raise NotImplementedError()

    def union(self):
        raise NotImplementedError()
