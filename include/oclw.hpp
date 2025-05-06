#ifndef MY_GPU_HPP_
#define MY_GPU_HPP_

#include <CL/cl.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "debug_utils.hpp"

class OCLW
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

    bool console_logs=true;

public:


    void init(int processing_unit_index=0, bool debug=true, bool print_device_names=false);

    void init_kernels(std::vector<std::string> kernel_names, std::string dir_path="");//add kernels to map

    void operator = (OCLW &_oclw);



    bool is_inited();

    cl::Buffer* get_variable(const std::string &key);//get buffer by key

    void set_variable(const std::string &key,const cl::Buffer &variable);//set buffer in dictionary

    void add_variable(const std::string &key, cl_mem_flags mem_flag, size_t bufsize);// add new variable

    void add_and_write_variable(const std::string &key, cl_mem_flags mem_flag, size_t bufsize, void *data);// add_variable and write_variable

    void delete_variable(const std::string &key);//delete buffer in dictionary


    void write_variable(const std::string &key, size_t bufsize, void *data);//write vector to buffer

    void read_variable(const std::string &key, size_t bufsize, void *data);//write buffer to output vector

    void process_oclw(const std::string &kernel_name, std::vector<std::string> variable_names, std::vector<float> floats, std::vector<long long> ints, size_t s1, size_t s2=0, size_t s3=0);//run kernel


    void set_console_logs(bool enabled=true)
    {
        console_logs=enabled;
    }

    std::vector<std::string> get_variables_keys();//get keys of variables map
};
#endif
