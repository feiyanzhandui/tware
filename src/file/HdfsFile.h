#ifndef HDFS_FILE_H
#define HDFS_FILE_H

#include <hdfs.h>
#include <string>
#include <vector>

#include "file/Block.h"
#include "file/File.h"
#include "serializer/Archive.h"

using std::string;
using std::vector;

namespace tupleware {
class HdfsFile : public File {
public:
    HdfsFile(hdfsFS fs, string filename, int mode) : File(filename) {
        m_fs = fs;
        //m_file = hdfsOpenFile(m_fs, filename.c_str(), mode, 0, 0, 0);
        //m_info = hdfsGetPathInfo(m_fs, filename.c_str());
        //m_localMgr = localMgr;
    }

    ~HdfsFile() {
        delete m_info;
    }

    virtual vector<Block *> *blocks() {
        vector<Block *> *blks;
        //for (int i = 0; i < m_info->mSize; i += ?)
        //    hdfsPread(m_fs, m_file, i, blks[i].data(), ?);
        //fix end of file
        return blks;
    }

    virtual void addBlock(Block *blk) {
        //int numBlks = blks.size();
        //for (int i = 0; i < numBlks; i++)
        //    hdfsWrite(m_fs, m_file, (void *) blks[i].data(), blks[i].size());
    }

    virtual void serialize(Archive *ar) {
    }

    virtual void deserialize(Archive *ar) {
    }
private:
    hdfsFS m_fs;
    hdfsFile m_file;
    hdfsFileInfo *m_info;
    //LocalManager *m_localMgr;
};
}
#endif
