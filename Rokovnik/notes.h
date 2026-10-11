#pragma once
#include <ctime>
#include <string>
#include <vector>
#include <optional>
#include "database.h"

namespace sql {
	class ResultSet;
}
struct ExamInfo {
	std::string subject;
	std::string room;
	int hour = 0;
	int minute = 0;
	int durationMin = 90;
};

struct Note {
	int id=-1;
	std::tm date{};
	std::string text;
	std::optional<ExamInfo> exam;
};

class Notes {
private:
	Database& db;

	static std::string dateToSql(const std::tm& date);
	static std::tm sqlToDate(const std::string& s);
	static Note rowToNote(sql::ResultSet& rs);
	static std::string timeToSql(int hour, int minute);
public:
	explicit Notes(Database& db);
	std::vector<Note> getAllNotes() const;
	std::vector<Note> getAllNotesForDate(const std::tm& date) const;
	void addNote(const Note& note);
	bool removeNote(int id);
	void editNote(int id,const std::tm& newDate,const std::optional<ExamInfo>& newExam,const std::string& newText);
	std::vector<Note> getUpcomingNotes() const;
	bool isPast(const std::tm& date) const;
	std::vector<Note> getAllExams() const;
	std::vector<Note> searchByText(const std::string& query) const;
};