/*
 * @Author: AChangAZha
 * @Date: 2021-11-28 10:07:22
 * @LastEditTime: 2021-12-07 20:08:29
 * @LastEditors: AChangAZha
 */
#ifndef STACK_H
#define STACK_H

#include "SeqList.h"

template <class T>
class Stack //栈模板类
{
public:
    //构造函数
    Stack() : top(0) {}
    //判断栈是否非空
    bool NotEmpty();
    //入栈
    void Push(const T &x);
    //出栈
    T &Pop();
    //获取栈顶元素
    T &GetTop();
    //获取栈当前元素数量
    int GetSize();

protected:
    SeqList<T> stack; //栈
    int top;          //栈顶下标
};

//判断栈是否非空
template <class T>
bool Stack<T>::NotEmpty()
{
    if (top <= 0)
        return false;
    else
        return true;
}

//入栈
template <class T>
void Stack<T>::Push(const T &x)
{
    stack.Add(x);
    top++;
}

//出栈
template <class T>
T &Stack<T>::Pop()
{
    if (top <= 0)
        throw "error";
    top--;
    return stack.Delete(top);
}

//获取栈顶元素
template <class T>
T &Stack<T>::GetTop()
{
    if (top <= 0)
        throw "error";
    return stack[top - 1];
}

//获取栈当前元素数量
template <class T>
int Stack<T>::GetSize()
{
    return top;
}

#endif // STACK_H
