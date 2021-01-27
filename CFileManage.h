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
    CFileManage(const char* pModuleName, int method = 0); /* ���캯������ģ����������ļ�������ʽͬm_method */
    ~CFileManage();
    bool SetDirTmp(const char* pTmpDir); /* ������ʱ���Ŀ¼(������) */
    bool SetDirTarget(const char* pTargetDir); /* ����Ŀ��Ŀ¼ */
    int WriteFileCacheBuf(const char* pBuf, unsigned uLen, bool bEndFlag = true); /* д�ļ����ݣ�bEndFlag��ʾ�Ƿ������� */
    const char* GetFileName(); /* ��ȡ��ǰ�ļ����մ��·��(�����ļ�����֮ǰ������Ч) */
    unsigned m_uCurFileSize; /* ��ǰ�ļ���д�����ݴ�С(�ڵ�ǰ�ļ�����д��֮ǰ������Ч) */

private:
    int CreateMutiDir(const char* pDir); /* ������ϵͳ���� */
    int Restore(); /* ������ϵͳ���� */

private:
    int m_method; /* ���������ʽ:0������1ѹ��Ϊgz.Ĭ��0 */
    FILE* m_pFile; /* �ļ���� */
    char m_arrDirTmp[PATH_MAX]; /* �洢��ʱĿ¼�� */
    char m_arrDir[PATH_MAX]; /* �洢����Ŀ¼�� */
    char m_arrFileNameTmp[PATH_MAX]; /* �洢��ʱ�ļ��� */
    char m_arrFileName[PATH_MAX]; /* �洢�����ļ��� */
    char m_modulename[NAME_MAX]; /* �洢ģ���� */
    time_t m_recordTime; /* �洢��ǰ����򿪵�ʱ��� */
    unsigned m_calc; /* �ļ��ۼƱ������ */
};
