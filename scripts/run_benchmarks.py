#!/usr/bin/python
from fabric.api import *
from fabric.operations import *

import time
import sys
import os

local('rm -f *results.txt')

print "\n\n"
print "        +------------------------------------------+"
print "        | [Jenkins]                                |"
print "        |       Test Summary                       |"
print "        |                                          |"
print "\t|  {0:15} {1:>18}      |".format("Test Name", "Time (sec)")
print "\t|" + "_"*42 + "|"

with shell_env(TUPLEWARE_HOME='/home/ubuntu/tupleware'):
    for file in os.listdir("/home/ubuntu/tupleware/build/bin"):
        if file.startswith("test-"):
	    cmd = "/home/ubuntu/tupleware/build/bin/" + file + ' localhost'
            start_time = time.time()
            with hide('everything'):
		local("export TUPLEWARE_HOME=/home/ubuntu/tupleware && " + cmd + " > " + file + "_results.txt")
            t =  time.time() - start_time
            print "\t|  {0:15} {1:18.2f}      |".format(file, float(t))
                    
print "        +------------------------------------------+\n\n"
print("\n")
