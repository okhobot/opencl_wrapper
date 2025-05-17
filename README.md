# opencl_wrapper
A class for more convenient use of the openсl library at the basic level


## Features
- Uses OpenCL
- Runs functions written in .cl files using parallel computing
- The class stores a dictionary for cl::Buffer, which can be used to store variables
- contains a one-header library for debugging in the console


## Usage
- The necessary code is written in the functions of the .cl files
- The file name must match the function name. For example, test_kernel.cl contains the __kernel void test_kernel function
- Before using it, you need to call the init method, in which you can specify the index of the device on which you want to execute the .cl file code
- Devices and their indexes can be obtained using the get_available_devices_names() method
- The set_logs_output method is used to set the log stream
- The init_kernel function is used to initialize new kernels

- It is recommended to use the release branch 


## Examples
- An example of using the library is described in the file main.cpp in the main branch
- An example of a .cl file is described in the file test.cl in the main branch


## License
GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007