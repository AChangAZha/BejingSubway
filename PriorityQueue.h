#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <SeqList.h>
#include <functional>

template <class T, class Com = std::less<T>>
class PriorityQueue
{

public:
    PriorityQueue() : size(0) {}
    void Push(const T &key);
    void Pop();
    const T &GetTop();
    void Clear();
    bool Empty() { return size == 0 ? true : false; }

private:
    int size;
    SeqList<T> queue;
    Com com;
};

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

//返回堆顶元素
template <class T, class Com>
const T &PriorityQueue<T, Com>::GetTop()
{
    if (Empty())
        throw("Priority queue is empty!");
    return queue[0];
}

//清空堆
template <class T, class Com>
void PriorityQueue<T, Com>::Clear()
{
    queue.Clear();
    size = 0;
}

#endif // PRIORITYQUEUE_H
