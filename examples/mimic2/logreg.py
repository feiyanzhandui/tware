import numpy as np
import sys
from sets import Set

#classifiers
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression

#preprocess
from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import StandardScaler

#eval
from sklearn.cross_validation import train_test_split
from sklearn.metrics import classification_report
from sklearn.metrics import confusion_matrix

#viz
from sklearn.metrics import roc_curve
import pylab as pl
from sklearn.metrics import confusion_matrix


def main():
    diseases = [('infectious',1,140),
                ('metabolic',240,280),
                ('blood',280,290),
                ('neurologic',320,390),
                ('heart_hypertensive',401,406),
                ('heart_ischemic',410,415),
                ('heart_failure',428,429),
                ('pulmonary',460,520),
                ('digestive',520,580),
                ('renal_insufficiency',580,630)]
    B = {d[0]: LogisticRegression(class_weight='auto', random_state=0) for d in diseases}
    #B = {d[0]: RandomForestClassifier(n_estimators=100, max_features=None, n_jobs=-1, random_state=0) for d in diseases}

    #classifiers = {d[0]:
        #[#KNeighborsClassifier(3),
         #SVC(kernel="linear", C=0.025),
         #SVC(gamma=2, C=1),
         #DecisionTreeClassifier(criterion='entropy',max_depth=4,min_samples_split=10,random_state=0)] for d in diseases}#,
         #RandomForestClassifier(max_depth=10, n_estimators=100, max_features=1)] for d in diseases}#,
         #AdaBoostClassifier()] for d in diseases}#,
         #GaussianNB(),
         #LDA(),
         #QDA()]for d in diseases}
         #RandomForestClassifier(n_estimators=200, max_features=None, n_jobs=-1, random_state=0)] for d in diseases}
         #GradientBoostingClassifier(n_estimators=1000, max_depth=None, max_features=None)] for d in diseases}
         #Pipeline([('feature_selection', LinearSVC(penalty='l1', dual=False)),
         #          ('classification', GaussianNB())])] for d in diseases}

    pts = {}
    for line in open(sys.argv[1]):
        raw = line.split(",")
        try:
            icd9 = float(raw[1])
            pt = raw[0]
            age = float(raw[3])
            if age >= 18:
                if pt not in pts:
                    x = [float(a) for a in raw[2:]]
                    pts[pt] = (x, set())
                for d in diseases:
                    if icd9 >= d[1] and icd9 < d[2]:
                        pts[pt][1].add(d[0])
                        break
        except:
            pass

    X = []
    Y = {d[0]: [] for d in diseases}
    for pt,(feat,codes) in sorted(pts.items()):
        X.append(feat)
        for d in diseases:
            if d[0] in codes:
                Y[d[0]].append(1)
            else:
                Y[d[0]].append(0)

    #scale
    #X = MinMaxScaler().fit_transform(X)
    #X = StandardScaler().fit_transform(X)

    #i = 0
    #while (True):
    #Y_pred = {}
    for disease,y in Y.items():
        print '********* DISEASE: ' + disease
        X_train,X_test,Y_train,Y_test = train_test_split(X, y, test_size=0.3,
                                            random_state=0)
        b = B[disease]
        b.fit(X_train, Y_train)
        y_pred = b.predict(X_test)
        #Y_pred[disease] = y_pred
        print classification_report(Y_test, y_pred)

    #for d1,y_h1 in Y_pred.items():
    #    for d2,y_h2 in Y_pred.items():
    #        print d1, d2, np.corrcoef(y_h1, y_h2)[0][1]
        #print confusion_matrix(Y_test, y_pred)
        #Y_prob = b.predict_proba(X_test)
        #print Y_prob
        #Y_pred = [1 if y_prob[1] > 0.4 else 0 for y_prob in Y_prob]
        #fpr,tpr,thresh = roc_curve(Y_test, Y_prob[:,1])
        #pl.plot(fpr, tpr, lw=1, label='roc')
        #pl.plot([0, 1], [0, 1], 'k--')
        #pl.xlim([0.0, 1.0])
        #pl.ylim([0.0, 1.0])
        #pl.xlabel('FPR')
        #pl.ylabel('TPR')
        #pl.savefig('prefilter.pdf', format='pdf')



        #cc = {}
        #for d1,y_h1 in Y_h.items():
        #    for d2,y_h2 in Y_h.items():
        #        if d1 != d2:
        #            cc[(d1,d2)] = np.corrcoef(y_h1, y_h2)[0][1]

        #maxcc = sorted(cc.items(), key=lambda x: x[1], reverse=True)[0]
        #print '********** MAXCC= ', maxcc

        #d1 = maxcc[0][0]
        #d2 = maxcc[0][1]
        #d = d1 + ',' + d2

        #y1 = Y[d1]
        #y2 = Y[d2]
        #y = [y1[i] or y2[i] for i in range(len(y1))]

        #del Y[d1]
        #del Y[d2]
        #Y[d] = y

        #del B[d1]
        #del B[d2]
        #B[d] = RandomForestClassifier(n_estimators=100, max_features=None, n_jobs=100, random_state=0)

        #i += 1
        #if i > 3:
        #    break

        #with open(k + '.dot', 'w') as f:
        #    f = tree.export_graphviz(B, out_file=f)
            #cm = confusion_matrix(Y_test, B.predict(X_test))
            #print cm
            #visualize(cm, 'lr', k)
    #visualize(confusion_matrix(Y, K_lr), "lr")

    #X10 = SelectPercentile(chi2, percentile=10).fit_transform(X, Y)

    #tree
    #print 'Computing tree...'
    #B_tree = {k: DecisionTreeClassifier() for k in categories}
    #for k,v in B_tree.items():
    #    for train,test in StratifiedKFold(Y[k], 3):
    #        X_train = [X[i] for i in train]
    #        Y_train = [Y[k][i] for i in train]
    #        X_test = [X[i] for i in test]
    #        Y_test = [Y[k][i] for i in test]
    #        v.fit(X_train, Y_train)
    #        cm = confusion_matrix(Y_test, v.predict(X_test))
    #        print k + ':'
    #        print cm
    #print "Computing tree..."
    #B_tree = {k: DecisionTreeClassifier() for k in categories}
    #for k,v in B_tree.items():
    #    v.fit(X, Y[k])
    #    print confusion_matrix(Y[k], v.predict(X))
    #B_tree = DecisionTreeClassifier()
    #B_tree.fit(X, Y)
    #K_tree = B_tree.predict(X)
    #visualize(confusion_matrix(Y, K_tree), "tree")

        #with open(d + '.dot', 'w') as f:
        #    f = tree.export_graphviz(b, out_file=f)

