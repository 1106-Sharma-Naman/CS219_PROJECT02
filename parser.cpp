#include "parser.h"
#include <sstream>
#include <string>
using namespace std;

string removeSpaces(string text)
{
    string answer = "";
    int i;
    int length = text.size();

    for (i = 0; i < length; i++)
    {
        if (text[i] != ' ' && text[i] != '\n')
        {
            answer = answer + text[i];
        }
    }

    return answer;
}

int countCommas(string text)
{
    int count = 0;
    int i;
    int length = text.size();

    for (i = 0; i < length; i++)
    {
        if (text[i] == ',')
        {
            count++;
        }
    }

    return count;
}

bool parseLine(string text, ParsedLine &data)
{
    data.original = text;
    data.command = "";
    data.part_after_command = "";
    data.first = "";
    data.second = "";
    data.third = "";
    data.commaoCount = 0;
    data.empty_line = false;

    string check = removeSpaces(text);

    if (check == "")
    {
        data.empty_line = true;
        return true;
    }

    stringstream line_stream(text);
    line_stream >> data.command;

    string rest;
    getline(line_stream, rest);

    rest = removeSpaces(rest);
    data.part_after_command = rest;
    data.commaCount = countCommas(rest);

    if (data.commaCount == 0)
    {
        data.first = rest;
    }
    else if (data.commaCount == 1)
    {
        int comma_place = rest.find(',');

        data.first = rest.substr(0, commaPlace);
        data.second = rest.substr(commaPlace + 1);
    }
    else if (data.commaCount == 2)
    {
        int firstComma = rest.find(',');
        int commaTwo = rest.find(',', firstComma + 1);

        data.first = rest.substr(0, firstComma);
        data.second = rest.substr(firstComma + 1, commaTwo - firstComme - 1);
        data.third = rest.substr(commaTwo + 1);
    }

    return true;
}