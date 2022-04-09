#ifndef __STUDENT__
#define __STUDENT__
#include<iostream>
#include<memory>
class Student{
    public:
        Student();
        ~Student();
    void read();
    private:
    class Impl;
    std::unique_ptr<Impl>impl;
};
#endif