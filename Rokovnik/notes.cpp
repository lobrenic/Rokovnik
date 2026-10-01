#include "notes.h"
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <iostream>
#include <sstream>

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
			std::put_time(&n.date,"%d.%m.%Y") << "$" <<
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
		if (p1 == std::string::npos || p2 == std::string::npos)continue;
		
		try {
			newNote.id = std::stoi(line.substr(0, p1));
		}
		catch (...) {
			continue;
		}

		
		std::string dateStr= line.substr(p1 + 1, p2 - p1-1);
		newNote.text = line.substr(p2 + 1);
		std::istringstream ss(dateStr);
		ss >> std::get_time(&newNote.date, "%d.%m.%Y");
		if (ss.fail()) continue;
		notes.push_back(newNote);
	}


}

const std::vector<Note>& Notes::getAllNotes() const {
	return notes;
}

	