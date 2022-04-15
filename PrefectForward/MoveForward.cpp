#include<iostream>
//利用模板偏特化 去除引用返回类型
template<typename T>
struct RemoveReference
{
    using type=T;
    RemoveReference()
    {
        std::cout<<"T"<<std::endl;
    }
};
template<typename T>
struct RemoveReference<T&>
{
    using type=T;
    RemoveReference()
    {
        std::cout<<"T&"<<std::endl;
    }
};
template<typename T>
struct RemoveReference<T&&>
{
    using type=T;
    RemoveReference()
    {
        std::cout<<"T&&"<<std::endl;
    }
};


//move 脱去引用，获得类型，然后全部返回右值引用
template<typename T>
typename RemoveReference<T>::type && myMove(T&& val)
{
    return static_cast<typename RemoveReference<T>::type&&>(val);
}


//左值完美转发
//调用该函数的lval都是左值,但T的实际类型可能是左值T&或者右值T&&,所以我们需要返回T的实际类型(通过引用折叠)
template<typename T>
T&& myForward(typename RemoveReference<T>::type& lval)
{
    return static_cast<T&&>(lval);
}
//右值完美转发
template<typename T>
T&& myForward(typename RemoveReference<T>::type&& rval)
{
    static_assert(!std::is_lvalue_reference<T>::value,"rval forward to lval!");
    return static_cast<T&&>(rval);
}



void check(int & val)
{
    std::cout<<"this is lval"<<std::endl;
}

void check(int && val)
{
    std::cout<<"this is rval"<<std::endl;
}

template<typename T>
void foo(T&& val)
{
    //val一定是左值
    std::cout<<"after forward:\n";
    //虽然val一定是左值,但T保存着val原本究竟是左值还是右值的信息,利用它实现完美转发
    check(myForward<T>(val));
}

int main()
{
    RemoveReference<int&&>AA;
    int x=222;
    check(myMove(x));//rval
    check(x);//lval
    check(1);//rval
    std::cout<<"--------------"<<std::endl;
    foo(1);//rval
    foo(myMove(x));//rval
    foo(x);//lval
    return 0;
}