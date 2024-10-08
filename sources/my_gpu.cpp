#include "my_gpu.hpp"

#define print_device_names 1
using namespace std;

void GPU::operator = (GPU &_gpu)
{
        gpu_queue=_gpu.gpu_queue;
        context=_gpu.context;
        contextDevices=_gpu.contextDevices;
        kernels=_gpu.kernels;
        kernel=_gpu.kernel;
        sourceCode=_gpu.sourceCode;
        source=_gpu.source;
        program=_gpu.program;
        iArg=_gpu.iArg;
}

void GPU::init_gpu(vector<std::string> kernel_names,std::string dir_path, int processing_unit_index)
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    std::vector<cl::Device> devices;
    cl::Device device;
    int dindx=0;

    if(print_device_names)
    {
        std::cout<<"device_names: ";
        for(int i=0;i<platforms.size();i++)
        {
            devices.clear();
            platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);
            for(int j=0;j<devices.size();j++)
            {
                std::cout<<devices[j].getInfo<CL_DEVICE_NAME>()<<"("<<dindx<<"); ";
                if(dindx==processing_unit_index)device=devices[j];
                dindx++;

            }

        }
        //std::cout<<endl<<"USING_GPU"<<endl;
        std::cout<<std::endl;
    }




    //platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
    //platforms[processing_unit_index].getDevices(CL_DEVICE_TYPE_ALL, &devices);
    //cout<<platforms.size()<<endl;

    cout<<"using: "<<device.getInfo<CL_DEVICE_NAME>()<<endl;
    contextDevices.push_back(device);
    cout<<"device initialized"<<endl;
    context=cl::Context(contextDevices);
    cout<<"context initialized"<<endl;
    gpu_queue=cl::CommandQueue(context, device);


    cout<<"queue initialized"<<endl;

    for(int i=0; i<kernel_names.size(); i++)
    {
        if(console_logs)cout<<"initializing the kernel: "<<kernel_names[i]<<endl;
        sourceFile.open((dir_path+kernel_names[i]+".cl"));
        if(sourceFile.peek()==EOF)call_error(0,"init_gpu","loading kernel error", "kernel name: "+dir_path+kernel_names[i]);
        sourceCode=std::string(std::istreambuf_iterator<char>(sourceFile),(std::istreambuf_iterator<char>()));
        source= cl::Program::Sources(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));
        program = cl::Program(context, source);
        program.build(contextDevices);
        kernels[kernel_names[i]]=cl::Kernel(program, kernel_names[i].c_str());
        sourceFile.close();
    }
    cout<<"done"<<endl<<endl;

}


void GPU::add_variable(std::string key, cl_mem_flags mem_flag, size_t bufsize)
{
    variables[key]=cl::Buffer(context, mem_flag, bufsize);
}
cl::Buffer* GPU::get_variable(std::string key)
{
    return &variables[key];
}
void GPU::set_variable(std::string key, cl::Buffer* variable)
{
    variables[key]=*variable;
}


void GPU::process_gpu(std::string kernel_name, std::vector<std::string> variable_names, std::vector<float> floats, std::vector<int> ints, int s1, int s2, int s3)
{
    iArg = 0;
    if(kernels.find(kernel_name)==kernels.end())
    {
        cout<<"process_gpu - kernel not found: "<<kernel_name<<endl;
        return;
    }
    kernel=kernels[kernel_name];

    for(int i=0; i<variable_names.size(); i++)
    {
        if(variables.find(variable_names[i])==variables.end())call_error(1,"process_gpu","null variable error: ",variable_names[i]);
        kernel.setArg(iArg++, variables[variable_names[i]]);
    }
    for(int i=0; i<floats.size(); i++)kernel.setArg(iArg++, floats[i]);
    for(int i=0; i<ints.size(); i++)kernel.setArg(iArg++, ints[i]);

    if(s2==0)gpu_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(s1));
    else if(s3==0)gpu_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(s1,s2));
    else gpu_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(s1,s2,s3));
    gpu_queue.finish();
}
