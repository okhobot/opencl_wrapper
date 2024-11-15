#ifndef MY_GPU_HPP_
#define MY_GPU_HPP_

#include <CL/cl.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "debugger.hpp"

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

public:

    void init_oclw(std::vector<std::string> kernel_names, std::string dir_path="", int processing_unit_index=0);//add kernels

    void operator = (OCLW &_oclw);



    bool is_inited();
    cl::Buffer* get_variable(std::string key);//get buffer by key
    void set_variable(std::string key,cl::Buffer* variable);//set buffer in dictionary
    void add_variable(std::string key, cl_mem_flags mem_flag, size_t bufsize);// add new variable


    template <typename T>
    void write_variable(std::string key, size_t bufsize, std::vector<T> &data)//write vector to buffer
    {
        oclw_queue.enqueueWriteBuffer(variables[key], CL_TRUE, 0, bufsize, data.data());
    }

    template <typename T>
    void read_variable(std::string key, size_t bufsize, std::vector<T> &data)//write buffer to output vector
    {
        oclw_queue.enqueueReadBuffer(variables[key], CL_TRUE, 0, bufsize, data.data());
    }

    void process_oclw(std::string kernel_name, std::vector<std::string> variable_names, std::vector<float> floats, std::vector<int> ints, int s1, int s2=0, int s3=0);//run kernel


};
#endif
