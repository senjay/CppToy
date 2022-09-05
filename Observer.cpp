#include<iostream>
#include<vector>
#include<algorithm>
#include<math.h>
#include<list>
#include<unordered_set>
#include<type_traits>
#include<thread>
#include<condition_variable>
using namespace std;
class Observer{
    public:
    virtual~Observer(){};
    virtual void doSome(){}
};
class Subject{
    public:
    void add(Observer*s){
        List.push_back(s);
    }
    void remove(Observer*s)
    {
        auto it=std::find(List.begin(),List.end(),s);
        List.erase(it);
    }
    void notify()
    {
        cout<<"notify Observer"<<endl;
        for(auto s:List)
        s->doSome();
    }
    vector<Observer*>List;
};
class A:public Observer{
    virtual void doSome()
    {
        cout<<"A"<<endl;
    }
};
class B:public Observer{
    virtual void doSome()
    {
        cout<<"B"<<endl;
    }
};
int main()
{
    Observer *a=new A();
    Observer *b=new B();
    Subject Subject;
    Subject.add(a);
    Subject.add(b);
    Subject.notify();
    Subject.remove(a);
    Subject.notify();
    return 0;
}