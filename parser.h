#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
using namespace std;

struct ParsedLine
{
    string original;
    string command;
    string part_after_command;
    string first;
    string second;
    string third;
    int commaCount;
    bool empty_line;
};

string removeSpaces(string text);
int countCommas(string text);
bool parseLine(string text, ParsedLine &data);

#endif