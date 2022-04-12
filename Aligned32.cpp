#include<iostream>
#include<vector>
//使申请的地址32位对齐
class AlignObj{
    public:
        static const int AlignedBit=32;
        AlignObj(){}
        void*operator new(std::size_t size)
        {
            void *origin=::operator new(size+AlignedBit);//保证里面有一个地址肯定是32的倍数
            std::size_t mask=~(AlignedBit-1);//位运算
            std::size_t alignloc=reinterpret_cast<size_t>(origin)&mask;//比origin小的32位地址
            void *align= reinterpret_cast<void*>(alignloc+AlignedBit);//32位倍数对齐的地址
            //在存放align的地址前存放origin地址用以delete
            //指针减一相当于地址减8，现代计算机默认16字节对齐，因此align和origin的距离不会小于8它会稳定在16
            *(reinterpret_cast<void**>(align)-1)=origin;
            return align;
        }
        void operator delete(void * ptr)
        {
            ::operator delete(*(reinterpret_cast<void**>(ptr)-1));
        }
};
int main()
{
    std::vector<AlignObj*>objs;
    for(int i=0;i<10;++i)
    {
        AlignObj *obj=new AlignObj();
        if(((size_t)obj)%AlignObj::AlignedBit==0)
            printf("%d is aligned %d\n",((size_t)obj),AlignObj::AlignedBit);
        else 
            printf("%d is not aligned %d\n",((size_t)obj),AlignObj::AlignedBit);
        objs.push_back(obj);
    }
    for(auto obj:objs)
        delete obj;
    return 0;
}