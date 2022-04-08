#include<iostream>
#include<thread>
#include<chrono>
#include<vector>
#include<shared_mutex>
template<class T>
class MTVector
{
    private:
        std::vector<T>V_;
        mutable std::shared_mutex smtx_;//读写锁 ,加mutable 是为了能在const 函数里改变它
    public:
    MTVector(){};
    void push_back(T val)
    {
        //加写锁，不能多个一起写，写的时候也不能被读
        smtx_.lock();
        V_.push_back(val);
        smtx_.unlock();
    }
    size_t size()const
    {
        //读取只加读锁，可以多个一起读，但不能写
        // smtx_.lock_shared();
        // size_t len=V_.size();
        // smtx_.unlock_shared();
        std::shared_lock sharelock(smtx_);//RAII
        size_t len=V_.size();
        return len;
    }
};
//访问者模式 每次push 不要一次次加锁,相当于再套了一层RAII
template<class T>
class MTAccessorVector
{
    private:
        std::vector<T>V_;
        mutable std::shared_mutex smtx_;
    public:
        class WriteAccessor{
            public:
            MTAccessorVector& that_;
            std::unique_lock<std::shared_mutex> unique_grad_;
            WriteAccessor(MTAccessorVector & V):that_(V),unique_grad_(that_.smtx_){}
            void push_back(T val)
            {
                that_.V_.push_back(val);
            }

        };
         class ReadAccessor{
            public:
            MTAccessorVector& that_;
            std::shared_lock<std::shared_mutex> shared_grad_;
            ReadAccessor(MTAccessorVector & V):that_(V),shared_grad_(that_.smtx_){}
            size_t size()const
            {
                size_t len=that_.V_.size();
                return len;
            }
        };
        WriteAccessor accessWrite()
        {
            return {*this};
        }
        ReadAccessor accessRead()
        {
            return {*this};
        }
};
int main()
{
    MTVector<int> L;
    MTAccessorVector<int>L2;
    std::thread p1([&](){
        auto acsWrite=L2.accessWrite();
        for(int i=0;i<10;++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            acsWrite.push_back(i);
        }
    });
    std::thread p2([&](){
        auto acsRead=L2.accessRead();
        for(int i=0;i<10;++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            printf("p2 %d\n",acsRead.size());
        }
    });
    std::thread p3([&](){
        auto acsRead=L2.accessRead();
        for(int i=0;i<10;++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            printf("p3 %d\n",acsRead.size());
        }
    });
    p1.join();
    //2 3可以一起读
    p2.join();
    p3.join();
    return 0;
}