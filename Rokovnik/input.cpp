#include "input.h"
#include "datetime.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

void clearScreen() {
	std::system("cls");
}
std::string readLine() {
	std::string s;
	if (!std::getline(std::cin, s))
		throw std::runtime_error("Ulaz je zatvoren");
	return s;
}
void pause(const std::string& message) {
	std::cout << "\n" << message;
	readLine();
}
int readInt() {
	std::string s = readLine();
	try {
		size_t pos;
		int value = std::stoi(s, &pos);
		if (pos != s.size())
			throw std::invalid_argument("");
		return value;
	}
	catch (const std::exception&) {
		throw std::invalid_argument("Unos nije ceo broj");
	}
	
}
bool readYesNo(const std::string& prompt) {
	std::cout << prompt;
	std::string s = readLine();
	if (s == "d" || s == "D") return true;
	if (s == "n" || s == "N") return false;
	throw std::invalid_argument("Unos mora biti d ili n");
}
bool readDate(const std::string prompt, std::tm& out) {
	std::cout << prompt;
	std::string s = readLine();
	if (s == "0")
		return false;
	if(!parseDate(s,out))
		throw std::invalid_argument("Neispravan datum");
	return true;
}
void readExamInfo(ExamInfo& e) {
	std::cout << "Predmet: ";
	e.subject = readLine();
	if (e.subject.empty())
		throw std::invalid_argument("Predmet ne sme biti prazan");
	std::cout << "Sala (moze prazno): ";
	e.room = readLine();

	std::cout << "Vreme pocetka (HH:MM): ";
	std::tm t{};
	std::istringstream ss(readLine());

	ss >> std::get_time(&t, "%H:%M");
	if (ss.fail())
		throw std::invalid_argument("Neispravno vreme");
	e.hour = t.tm_hour;
	e.minute = t.tm_min;

	std::cout << "Trajanje u minutima: ";
	e.durationMin = readInt();
	if (e.durationMin <= 0 || e.durationMin > 600)
		throw std::invalid_argument("Trajanje mora biti izmedju 1 i 600 minuta");
}