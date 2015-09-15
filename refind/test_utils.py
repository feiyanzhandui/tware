import json
import refind
import requests
from server import *
import multiprocessing
import time
import unittest

server = Server(('', 8080), RequestHandler, "examples")

t = multiprocessing.Process(target=server.serve_forever)
#t.daemon = True
t.start()

def reset():
    server.tasks = {}
    server.executors = {}
    server.catalog = server.mgr.dict()
    server.results = server.mgr.dict()

    server.id = 0

def run(command):
    url = 'http://localhost:8080'
    #url = 'http://10.116.60.7:8080'
    headers = {'Content-Type':'application/json'}
    t = time.time()
    r = requests.post(url, headers=headers, data=command)
    j = None
    if r.text != 'None':
        j = json.loads(r.text)
    elapsed = time.time() - t;
    #print 'roundtrip time : ', elapsed, 'status : ', r.status_code
    return j

def load(schema_file, data_file):
    cmd = json.dumps({'type':'execute','task':{'type':'load','schema_file':schema_file, 'data_file':data_file}})
    return run(cmd)

def select(source, predicate):
    cmd = json.dumps({'type':'execute','task':{'type':'select','source':source,'predicate':predicate}})
    return run(cmd)

def project(source, attributes):
    cmd = json.dumps({'type':'execute','task':{'type':'project','source':source,'attributes':attributes}})
    return run(cmd)

def union(sources):
    cmd = json.dumps({'type':'execute','task':{'type':'union','sources':sources}})
    return run(cmd)

def intersect(sources):
    cmd = json.dumps({'type':'execute','task':{'type':'intersect','sources':sources}})
    return run(cmd)

def difference(left,right):
    cmd = json.dumps({'type':'execute','task':{'type':'difference','left':left,'right':right}})
    return run(cmd)

def classify(label, features):
    cmd = json.dumps({'type':'execute','task':{'type':'classify','params':{},'classifier':'logistic_regression','label':label,'features':features}})
    return run(cmd)

def lookup(uuid, page_size=1000, page_num=0):
    time.sleep(1)
    cmd = json.dumps({'type':'lookup','uuid':uuid,'page_size':page_size,'page_num':page_num})
    return run(cmd)

def catalog():
    cmd = json.dumps({'type':'catalog'})
    return run(cmd)

def tasks():
    cmd = json.dumps({'type':'tasks'})
    return run(cmd)
