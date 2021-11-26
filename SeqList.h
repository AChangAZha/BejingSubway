#ifndef SEQLIST_H
#define SEQLIST_H

#include <cstddef>

template <class T>
class SeqList //顺序表模板类
{
public:
    //构造函数
    SeqList() : data(NULL), size(0), maxSize(0) {}
    //拷贝构造函数
    SeqList(const SeqList &list);
    //赋值运算符
    SeqList<T> &operator=(const SeqList &list);
    //扩容
    bool Dilatation();
    //添加元素
    bool Add(const T &x);
    //获取当前顺序表长度
    int GetSize() const;
    //获取元素
    T &operator[](const int &index) const;
    T &Delete(const int &index);
    void Clear();
    //析构函数
    ~SeqList();

protected:
    T *data;     //数据数组
    int size;    //当前顺序表长度
    int maxSize; //顺序表最大容量
};

//拷贝构造函数
template <class T>
SeqList<T>::SeqList(const SeqList &list)
{
    size = list.size;
    maxSize = list.maxSize;
    data = new T[maxSize];
    for (int i = 0; i != size; ++i)
        data[i] = list.data[i];
}

//赋值运算符
template <class T>
SeqList<T> &SeqList<T>::operator=(const SeqList &list)
{
    if (this != &list) //防止自赋值
    {
        delete[] data;
        size = list.size;
        maxSize = list.maxSize;
        data = new T[maxSize];
        for (int i = 0; i != size; ++i)
            data[i] = list.data[i];
    }
    return *this;
}

//扩容
template <class T>
bool SeqList<T>::Dilatation()
{
    maxSize = (maxSize == 0 ? 1 : 2 * maxSize);
    T *newData = new T[maxSize];
    for (int i = 0; i < size; i++)
    {
        newData[i] = data[i];
    }
    if (data != NULL)
        delete[] data;
    data = newData;
    return true;
}

//添加元素
template <class T>
bool SeqList<T>::Add(const T &x)
{
    if (size == maxSize)
    {
        Dilatation();
    }
    data[size] = x;
    size++;
    return true;
}

//获取当前顺序表长度
template <class T>
int SeqList<T>::GetSize() const
{
    return size;
}

//获取元素
template <class T>
T &SeqList<T>::operator[](const int &index) const
{
    if (index < size && index >= 0)
        return data[index];
    else
        throw "error";
}

template <class T>
T &SeqList<T>::Delete(const int &index)
{
    if (index < 0 || index > size - 1)
        throw "error";
    T &x = data[index];
    for (int i = index + 1; i <= size - 1; i++)
    {
        data[i - 1] = data[i];
    }
    size--;
    return x;
}

template <class T>
void SeqList<T>::Clear()
{
    size = 0;
}

//析构函数
template <class T>
SeqList<T>::~SeqList()
{
    if (data != NULL)
        delete[] data;
    data = NULL;
}

#endif // SEQLIST_H
