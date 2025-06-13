#ifndef MY_GPU_HPP_
#define MY_GPU_HPP_

#include <CL/cl.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <debug_utils.hpp>

#if OCLW_EXPORTS
#define OCLW_API __attribute__((visibility ("default")))
#else
#define OCLW_API
#endif //OCLW_EXPORTS

class OCLW_API OCLW
{
private:
    std::map<std::string, cl::Buffer> variables;
    std::map<std::string,cl::Kernel> kernels;

    std::vector<cl::Device> contextDevices;

    cl::CommandQueue oclw_queue;
    cl::Context context;
    cl::Kernel kernel;
    cl::Program::Sources source;
    cl::Program program;

    std::ifstream sourceFile;
    std::string sourceCode;

    int iArg;
    bool inited=false;

    std::ostream *logs_output=0;

public:

    void operator = (OCLW &_oclw);

    void init(int device_index=0);

    void init_kernels(std::vector<std::string> kernel_names, std::string dir_path="");//add kernels to map


    void process_oclw(const std::string &kernel_name, std::vector<std::string> variable_names, std::vector<float> floats, std::vector<long long> ints, size_t s1, size_t s2=0, size_t s3=0);//run kernel
    //variable_names - names of buffers in dictionary
    //floats - float variables
    //ints - float variables
    //s1,s2,s3 - NDRange params
    //the arguments are passed to the .cl file in the same sequence as in this function.


    void read_variable(const std::string &key, size_t bufsize, void *data)//read buffer to output vector
    {
        oclw_queue.enqueueReadBuffer(variables[key], CL_TRUE, 0, bufsize, data);
    }



    //setters
    void set_logs_output(std::ostream *out)//set output stream for logs
    {
        logs_output=out;
    }
    void set_variable(const std::string &key, const cl::Buffer &variable)//set buffer in dictionary
    {
        variables[key]=variable;
    }
    void add_variable(const std::string &key, cl_mem_flags mem_flag, size_t bufsize)// add new buffer to dictionary
    {
        variables[key]=cl::Buffer(context, mem_flag, bufsize);
    }
    void delete_variable(const std::string &key)//delete buffer from dictionary
    {
        variables.erase(key);
    }
    void write_variable(const std::string &key, size_t bufsize, void *data)//write vector to buffer
    {
        oclw_queue.enqueueWriteBuffer(variables[key], CL_TRUE, 0, bufsize, data);
    }
    void add_and_write_variable(const std::string &key, cl_mem_flags mem_flag, size_t bufsize, void *data)// add_variable and write_variable
    {
        variables[key]=cl::Buffer(context, mem_flag, bufsize);
        oclw_queue.enqueueWriteBuffer(variables[key], CL_TRUE, 0, bufsize, data);
    }


    //getters
    std::vector<std::string> get_variables_keys();//get keys of variables map

    std::vector<std::string> get_available_devices_names();

    cl::Buffer* get_variable(const std::string &key)
    {
        return &variables[key];   //get buffer by key
    }

    cl::CommandQueue* get_cl_command_queue_ptr()
    {
        return &oclw_queue;
    }

    bool is_inited()
    {
        return inited;
    }
};
#endif
