#include<bits/stdc++.h>
using namespace std;
/***
 * 拷贝构造和operator=为私有
 * 构造为protected 允许子类访问
https://www.cnblogs.com/sunchaothu/p/10389842.html
类内的三种权限
public: 可以被类的函数、子类的函数、友元函数、类的实例对象访问
protected:可以被类的函数、子类的函数、友元函数访问，无法被类的实例对象访问
private:可以被类的函数、友元函数访问，无法被子类、类的实例对象访问

 ***/
template<typename T>
class singleton
{

    private:
       singleton(const singleton& anothor);
       singleton& operator = (const singleton<T>&);
    protected:
       singleton()   { }//构造函数需要是 protected，这样子类才能继承；
       virtual ~singleton() { }
    public:
       static T* getsingleton()
       {
           static T instance; //this is important.
           return &instance;
       }
};
class Obj:public singleton<Obj>{

    friend class singleton<Obj>;//声明友元 这样基类才可以访问自己的私有构造函数
    private:
        Obj(){val=888;};
        ~Obj(){};
        Obj(const Obj&)=delete;
        Obj& operator =(const Obj&)= delete;
    public:
        void PrintVal(){cout<<val<<endl;};
        void changeVal(int v){val=v;};
    private:
        int val;
};
int main()
{
    Obj* instance= Obj::getsingleton();
    Obj* instance2= Obj::getsingleton();
    instance->PrintVal();//888
    instance->changeVal(11);
    instance->PrintVal();//11
    instance2->PrintVal();//11
    return 0;
}