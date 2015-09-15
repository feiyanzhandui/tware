#sudo ssh-keygen -f "/home/ubuntu/.ssh/id_rsa" -t rsa -P ""
sudo cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys

echo '<?xml version="1.0" encoding="UTF-8"?>' > /usr/local/hadoop/etc/hadoop/core-site.xml
echo '<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>' >> /usr/local/hadoop/etc/hadoop/core-site.xml
echo '<configuration> ' >> /usr/local/hadoop/etc/hadoop/core-site.xml
echo '<property> ' >> /usr/local/hadoop/etc/hadoop/core-site.xml
echo '   <name>fs.default.name</name> ' >> /usr/local/hadoop/etc/hadoop/core-site.xml
echo '   <value>hdfs://localhost:9000</value> ' >> /usr/local/hadoop/etc/hadoop/core-site.xml
echo '</property> ' >> /usr/local/hadoop/etc/hadoop/core-site.xml
echo '</configuration> ' >> /usr/local/hadoop/etc/hadoop/core-site.xml

mkdir /home/ubuntu/namenode
mkdir /home/ubuntu/datanode

echo '<?xml version="1.0" encoding="UTF-8"?>' > /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo '<?xml-stylesheet type="text/xsl" href="configuration.xsl"?> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo '<configuration> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo '<property> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo '   <name>dfs.replication</name> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo '   <value>1</value> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo ' </property> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo ' <property> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo '   <name>dfs.namenode.name.dir</name> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo '   <value>file:/home/ubuntu/namenode</value> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo ' </property> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo ' <property> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo '   <name>dfs.datanode.data.dir</name> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo '   <value>file:/home/ubuntu/datanode</value> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo ' </property> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml
echo ' </configuration> ' >> /usr/local/hadoop/etc/hadoop/hdfs-site.xml

hdfs namenode -format
start-dfs.sh
