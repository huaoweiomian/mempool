#include "list.h"
template<class T>
LIST<T>::LIST()
{
    head.next = NULL;
}


template<class T>
void LIST<T>::insert(T v){
    NODE<T>* tmp = new NODE<T>(v);
    tmp->next = head.next;
    head.next = tmp;
}

template<class T>
bool LIST<T>::remove(NODE<T> *v)
{
    if(v == NULL)
        return false;
    NODE<T>* tmp = head.next;
    NODE<T>* pre = &head;
    while (tmp != v) {
        if(tmp == NULL)
            return false;
        pre = tmp;
        tmp = tmp->next;
    }
    pre->next = tmp->next;
    return true;
}

template<class T>
LIST<T>::~LIST()
{
    NODE<T>* v = head.next;
    while(v != NULL){
        NODE<T>* tmp = v;
        v = v->next;
        delete tmp;
    }
}
