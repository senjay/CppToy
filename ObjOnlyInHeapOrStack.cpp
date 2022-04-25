#include<iostream>
/*
对象创建限制在堆上
构造和析构设为protected:防止类外创建，同时派生类也能访问
提供一个静态函数生成堆上对象，并提供一个显式的销毁函数
*/
class A
{
protected:
    A() {}
    ~A() {}
public:
    static A* create()
    {
        return new A;
    }
    void destory()
    {
        delete this;
    }
};
/*
限制对象创建在栈上
new,delete设置为私有
*/
class B{
    public:
        B(){};
    private:
    void* operator new(size_t size)
    {
        return ::operator new(size);
    }
    void operator delete(void *ptr)
    {
        ::operator delete(ptr);
    }
};
int main()
{
    A *a=A::create();
    B b;
    return 0;
}