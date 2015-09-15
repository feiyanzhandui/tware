#!/usr/bin/python
import json
import re
import traceback
import uuid
from BaseHTTPServer import BaseHTTPRequestHandler
from BaseHTTPServer import HTTPServer
from multiprocessing import Manager
from SocketServer import ThreadingMixIn

from executor import BasicExecutor
from executor import IncrementalExecutor
from task import parse_task
from result import EmptyResult
from result import TransformResult

from task import *

BODY = '''
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "file/Block.h"
#include "frontend/Context.h"
#include "job/Executor.h"
#include "net/Socket.h"
#include "util/Runnable.h"

using std::get;
using std::string;
using std::tuple;
using std::unordered_map;
using std::vector;

using namespace tupleware;

class TaskExecutor : public Executor {
public:
  TaskExecutor(Context *ctx, Socket *sock) : Executor(ctx, sock) {}

  virtual void run() {
    auto d1 = outType();
    Block *b0 = getAny(IN);
    while (b0 != NULL) {
      auto d0 = b0->data();
      for (int i0 = 0; i0 < d0.size(); i0++) {
        auto t0 = d0[i0];
XXX
      }
      b0 = getAny(IN);
    }
    Block *b1 = new Block(0, d1);
    addBlock(OUT, b1);
    sendResult();
    m_sock->close();
  }
};
'''

def codegen(code, curr, results):
    task = results[curr].task
    next = None
    if isinstance(task, ClassifyTask):
        code = 'd1 = ' + task.classifier + '(t0);\n'
        next = task.features
    elif isinstance(task, SelectTask):
        if len(code) == 0:
            code = 'd1.append(t0);\n'
        pred = task.predicate
        pred = pred.replace('and', '&&')
        pred = pred.replace('or', '||')
        pred = pred.replace('not', '!')
        pred = re.sub(r'([A-z]+)', r't0["\1"]', pred)
        code = 'if (' + pred + ') {\n' + code + '}\n'
        next = task.source
    elif isinstance(task, ProjectTask):
        next = task.source
    else:
        return code
    return codegen(code, next, results)

class Server(ThreadingMixIn, HTTPServer):
    def __init__(self, addr, handler, file_dir):
        HTTPServer.__init__(self, addr, handler)

        self.mgr = Manager()
        self.tasks = {}
        self.executors = {}
        self.catalog = self.mgr.dict()
        self.results = self.mgr.dict()

        self.id = 0

class RequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        response = 200
        result = None
        try:
            content_length = int(self.headers.getheader('content-length'))
            req_msg = json.loads(self.rfile.read(content_length))
            #print req_msg

            tasks = self.server.tasks
            executors = self.server.executors
            catalog = self.server.catalog
            results = self.server.results

            req_type = req_msg['type']
            if req_type == 'catalog':
                result = json.dumps(dict(catalog))
            elif req_type == 'cancel':
                req_uuid = req_msg['uuid']
                result = json.dumps({'uuid': req_uuid})
                #executors[req_uuid].terminate()
            elif req_type == 'execute':
                task_msg = req_msg['task']
                if str(task_msg) in tasks:
                    req_uuid = tasks[str(task_msg)]
                    result = json.dumps({'uuid': req_uuid})
                    pass
                else:
                    req_uuid = str(self.server.id)
                    self.server.id += 1
                    #req_uuid = str(uuid.uuid4())
                    tasks[str(task_msg)] = req_uuid
                    task = parse_task(req_uuid, task_msg)
                    results[req_uuid] = EmptyResult(task)
                    executor = BasicExecutor(catalog, results, task)
                    executors[req_uuid] = executor
                    executor.start()
                    result = json.dumps({'uuid': req_uuid})
            elif req_type == 'lookup':
                req_uuid = req_msg['uuid']
                result = results[req_uuid]
                #print req_uuid, ':', result.to_json()
                if isinstance(result, TransformResult):
                    page_size = int(req_msg['page_size'])
                    page_num = int(req_msg['page_num'])

                    i = page_size * page_num
                    j = i + page_size
                    #print i, j
                    result = result.data[i:j].to_json(orient='records')
                else:
                    result = result.to_json()
                    #print result
            elif req_type == 'tasks':
                result = json.dumps(['frequent_itemsets',
                                     'logistic_regression',
                                     'naive_bayes',
                                     'perceptron',
                                     'svm'])
            elif req_type == 'codegen':
                req_uuid = req_msg['uuid']
                code = BODY.replace('XXX', codegen('', req_uuid, results))
                code = code.replace('\n', '\r\n')
                result = json.dumps({'code': code})
            else:
                result = 'not implemented'
                #result = ErrorResult('not implemented')
                raise NotImplementedError()
        except:
            print traceback.format_exc()
            response = 500
            result = 'malformed request\n'
            #result = ErrorResult('malformed request')

        self.send_response(response)
        self.send_header('Content-type','application/json')
        self.end_headers()
        self.wfile.write(result)
