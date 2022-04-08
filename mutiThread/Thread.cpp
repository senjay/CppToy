#include<iostream>
#include<thread>
#include<chrono>
#include<vector>
#include<future>
using namespace std;
int download(string filename,int size)
{
    std::cout<<"start download "<<filename<<std::endl;
    for(int i=0;i<size;++i)
    {
        std::cout<<i<<std::endl;
    }
    std::cout<<"finish!"<<std::endl;
    return 404;
}
/*
一、以下函数会有问题,fun1()退出后,根据RAII p生命周期结束，线程直接销毁,
除非调用p.detach(), 即让该线程不再由当前p对象管理，而是在线程退出以后自己销毁自己
二、fun2()退出后,由于p是在堆上，线程不会销毁，但p也成为了野指针
*/
void fun1()
{
    std::thread p=std::thread(download,"fun1.txt",10);
    //p.detach();//注释会出错，或者p.join()阻塞
}

void fun2()
{
    std::thread *p=new std::thread(download,"fun2.txt",10);
}

class Test{
    public:
        Test()=default;
        void testThreadLife()
        {
            fun1();
            fun2();
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        void testFuture()
        {
            //异步future,通俗:表示这个变量在未来一定会有的
            std::future<int> status=std::async(download,"status.txt",10);
            while(true)
            {
                std::future_status fs=status.wait_for(std::chrono::milliseconds(10));
                if(fs==std::future_status::ready)
                {
                    std::cout<<"arealdy"<<std::endl;
                    break;
                }
                else
                {
                    std::cout<<"not arealdy"<<std::endl;
                }
            }
            int res=status.get();//如果直接get,相当于阻塞，上面的while循环相当于每次去看一下它的状态
            std::cout<<"status:"<<res<<std::endl;
        }
        void testMtx()
        {
            std::vector<int>L;
            std::mutex mtx;
            std::thread t1([&](){
                for(int i=0;i<10;++i)
                {
                    std::lock_guard grad(mtx);//RAII
                    L.push_back(i);
                }
            });
            std::thread t2([&](){
                for(int i=0;i<10;++i)
                {
                    std::unique_lock grad(mtx);//unique_lock可以主动调用unlock
                    L.push_back(i);
                    grad.unlock();
                    std::cout<<"thread 2 push: "<<i<<endl;
                }
            });
            t1.join();
            t2.join();
            std::cout<<"L size:"<<L.size()<<std::endl;
        }
        void testDeadLock()
        {
            std::vector<int>L;
            std::mutex mtx1,mtx2;
            std::thread t1([&](){
                for(int i=0;i<10;++i)
                {
                    //两个线程上锁顺序得一样
                    //或者std::lock(mtx1,mtx2);保证上锁顺序不会产生锁   
                   // std::scoped_lock(mtx1,mtx2);RAII思想的std::lock(),无需手动解锁
                    mtx1.lock();
                    mtx2.lock();
                    L.push_back(i);
                    mtx2.unlock();
                    mtx1.unlock();
                }
            });
            std::thread t2([&](){
                for(int i=0;i<10;++i)
                {
                    mtx1.lock();
                    mtx2.lock();
                    L.push_back(i);
                    mtx2.unlock();
                    mtx1.unlock();
                    std::cout<<"thread 2 push: "<<i<<endl;
                }
            });
            t1.join();
            t2.join();
            std::cout<<"L size:"<<L.size()<<std::endl;
        }
        void testRecurLock()
        {
            std::mutex mtx1;//多次上锁会一直阻塞
            std::recursive_mutex mtx2;//同一个线程可以上多次锁,通过计数来实现解锁lock+1 unlock-1 0解锁
            std::thread t1([&](){
               // mtx1.lock();
                mtx2.lock();
                std::cout<<"mtx1 locked"<<std::endl;
                mtx2.lock();
                //mtx1.lock();
                mtx2.unlock();
                mtx2.unlock();
                std::cout<<"mtx1 unlocked"<<std::endl;
            });
            t1.join();
        }

};


int main()
{
    
    Test T;
    //T.testThreadLife();
    //T.testFuture();
    T.testRecurLock();
    return 0;
}