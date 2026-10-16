#pragma once
#include <iostream>
#include "notes.h"
#include <string>
#include <ctime>

void clearScreen();
std::string readLine();
void pause(const std::string& message);
int readInt();
bool readYesNo(const std::string& prompt);
bool readDate(const std::string prompt, std::tm& out);
void readExamInfo(ExamInfo& e);