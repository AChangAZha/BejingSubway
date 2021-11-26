#ifndef STACK_H
#define STACK_H

#include "SeqList.h"

template <class T>
class Stack
{
public:
    Stack():top(0){}
    bool NotEmpty();
    void Push(const T &x);
    T &Pop();
    T &GetTop();
    int GetSize();
    SeqList<T> stack;
    int top;


protected:
};


template <class T>
bool Stack<T>::NotEmpty()
{
    if(top<=0)
        return false;
    else
        return true;
}

template <class T>
void Stack<T>::Push(const T &x)
{
    stack.Add(x);
    top++;
}

template <class T>
T &Stack<T>::Pop()
{
    if(top<=0)
        throw "error";
    top--;
    return stack.Delete(top);
}

template <class T>
T &Stack<T>::GetTop()
{
    if(top<=0)
        throw "error";
    return stack[top-1];
}


template <class T>
int Stack<T>::GetSize()
{
    return top;
}

#endif // STACK_H
