#include <iostream>
#include "helpers.h"
#include "MyGraph.h"

int main(int argc, char **argv) 
{
    while (!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);
        std::string command = line_parser::get_command(line, graph);
        line_parser::analyze_command(command, line, graph);    }
    
 return 0;
}
