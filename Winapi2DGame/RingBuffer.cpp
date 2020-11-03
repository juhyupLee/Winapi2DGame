#include <iostream>
#include "RingBuffer.h"
RingBuffer::RingBuffer()
    :
    m_Buffer{ 0, },
    m_Front(0),
    m_Rear(0),
    m_UsedSize(0),
    m_FreeSize(RING_BUFFER_SIZE-1)
{
}

RingBuffer::~RingBuffer()
{
}

int RingBuffer::Enqueue(char* buffer, int32_t size)
{
    //-----------------------------------
    // �������� ���� ����� ���� ������� �۴ٸ� size�� ����������ιٲ�
    //-----------------------------------
    if (m_FreeSize < size)
    {
        size = m_FreeSize;
    }

    int diff = 0;
    int directSize = GetDirectEnqueueSize();
    //-----------------------------------
    // Enqueue ������ m_Rear�� �ܻ� �����ϰ�, Enqeue���Ѵ�.
    //-----------------------------------
    m_Rear = (m_Rear + 1) % RING_BUFFER_SIZE;

    //-----------------------------------
    // ���ڷ� ���� �����  ���ӵ� ������ ���� �� ũ�ٸ�, memcpy�� �������ؾ��Ѵ�
    //-----------------------------------
    if (directSize < size)
    {
        memcpy(m_Buffer + m_Rear, buffer, directSize);
        m_Rear = (m_Rear +directSize-1) % RING_BUFFER_SIZE;
        diff = size - directSize;

        m_Rear = (m_Rear + 1) % RING_BUFFER_SIZE;
        memcpy(m_Buffer + m_Rear, buffer + directSize, diff);
        m_Rear = (m_Rear+ diff-1) % RING_BUFFER_SIZE;
    }
    else
    {
        memcpy(m_Buffer + m_Rear, buffer, size);
        m_Rear = (m_Rear +size-1) % RING_BUFFER_SIZE;
    }

    m_UsedSize += size;
    m_FreeSize -= size;
    return size;
}

int RingBuffer::Dequeue(char* buffer, int32_t size)
{

    //-------------------------------------------
    //�����ۿ� ������� ����� ���ڷ� ���� ������� ������ ����
    //-------------------------------------------
    if (m_UsedSize < size)
    {
        size = m_UsedSize;
    }
    int directSize = GetDirectDequeueSize();

    //-----------------------------------
    // Enqueue ������ m_Rear�� �ܻ� �����ϰ�, Enqeue���Ѵ�.
    //-----------------------------------
    m_Front = (m_Front + 1) % RING_BUFFER_SIZE;
    //-------------------------------------------
    // �����Ҵ��Ҽ��ִ� ����� ���� ������� �۴ٸ� �ι������� ��ť�� �ؾ���.
    //-------------------------------------------
    if (directSize < size)
    {
        memcpy(buffer, m_Buffer + m_Front, directSize);
        m_Front = (m_Front + directSize -1) % RING_BUFFER_SIZE;;

        int diff = size - directSize;
        m_Front = (m_Front + 1) % RING_BUFFER_SIZE;
        memcpy(buffer + directSize, m_Buffer + m_Front, diff);
        m_Front = (m_Front+diff -1 ) % RING_BUFFER_SIZE;
    }
    else
    {
        memcpy(buffer, m_Buffer + m_Front, size);
        m_Front = (m_Front+size -1 ) % RING_BUFFER_SIZE;
    }
    m_UsedSize -= size;
    m_FreeSize += size;
    return size;
}

int32_t RingBuffer::GetFreeSize() const
{
    return m_FreeSize;
}

int32_t RingBuffer::GetUsedSize() const
{
    return m_UsedSize;
}

int32_t RingBuffer::GetDirectEnqueueSize() const
{

     //-----------------------------------
    // �������� ����� ��á�ٸ�, Direct Size�� ���Ҽ� ����. 0��ȯ
    //-----------------------------------
    if ((m_Rear + 1) % RING_BUFFER_SIZE == m_Front)
    {
        return 0;
    }

    int directSize = 0;

    int rearNext = (m_Rear + 1) % RING_BUFFER_SIZE;

    //-----------------------------------
    // mRear < mFront < �ε��� ��
    // �̷���  ���ӵ� �ε����� m_Front ��������.
    //-----------------------------------

    if (rearNext < m_Front)
    {
        directSize = m_Front - rearNext;
    }
    //-----------------------------------
    // �װ��� �ƴ϶��, m_Rear ~ �ε����������� ���ӵ� �޸� �Ҵ������.
    //-----------------------------------
    else
    {
        directSize = (RING_BUFFER_SIZE - 1) - rearNext + 1;
    }

    if (directSize == 0)
    {
        int a = 10;
    }

    return directSize;
}

