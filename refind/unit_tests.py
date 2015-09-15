from test_utils import *
import unittest

class Tests(unittest.TestCase):
    def test_load(self):
        reset()
        load("examples/mimic2_schema.json","examples/sample.csv")
        self.assertEquals(len(catalog().keys()),1)

    def test_project(self):
        reset()
        attrs = ["age","sex","weight"]
        r = load("examples/mimic2_schema.json","examples/sample.csv")
        r = project(r['uuid'], attrs)
        cols = lookup(r['uuid'])[0].keys()
        for a in attrs:
            self.assertTrue(a in cols)
        self.assertTrue(len(cols) == len(attrs))

    def test_select(self):
        reset()
        r = load("examples/mimic2_schema.json","examples/sample.csv")
        r = select(r['uuid'], "age<40")

        for t in lookup(r['uuid']):
            age = float(t['age'])
            self.assertTrue(age < 40)

    def test_classify(self):
        reset()
        r = load("examples/mimic2_schema.json","examples/sample.csv")
        feats = project(r['uuid'], ["age","sex","weight"])
        lab = project(r['uuid'], ["metabolic"])
        c = classify(lab['uuid'], feats['uuid'])
        vals = ["fp","tp","fn","tn","f1","auc","precision","recall","tpr","fpr"]
        res = lookup(c['uuid'])
        for k in res.keys():
            self.assertTrue(k in vals)
        self.assertTrue(len(res.keys()) == len(vals))

    def test_select_classify(self):
        reset()
        r = load("examples/mimic2_schema.json","examples/sample.csv")
        r = select(r['uuid'], "age<40")
        for t in lookup(r['uuid'],100,0):
            age = float(t['age'])
            self.assertTrue(age < 40)
        feats = project(r['uuid'], ["age","sex","weight"])
        lab = project(r['uuid'], ["metabolic"])
        c = classify(lab['uuid'], feats['uuid'])
        vals = ["fp","tp","fn","tn","f1","auc","precision","recall","tpr","fpr"]
        res = lookup(c['uuid'])
        for k in res.keys():
            self.assertTrue(k in vals)
        self.assertTrue(len(res.keys()) == len(vals))
    '''
    def test_difference(self):
        reset()
        r = load("examples/mimic2_schema.json","examples/sample.csv")
        left = select(r['uuid'], "age<50")
        right = select(r['uuid'], "age<18")
        res = difference(left['uuid'], right['uuid'])
        for t in lookup(res['uuid']):
            if t['age'] != None:
                age = float(t['age'])
                self.assertTrue(age > 18 and age < 50)

    def test_intersect(self):
        reset()
        r = load("examples/mimic2_schema.json","examples/sample.csv")
        r1 = select(r['uuid'], "age>30")
        r2 = select(r['uuid'], "age<55")
        res = intersect([r1['uuid'], r2['uuid']])
        for t in lookup(res['uuid']):
            if t['age'] != None:
                age = float(t['age'])
                self.assertTrue(age < 55 and age > 30)

    def test_union(self):
        reset()
        r = load("examples/mimic2_schema.json","examples/sample.csv")
        r1 = select(r['uuid'], "age<18")
        r2 = select(r['uuid'], "age>65")
        res = union([r1['uuid'], r2['uuid']])
        for t in lookup(res['uuid']):
            if t['age'] != None:
                age = float(t['age'])
                self.assertTrue(age < 18 or age > 65)
    '''
    def test_duplicate(self):
        reset()
        load("examples/mimic2_schema.json","examples/sample.csv")
        self.assertEquals(len(catalog().keys()),1)
        load("examples/mimic2_schema.json","examples/sample.csv")
        self.assertEquals(len(catalog().keys()),1)

        reset()
        r = load("examples/mimic2_schema.json","examples/sample.csv")
        r1 = select(r['uuid'], "age<18")
        r2 = select(r['uuid'], "age<18")
        for t in lookup(r2['uuid']):
            age = float(t['age'])
            self.assertTrue(age < 40)

        reset()
        attrs = ["age","sex","weight"]
        r = load("examples/mimic2_schema.json","examples/sample.csv")
        r1 = project(r['uuid'], attrs)
        r2 = project(r['uuid'], attrs)
        cols = lookup(r2['uuid'])[0].keys()
        for a in attrs:
            self.assertTrue(a in cols)
        self.assertTrue(len(cols) == len(attrs))

        reset()
        r = load("examples/mimic2_schema.json","examples/sample.csv")
        feats = project(r['uuid'], ["age","sex","weight"])
        lab = project(r['uuid'], ["metabolic"])
        c1 = classify(lab['uuid'], feats['uuid'])
        c2 = classify(lab['uuid'], feats['uuid'])
        vals = ["fp","tp","fn","tn","f1","auc","precision","recall","tpr","fpr"]
        res = lookup(c2['uuid'])
        for k in res.keys():
            self.assertTrue(k in vals)
        self.assertTrue(len(res.keys()) == len(vals))

if __name__ == '__main__':
    unittest.main()
