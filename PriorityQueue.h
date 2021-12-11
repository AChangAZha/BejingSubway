#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <SeqList.h>
#include <functional>

template <class T, class Com = std::less<T>>
class PriorityQueue //优先级队列模板类
{
public:
    //构造函数
    PriorityQueue() : size(0) {}
    //入队
    void Push(const T &key);
    //出队
    void Pop();
    //获取队头元素
    const T &GetTop();
    //清空队列
    void Clear();
    //判断队列是否已空
    bool Empty() { return size == 0 ? true : false; }

private:
    int size;         //当前队列元素数量
    SeqList<T> queue; //队列
    Com com;          //比较
};

//入队
template <class T, class Com>
void PriorityQueue<T, Com>::Push(const T &key)
{
    queue.Add(key);
    size++;
    int child = size - 1;
    int parent = (child - 1) / 2;
    while (child)
    {
        if (com(queue[parent], queue[child]))
        {
            T tmp = queue[parent];
            queue[parent] = queue[child];
            queue[child] = tmp;
            child = parent;
            parent = (child - 1) / 2;
        }
        else
        {
            return;
        }
    }
}

//出队
template <class T, class Com>
void PriorityQueue<T, Com>::Pop()
{
    if (size == 0)
        return;
    queue[0] = queue[size - 1];
    queue.Delete(size - 1);
    size--;
    int parent = 0;
    int child = parent * 2 + 1;
    while (child < size)
    {
        if (child + 1 < size && com(queue[child], queue[child + 1]))
        {
            child = child + 1;
        }

        if (com(queue[parent], queue[child]))
        {
            T tmp = queue[parent];
            queue[parent] = queue[child];
            queue[child] = tmp;
            parent = child;
            child = parent * 2 + 1;
        }
        else
        {
            return;
        }
    }
}

//获取队头元素
template <class T, class Com>
const T &PriorityQueue<T, Com>::GetTop()
{
    if (Empty())
        throw("Priority queue is empty!");
    return queue[0];
}

//清空队列
template <class T, class Com>
void PriorityQueue<T, Com>::Clear()
{
    queue.Clear();
    size = 0;
}

#endif // PRIORITYQUEUE_H
