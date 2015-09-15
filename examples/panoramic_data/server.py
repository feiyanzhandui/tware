#!/usr/bin/python
from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
from urlparse import urlparse, parse_qs
import json
from file_utils import *

PORT_NUMBER = 8080

class RequestHandler(BaseHTTPRequestHandler):
    
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type','text/html')
        self.end_headers()

        if "?" not in self.path:
            endpt = self.path
        else:
            endpt = self.path.split("?",1)[0]
        endpt = endpt.replace("/", "")

        # http://localhost:8080/job/?q={"job":"logreg","file_id":0,"attrs":[0,1],"samples":100}
        if endpt == "job":
            params = parse_qs(urlparse(self.path).query)
            obj = json.loads(params['q'][0])
            
            if obj['job'] == "kmeans":
                #run kmeans
                self.wfile.write("kmeans json")
            elif obj['job'] == "logreg":
                #run logreg
                self.wfile.write("logreg json")

        # http://localhost:8080/files/
        elif endpt == "files":
            self.wfile.write(json.dumps(self.files))
        return

def main():
    handler = RequestHandler
    handler.files = load_files()
    server = HTTPServer(('', PORT_NUMBER), handler)
    print 'Started httpserver on port ' , PORT_NUMBER
    server.serve_forever()

if __name__ == "__main__":
    main()
