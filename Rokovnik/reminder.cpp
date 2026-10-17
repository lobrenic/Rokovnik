#define _CRT_SECURE_NO_WARNINGS
#include "reminder.h"
#include <iomanip>
#include <sstream>
#include "datetime.h"
std::string buildReminderText(const std::vector<Note>& exams) {
	std::ostringstream out;
	std::string until;
	for (const Note& n : exams) {
		formatDaysUntil(until, n.date);
		const ExamInfo& e = *n.exam;
		out << e.subject << " - "
			<< std::put_time(&n.date, "%d.%m.%Y") << " u "
			<< std::setfill('0') << std::setw(2) << e.hour << ":"
			<< std::setw(2) << e.minute << std::setfill(' ');
		if (!e.room.empty())
			out << ", sala " << e.room;
		out << " (" << until << ")\n";
	}
	return out.str();
}