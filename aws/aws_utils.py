#!/usr/bin/python
import boto.ec2
from fabric.api import *
from fabric.operations import *
from optparse import OptionParser
import os
import sys
import time

def connect():
    if os.getenv('AWS_ACCESS_KEY_ID') == None:
        logger.error("AWS_ACCESS_KEY_ID envorinment variable not set.")
    if os.getenv('AWS_SECRET_ACCESS_KEY') == None:
        logger.error("AWS_SECRET_ACCESS_KEY envorinment variable not set.")
        exit(1)
    return boto.ec2.connect_to_region("us-east-1",
        aws_access_key_id=os.environ['AWS_ACCESS_KEY_ID'],
        aws_secret_access_key=os.environ['AWS_SECRET_ACCESS_KEY'])

def launch_instances(conf, conn):
    ami = conf.get("EC2", "ami")
    num_workers = conf.get("EC2", "num_workers")
    cluster_name = conf.get("EC2", "cluster_name")
    instance_type = conf.get("EC2", "type")

    reservation=conn.run_instances(image_id=ami,
                                   min_count=num_workers,
                                   max_count=num_workers,
                                   key_name=cluster_name,
                                   security_groups=[cluster_name+'Group'],
                                   instance_type=instance_type)
    print "Requesting " + num_workers + " Instances..."
    instances=reservation.instances
    for instance in instances:
        time.sleep(1)
        instance.start()
    for instance in instances:
        print "\t" + instance.id+' started'
        while(instance.update()!='running'):
            time.sleep(1)

    print "\nWaiting for instances to boot up..."
    sys.stdout.write('\t')
    sys.stdout.flush()
    while all(e != 'ok' for e in get_statuses(conf, conn)):
        sys.stdout.write('.')
        sys.stdout.flush()
        time.sleep(5)
    assert(len(get_statuses(conf, conn)) == int(num_workers))
    print "\n\tAll instances launched successfully\n"

def terminate_instances(conf, conn):
    instances = map(lambda x: x.id, get_instances(conf, conn))
    conn.terminate_instances(instances)

def make_key(conf, conn):
    cluster_name = conf.get("EC2", "cluster_name")
    keys=conn.get_all_key_pairs()
    if cluster_name in map(lambda x: x.name, keys):
        print "key already exists, please choose another"
        exit()
    keyPair=conn.create_key_pair(cluster_name)
    keyPair.save('~/.ssh/')

def delete_key(conf, conn):
    cluster_name = conf.get("EC2", "cluster_name")
    keys=conn.get_all_key_pairs()
    if cluster_name not in map(lambda x: x.name, conn.get_all_key_pairs()):
        print "Cannot delete key.  Key doesn't exist"
        exit()
    else:
        while cluster_name in map(lambda x: x.name, conn.get_all_key_pairs()):
           conn.delete_key_pair(cluster_name)

def make_security_group(conf, conn):
    cluster_name = conf.get("EC2", "cluster_name")
    securityGroup=conn.create_security_group(name=cluster_name+'Group',
                                             description='Tupleware')
    securityGroup.add_rule('ssh',22,22,None,None,'0.0.0.0/0', None)
    securityGroup.add_rule('http',80,80,None,None,'0.0.0.0/0', None)
    securityGroup.authorize('tcp',0,65535,'0.0.0.0/0')
    securityGroup.authorize('udp',0,65535,'0.0.0.0/0')

def delete_security_group(conf, conn):
    cluster_name = conf.get("EC2", "cluster_name")
    conn.delete_security_group(name=cluster_name+'Group')

def get_statuses(conf, conn):
    cluster_name = conf.get("EC2", "cluster_name")
    instances = map(lambda x: x.id, get_instances(conf, conn))
    return map(lambda x: x.system_status.status,
            conn.get_all_instance_status(instance_ids=instances))

def get_instances(conf, conn):
    cluster_name = conf.get("EC2", "cluster_name")
    return conn.get_only_instances(filters={'key-name' : cluster_name,'instance-state-name' :
'running'})
