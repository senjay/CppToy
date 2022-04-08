#include<iostream>
#include<thread>
#include<chrono>
#include<list>
#include<condition_variable>
//cv 实现生产者消费者
template<class T>
class MTQueue{
    public:
    std::mutex mtx_;
    std::condition_variable cv_;
    std::list<T>q_;

    void push(T val)
    {
        std::unique_lock lck(mtx_);
        q_.push_back(val);
        cv_.notify_one();
    }
    T pop()
    {
        std::unique_lock lck(mtx_);
        cv_.wait(lck,[&](){return !q_.empty();});
        T temp=q_.front();
        q_.pop_front();
        return temp;
    }
    void push_many(std::initializer_list<T> vals)
    {
        std::unique_lock lck(mtx_);
        std::copy(vals.begin(),vals.end(),std::back_insert_iterator(q_));
        cv_.notify_all();
    }
};
int main()
{
    MTQueue<int> foods;
    std::thread p1 ([&](){
        for(int i=0;i<5;++i)
        {
            int fid=foods.pop();
            printf("p%d get food:%d\n",std::this_thread::get_id(),fid);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    std::thread p2 ([&](){
        for(int i=0;i<5;++i)
        {
            int fid=foods.pop();
            printf("p%d get food:%d\n",std::this_thread::get_id(),fid);
        }
    });
    for(int i=0;i<10;++i)
        foods.push_many({2*i,2*i+1});

    p1.join();
    p2.join();
    return 0;
}