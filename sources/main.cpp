#include <iostream>
#include <vector>
#include <oclw.hpp>
#include <chrono>
int main()
{
    OCLW oclw;
    oclw.init_oclw({"test_kernel"},"kernels/");

    std::vector<int> vec(100);
    for(int i=0;i<vec.size();i++)
    {
        vec[i]=i;
    }

    auto msn=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    oclw.add_variable("vec",CL_MEM_READ_WRITE,vec.size()*sizeof(int));
    oclw.write_variable("vec",vec.size()*sizeof(int),vec);
    oclw.process_oclw("test_kernel",{"vec"},{},{vec.size()},vec.size());

    std::cout<<"oclw ms spent: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()-msn<<std::endl;

    return 0;
}
