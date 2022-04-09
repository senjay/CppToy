#include"Student.h"
class Student::Impl{
    
    public:
    void read()
    {
        name_="mask";
        std::cout<<name_<<std::endl;
    }
    public:
    std::string name_;
};
Student::Student()
{
    impl=std::make_unique<Impl>();
}
Student::~Student(){}
void Student:: read()
{
    impl->read();
}