#!/usr/bin/python
import json
import pandas as pd
import traceback
import uuid
from BaseHTTPServer import BaseHTTPRequestHandler
from BaseHTTPServer import HTTPServer
from csv import DictReader

from cache import Cache
from executor import BasicExecutor
from util import Timer

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
                   ('gcs',False),
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

class Server(HTTPServer):
    def __init__(self, addr, handler, file_dir):
        HTTPServer.__init__(self, addr, handler)

        self.catalog = {}
        self.cache = Cache()

        #file_uuid = str(uuid.uuid4())
        file_uuid = '0'
        self.cache[file_uuid] = pd.read_csv(file_dir)

        self.catalog['mimic2'] = {'uuid': file_uuid,
                                  'schema': schema}

class RequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        t = Timer()
        t.start()
        response = 200
        result = {}
        try:
            content_length = int(self.headers.getheader('content-length'))
            req = json.loads(self.rfile.read(content_length))
            print req

            req_type = req['type']
            result = None
            if req_type == 'catalog':
                result = json.dumps(self.server.catalog)
            elif req_type == 'execute':
                task = req['args']['task']
                json.dumps(BasicExecutor(self.server.cache, task).execute())
            elif req_type == 'lookup':
                uuid = req['args']['uuid']
                result = self.server.cache[uuid]
                if type(result) is pd.DataFrame:
                    page_size = int(req['args']['page_size'])
                    page_num = int(req['args']['page_num'])
                    i = page_size * page_num
                    j = i + page_size
                    result = result[i:j]
                result = result.to_json()
        except:
            print traceback.format_exc()
            response = 500
            result = '{}'
        t.stop()

        self.send_response(response)
        self.send_header('Content-type','application/json')
        self.end_headers()
        self.wfile.write(result)

        print 'Run Time:', t.time()
