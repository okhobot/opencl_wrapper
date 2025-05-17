#include <iostream>
#include <vector>
#include <oclw.hpp>
#include <chrono>
int main()
{
    OCLW oclw;

    std::vector<std::string> device_names =oclw.get_available_devices_names();
    for(int i=0; i<device_names.size(); i++)
        std::cout<<device_names[i]<<" ";
    std::cout<<std::endl;

    oclw.set_logs_output(&std::cout);
    oclw.init();
    oclw.init_kernels({"test_kernel"},"kernels/");

    std::vector<int> vec(1000);
    for(int i=0; i<vec.size(); i++)
    {
        vec[i]=vec.size()-i-1;
    }

    auto ms=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    oclw.add_and_write_variable("vec",CL_MEM_READ_WRITE,vec.size()*sizeof(int), vec.data());

    oclw.process_oclw("test_kernel", {"vec"}, {}, {vec.size()},vec.size());

    std::cout<<"oclw ms spent: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()-ms<<std::endl;

    //for(int i=0;i<vec.size();i++)std::cout<<vec[i]<<" ";

    return 0;
}
