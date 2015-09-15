#!/usr/bin/python
import numpy as np
import random
import sys
from BaseHTTPServer import BaseHTTPRequestHandler
from BaseHTTPServer import HTTPServer
from csv import DictReader
from json import dumps
from json import loads
from sklearn.cross_validation import StratifiedShuffleSplit
from sklearn.cross_validation import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import auc
from sklearn.metrics import confusion_matrix
from sklearn.metrics import roc_curve
from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier

class Server(HTTPServer):
    def __init__(self, addr, handler, filedir):
        HTTPServer.__init__(self, addr, handler)
        mimic2_file = 'mimic2_old'
        self.files = {}
        self.files['mimic2'] = self.load_mimic2(filedir + mimic2_file)

    def load_mimic2(self, filename):
        icd9s = [('infectious',1,140),
                 ('metabolic',240,280),
                 ('blood',280,290),
                 ('neurologic',320,390),
                 ('heart_hypertensive',401,406),
                 ('heart_ischemic',410,415),
                 ('heart_failure',428,429),
                 ('pulmonary',460,520),
                 ('digestive',520,580),
                 ('renal_insufficiency',580,630)]

        tasks = {'forest': {'num_estimators': {'default': 10,
                                               'min': 0,
                                               'max': 1000}},
                 'logreg': {},
                 'tree': {}}

        schema = [['icd9',[(icd9[0],False) for icd9 in icd9s]],
                  ['demo',[('sex',False),
                           ('age',True),
                           ('race',False),
                           ('marital',False),
                           ('religion',False)]],
                  ['phys',[('height',True),
                           ('weight',True),
                           ('bmi',True),
                           ('temperature',True),
                           ('heart_rate',True),
                           ('resp_rate',True),
                           ('systolic_bp',True),
                           ('diastolic_bp',True),
                           ('spo2',True),
                           ('sapsi',False),
                           ('sofa',False),
                           ('gsc',False),
                           ('braden',False)]],
                  ['blood',[['bmp',[('sodium',True),
                                    ('potassium',True),
                                    ('chloride',True),
                                    ('magnesium',True),
                                    ('calcium',True),
                                    ('anion_gap',True),
                                    ('bun',True),
                                    ('creatinine',True),
                                    ('glucose',True)]],
                            ['abg',[('ph',True),
                                    ('be',True),
                                    ('total_co2',True),
                                    ('total_o2',True),
                                    ('pco2',True),
                                    ('po2',True)]],
                            ['cbc',[('wbc',True),
                                    ('rbc',True),
                                    ('hgb',True),
                                    ('hct',True),
                                    ('mcv',True),
                                    ('mch',True),
                                    ('mchc',True),
                                    ('rdw',True),
                                    ('plates',True),
                                    ('neuts',True),
                                    ('lymphs',True),
                                    ('monos',True),
                                    ('basos',True),
                                    ('eos',True),
                                    ('pt',True),
                                    ('inr_pt',True),
                                    ('ptt',True)]],
                            ['cardiac',[('ckmb',True),
                                        ('cpkmb',True),
                                        ('ldh',True),
                                        ('bnp',True),
                                        ('tropi',True),
                                        ('tropt',True)]],
                            ['hepatic',[('total_bili',True),
                                        ('direct_bili',True),
                                        ('indirect_bili',True),
                                        ('albumin',True),
                                        ('tg',True)]]]]]

        attrs = [icd9[0] for icd9 in icd9s]
        data = []
        with open(filename, 'r') as f:
            reader = DictReader(f)
            attrs += reader.fieldnames[1:]
            for pt in reader:
                pt_icd9s = []
                for pt_icd9 in pt['icd9'].split():
                    try:
                        pt_icd9 = float(pt_icd9)
                        for icd9 in icd9s:
                            if pt_icd9 >= icd9[1] and pt_icd9 < icd9[2]:
                                pt_icd9s.append(icd9[0])
                                break
                    except:
                        pass
                del pt['icd9']
                try:
                    pt = {k: float(v) for k,v in pt.items()}
                    for icd9 in icd9s:
                        if icd9[0] in pt_icd9s:
                            pt[icd9[0]] = 1
                        else:
                            pt[icd9[0]] = 0
                    data.append(pt)
                except:
                    pass

        return {'tasks': tasks,
                'schema': schema,
                'attributes': attrs,
                'data': data}

class RequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        self.send_response(200)
        self.send_header('Content-type','application/json')
        self.end_headers()

        content_length = int(self.headers.getheader('content-length'))
        qry = loads(self.rfile.read(content_length))
        print qry

        cmd = qry['command']
        if cmd == 'files':
            self.wfile.write(dumps([k for k in self.server.files]))
        else:
            filename = qry['filename']
            f = self.server.files[filename]

            if cmd == 'tasks':
                self.wfile.write(dumps(f['tasks']))
            elif cmd == 'schema':
                self.wfile.write(dumps(f['schema']))
            else:
                data = f['data']

                if 'limit' in qry:
                    limit = int(qry['limit'])
                    page = int(qry['page']) if 'page' in qry else 0
                    start = page * limit
                    end = start + limit
                    data = data[start:end]

                if 'select' in qry:
                    select = qry['select'].split()
                    attrs = f['attributes']
                    pred = ' '.join(['d["' + tok + '"]' if tok in attrs else tok
                                     for tok in select])
                    data = [d for d in data if eval(pred)]

                if 'project' in qry:
                    project = qry['project'].split()
                    data = [{attr: d[attr] for attr in project} for d in data]

                if cmd == 'data':
                    self.wfile.write(dumps(data))
                elif cmd == 'classify':
                    #size = len(data) * 0.1
                    #data = random.sample(data, size)

                    labels = qry['labels'].split()
                    X = []
                    Y = {label: [] for label in labels}
                    for d in data:
                        X.append([d[attr] for attr in d if attr not in labels])
                        for label in labels:
                            Y[label].append(d[label])

                    if 'scale' in qry:
                        scale = qry['scale']
                        if scale == 'minmax':
                            X = MinMaxScaler().fit_transform(X)
                        elif scale == 'standard':
                            X = StandardScaler().fit_transform(X)

                    B = {}
                    classifier = qry['classifier']
                    if classifier == 'forest':
                        n_estimators = 100
                        max_features = None
                        B = {y: RandomForestClassifier(n_estimators=n_estimators,
                                        max_features=max_features, n_jobs=-1,
                                        random_state=0) for y in labels}
                    elif classifier == 'logreg':
                        B = {y: LogisticRegression(class_weight='auto',
                                        random_state=0) for y in labels}
                    elif classifier == 'tree':
                        B = {y: DecisionTreeClassifier(max_features=None,
                                        random_state=0) for y in labels}

                    Y_pred = {}
                    H = {}
                    for label, y in Y.items():
                        X_train, X_test, y_train, y_test = train_test_split(X, y,
                                                                   test_size=0.4,
                                                                   random_state=0)
                        b = B[label]
                        b.fit(X_train, y_train)
                        y_prob = b.predict_proba(X_test)
                        y_pred = [1 if y[1] >= 0.5 else 0 for y in y_prob]
                        Y_pred[label] = y_pred

                        cm = confusion_matrix(y_test, y_pred)
                        tp = float(cm[1][1])
                        fp = float(cm[1][0])
                        tn = float(cm[0][0])
                        fn = float(cm[0][1])

                        precision_0 = tp / (tp + fp)
                        precision_1 = tn / (tn + fn)
                        precision = (precision_0 + precision_1) * 0.5

                        recall_0 = tn / (tn + fp)
                        recall_1 = tp / (tp + fn)
                        recall = (recall_0 + recall_1) * 0.5

                        f1_0 = 2 * tn / (2 * tn + tp + fp)
                        f1_1 = 2 * tp / (2 * tp + fp + fn)
                        f1 = (f1_0 + f1_1) * 0.5

                        fpr, tpr, thresh = roc_curve(y_test, y_prob[:, 1])
                        roc_auc = auc(fpr, tpr)

                        H[label] = {'tp': tp,
                                    'fp': fp,
                                    'tn': tn,
                                    'fn': fn,
                                    'precision_0': precision_0,
                                    'precision_1': precision_1,
                                    'precision': precision,
                                    'recall_0': recall_0,
                                    'recall_1': recall_1,
                                    'recall': recall,
                                    'f1_0': f1_0,
                                    'f1_1': f1_1,
                                    'f1': f1,
                                    'fpr': fpr.tolist()[::10],
                                    'tpr': tpr.tolist()[::10],
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
                        H['corr'] = Y_corr

                    #print H
                    self.wfile.write(dumps(H))

def main():
    port = int(sys.argv[1])
    filedir = sys.argv[2]
    server = Server(('',port), RequestHandler, filedir)
    print 'Starting server on port: ' + str(port)
    server.serve_forever()

if __name__ == "__main__":
    main()
