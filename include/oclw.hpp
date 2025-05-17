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


    void init(int device_index=0);

    void init_kernels(std::vector<std::string> kernel_names, std::string dir_path="");//add kernels to map

    void operator = (OCLW &_oclw);



    bool is_inited();

    cl::Buffer* get_variable(const std::string &key);//get buffer by key

    void set_variable(const std::string &key,const cl::Buffer &variable);//set buffer in dictionary

    void add_variable(const std::string &key, cl_mem_flags mem_flag, size_t bufsize);// add new buffer to dictionary

    void add_and_write_variable(const std::string &key, cl_mem_flags mem_flag, size_t bufsize, void *data);// add_variable and write_variable

    void delete_variable(const std::string &key);//delete buffer from dictionary


    void write_variable(const std::string &key, size_t bufsize, void *data);//write vector to buffer

    void read_variable(const std::string &key, size_t bufsize, void *data);//read buffer to output vector

    void process_oclw(const std::string &kernel_name, std::vector<std::string> variable_names, std::vector<float> floats, std::vector<long long> ints, size_t s1, size_t s2=0, size_t s3=0);//run kernel
    //variable_names - names of buffers in dictionary
    //floats - float variables
    //ints - float variables
    //s1,s2,s3 - NDRange params
    //the arguments are passed to the .cl file in the same sequence as in this function.


    void set_logs_output(std::ostream *out)//set output stream for logs
    {
        logs_output=out;
    }

    std::vector<std::string> get_variables_keys();//get keys of variables map

    std::vector<std::string> get_available_devices_names();
};
#endif
