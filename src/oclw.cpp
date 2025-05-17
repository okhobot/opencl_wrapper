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

std::vector<std::string> OCLW::get_available_devices_names()
{
    std::vector<std::string> names;

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    std::vector<cl::Device> devices;
    cl::Device device;
    int device_indx=0;
    for(int i=0; i<platforms.size(); i++)
    {
        devices.clear();
        platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);

        for(int j=0; j<devices.size(); j++)
        {
            names.push_back(devices[j].getInfo<CL_DEVICE_NAME>()+"("+std::to_string(device_indx)+");");
            device_indx++;
        }
    }
    return names;
}

void OCLW::init(int device_index)
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    std::vector<cl::Device> devices;
    cl::Device device;
    int device_indx=0;

    // searching for a device
    for(int i=0; i<platforms.size(); i++)
    {
        devices.clear();
        platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices);
        for(int j=0; j<devices.size(); j++)
        {
            if(device_indx==device_index)
            {
                device=devices[j];
                inited=true;
            }
            device_indx++;
        }

    }

    if(!inited)debug_utils::call_error(1,"oclw_init","out of range","can not found device with device_index = ", {device_index});

    if(logs_output)(*logs_output)<<"using: "<<device.getInfo<CL_DEVICE_NAME>()<<std::endl;
    contextDevices.push_back(device);
    if(logs_output)(*logs_output)<<"device initialized"<<std::endl;
    context=cl::Context(contextDevices);
    if(logs_output)(*logs_output)<<"context initialized"<<std::endl;
    oclw_queue=cl::CommandQueue(context, device);


    if(logs_output)(*logs_output)<<"queue initialized"<<std::endl;



}



void OCLW::init_kernels(std::vector<std::string> kernel_names,std::string dir_path)
{
    if(!inited)
    {
        debug_utils::call_warning("oclw_init_kernels","warning","oclw not initialazed. initialazing...");
        init();
    }

    for(int i=0; i<kernel_names.size(); i++) // adding kernels
    {
        if(logs_output)(*logs_output)<<"initializing the kernel: "<<kernel_names[i]<<std::endl;
        sourceFile.open((dir_path+kernel_names[i]+".cl"));
        if(sourceFile.peek()==EOF)debug_utils::call_error(0,"oclw_init_kernel","loading kernel error", "kernel name: "+dir_path+kernel_names[i]);
        sourceCode=std::string(std::istreambuf_iterator<char>(sourceFile),(std::istreambuf_iterator<char>()));
        source= cl::Program::Sources(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));
        program = cl::Program(context, source);
        program.build(contextDevices);
        kernels[kernel_names[i]]=cl::Kernel(program, kernel_names[i].c_str());
        sourceFile.close();
    }



    if(logs_output)(*logs_output)<<"done"<<std::endl;

}


bool OCLW::is_inited()
{
    return inited;
}
cl::Buffer* OCLW::get_variable(const std::string &key)
{
    return &variables[key];
}
void OCLW::set_variable(const std::string &key, const cl::Buffer &variable)
{
    variables[key]=variable;
}
void OCLW::add_variable(const std::string &key, cl_mem_flags mem_flag, size_t bufsize)
{
    variables[key]=cl::Buffer(context, mem_flag, bufsize);
}

void OCLW::delete_variable(const std::string &key)
{
    variables.erase(key);
}

void OCLW::write_variable(const std::string &key, size_t bufsize, void *data)
{
    oclw_queue.enqueueWriteBuffer(variables[key], CL_TRUE, 0, bufsize, data);
}

void OCLW::read_variable(const std::string &key, size_t bufsize, void *data)
{
    oclw_queue.enqueueReadBuffer(variables[key], CL_TRUE, 0, bufsize, data);
}

void OCLW::add_and_write_variable(const std::string &key, cl_mem_flags mem_flag, size_t bufsize, void *data)
{
    variables[key]=cl::Buffer(context, mem_flag, bufsize);
    oclw_queue.enqueueWriteBuffer(variables[key], CL_TRUE, 0, bufsize, data);
}

std::vector<std::string> OCLW::get_variables_keys()
{
    std::vector<std::string> keys;

    for(std::map<std::string,cl::Buffer>::iterator it = variables.begin(); it != variables.end(); ++it)
        keys.push_back(it->first);
    return keys;
}

void OCLW::process_oclw(const std::string &kernel_name, std::vector<std::string> variable_names, std::vector<float> floats, std::vector<long long> ints, size_t s1, size_t s2, size_t s3)
{
    if(!inited)
    {
        debug_utils::call_warning("process_oclw","oclw not initialazed","kernel_name: "+kernel_name);
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
        if(variables.find(variable_names[i])==variables.end())debug_utils::call_error(1,"process_gpu","null variable error: ","variable_name, kernel_name:"+variable_names[i]+", "+kernel_name);
        kernel.setArg(iArg++, variables[variable_names[i]]);
    }
    for(int i=0; i<floats.size(); i++)kernel.setArg(iArg++, floats[i]);// adding floats to kernel
    for(int i=0; i<ints.size(); i++)kernel.setArg(iArg++, ints[i]);// adding ints to kernel

    if(s2==0)oclw_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(s1)); // starting a one-dimensional cycle
    else if(s3==0)oclw_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(s1,s2)); // starting a two-dimensional cycle
    else oclw_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(s1,s2,s3)); // starting a three-dimensional cycle
    oclw_queue.finish();
}
