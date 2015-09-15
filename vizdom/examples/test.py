import requests
import time
import json

def run(command) :
    #print command
    url = 'http://localhost:8080'
    #url = 'http://10.116.60.7:8080'
    headers = {'Content-Type':'application/json'}
    t = time.time()
    r = requests.post(url, headers=headers, data=command)
    j = None
    if r.text != 'None':
        j = json.loads(r.text)
    elapsed = time.time() - t;
    print 'roundtrip time : ', elapsed
    return j
    
def union(uuid, data) :
    project_com= json.dumps({'type':'execute','args':{'task':{'uuid':uuid,'type':'union', 'args':{'source':data}}}})
    run(project_com)

def project(uuid, data, attributes) :
    project_com= json.dumps({'type':'execute','args':{'task':{'uuid':uuid,'type':'project', 'args':{'source':data,'attributes':attributes}}}})
    run(project_com)

def lookup(data, pagesize, pagenum) :
    lookup_com = json.dumps({'type':'lookup','args':{'uuid':data, 'page_size' : pagesize, 'page_num': pagenum}})
    return run(lookup_com)

# get the current files loaded in the server
#curl -H "Content-Type: application/json" -X POST -d '{"type":"catalog"}' http://10.116.60.7:8080

# get the data for a uuid from the cache (uuid = 100)
#curl -H "Content-Type: application/json" -X POST -d '{"type":"lookup", "args":{"uuid":"3"}}' http://localhost:8080

# do a project on uuid 100 and store result in uuid 0
#curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","args":{"task":{"uuid":"1", "type":"project", "args":{"source":"0", "attributes":["age"]}}}}' http://localhost:8080

# do a project on uuid 100 and store result in uuid 1
#curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","args":{"task":{"uuid":"2", "type":"project", "args":{"source":"0", "attributes":["metabolic"]}}}}' http://localhost:8080

# classify using age as a feature (uuid 0) and metabolic as labels (uuid 1).  store result to uuid = 2
#curl -H "Content-Type: application/json" -X POST -d '{"type":"execute", "args":{"task":{"uuid":"3","type":"classify", "args":{"classifier": {"type":"logistic_regression", "params":{},"labels":"2", "features":"1"}}}}}' http://localhost:8080

# lookup result of classify
#curl -H "Content-Type: application/json" -X POST -d '{"type":"lookup", "args":{"uuid":"2"}}' http://10.116.60.7:8080

# do a select on the base data (uuid = 100) for people who are 21 and store result in uuid 5
#curl -H "Content-Type: application/json" -X POST -d '{"type":"execute", "task":{"uuid":"5","type":"select", "args":{"data":"0", "predicate":"((age > 20) and (age < 22))"}}}' http://10.116.60.7:8080

catalog_com = json.dumps({'type':'catalog'})
base_uuid = run(catalog_com)['mimic2']['uuid']
print base_uuid

project(1, base_uuid, ['age'])
project(2, base_uuid, ['age', 'sex'])
project(3, base_uuid, ['sex'])

union(4, [1,2])
print lookup(4, 100, 0)

union(5, [1,3])
print lookup(5, 100, 0)


#f = open('C:\\Temp\\test.txt','w')
#for c in j:
#f.write(r.text)
#f.flush()
#f.close()