#        cc = {}
#        for d1,y_h1 in Y_h.items():
#            for d2,y_h2 in Y_h.items():
#                if d1 != d2:
#                    cc[(d1,d2)] = np.corrcoef(y_h1, y_h2)[0][1]

#        maxcc = sorted(cc.items(), key=lambda x: x[1], reverse=True)[0]
#        print '********** MAXCC= ', maxcc

#        d1 = maxcc[0][0]
#        d2 = maxcc[0][1]
#        d = d1 + ',' + d2

#        y1 = Y[d1]
#        y2 = Y[d2]
#        y = [y1[i] or y2[i] for i in range(len(y1))]

#        del Y[d1]
#        del Y[d2]
#        Y[d] = y

#        del B[d1]
#        del B[d2]
#        B[d] = LogisticRegression(random_state=0)

        #i += 1
        #if i > 3:
        #    break

        #with open(k + '.dot', 'w') as f:
        #    f = tree.export_graphviz(B, out_file=f)
            #cm = confusion_matrix(Y_test, B.predict(X_test))
            #print cm
            #visualize(cm, 'lr', k)
    #visualize(confusion_matrix(Y, K_lr), "lr")

    #X10 = SelectPercentile(chi2, percentile=10).fit_transform(X, Y)

    #B_tree = {k: DecisionTreeClassifier() for k in categories}
    #for k,v in B_tree.items():
    #    v.fit(X, Y[k])
    #    print confusion_matrix(Y[k], v.predict(X))
    #B_tree = DecisionTreeClassifier()
    #B_tree.fit(X, Y)
    #K_tree = B_tree.predict(X)
    #visualize(confusion_matrix(Y, K_tree), "tree")

if __name__ == "__main__":
    main()
