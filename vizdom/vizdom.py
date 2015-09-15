#!/usr/bin/python
from argparse import ArgumentParser

from server import RequestHandler
from server import Server

def main():
    parser = ArgumentParser()
    parser.add_argument('-p', '--port', required=True)
    parser.add_argument('-f', '--file_dir', required=True)
    args = parser.parse_args()

    port = int(args.port)
    file_dir = args.file_dir

    server = Server(('',port), RequestHandler, file_dir)
    print 'Starting server on port: ' + str(port)
    server.serve_forever()

if __name__ == '__main__':
    main()
