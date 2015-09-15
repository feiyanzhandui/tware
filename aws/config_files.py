#
# Config Files
#
def get_core_site(master):
    return """
    <configuration>
      <property>
        <name>fs.default.name</name>
        <value>hdfs://""" + master + """:9000</value>
      </property>
    </configuration>"""

def get_hdfs_site():
    return """
    <configuration>
      <property>
         <name>dfs.replication</name>
         <value>1</value>
      </property>
      <property>
        <name>dfs.namenode.name.dir</name>
        <value>file:/mnt/namenode</value>
      </property>
      <property>
        <name>dfs.datanode.data.dir</name>
        <value>file:/mnt/datanode</value>
      </property>
    </configuration>"""

def get_mapred_site(master):
    return """
    <configuration>
      <property>
        <name>mapred.job.tracker</name>
        <value>""" + master + """:54311</value>
      </property>
      <property>
        <name>mapreduce.framework.name</name>
        <value>yarn</value>
      </property>
    </configuration>"""

def get_yarn_site():
    return """
    <configuration>
    <property>
       <name>yarn.nodemanager.aux-services</name>
       <value>mapreduce_shuffle</value>
    </property>
    <property>
       <name>yarn.nodemanager.aux-services.mapreduce.shuffle.class</name>
       <value>org.apache.hadoop.mapred.ShuffleHandler</value>
    </property>
    </configuration>"""
