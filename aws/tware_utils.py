from aws_utils import *
import boto.ec2
from config_files import *
from fabric.api import *
from fabric import *
import os
import sys

curr_dir = os.getcwd()

def configure_tupleware(conf, conn):
    env.key_filename = "~/.ssh/" + conf.get("EC2", "cluster_name") + ".pem"
    instances = get_instances(conf, conn)
    ips = map(lambda x: "ubuntu@" + x.public_dns_name, instances)
    master = [ips[0]]
    workers = ips[1:]
    execute(build_tupleware, hosts=ips)
    execute(get_public_key, hosts=master)
    execute(add_public_keys, hosts=workers)
    local('rm -rf ' + curr_dir + '/tmp')
    execute(config_files, master, workers, hosts=ips)
    execute(launch_hdfs, hosts=master)
    execute(launch_master, hosts=master)
    execute(launch_worker, master, hosts=workers)

def get_public_key():
    run('rm -rf ~/.ssh/id_rsa*')
    run('ssh-keygen -f "/home/ubuntu/.ssh/id_rsa" -t rsa -P ""')
    run('cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys')
    os.mkdir("tmp")
    get('/home/ubuntu/.ssh/id_rsa.pub', curr_dir + "/tmp")

@parallel
def add_public_keys():
    get('/home/ubuntu/.ssh/authorized_keys', curr_dir + "/tmp")
    local('cat ' + curr_dir + '/tmp/id_rsa.pub >> ' + curr_dir + '/tmp/authorized_keys')
    put(curr_dir + '/tmp/authorized_keys','/home/ubuntu/.ssh')

@parallel
def config_files(master, workers):
    #extract ip address for all master and workers
    master = master[0][7:]
    workers = map(lambda x: x[7:], workers)

    # Load Config Files defined in config_files.py
    core_site_string = get_core_site(master)
    hdfs_site_string = get_hdfs_site()
    mapred_site_string = get_mapred_site(master)
    yarn_site_string = get_yarn_site()

    conf_dir = "/usr/local/hadoop/etc/hadoop/"
    run("echo 'export JAVA_HOME=/usr/lib/jvm/java-1.7.0-openjdk-amd64' >> " + conf_dir + "hadoop-env.sh")

    for f in ["masters", "slaves", "core-site.xml", "hdfs-site.xml", "mapred-site.xml", "yarn-site.xml"]:
        run("rm -rf " + conf_dir + f)
        run("touch " + conf_dir + f)

    for worker in workers + [master]:
        run("echo " + worker + " >> " + conf_dir + "slaves")

    run("echo '" + core_site_string + "' > " + conf_dir + "core-site.xml")
    run("echo '" + hdfs_site_string + "' > " + conf_dir + "hdfs-site.xml")
    run("echo '" + mapred_site_string + "' > " + conf_dir + "mapred-site.xml")
    run("echo '" + yarn_site_string + "' > " + conf_dir + "yarn-site.xml")

    sudo("chown ubuntu /mnt")

def launch_hdfs():
    run('/usr/local/hadoop/bin/hdfs namenode -format')
    with cd("/usr/local/hadoop/sbin"):
        run("./start-dfs.sh")

@parallel
def build_tupleware():
    run("sudo apt-get install -q -y git ")
    run("rm -rf ~/tupleware")
    run("git clone --quiet https://tupleware:tupleware2013@github.com/BrownBigData/tupleware")
    with cd("tupleware"):
        run("rm -rf build")
        run("mkdir build")
        with cd("build"):
            run("cmake ../")
            run("make")

def launch_master():
    with cd("~/tupleware/build/bin"):
        run("./master 2>&1 > master_log &")
        time.sleep(2)

@parallel
def launch_worker(master):
    master = master[0][7:]
    with cd("~/tupleware/build/bin"):
        run("./worker " + master + " 2>&1 > worker_log &")
