#pragma once
#include <iostream>
#include <vector>
#include<windows.h>

#define console_logs 1

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
void set_color(ConsoleColor text=White, ConsoleColor background=Black);
void call_error(bool critical, std::string func, std::string type, std::string data="none",std::vector<float> int_data=std::vector<float>());
