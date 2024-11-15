#include "debug_utils.hpp"

void set_color(ConsoleColor text, ConsoleColor background)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
void call_error(bool critical,std::string func, std::string type, std::string data, std::vector<float> float_data)
{
    set_color(Red);
    if(data != "none")
    {
        std::cout<<type<<" in "<<func<<" - "<<data;
        for(int i=0;i<float_data.size();i++)std::cout<<float_data[i]<<"; ";
        std::cout<<std::endl;
    }
    else std::cout<<type<<" in "<<func<<std::endl;

    set_color();

    if(critical)
    {
        std::string tmp;
        std::cin>>tmp;
    }
}

void call_warning(std::string func, std::string type, std::string data, std::vector<float> float_data)
{
    set_color(Yellow);
    if(data != "none")
    {
        std::cout<<type<<" in "<<func<<" - "<<data;
        for(int i=0;i<float_data.size();i++)std::cout<<float_data[i]<<"; ";
        std::cout<<std::endl;
    }
    else std::cout<<type<<" in "<<func<<std::endl;

    set_color();
}
