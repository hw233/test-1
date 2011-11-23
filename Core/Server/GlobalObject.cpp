#include "Config.h"
#include "GlobalObject.h"

MemBlockPool::MemBlockPool(size_t size)
    :m_Size(size),m_pBlockList(NULL)
{
}

MemBlockPool::~MemBlockPool()
{
    while(m_pBlockList != NULL)
    {
        MemBlock* ptr = m_pBlockList;
        m_pBlockList = m_pBlockList->next;
        free(ptr);
    }
}

void* MemBlockPool::Alloc()
{
    void* ret = NULL;
    m_Mutex.lock();
    if(m_pBlockList != NULL)
    {
        ret = &m_pBlockList->data;
        m_pBlockList = m_pBlockList->next;
    }
    m_Mutex.unlock();
    if(ret == NULL)
    {
        MemBlock* block = (MemBlock*)malloc(sizeof(MemBlock) + m_Size);
        if(block != NULL)
        {
            ret = &block->data;
        }
    }
    return ret;
}

void MemBlockPool::Free(void* ptr)
{
    if(ptr == NULL) return;

    const static size_t DATA_OFFSET = (char*)(&((MemBlock*)1)->data) - (char*)(1);
    MemBlock* block = (MemBlock*)((char*)ptr - DATA_OFFSET);
    m_Mutex.lock();
    block->next = m_pBlockList;
    m_pBlockList = block;
    m_Mutex.unlock();
}


//各级内存块大小
size_t MEMPOOL_SIZE[] = {128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};

bool GlobalObject::Init()
{
    for(UInt32 i = 0; i < MEMPOOL_NUM; ++i)
    {
        m_Pools[i] = new MemBlockPool(MEMPOOL_SIZE[i]);
    }
	return true;
}

void GlobalObject::UnInit()
{
	//?ͷ???Ϣ???????е???Ϣ
	for (int i = 0; i < MAX_THREAD_NUM; i++)
	{
		FastMutex::ScopedLock lock(m_MsgQueueCs[i]);
		while (!m_MsgQueue[i].Empty())
		{
            FreeMsgBlock(m_MsgQueue[i].Pop());
		}
		m_MsgQueue[i].Clear();
	}

    for(UInt32 i = 0; i < MEMPOOL_NUM; ++i)
    {
        delete m_Pools[i];
        m_Pools[i] = NULL;
    }
}

void* GlobalObject::AllocMsgBlock(size_t size)
{
    char* ptr = NULL;

    size += sizeof(size_t);
    if(size > 65536)
    {
        ptr = (char*)malloc(size); //直接分配
    }
    else
    {
        size_t index = 0;
        size_t poolSize = 128;
        while(size > poolSize){
            ++index;
            poolSize *= 2;
        }
        ptr = (char*)m_Pools[index]->Alloc();
    }
    if(ptr == NULL) return NULL;
    *(size_t*)ptr = size;
    return ptr + sizeof(size_t);
}

void GlobalObject::FreeMsgBlock(void* ptr)
{
    if(ptr == NULL) return;

    char* mem = (char*)ptr - sizeof(size_t);
    size_t size = *(size_t*)mem;
    if(size > 65536)
    {
        free(mem); //直接回收
    }
    else
    {
        size_t index = 0;
        size_t poolSize = 128;
        while(size > poolSize){
            ++index;
            poolSize *= 2;
        }
        m_Pools[index]->Free(mem);
    }
}




