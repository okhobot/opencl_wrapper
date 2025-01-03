#include "oclw.hpp"


void OCLW::operator = (OCLW &_oclw)
{
        oclw_queue=_oclw.oclw_queue;
        context=_oclw.context;
        contextDevices=_oclw.contextDevices;
        kernels=_oclw.kernels;
        sourceCode=_oclw.sourceCode;
        source=_oclw.source;
        program=_oclw.program;
        iArg=_oclw.iArg;
}

void OCLW::init(int device_index,bool debug, bool print_device_names)
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    std::vector<cl::Device> devices;
    cl::Device device;
    int device_indx=0;

     // searching for a device
        if(print_device_names) std::cout<<"device_names: ";
        for(int i=0;i<platforms.size();i++)
        {
            devices.clear();
            platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);
            for(int j=0;j<devices.size();j++)
            {
                if(print_device_names) std::cout<<devices[j].getInfo<CL_DEVICE_NAME>()<<"("<<device_indx<<"); ";
                if(device_indx==device_index)
                {
                    device=devices[j];
                    inited=true;
                }
                device_indx++;

            }

        }
        if(print_device_names) std::cout<<std::endl;

    if(!inited)call_error(1,"oclw_init","out of range","can not found device with device_index = ",{device_index});

    std::cout<<"using: "<<device.getInfo<CL_DEVICE_NAME>()<<std::endl;
    contextDevices.push_back(device);
    std::cout<<"device initialized"<<std::endl;
    context=cl::Context(contextDevices);
    std::cout<<"context initialized"<<std::endl;
    oclw_queue=cl::CommandQueue(context, device);


    std::cout<<"queue initialized"<<std::endl;



}



void OCLW::init_kernels(std::vector<std::string> kernel_names,std::string dir_path)
{
    if(!inited)
    {
        call_warning("oclw_init_kernels","warning","oclw not initialazed. initialazing...");
        init();
    }

    for(int i=0; i<kernel_names.size(); i++) // adding kernels
    {
        if(console_logs)std::cout<<"initializing the kernel: "<<kernel_names[i]<<std::endl;
        sourceFile.open((dir_path+kernel_names[i]+".cl"));
        if(sourceFile.peek()==EOF)call_error(0,"oclw_init_kernel","loading kernel error", "kernel name: "+dir_path+kernel_names[i]);
        sourceCode=std::string(std::istreambuf_iterator<char>(sourceFile),(std::istreambuf_iterator<char>()));
        source= cl::Program::Sources(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));
        program = cl::Program(context, source);
        program.build(contextDevices);
        kernels[kernel_names[i]]=cl::Kernel(program, kernel_names[i].c_str());
        sourceFile.close();
    }



    std::cout<<"done"<<std::endl<<std::endl;

}


bool OCLW::is_inited()
{
    return inited;
}
cl::Buffer* OCLW::get_variable(std::string key)
{
    return &variables[key];
}
void OCLW::set_variable(std::string key, cl::Buffer* variable)
{
    variables[key]=*variable;
}
void OCLW::add_variable(std::string key, cl_mem_flags mem_flag, size_t bufsize)
{
    variables[key]=cl::Buffer(context, mem_flag, bufsize);
}

void OCLW::process_oclw(std::string kernel_name, std::vector<std::string> variable_names, std::vector<float> floats, std::vector<int> ints, int s1, int s2, int s3)
{
    if(!inited)
    {
        call_error(1,"process_oclw","initialization error","oclw not initialazed");
        init();
    }

    iArg = 0;
    if(kernels.find(kernel_name)==kernels.end())//checking for the presence of a kernel
    {
        std::cout<<"process_oclw - kernel not found: "<<kernel_name<<std::endl;
        return;
    }
    kernel=kernels[kernel_name];

    for(int i=0; i<variable_names.size(); i++)// adding variables to kernel
    {
        if(variables.find(variable_names[i])==variables.end())call_error(1,"process_gpu","null variable error: ",variable_names[i]);
        kernel.setArg(iArg++, variables[variable_names[i]]);
    }
    for(int i=0; i<floats.size(); i++)kernel.setArg(iArg++, floats[i]);// adding floats to kernel
    for(int i=0; i<ints.size(); i++)kernel.setArg(iArg++, ints[i]);// adding ints to kernel

    if(s2==0)oclw_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(s1)); // starting a one-dimensional cycle
    else if(s3==0)oclw_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(s1,s2)); // starting a two-dimensional cycle
    else oclw_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(s1,s2,s3)); // starting a three-dimensional cycle
    oclw_queue.finish();
}
