#include "processor.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

unsigned int reg_values[8];
unsigned int memory_values[5];
bool memory_used[5];
int zero_flag = 0;

bool isRegisterName(string txt)
{
    if (txt.size() != 2)
    {
        return false;
    }

    if (txt[0] != 'R')
    {
        return false;
    }

    if (txt[1] < '0' || txt[1] > '7')
    {
        return false;
    }

    return true;
}

int registerNumber(string txt)
{
    return txt[1] - '0';
}

bool isImmediateValue(string txt)
{
    if (txt.size() < 2)
    {
        return false;
    }

    if (txt[0] != '#')
    {
        return false;
    }

    return true;
}

bool isMemoryAddress(string txt)
{
    if (txt.size() < 3)
    {
        return false;
    }

    if (txt[0] != '0')
    {
        return false;
    }

    if (txt[1] != 'x' && txt[1] != 'X')
    {
        return false;
    }

    return true;
}

unsigned int readNumber(string txt)
{
    unsigned int number = 0;

    if (txt.size() > 0 && txt[0] == '#')
    {
        txt = txt.substr(1);
    }

    if (txt.size() > 1 && txt[0] == '0' && (txt[1] == 'x' || txt[1] == 'X'))
    {
        string only_hex = txt.substr(2);
        stringstream ss;
        ss << hex << only_hex;
        ss >> number;
    }
    else
    {
        stringstream ss;
        ss << txt;
        ss >> number;
    }

    return number;
}

bool getValueFromText(string text, unsigned int &value)
{
    if (isRegisterName(text))
    {
        value = reg_values[registerNumber(text)];
        return true;
    }

    if (isImmediateValue(text))
    {
        value = readNumber(text);
        return true;
    }

    return false;
}

bool addressInRange(unsigned int address)
{
    if (address < 0x100 || address > 0x110)
    {
        return false;
    }

    if ((address - 0x100) % 4 != 0)
    {
        return false;
    }

    return true;
}

int addressToIndex(unsigned int address)
{
    return (address - 0x100) / 4;
}

void printRegisterArray()
{
    int i;

    cout << "Register array:" << endl;

    for (i = 0; i < 8; i++)
    {
        cout << "R" << i << " =0x" << hex << reg_values[i] << dec;

        if (i != 7)
        {
            cout << " ";
        }
    }

    cout << endl;
}

void printMemoryArray()
{
    int i;

    cout << "Memory array:" << endl;

    for (i = 0; i < 5; i++)
    {
        if (memory_used[i] == false)
        {
            cout << "___";
        }
        else
        {
            cout << "0x" << hex << memory_values[i] << dec;
        }

        if (i != 4)
        {
            cout << ",";
        }
    }

    cout << endl;
}

void printArrays()
{
    printRegisterArray();
    printMemoryArray();
}

void printInvalidInstruction()
{
    cout << "Invalid Instruction." << endl;
}

void printInvalidCount()
{
    cout << "Invalid Instruction. Invalid Operand Count." << endl;
}

void printMemoryError()
{
    cout << "Invalid Instruction. out of range.\n";
}

void printInvalidRegister()
{
    cout << "Invalid register" << endl;
}

void doMOV(ParsedLine data)
{
    if (data.commaCount != 1)
    {
        printInvalidCount();
        return;
    }

    if (!isRegisterName(data.first))
    {
        printInvalidInstruction();
        return;
    }

    if (isRegisterName(data.second))
    {
        reg_values[registerNumber(data.first)] = reg_values[registerNumber(data.second)];
        printArrays();
        return;
    }

    if (isImmediateValue(data.second))
    {
        reg_values[registerNumber(data.first)] = readNumber(data.second);
        printArrays();
        return;
    }

    printInvalidInstruction();
}

