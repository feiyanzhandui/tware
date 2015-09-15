#!/usr/bin/python
import csv

infile = '/tmp/mimic2_out.csv'
outfile = '/tmp/mimic2.csv'

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

attrs = [icd9[0] for icd9 in icd9s]
data = []
with open(infile, 'r') as f:
    reader = csv.DictReader(f)
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
            for icd9 in icd9s:
                if icd9[0] in pt_icd9s:
                    pt[icd9[0]] = 1
                else:
                    pt[icd9[0]] = 0
            data.append(pt)
        except Exception as e:
            print str(e)
            pass

fields = [icd9[0] for icd9 in icd9s] + reader.fieldnames[1:]
with open(outfile, 'w') as f:
    writer = csv.DictWriter(f, fields)
    writer.writeheader()
    for d in data:
        try:
            writer.writerow(d)
        except:
            pass
