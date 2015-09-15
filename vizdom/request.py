#!/usr/bin/python
import json

def parse_request(req_msg):
    req_msg = json.loads(req_msg)
    req_type = req_msg['type']
    if req_type == 'catalog':
        res = json.dumps(self.server.catalog)
    elif req_type == 'execute':
        task = req['args']['task']
        res = json.dumps(BasicExecutor(self.server.cache, task).execute())
    elif req_type == 'lookup':
        uuid = req['args']['uuid']
        res = self.server.cache[uuid].to_json()
    return None

class Request():
    def __init__(self, task, complete, error):
        self.req_type
        self.task = task
        self.complete = complete
        self.error = error
        self.timestamp = 0

    def to_json(self):
        raise NotImplementedError()

class CatalogRequest(Result):
    def __init__(self):
        pass

class TransformResult(Result):
    def __init__(self, task, complete, error, data):
        pass