void doMathOperation(ParsedLine data)
{
    if (data.commaCount != 2)
    {
        printInvalidCount();
        return;
    }

    if (!isRegisterName(data.first))
    {
        printInvalidInstruction();
        return;
    }

    if (isImmediateValue(data.second))
    {
        printInvalidRegister();
        return;
    }

    unsigned int value_two = 0;
    unsigned int value_three = 0;

    if (!getValueFromText(data.second, value_two))
    {
        printInvalidInstruction();
        return;
    }

    if (!getValueFromText(data.third, value_three))
    {
        printInvalidInstruction();
        return;
    }

    int where_to_save = registerNumber(data.first);

    if (data.command == "ADD")
    {
        reg_values[where_to_save] = value_two + value_three;
    }
    else if (data.command == "SUB")
    {
        reg_values[where_to_save] = value_two - value_three;
    }
    else if (data.command == "AND")
    {
        reg_values[where_to_save] = value_two & value_three;
    }
    else if (data.command == "OR")
    {
        reg_values[where_to_save] = value_two | value_three;
    }
    else if (data.command == "XOR")
    {
        reg_values[where_to_save] = value_two ^ value_three;
    }

    printArrays();
}

void doCMP(ParsedLine data)
{
    if (data.commaCount != 1)
    {
        printInvalidCount();
        return;
    }

    if (isImmediateValue(data.first))
    {
        printInvalidInstruction();
        return;
    }

    unsigned int value_one = 0;
    unsigned int value_two = 0;

    if (!getValueFromText(data.first, value_one))
    {
        printInvalidInstruction();
        return;
    }

    if (!getValueFromText(data.second, value_two))
    {
        printInvalidInstruction();
        return;
    }

    unsigned int result = value_one - value_two;

    if (result == 0)
    {
        zero_flag = 1;
    }
    else
    {
        zero_flag = 0;
    }

    printArrays();
}

void doLOAD(ParsedLine data)
{
    if (data.commaCount != 1)
    {
        printInvalidCount();
        return;
    }

    if (!isRegisterName(data.first))
    {
        printInvalidInstruction();
        return;
    }

    if (!isMemoryAddress(data.second))
    {
        printInvalidInstruction();
        return;
    }

    unsigned int address = readNumber(data.second);

    if (!addressInRange(address))
    {
        printMemoryError();
        return;
    }

    int index = addressToIndex(address);
    reg_values[registerNumber(data.first)] = memory_values[index];

    printArrays();
}

void doSTORE(ParsedLine data)
{
    if (data.commaCount != 1)
    {
        printInvalidCount();
        return;
    }

    if (!isRegisterName(data.first))
    {
        printInvalidInstruction();
        return;
    }

    if (!isMemoryAddress(data.second))
    {
        printInvalidInstruction();
        return;
    }

    unsigned int address = readNumber(data.second);

    if (!addressInRange(address))
    {
        printMemoryError();
        return;
    }

    int index = addressToIndex(address);
    memory_values[index] = reg_values[registerNumber(data.first)];
    memory_used[index] = true;

    printArrays();
}

void doBranch(ParsedLine data)
{
    if (data.commaCount != 0)
    {
        printInvalidCount();
        return;
    }

    if (data.first == "")
    {
        printInvalidInstruction();
        return;
    }

    if (data.command == "BAL")
    {
        cout << "Branch taken." << endl;
    }
    else if (data.command == "BEQ")
    {
        if (zero_flag == 1)
        {
            cout << "Branch taken." << endl;
        }
        else
        {
            cout << "Branch not taken." << endl;
        }
    }
    else if (data.command == "BNE")
    {
        if (zero_flag == 0)
        {
            cout << "Branch taken." << endl;
        }
        else
        {
            cout << "Branch not taken." << endl;
        }
    }

    printArrays();
}

void startProcessor()
{
    int i;

    for (i = 0; i < 8; i++)
    {
        reg_values[i] = 0;
    }

    for (i = 0; i < 5; i++)
    {
        memory_values[i] = 0;
        memory_used[i] = false;
    }

    zero_flag = 0;
}

void runInstruction(ParsedLine data)
{
    if (data.empty_line == true)
    {
        return;
    }

    cout << data.original << endl;

    if (data.command == "MOV")
    {
        doMOV(data);
    }
    else if (data.command == "ADD" || data.command == "SUB" || data.command == "AND" || data.command == "OR" || data.command == "XOR")
    {
        doMathOperation(data);
    }
    else if (data.command == "CMP")
    {
        doCMP(data);
    }
    else if (data.command == "LOAD")
    {
        doLOAD(data);
    }
    else if (data.command == "STORE")
    {
        doSTORE(data);
    }
    else if (data.command == "BAL" || data.command == "BEQ" || data.command == "BNE")
    {
        doBranch(data);
    }
    else
    {
        printInvalidInstruction();
    }

    cout << endl;
}