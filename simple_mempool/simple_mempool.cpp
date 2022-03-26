#include "simple_mempool.h"
const int SIMPLE_MEMPOOL::c_num = 100;
SIMPLE_MEMPOOL::SIMPLE_MEMPOOL(int size)
{
    m_size = size;
    head = current = NULL;
}

bool SIMPLE_MEMPOOL::init()
{
    current = head = new MCHUNK();
    return head->init(m_size, c_num);
}

void* SIMPLE_MEMPOOL::salloc()
{
    char* ret = NULL;
    if(current->hasfree()){
        ret = current->buff_f();
        mp[ret] = current;
        return ret;
    }
    if(!add_chunk())
        return NULL;
    ret = current->buff_f();
    mp[ret] = current;
    return ret;
}

void SIMPLE_MEMPOOL::sdealloc(void *buff)
{
    unordered_map<void*, MCHUNK*>::iterator it = mp.find(buff);
    if(mp.end() == it){
        return;
    }
    MCHUNK* tmp = it->second;
    tmp->free_f(buff);
}

void SIMPLE_MEMPOOL::clean_without_used()
{
    MCHUNK *tmp = head;
    MCHUNK *pre = NULL;
    while(tmp != NULL){
        if(tmp->hasused()){
            pre = tmp;
            tmp = tmp->next;
            continue;
        }
        if(tmp == head){
            head = head->next;
            delete tmp;
            tmp = head;
            continue;
        }
        pre->next = tmp->next;
        delete tmp;
        tmp = pre->next;
    }
}

bool ITEM::new_buff(int argsize, bool argneed_release)
{
    need_release = argneed_release;
    size = argsize;
    buff = new char[size]();
    return buff != NULL;
}

void ITEM::new_buff(int argsize, char *argbuff)
{
    size = argsize;
    buff = argbuff;
}

char *ITEM::buff_f(int start)
{
    return buff + start;
}

bool MCHUNK::init(int argsize, int num)
{
    if(false == bigbuff.new_buff(argsize*num,true))
        return false;
    free = new MBLOCK();
    MBLOCK* tmp = free;
    for(int i = 0; i < num; ++i){
        tmp->new_buff(argsize, bigbuff.buff_f(argsize*i));
        tmp->next = new MBLOCK();
        tmp = tmp->next;
    }
    return true;
}

bool SIMPLE_MEMPOOL::add_chunk()
{
    MCHUNK* tmp = new MCHUNK();
    if(NULL == tmp)
        return false;
    if(!tmp->init(m_size, c_num))
            return false;
    current->next = tmp;
    current = tmp;
    return true;
}

void SIMPLE_MEMPOOL::destroy()
{
    MCHUNK* tmp = NULL;
    while(head != NULL){
        tmp = head;
        head = head->next;
        delete tmp;
    }
    head = current = NULL;
    mp.clear();
    m_size = 0;
}

char *MCHUNK::buff_f()
{
    MBLOCK* tmp = free;
    free = free->next;
    tmp->next = used;
    used = tmp;
    return tmp->buff;
}

bool MCHUNK::free_f(void *buff)
{
    MBLOCK * tmp = remove_from_used((char*)buff);
    if(tmp == NULL)
        return false;
    tmp->next = free->next;
    free = tmp;
    return true;
}

MCHUNK::~MCHUNK()
{
    while(free!=NULL){
        MBLOCK* tmp = free;
        free = free->next;
        delete tmp;
    }
    while(used!=NULL){
        MBLOCK* tmp = used;
        used = used->next;
        delete tmp;
    }
}

MBLOCK *MCHUNK::remove_from_used(char *buff)
{
    MBLOCK * dest = used;
    MBLOCK * pre = NULL;
    while(dest != NULL){
        if(dest->buff == buff)
            break;
        pre = dest;
        dest = dest->next;
    }
    if(dest == NULL)
        return NULL;
    if(dest == used){
        used = used->next;
        return dest;
    }
    pre->next = dest->next;
    return dest;
}


