#pragma once
#include <stdio.h>
#ifdef __linux
#include <linux/linits.h>
#else
#include "limits.h"
#endif // __linux

class CFileManage
{
public:
    CFileManage(const char* pModuleName, int method = 0); /* 构造函数传入模块名、结果文件保存形式同m_method */
    ~CFileManage();
    bool SetDirTmp(const char* pTmpDir); /* 设置临时存放目录(性能盘) */
    bool SetDirTarget(const char* pTargetDir); /* 设置目标目录 */
    int WriteFileCacheBuf(const char* pBuf, unsigned uLen, bool bEndFlag = true); /* 写文件内容，bEndFlag表示是否输出结果 */
    const char* GetFileName(); /* 获取当前文件最终存放路径(在新文件生成之前访问有效) */
    unsigned m_uCurFileSize; /* 当前文件已写入数据大小(在当前文件结束写入之前访问有效) */

private:
    int CreateMutiDir(const char* pDir); /* 调用了系统命令 */
    int Restore(); /* 调用了系统命令 */

private:
    int m_method; /* 结果保存形式:0不处理，1压缩为gz.默认0 */
    FILE* m_pFile; /* 文件句柄 */
    char m_arrDirTmp[PATH_MAX]; /* 存储临时目录名 */
    char m_arrDir[PATH_MAX]; /* 存储最终目录名 */
    char m_arrFileNameTmp[PATH_MAX]; /* 存储临时文件名 */
    char m_arrFileName[PATH_MAX]; /* 存储最终文件名 */
    char m_modulename[NAME_MAX]; /* 存储模块名 */
    time_t m_recordTime; /* 存储当前句柄打开的时间戳 */
    unsigned m_calc; /* 文件累计保存个数 */
};
