#pragma once
#include <stdlib.h>
/*
** ���̶߳�д���У�����mq��
** ���ݽṹΪ�ػ���������
** ����3������γɻ�
** ���ֻ����ָ��
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
    /* uNum������ */
    bool InitCircle(unsigned uNum);
    /* pDataNode �û�����ָ�� */
    bool WriteCircle(void* pDataNode);
    /* �����û�����ָ���NULL */
    void* ReadCircle();

protected:
    CircleNode* m_pHead;
    CircleNode* m_pTail;

private:
    CircleNode* m_MemHandle; /* �ڴ��� */
};

/*
** ���̶߳�д���У�����mq��
** ���ݽṹΪ�ػ���������
** ����3������γɻ�
** ��㱣����������
*/
class CCircleMQ :
    private CCircle
{
public:
    CCircleMQ() {};
    ~CCircleMQ();
    /* uNum ������; uDataSize ���ݽ��ռ����ʵ�ڴ��С */
    bool InitMQ(unsigned uNum, unsigned uDataSize);
    /* pData ���ݽ��ָ��; uDataSize ���ݽ��ռ����ʵ�ڴ��С */
    bool WriteMQ(void* pData, unsigned uDataLen);
    /* �����û�����ָ���NULL */
    void* ReadMQ();

private:
    unsigned m_DataSize; /* �洢�û��ṹ���С,����д��ʱУ�� */
    char* m_MemHandle; /* �ڴ��� */
};
