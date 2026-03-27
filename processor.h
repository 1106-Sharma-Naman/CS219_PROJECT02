#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include <string>
using namespace std;

#include "parser.h"

void startProcessor();
void runInstruction(ParsedLine data);

#endif