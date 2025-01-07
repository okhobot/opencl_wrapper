#pragma once
#include <iostream>
#include <vector>
#include<windows.h>

namespace debug_utils
{
    enum ConsoleColor
    {
        Black         = 0,
        Blue          = 1,
        Green         = 2,
        Cyan          = 3,
        Red           = 4,
        Magenta       = 5,
        Brown         = 6,
        LightGray     = 7,
        DarkGray      = 8,
        LightBlue     = 9,
        LightGreen    = 10,
        LightCyan     = 11,
        LightRed      = 12,
        LightMagenta  = 13,
        Yellow        = 14,
        White         = 15
    };
    inline void set_color(ConsoleColor text=White, ConsoleColor background=Black);//set text color
    inline void call_error(bool critical, std::string func, std::string type, std::string data="none",std::vector<float> float_data=std::vector<float>());//print error to std::out
    inline void call_warning(std::string func, std::string type, std::string data, std::vector<float> float_data=std::vector<float>());//print warning to std::out



    //definitions
    void set_color(ConsoleColor text, ConsoleColor background)
    {
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
    }
    void call_error(bool critical,std::string func, std::string type, std::string data, std::vector<float> float_data)
    {
        set_color(Red);

        std::cout<<type<<" in "<<func;

        if(data != "none")
        {
            std::cout<<" - "<<data;
            for(int i=0; i<float_data.size(); i++)std::cout<<float_data[i]<<"; ";
            std::cout<<std::endl;
        }


        set_color();

        std::cout<<std::endl;
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
            for(int i=0; i<float_data.size(); i++)std::cout<<float_data[i]<<"; ";
            std::cout<<std::endl;
        }
        else std::cout<<type<<" in "<<func<<std::endl;

        set_color();
    }
}
