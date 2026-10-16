#define _CRT_SECURE_NO_WARNINGS
#include "datetime.h"
#include <ctime>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
int daysUntil(const std::tm& date) {
	std::tm target = date;
	target.tm_hour = 12;
	target.tm_min = 0;
	target.tm_sec = 0;
	target.tm_isdst = -1;

	std::time_t now = std::time(nullptr);
	std::tm today = *std::localtime(&now);
	today.tm_hour = 12;
	today.tm_min = 0;
	today.tm_sec = 0;
	today.tm_isdst = -1;

	double diff = std::difftime(std::mktime(&target), std::mktime(&today));
	return (int)std::round(diff / (60 * 60 * 24));
}
void formatDaysUntil(std::string& str, const std::tm& date) {
	int days = daysUntil(date);
	int years, mons, daysM;
	if (days < 0) {
		str = "Passed";
		return;
	}
	switch (days) {
	case 0:
		str = "Today";
		break;
	case 1:
		str = "Tomorrow";
		break;
	default: {
		years = days / 365;
		mons = (days % 365) / 30;
		daysM = days % 365 % 30;
		str = "Time until: ";
		std::string parts;
		if (years)
			parts += std::to_string(years) + (years == 1 ? " year" : " years");
		if (mons) {
			if (!parts.empty()) parts += ", ";
			parts += std::to_string(mons) + (mons == 1 ? " month" : " months");
		}
		if (daysM) {
			if (!parts.empty()) parts += ", ";
			parts += std::to_string(daysM) + (daysM == 1 ? " day" : " days");
		}
		str += parts;
		break;
	}
	}
}
bool parseDate(const std::string& s, std::tm& out) {
	std::tm d{};
	std::istringstream ss(s);
	ss >> std::get_time(&d, "%d.%m.%Y");
	if (ss.fail())
		return false;
	std::tm copy = d;
	copy.tm_hour = 12;
	std::mktime(&copy);
	if (copy.tm_mday != d.tm_mday || copy.tm_mon != d.tm_mon || copy.tm_year != d.tm_year)
		return false;
	out = d;
	return true;
}