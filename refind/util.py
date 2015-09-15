#!/usr/bin/python
import time
from sklearn.linear_model import SGDClassifier
from sklearn.metrics import auc
from sklearn.metrics import classification_report
from sklearn.metrics import roc_curve
from sklearn.naive_bayes import GaussianNB

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

def get_classifier(classifier, cw):
    if classifier == 'logistic_regression':
        return SGDClassifier(loss='log', shuffle=False, class_weight=cw)
    if classifier == 'naive_bayes':
        return GaussianNB()
    elif classifier == 'perceptron':
        return SGDClassifier(loss='perceptron', shuffle=False, class_weight=cw)
    elif classifier == 'svm':
        return SGDClassifier(loss='hinge', shuffle=False, class_weight=cw)
    raise NotImplementedError()

def classify_test(b, y_test):
    pass

def classify_stats(cm, y_test, y_prob, tile_size):
    #print cm
    #print classification_report(y_test, y_pred)
    tp = float(cm[1][1])
    fp = float(cm[0][1])
    tn = float(cm[0][0])
    fn = float(cm[1][0])
    #precision = tp / (tp + fp)
    #recall = tp / (tp + fn)
    #f1 = 2 * tp / (2 * tp + fp + fn)
    p_support = (tp + fn) / tile_size
    n_support = (tn + fp) / tile_size
    precision = tp / max((tp + fp), 1) * p_support + tn / max((tn + fn), 1) * n_support
    recall = tp / max((tp + fn), 1) * p_support + tn / max((tn + fp), 1) * n_support
    f1 = 2 * precision * recall / (precision + recall)
    fpr, tpr, thresholds = roc_curve(y_test, y_prob[:, 1])
    roc_auc = auc(fpr, tpr)
    stats = {'tp': tp,
             'fp': fp,
             'tn': tn,
             'fn': fn,
             'precision': precision,
             'recall': recall,
             'f1': f1,
             'fpr': fpr.tolist(),
             'tpr': tpr.tolist(),
             'auc': roc_auc}
    return stats
