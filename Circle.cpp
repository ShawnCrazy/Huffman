#include "Circle.h"
#include "assert.h"
#include "string.h"
CCircle::~CCircle()
{
    if (m_MemHandle)
    {
        free(m_MemHandle);
    }
}
bool CCircle::InitCircle(unsigned uNum)
{
    assert(uNum >= 3);
    CircleNode* pCursor;
    m_MemHandle = (CircleNode*)malloc(sizeof(CircleNode) * uNum);
    if (m_MemHandle)
    {
        m_pHead = m_MemHandle;
        m_pTail = m_MemHandle + uNum - 1;
        for (pCursor = m_pHead; pCursor != m_pTail;)
        {
            pCursor->pNext = pCursor + 1;
            pCursor = pCursor->pNext; /* 游标后移 */
        }
        pCursor->pNext = m_pHead; /* 终点连接起点形成环 */
        m_pTail = m_pHead; /* 初始化完成,设置初始状态 */
        return true;
    }
    return false;
}

bool CCircle::WriteCircle(void* pDataNode)
{
    if (m_pTail->pNext != m_pHead)
    {
        m_pTail->pData = pDataNode;
        m_pTail = m_pTail->pNext;
        return true;
    }
    return false;
}

void* CCircle::ReadCircle()
{
    void* pRet = NULL;
    if (m_pHead != m_pTail)
    {
        pRet = m_pHead->pData;
        m_pHead = m_pHead->pNext;
    }
    return pRet;
}

CCircleMQ::~CCircleMQ()
{
    if (m_MemHandle)
    {
        free(m_MemHandle);
    }
}

bool CCircleMQ::InitMQ(unsigned uNum, unsigned uDataSize)
{
    assert(this->InitCircle(uNum)); /* 初始链表失败直接退出 */
    m_DataSize = uDataSize;
    m_MemHandle = (char*)malloc(uDataSize * uNum); /* 用户的数据结构未知，分配单字节的空间 */
    assert(m_MemHandle); /* 创建内存失败直接退出 */
    char* pTail = m_MemHandle + uDataSize * uNum;
    for (char* pCursor = m_MemHandle; pCursor < pTail; pCursor += uDataSize)
    {
        m_pTail->pData = pCursor; /* 初始结束后头尾指向同一结点 */
        m_pTail = m_pTail->pNext;
    }
    assert(m_pHead == m_pTail);
    return true;
}

bool CCircleMQ::WriteMQ(void* pData, unsigned uDataLen)
{
    assert(m_DataSize == uDataLen); /* 为了避免内存拷贝越界 */
    if (m_pTail->pNext != m_pHead)
    {
        memcpy(m_pTail->pData, pData, uDataLen);
        m_pTail = m_pTail->pNext;
        return true;
    }
    return false;
}

void* CCircleMQ::ReadMQ()
{
    return this->ReadCircle();
}