int32_t RingBuffer::GetDirectDequeueSize() const
{
    //------------------------------------------
    // �����۰� ����ִٸ�, 0����ȯ
    //------------------------------------------
    if (m_Front == m_Rear)
    {
        return 0;
    }


    int nextFront = (m_Front + 1) % RING_BUFFER_SIZE;

    int32_t directSize = 0;
    //�Ϲ����� ���
    if (nextFront < m_Rear)
    {
        directSize = m_Rear - nextFront+1;
    }
    else
    {
        directSize = (RING_BUFFER_SIZE - 1) - nextFront+1;
    }

    return directSize;

}

void RingBuffer::ClearBuffer()
{
    m_FreeSize = RING_BUFFER_SIZE;
    m_UsedSize = 0;
    m_Front = 0;
    m_Rear = 0;
}

void RingBuffer::MoveRear(int size)
{
    if (size < 0)
    {
        return;
    }
    m_Rear = (m_Rear + size) % RING_BUFFER_SIZE;
    m_UsedSize += size;
    m_FreeSize -= size;
}

void RingBuffer::MoveFront(int size)
{
    if (size < 0)
    {
        return;
    }
    //m_Front = (m_Front + 1) % RING_BUFFER_SIZE;;
    m_Front = (m_Front + size ) % RING_BUFFER_SIZE;
    m_UsedSize -= size;
    m_FreeSize += size;
}

char* RingBuffer::GetFrontBufferPtr(void)
{
    //--------------------------------------
    // FrontBufferPtr�� �������ϴ°��� ����
    // dequeue���Ϸ��� ��Ȳ�ε�, ť�� ����ִٸ�
    // null����ȯ
    //--------------------------------------
    if (m_Front == m_Rear)
    {
        return nullptr;
    }

    return m_Buffer + ((m_Front+1)%RING_BUFFER_SIZE);
}

char* RingBuffer::GetRearBufferPtr(void)
{
    //--------------------------------------
    // Enqueue���Ϸ��� ��Ȳ�ε�, ť�� �����ִٸ�,
    // null����ȯ
    //--------------------------------------
    if ((m_Rear+1)%RING_BUFFER_SIZE == m_Front)
    {
        return nullptr;
    }

    return m_Buffer + ((m_Rear + 1) % RING_BUFFER_SIZE); 
}

int RingBuffer::Peek(char* dest, int size)
{
    //-------------------------------------------
   //�����ۿ� ������� ����� ���ڷ� ���� ������� ������ ���� ������� ������ιٲ�.
   //-------------------------------------------
    if (m_UsedSize < size)
    {
        size = m_UsedSize;
    }
    int directSize = GetDirectDequeueSize();

    //-----------------------------------
    // Peek�̱⶧���� ����Ʈ �纻 ����.
    //-----------------------------------
    int tempFront = m_Front;

    //-----------------------------------
    // Enqueue ������ m_Rear�� �ܻ� �����ϰ�, Enqeue���Ѵ�.
    //-----------------------------------
    
    tempFront = (tempFront + 1) % RING_BUFFER_SIZE;
    //-------------------------------------------
    // �����Ҵ��Ҽ��ִ� ����� ���� ������� �۴ٸ� �ι������� ��ť�� �ؾ���.
    //-------------------------------------------
    if (directSize < size)
    {
        memcpy(dest, m_Buffer + tempFront, directSize);
        tempFront = (tempFront + directSize - 1) % RING_BUFFER_SIZE;;

        int diff = size - directSize;
        tempFront = (tempFront + 1) % RING_BUFFER_SIZE;
        memcpy(dest + directSize, m_Buffer + tempFront, diff);
    }
    else
    {
        memcpy(dest, m_Buffer + tempFront, size);
    }

    return size;
}
