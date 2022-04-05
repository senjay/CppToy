#include<bits/stdc++.h>
using namespace std;
/***
实现:
构造函数
移动构造
移动operator=
析构
operator->
operator*
operator bool 是否有资源
release() 返回资源，释放所有权

禁用：
拷贝
=
***/

template<class T>
class Unique_ptr{

public:
    Unique_ptr(T* p):ptr_(p){std::cout<<"Call Constructor"<<endl;};
    Unique_ptr(Unique_ptr<T>&& r)
    {
        ptr_=r.ptr_;
        r.ptr_=nullptr;
        std::cout<<"Call move Constructor"<<endl;
    }
    Unique_ptr&operator=( Unique_ptr<T>&& r)
    {
        ptr_=r.ptr_;
        r.ptr_=nullptr;
        std::cout<<"Call move operator= "<<endl;
        return *this;
    }
    ~Unique_ptr()
    {
        if(ptr_)delete ptr_;
        std::cout<<"Call Destructor"<<endl;
    }
    T* release()
    {
        T* temp=ptr_;
        ptr_=nullptr;
         std::cout<<"Call release "<<endl;
        return temp;
    }

    T& operator*(){return *ptr_;};
    T* operator->(){return ptr_;};
    operator bool()const {return ptr_!=nullptr;};
    Unique_ptr(const Unique_ptr<T>& r)=delete;
    Unique_ptr&operator=(const Unique_ptr<T>& r)=delete;
private:
    T* ptr_;
};

class Obj{
    public:
    Obj(int v):val(v){};
    int val;
};
void PringVal(Unique_ptr<Obj>&s)
{
    if(s)
    cout<<"val::"<<s->val<<endl;
    else 
    cout<<"empty"<<endl;
}
int main()
{
    Unique_ptr<Obj>p1(new Obj(789));
    Unique_ptr<Obj>p2=std::move(p1);//Call move Constructor
    PringVal(p1);// empty
    PringVal(p2);//val::789
    p1=std::move(p2);//Call move operator=
    PringVal(p2);// empty
    Obj* obj=p1.release();
    PringVal(p1);//val::789
    cout<<obj->val<<endl;//789
    return 0;
}