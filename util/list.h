#ifndef LIST_H
#define LIST_H
#include <stdio.h>
template<class T>
class NODE
{
public:
    NODE(){}
    NODE(T v){data = v;}
    T data;
    NODE* next;
    ~NODE(){}
};

template<class T>
class LIST
{
public:
    LIST();
    NODE<T> head;
    void insert(NODE<T>* v);
    void insert(T v);
    bool remove(NODE<T>* v);
    ~LIST();
};
#endif // LIST_H
