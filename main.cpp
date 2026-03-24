// By - Naman Sharma
// Project 02 CS219
// Date - 03/23/2026

#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"
#include "processor.h"

using namespace std;

int main()
{
    ifstream file("pp2_input.txt");

    if (!file.is_open())
    {
        cout << "Could not open input file." << endl;
        return 1;
    }

    startProcessor();

    string one_line;

    while (getline(file, one_line))
    {
        ParsedLine data;
        parseLine(one_line, data);
        runInstruction(data);
    }

    file.close();

    return 0;
}