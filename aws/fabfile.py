#!/usr/bin/python
from aws_utils import *
import boto.ec2
import ConfigParser
from fabric import *
import os
import sys
import time
from tware_utils import *

conf = ConfigParser.ConfigParser()
conf.read("ec2.config")

@task
def launch_cluster():
    conn = connect()
    make_key(conf, conn)
    make_security_group(conf, conn)
    launch_instances(conf, conn)
    configure_tupleware(conf, conn)

@task
def resume_cluster():
    conn = connect()
    launch_instances(conf, conn)
    configure_tupleware(conf, conn)
 
@task
def destroy_cluster():
    conn = connect()
    terminate_instances(conf, conn)
    time.sleep(30) #fix this to make sure instance is termianted
    delete_key(conf, conn)
    delete_security_group(conf,conn)
    cluster_name = conf.get("EC2", "cluster_name")
    local("rm ~/.ssh/" + cluster_name)

@task
def stop_cluster():
    conn = connect()
    terminate_instances(conf, conn)

@task
def benchmark():
    conn = connect()
    if len(get_instances(conf, conn)) == 0:
        logger.error("Cluster not launched")    
    configure_tupleware(conf, conn)  
