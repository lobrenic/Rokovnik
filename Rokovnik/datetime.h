#pragma once
#include <ctime>
#include <string>
int daysUntil(const std::tm& date);
void formatDaysUntil(std::string& str, const std::tm& date);
bool parseDate(const std::string& s, std::tm& out);