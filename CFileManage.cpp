#define _CRT_SECURE_NO_WARNINGS
#include "CFileManage.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

CFileManage::CFileManage(const char* pModuleName, int method) :
    m_uCurFileSize(0), m_pFile(NULL), m_calc(0), m_recordTime(0)
{
    strcpy(m_modulename, pModuleName);
    m_method = method;
    strcpy(m_arrDirTmp, "/home/tmpfs/default/");
    strcat(m_arrDirTmp, pModuleName);
    strcpy(m_arrDir, "/home/WaDataNewGB/wangluo/app_behavior/default/");
    strcat(m_arrDir, pModuleName);
    memset(m_arrFileNameTmp, 0, PATH_MAX);
    memset(m_arrFileName, 0, PATH_MAX);
}

CFileManage::~CFileManage()
{
    if (m_pFile)
    {
        fflush(m_pFile);
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

bool CFileManage::SetDirTmp(const char* pDirTmp)
{
#ifdef __linux
    sprintf(m_arrDirTmp, "%s/%s/", pDirTmp, m_modulename);
    sprintf(m_arrFileNameTmp, "%s/%s/", pDirTmp, m_modulename);
#else
    sprintf(m_arrDirTmp, "%s\\%s\\", pDirTmp, m_modulename);
    sprintf(m_arrFileNameTmp, "%s\\%s\\", pDirTmp, m_modulename);
#endif // __linux
    return true;
}

bool CFileManage::SetDirTarget(const char* pDirTarget)
{
#ifdef __linux
    sprintf(m_arrDir, "%s/%s/", pDirTarget, m_modulename);
    sprintf(m_arrFileName, "%s/%s/", pDirTarget, m_modulename);
#else
    sprintf(m_arrDir, "%s\\%s\\", pDirTarget, m_modulename);
    sprintf(m_arrFileName, "%s\\%s\\", pDirTarget, m_modulename);
#endif // __linux
    return true;
}

const char* CFileManage::GetFileName()
{
    return m_arrFileName;
}

int CFileManage::WriteFileCacheBuf(const char* pBuf, unsigned uLen, bool bEndFlag)
{
    if (NULL == m_pFile)
    {
        m_uCurFileSize = 0;
        m_recordTime = time(NULL);
        char arrTime[15] = { 0 };
        CreateMutiDir(m_arrDirTmp);
        CreateMutiDir(m_arrDir);
        strftime(arrTime, sizeof(arrTime), "%Y%m%d%H%M%S", localtime((time_t*)&m_recordTime));
#ifdef __linux
        sprintf(m_arrFileNameTmp, "%s/%s_%u", m_arrDirTmp, arrTime, m_calc);
        sprintf(m_arrFileName, "%s/%s_%u.gz", m_arrDir, arrTime, m_calc);
#else
        sprintf(m_arrFileNameTmp, "%s\\%s_%u", m_arrDirTmp, arrTime, m_calc);
        sprintf(m_arrFileName, "%s\\%s_%u", m_arrDir, arrTime, m_calc);
#endif // __linux
        ++m_calc;
        m_pFile = fopen(m_arrFileNameTmp, "wb");
    }
    m_uCurFileSize += uLen;
    int res = fwrite(pBuf, uLen, 1, m_pFile);
    if (bEndFlag)
    {
        fflush(m_pFile);
        fclose(m_pFile);
        m_uCurFileSize = 0;
        m_pFile = NULL;
        Restore();
    }
    return res;
}

int CFileManage::CreateMutiDir(const char* pDir)
{
    char tmp[PATH_MAX] = { 0 };
#ifdef __linux
    strcat(tmp, "mkdir -p ");
    strcat(tmp, pDir);
#else
    strcat(tmp, "md ");
    strcat(tmp, pDir);
#endif // __linux
    return system(tmp); /* 移入线程有可能导致保存文件时,打开句柄失败 */
}

int CFileManage::Restore()
{
    char tmp[PATH_MAX] = { 0 };
#ifdef __linux
    switch m_method:
    case 0:
        sprintf(tmp, "mv %s %s;", m_arrFileNameTmp, m_arrFileName);
        break;
    case 1:
        sprintf(tmp, "gzip %s; mv %s.gz %s;", m_arrFileNameTmp, m_arrFileName);
        break;
    default:
        break;
#else
    sprintf(tmp, "move %s %s;", m_arrFileNameTmp, m_arrFileName);
#endif // __linux
    return system(tmp); /* 这句可以放入线程处理 */
}