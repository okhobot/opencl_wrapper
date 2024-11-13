#include "debugger.hpp"
void set_color(ConsoleColor text, ConsoleColor background)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
void call_error(bool critical,std::string func, std::string type, std::string data, std::vector<float> int_data)
{
    set_color(Red);
    if(data != "none")
    {
        std::cout<<type<<" in "<<func<<" - "<<data;
        for(int i=0;i<int_data.size();i++)std::cout<<int_data[i]<<"; ";
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
