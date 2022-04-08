#include<iostream>
#include<chrono>
using namespace std;

template<class T>
class Timer{
    private:
    std::chrono::steady_clock::time_point start_;
    using DurationType=T;
    using TimeCountType=typename T::rep;//嵌套模板的类型得加typename ,因为得与静态变量区分

    public:
    Timer()
    {
        start_=std::chrono::steady_clock::now();
    }
    TimeCountType  value()
    {
        std::chrono::steady_clock::time_point end=std::chrono::steady_clock::now();
        DurationType dur=std::chrono::duration_cast<DurationType>(end-start_);
        return dur.count();
    }
    TimeCountType reset()
    {
        std::chrono::steady_clock::time_point end=std::chrono::steady_clock::now();
        DurationType dur=std::chrono::duration_cast<DurationType>(end-start_);
        start_=end;
        return dur.count();
    }
};


int main()
{
    Timer<std::chrono::duration<double,milli>> timer1;
    Timer<std::chrono::milliseconds> timer2;
    for(long long int i=0;i<10000000;++i) i+=1;
    std::cout<<timer1.value()<<std::endl;
    std::cout<<timer1.reset()<<std::endl;
    for(long long int i=0;i<10000000;++i) i+=1;
    std::cout<<timer1.reset()<<std::endl;
}