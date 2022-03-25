#ifndef SIMPLE_MEMPOOL_H
#define SIMPLE_MEMPOOL_H
#include "../util/list.h"
#include <unordered_map>
using std::unordered_map;
const int c_size = 64;//默认最小单元为64字节，这个变量暂时没用.
struct ITEM{
    ITEM(){
        size = 0;
        buff = NULL;
        next = NULL;
        need_release = false;
    }
    bool new_buff(int argsize, bool argneed_release);
    void new_buff(int argsize, char* argbuff);
    char *buff_f(int start);
    char* buff;
    int size;
    ITEM* next;
    bool need_release;
    ~ITEM(){
        if(false == need_release)
            return;
        clean();
    }
private:
    void clean(){
        if(buff == NULL)
            return;
        delete [] buff;
        size = 0;
    }
};
typedef ITEM MBLOCK;

struct MCHUNK{
    MCHUNK(){
        free = used = NULL;
        next = head = current = NULL;
    }
    bool init(int argsize, int num);
    MBLOCK bigbuff;
    MBLOCK* free;
    MBLOCK* used;
    MCHUNK* next;
    MCHUNK* head;
    MCHUNK* current;

    char *buff_f();
    bool free_f(void* buff);
    bool hasfree(){
        if(current == NULL)
            return false;
        return current->free != NULL;
    }
    bool hasused(){
        if(current == NULL)
            return false;
        return current->used != NULL;
    }
private:
    MBLOCK* remove_from_used(char *buff);
};


class SIMPLE_MEMPOOL
{
public:
    SIMPLE_MEMPOOL(int size);//最小单元的大小
    bool init();
    void* salloc();
    void sdealloc(void* buff);
    void clean();
    ~SIMPLE_MEMPOOL();
private:
    bool add_chunk();
    MCHUNK* head;
    MCHUNK* current;
    unordered_map<void*, MCHUNK*> mp;

    int m_size;//记录最小单元的大小
    //当内存池中内存不够时，一次申请多少个单元
    const static int c_num;
    void destroy();
};

#endif // SIMPLE_MEMPOOL_H
