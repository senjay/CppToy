#include<iostream>
#include<thread>
#include<chrono>
#include<condition_variable>
class Test{
    public:
    void test1()
    {
        std::condition_variable cv;
        std::mutex mtx;
        std::thread p1([&]()
        {
            /*
                为什么要用unique_lock?
                notify_one的话只会唤醒一个线程，因为要保证多个线程被唤醒时，只有一个被启动，
                如果不需要，在wait返回后调用unlock()即可
                PS:wait的过程中会暂时unlock这个锁
            */
            std::unique_lock lck(mtx);
             /*
                此处会释放lock，然后在cv上等待，直到其它线程通过cv.notify_xxx来唤醒当前线程，
                cv被唤醒后会再次对lock进行上锁，然后wait函数才会返回。
                wait返回后可以安全的使用mutex保护的临界区内的数据。此时mutex仍为上锁状态
            */
            cv.wait(lck);
            std::cout<<"finish"<<std::endl;
        });
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout<<"notyfy all..."<<std::endl;
        cv.notify_all();
        p1.join();
    }
    void testLambda()
    {
        std::condition_variable cv;
        std::mutex mtx;
        bool ready=false;
        std::thread p1([&]()
        {
            std::unique_lock lck(mtx);
            cv.wait(lck,[&](){return ready;});//只有后面lambda返回为真才会唤醒
            std::cout<<"finish"<<std::endl;
        });
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout<<"not ready,but try notyfy all..."<<std::endl;
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout<<"areadly ready,notyfy all..."<<std::endl;
        ready=true;
        cv.notify_all();
        p1.join();
    }


};


int main()
{
    Test T;
    T.testLambda();
    return 0;
}