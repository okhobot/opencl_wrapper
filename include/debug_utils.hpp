#pragma once
#include <iostream>
#include <vector>

namespace debug_utils
{
enum ConsoleColor
{
    Black = 0,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    LightGray,
    DarkGray,
    LightRed,
    LightGreen,
    LightYellow,
    LightBlue,
    LightMagenta,
    LightCyan,
    White
};



inline void set_color(ConsoleColor text=White, ConsoleColor background=Black)//set text color in std::cout
{
    int text_code = static_cast<int>(text);
    int bg_code = static_cast<int>(background);

    // ANSI: 30-37 = foreground standard, 90-97 = foreground bright
    //       40-47 = background standard, 100-107 = background bright
    int fg = (text_code <= 7) ? 30 + text_code : 90 + (text_code - 8);
    int bg = (bg_code <= 7) ? 40 + bg_code : 100 + (bg_code - 8);

    std::cout << "\033[" << fg << ";" << bg << "m";
}
inline void call_error(bool critical, std::string func, std::string type, std::string data="none",std::vector<float> float_data=std::vector<float>())//print error to std::cout
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

inline void call_warning(std::string func, std::string type, std::string data="none", std::vector<float> float_data=std::vector<float>())//print warning to std::cout
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
