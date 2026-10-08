#define _CRT_SECURE_NO_WARNINGS
#include "notes.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <ctime>
void Notes::addNote(const Note& note) {
	Note n = note;
	n.id = generateId();
	notes.push_back(n);
	save();
}

int Notes::generateId() const{
	int maxId = 0;
	for (const Note& n : notes) {
		if (n.id > maxId)
			maxId = n.id;

	}
	return maxId + 1;
}

void Notes::save()const {
	std::ofstream File(filename);
	for (const Note& n : notes) {
		File << n.id << '$' <<
			std::put_time(&n.date,"%d.%m.%Y") << '$' <<typeToChar(n.type)<<'$'<<
			n.text << std::endl;

	}
	File.close();
}

Notes::Notes(const std::string& filename) : filename(filename) {
	load();
}

void Notes::load() {
	notes.clear();
	std::ifstream File(filename);
	std::string line;
	while (std::getline(File, line)) {
		Note newNote{};
		auto p1 = line.find('$');
		auto p2 = line.find('$', p1 + 1);
		auto p3 = line.find('$', p2 + 1);
		if (p1 == std::string::npos || p2 == std::string::npos ||p3==std::string::npos)continue;
		
		try {
			newNote.id = std::stoi(line.substr(0, p1));
		}
		catch (...) {
			continue;
		}

		
		std::string dateStr= line.substr(p1 + 1, p2 - p1-1);
		std::string typeStr = line.substr(p2 + 1, p3 - p2 - 1);
		if (!typeStr.empty())
			newNote.type = charToType(typeStr[0]);
		newNote.text = line.substr(p3+1);
		std::istringstream ss(dateStr);
		ss >> std::get_time(&newNote.date, "%d.%m.%Y");
		if (ss.fail()) continue;
		notes.push_back(newNote);
	}


}

const std::vector<Note>& Notes::getAllNotes() const {
	return notes;
}

std::vector<Note> Notes::getAllNotesForDate(const std::tm& date) const {
	std::vector<Note> noteV;
	for (const Note& n : notes) {
		if (n.date.tm_year == date.tm_year &&
			n.date.tm_mday == date.tm_mday &&
			n.date.tm_mon == date.tm_mon) {
			noteV.push_back(n);
		}
	}
	return noteV;
	
}
bool Notes::removeNote(int id) {
	for (auto it = notes.begin(); it != notes.end(); it++) {
		if (it->id == id) {
			notes.erase(it);
			save();
			return true;
		}
	}
	return false;
}
void Notes::editNote(int id, const std::tm& newDate,NoteType newType,const std::string& newText) {
	
	for (Note& n : notes) {
		if (n.id == id) {
			n.text = newText;
			n.date = newDate;
			n.type = newType;
			save();
			return;
		}
	}
	throw std::out_of_range("Beleska ne postoji");
}

std::vector<Note> Notes::getUpcomingNotes() const {
	std::vector<Note> res;
	for (const Note& n : notes) {
		if (!isPast(n.date))
			res.push_back(n);
	}
	return res;
}

bool Notes::isPast(const std::tm& date) const{
	std::time_t timestamp = time(nullptr);
	std::tm today = *localtime(&timestamp);
	if (date.tm_year != today.tm_year)
		return date.tm_year < today.tm_year;
	if (date.tm_mon != today.tm_mon)
		return date.tm_mon < today.tm_mon;
	return date.tm_mday < today.tm_mday;

}

std::vector<Note> Notes::getAllExams() const {
	std::vector<Note> temp=getUpcomingNotes();
	std::vector<Note> exams;
	for (const Note& n : temp) {
		if (n.type == NoteType::Exam)
			exams.push_back(n);
	}
	return exams;
}

void Notes::searchByText(const std::string& query, std::vector<Note>& list) const {
	std::string queryCopy = query;
	toLower(queryCopy);
	list.clear();
	for (const Note& note : notes) {
		std::string textCopy=note.text;
		toLower(textCopy);
		if (textCopy.find(queryCopy) != std::string::npos)
			list.push_back(note);

	}
	
}

void Notes::toLower(std::string& s)const {
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
		return std::tolower(c);
	});
}