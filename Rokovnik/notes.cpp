#include "notes.h"
#include <fstream>
#include <string>
#include <iomanip>
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

Notes::Notes(const std::string& filename) : filename(filename) {}

