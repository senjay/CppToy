#include<bits/stdc++.h>
using namespace std;
/*
构造函数
拷贝构造函数
operator=
operator->
operator*
*/
/*
引用计数线程安全实现思路
再定义一个std::mutex * mutex,构造函数初始化的时候 再mutex =new mtuex()
每次对计数加减时 加锁
同时所有ptr要共享这个mutex
*/
template <typename T>
class Shared_ptr{
public: 
    Shared_ptr(T*p)
    {
        ptr_=p;
        cnt_=new int(1);
        std::cout<<"Call Constructor"<<endl;
    }

    //拷贝构造 记得cnt+1
    Shared_ptr(const Shared_ptr<T>& S)
    {
        cnt_=S.cnt_;
        ptr_=S.ptr_;
        (*cnt_)++;
        std::cout<<"Call copy constructor"<<endl;
    }
    /*
        重点 每次=赋值，得把原来等号左边的cnt-1,
        但由于可能出现P1=P1的情况,得先把右边的Cnt+1,
        或者 进行 r.ptr_==this.ptr 的判断,是就直接返回
    */
    Shared_ptr<T>& operator=(const Shared_ptr<T>& r)
    {
        //必须得先等号右边的cnt+1 否则如果遇到这种情况 P1=P1，或把P1销毁,
        //虽然r是const 但改变的是r的成员的指针指向的值
        (*(r.cnt_))++;
        if(!ptr_)
        {
            (*cnt_)--;
            if((*cnt_)==0)
            {
                delete ptr_;
                delete cnt_;
            }
        }
        cnt_=r.cnt_;
        ptr_=r.ptr_;
        std::cout<<"Call operator= "<<endl;
        return *this;
    }
    //析构 cnt-1 ,如果cnt==0 则delete
    ~Shared_ptr()
    {
        (*cnt_)--;
        if((*cnt_)==0)
        {
            delete cnt_;
            delete ptr_;
        }
        std::cout<<"Call Destructor"<<endl;
    }
    //指针对象的->运算符
    T* operator->()
    {
        return ptr_;
    }
    T& operator*()
    {
        return (*ptr_);
    }
    int getCnt(){return *(cnt_);};
private:
    T* ptr_;
    int *cnt_;
};


class Obj{
    public:
        Obj(int v){val=v;}
    int val;
};

void PringCnt(Shared_ptr<Obj>&s)
{
    cout<<"cnt_:"<<s.getCnt()<<endl;
}
int main() {
    Shared_ptr<Obj>p1(new Obj(888));
    Shared_ptr<Obj>p2(p1);
    Shared_ptr<Obj>p3=p1;
    Shared_ptr<Obj>p4(new Obj(999));
    PringCnt(p4);
    p4=p4;
    PringCnt(p4);
    PringCnt(p4);
    cout<<p1->val<<endl;
    cout<<(*p1).val<<endl;
}