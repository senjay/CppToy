#include<iostream>
#include<thread>
#include<chrono>
#include<vector>
#include<atomic>
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
或者如下使用atomic
*/
class ReferenceCnt
{
    public:
        ReferenceCnt()
        {
            cnt_.store(1);
        }
        void addOne()
        {
            cnt_++;
        }
        void subOne()
        {
            cnt_.fetch_sub(1);
        }
        bool iszero()
        {
            int expect=0;
            return cnt_.compare_exchange_strong(expect,0);
        }
        int value()
        {
            return cnt_.load();
        }
    private:
    std::atomic<int> cnt_;
};
template <typename T>
class Shared_ptr{
public:
    Shared_ptr():ptr_(nullptr),refCnt_(nullptr){};
    Shared_ptr(T*p)
    {
        ptr_=p;
        refCnt_=new ReferenceCnt();
        std::cout<<"Call Constructor"<<endl;
    }

    //拷贝构造 记得cnt+1
    Shared_ptr(const Shared_ptr<T>& S)
    {
        ptr_=S.ptr_;
        refCnt_=S.refCnt_;
        refCnt_->addOne();
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
        r.refCnt_->addOne();
        if(ptr_!=nullptr)
        {
            refCnt_->subOne();
            if(refCnt_->iszero())
            {
                delete ptr_;
                delete refCnt_;
            }
        }
        refCnt_=r.refCnt_;
        ptr_=r.ptr_;
        std::cout<<"Call operator= "<<endl;
        return *this;
    }
    //析构 cnt-1 ,如果cnt==0 则delete
    ~Shared_ptr()
    {
        if(ptr_==nullptr)return;
        refCnt_->subOne();
        if(refCnt_->iszero())
        {
            delete refCnt_;
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
    int getCnt(){return refCnt_->value();};
private:
    T* ptr_;
    ReferenceCnt *refCnt_;
};


class Obj{
    public:
        Obj(int v){val=v;}
    int val;
};

void PringCnt(Shared_ptr<Obj>&s)
{
    printf("cnt_:%d\n",s.getCnt());
}


int main() {

    Shared_ptr<Obj>p1(new Obj(888));
    std::vector<Shared_ptr<Obj>>L(25);
    std::thread t1([&](){
        for(int i=0;i<10;++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
            L[i]=p1;
            PringCnt(p1);
        }
    });
    std::thread t2([&](){
        for(int i=0;i<10;++i)
        {
              L[i+10]=p1;
             PringCnt(p1);
        }
    });
    t1.join();
    t2.join();
    //
    std::cout<<"now p1 should have 21,it has "<<p1.getCnt()<<std::endl;
    Shared_ptr<Obj>p2(p1);
    Shared_ptr<Obj>p3=p1;
    std::cout<<"now p1 should have 23,it has "<<p1.getCnt()<<std::endl;
    Shared_ptr<Obj>p4(new Obj(999));
    std::cout<<"now p4 should have 1,it has "<<p4.getCnt()<<std::endl;
    p4=p4;
    std::cout<<"now p4 should have 1,it has "<<p4.getCnt()<<std::endl;   
    cout<<p1->val<<endl;
    cout<<(*p1).val<<endl;
}