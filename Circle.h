#pragma once
#include <stdlib.h>
/*
** 多线程读写队列（类似mq）
** 数据结构为回环单向链表
** 至少3个结点形成环
** 结点只保存指针
*/
class CCircle
{
#pragma pack(push, 1)
    typedef struct _CircleNode
    {
        _CircleNode* pNext;
        void* pData;
        _CircleNode() :pData(NULL) {}
    } CircleNode;
#pragma pack(pop)

public:
    CCircle() {};
    ~CCircle();
    /* uNum结点个数 */
    bool InitCircle(unsigned uNum);
    /* pDataNode 用户数据指针 */
    bool WriteCircle(void* pDataNode);
    /* 返回用户数据指针或NULL */
    void* ReadCircle();

protected:
    CircleNode* m_pHead;
    CircleNode* m_pTail;

private:
    CircleNode* m_MemHandle; /* 内存句柄 */
};

/*
** 多线程读写队列（类似mq）
** 数据结构为回环单向链表
** 至少3个结点形成环
** 结点保存数据内容
*/
class CCircleMQ :
    private CCircle
{
public:
    CCircleMQ() {};
    ~CCircleMQ();
    /* uNum 结点个数; uDataSize 数据结点占用真实内存大小 */
    bool InitMQ(unsigned uNum, unsigned uDataSize);
    /* pData 数据结点指针; uDataSize 数据结点占用真实内存大小 */
    bool WriteMQ(void* pData, unsigned uDataLen);
    /* 返回用户数据指针或NULL */
    void* ReadMQ();

private:
    unsigned m_DataSize; /* 存储用户结构体大小,数据写入时校验 */
    char* m_MemHandle; /* 内存句柄 */
};
