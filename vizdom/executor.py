#!/usr/bin/python
import numpy as np
import pandas as pd
from sklearn.cross_validation import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import SGDClassifier
from sklearn.metrics import auc
from sklearn.metrics import confusion_matrix
from sklearn.metrics import roc_curve

from result import ClassifyResult
from result import Result
from result import TransformResult

class Executor():
    def __init__(self, cache, task):
        self.cache = cache
        self.task = task

    def execute(self):
        task_type = self.task['type']
        task_uuid = self.task['uuid']
        task_args = self.task['args']

        target = None
        if task_type == 'classify':
            self.classify(task_uuid, task_args)
        elif task_type == 'difference':
            self.difference(task_uuid, task_args)
        elif task_type == 'intersect':
            self.intersect(task_uuid, task_args)
        elif task_type == 'project':
            self.project(task_uuid, task_args)
        elif task_type == 'select':
            self.select(task_uuid, task_args)
        elif task_type == 'union':
            self.union(task_uuid, task_args)

    def classify(self, uuid, args):
        raise NotImplementedError()

    def difference(self, uuid, args):
        raise NotImplementedError()

    def intersect(self, uuid, args):
        raise NotImplementedError()

    def project(self, uuid, args):
        raise NotImplementedError()

    def select(self, uuid, args):
        raise NotImplementedError()

    def union(self, uuid, args):
        raise NotImplementedError()

class BasicExecutor(Executor):
    def __init__(self, cache, task):
        Executor.__init__(self, cache, task)

    def classify(self, uuid, args):
        classifier = args['classifier']
        classifier_type = classifier['type']
        classifier_params = classifier['params']
        classifier_labels = classifier['labels']
        classifier_features = classifier['features']

        b = None
        if classifier_type == 'forest':
            b = RandomForestClassifier(n_jobs=-1, random_state=0)
            #n_estimators = 100
            #max_features = None
            #b = RandomForestClassifier(n_estimators=n_estimators,
            #            max_features=max_features, n_jobs=-1, random_state=0)
        elif classifier_type == 'logreg':
            b = SGDClassifier(loss='log', class_weight='auto', random_state=0)
        elif classifier_type == 'svm':
            b = SGDClassifier(loss='squared_hinge', class_weight='auto', random_state=0)

        Y_data = self.cache[classifier_labels]
        X_data = self.cache[classifier_features]

        labels = Y_data.columns
        B = {label: b for label in labels}
        Y = {label: np.array(Y_data[label]) for label in labels}
        X = np.array(X_data)
        Y_pred = {}
        stats = {}
        for label, y in Y.items():
            X_train, X_test, y_train, y_test = train_test_split(X, y,
                                                       test_size=0.4,
                                                       random_state=0)
            b = B[label]
            b.fit(X_train, y_train)

            y_prob = None
            if classifier_type == 'svm':
                bb = SGDClassifier(loss='log', random_state=0)
                bb.fit(b.predict(X_train).reshape((-1,1)), y_train)
                y_prob = bb.predict_proba(b.predict(X_test).reshape((-1,1)))
            else:
                y_prob = b.predict_proba(X_test)
            y_pred = [1 if y[1] >= 0.5 else 0 for y in y_prob]
            Y_pred[label] = y_pred

            cm = confusion_matrix(y_test, y_pred)
            tp = float(cm[1][1])
            fp = float(cm[1][0])
            tn = float(cm[0][0])
            fn = float(cm[0][1])

            #precision_0 = tp / (tp + fp)
            #precision_1 = tn / (tn + fn)
            #precision = (precision_0 + precision_1) * 0.5
            precision = 1

            #recall_0 = tn / (tn + fp)
            #recall_1 = tp / (tp + fn)
            #recall = (recall_0 + recall_1) * 0.5
            recall = 1

            #f1_0 = 2 * tn / (2 * tn + tp + fp)
            #f1_1 = 2 * tp / (2 * tp + fp + fn)
            #f1 = (f1_0 + f1_1) * 0.5
            f1 = 1

            fpr, tpr, thresholds = roc_curve(y_test, y_prob[:, 1])
            roc_auc = auc(fpr, tpr)

            stats[label] = {'tp': tp,
                            'fp': fp,
                            'tn': tn,
                            'fn': fn,
                            'precision': precision,
                            'recall': recall,
                            'f1': f1,
                            'fpr': fpr.tolist(),
                            'tpr': tpr.tolist(),
                            'auc': roc_auc}

        Y_corr = {}
        for label1, y_pred1 in Y_pred.items():
            for label2, y_pred2 in Y_pred.items():
                key1 = str((label1,label2))
                key2 = str((label2,label1))
                if label1 == label2:
                    Y_corr[key1] = 1.0
                elif key1 not in Y_corr and key2 not in Y_corr:
                    y_corr = np.corrcoef(y_pred1, y_pred2)[0][1]
                    Y_corr[key1] = y_corr
                    Y_corr[key2] = y_corr

        if len(Y_corr) > 1:
            stats['corr'] = Y_corr

        self.cache[uuid] = ClassifyResult(None, 1.0, 0.0, B, stats)

    def difference(self, uuid, args):
        source1 = args['source1']
        source2 = args['source2']
        result = self.cache[source1]
        result = result[~result.isin(self.cache[source2])]
        self.cache[uuid] = result

    def intersect(self, uuid, args):
        source = args['source']
        head = source[0]
        result = self.cache[head]
        for s in source[1:]:
            result = result[result.isin(s)].dropna(how='all')
        self.cache[uuid] = result

    def project(self, uuid, args):
        source = args['source']
        attributes = args['attributes']
        self.cache[uuid] = self.cache[source][attributes]

    def select(self, uuid, args):
        source = args['source']
        predicate = args['predicate']
        predicate = predicate.replace(' and ', ' & ')
        predicate = predicate.replace(' or ', ' | ')
        predicate = predicate.replace(' not ', ' ~')
        result = self.cache[source].query(predicate)
        self.cache[uuid] = result

    def union(self, uuid, args):
        source = args['source']
        result = pd.concat([self.cache[s] for s in source]).drop_duplicates()
        self.cache[uuid] = result

class IncrementalExecutor(Executor):
    def __init__(self, cache, task):
        Executor.__init__(self, cache, task)

    def classify(self, uuid, args):
        raise NotImplementedError()

    def difference(self, uuid, args):
        raise NotImplementedError()

    def intersect(self, uuid, args):
        raise NotImplementedError()

    def project(self, uuid, args):
        raise NotImplementedError()

    def select(self, uuid, args):
        raise NotImplementedError()

    def union(self, uuid, args):
        raise NotImplementedError()

class SparkExecutor(Executor):
    def __init__(self, cache, task):
        Executor.__init__(self, cache, task)

    def classify(self, uuid, args):
        raise NotImplementedError()

    def difference(self, uuid, args):
        raise NotImplementedError()

    def intersect(self, uuid, args):
        raise NotImplementedError()

    def project(self, uuid, args):
        raise NotImplementedError()

    def select(self, uuid, args):
        raise NotImplementedError()

    def union(self, uuid, args):
        raise NotImplementedError()

class TuplewareExecutor(Executor):
    def __init__(self, cache, task):
        Executor.__init__(self, cache, task)

    def classify(self, uuid, args):
        raise NotImplementedError()

    def difference(self, uuid, args):
        raise NotImplementedError()

    def intersect(self, uuid, args):
        raise NotImplementedError()

    def project(self, uuid, args):
        raise NotImplementedError()

    def select(self, uuid, args):
        raise NotImplementedError()

    def union(self, uuid, args):
        raise NotImplementedError()
