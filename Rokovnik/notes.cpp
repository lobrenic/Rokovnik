#define _CRT_SECURE_NO_WARNINGS
#include "notes.h"
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <jdbc/cppconn/connection.h>
#include<jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/exception.h>
#include <memory>
namespace {
	const char* SELECT_NOTES =
		"SELECT n.id, n.note_date, n.text, "
		"e.subject, e.room, e.start_time, e.duration_min "
		"FROM note n LEFT JOIN exam e ON e.note_id = n.id ";
}
void Notes::addNote(const Note& note) {
	if (isPast(note.date))
		throw std::invalid_argument("Datum je u proslosti");
	sql::Connection& con = db.connection();
	try {
		con.setAutoCommit(false);
		{
			std::unique_ptr<sql::PreparedStatement> ps(
				con.prepareStatement("INSERT INTO note (note_date, text) VALUES (?, ?)"));
			ps->setString(1, dateToSql(note.date));
			ps->setString(2, note.text);
			ps->executeUpdate();
		}
		if (note.exam) {
			int newId = 0;
			{
				std::unique_ptr<sql::Statement> st(con.createStatement());
				std::unique_ptr<sql::ResultSet> rs(st->executeQuery("SELECT LAST_INSERT_ID()"));
				if (rs->next())
					newId = rs->getInt(1);
			}
			const ExamInfo& e = *note.exam;
			
			std::unique_ptr<sql::PreparedStatement> ps(
				con.prepareStatement(
					"INSERT INTO exam (note_id, subject, room, start_time, duration_min) "
					"VALUES (?, ?, ?, ?, ?)"));
			ps->setInt(1, newId);
			ps->setString(2, e.subject);
			ps->setString(3, e.room);
			ps->setString(4, timeToSql(e.hour, e.minute));
			ps->setInt(5, e.durationMin);
			ps->executeUpdate();
		}
		con.commit();
		con.setAutoCommit(true);
	}
	catch (const sql::SQLException& ex) {
		try {
			con.rollback();
			con.setAutoCommit(true);
		}catch(...){}
		throw std::runtime_error(std::string("Greska pri dodavanju beleske: ") + ex.what());
	}
}

Notes::Notes(Database& db) : db(db) {}



std::vector<Note> Notes::getAllNotes() const {
	std::vector<Note> result;
	try {
		sql::Connection& con = db.connection();
		std::unique_ptr<sql::PreparedStatement> ps(
			con.prepareStatement(std::string(SELECT_NOTES) + "ORDER BY n.note_date, n.id"));
		std::unique_ptr <sql::ResultSet> rs(ps->executeQuery());
		while (rs->next())
			result.push_back(rowToNote(*rs));

	}
	catch (const sql::SQLException& e) {
		throw std::runtime_error(std::string("Greska pri citanju beleski: ") + e.what());
	}
	return result;
}

std::vector<Note> Notes::getAllNotesForDate(const std::tm& date) const {
	std::vector<Note> result;
	try {
		sql::Connection& con = db.connection();
		std::unique_ptr<sql::PreparedStatement> ps(
			con.prepareStatement(std::string(SELECT_NOTES) +
				"WHERE n.note_date=? ORDER BY n.id")
		);
		ps->setString(1, dateToSql(date));
		std::unique_ptr<sql::ResultSet> rs(ps->executeQuery());
		while (rs->next())
			result.push_back(rowToNote(*rs));
	}
	catch (const sql::SQLException& e) {
		throw std::runtime_error(std::string("Greska pri citanju beleski: ") + e.what());
	}
	return result;
}
bool Notes::removeNote(int id) {
	try {
		sql::Connection& con = db.connection();
		std::unique_ptr<sql::PreparedStatement> ps(
			con.prepareStatement("DELETE FROM note WHERE id=?"));
		ps->setInt(1, id);
		int affected = ps->executeUpdate();
		return affected > 0;
	}
	catch (const sql::SQLException& e) {
		throw std::runtime_error(std::string("Greska pri brisanju beleske: ") + e.what());
	}
}
void Notes::editNote(int id, const std::tm& newDate,const std::optional<ExamInfo>& newExam,const std::string& newText) {
		if (isPast(newDate))
			throw std::invalid_argument("Datum je u proslosti");
		sql::Connection& con = db.connection();
		try {
			con.setAutoCommit(false);
			bool exists = false;
			{
				std::unique_ptr<sql::PreparedStatement> ps(
					con.prepareStatement("SELECT id FROM note WHERE id=? FOR UPDATE"));
				ps->setInt(1, id);
				std::unique_ptr<sql::ResultSet> rs(ps->executeQuery());
				exists = rs->next();
			}
			if (!exists) {
				con.rollback();
				con.setAutoCommit(true);
				throw std::out_of_range("Beleska ne postoji");
			}
			{
				std::unique_ptr<sql::PreparedStatement> ps(
					con.prepareStatement("UPDATE note SET note_date=?, text=? WHERE id=?"));
				ps->setString(1, dateToSql(newDate));
				ps->setString(2, newText);
				ps->setInt(3, id);
				ps->executeUpdate();
			}
			if (newExam) {
				const ExamInfo& e = *newExam;
				std::unique_ptr<sql::PreparedStatement> ps(
					con.prepareStatement(
						"INSERT INTO exam (note_id, subject, room, start_time, duration_min) "
						"VALUES (?, ?, ?, ?, ?) "
						"ON DUPLICATE KEY UPDATE subject=VALUES(subject), room = VALUES(room), "
						"start_time = VALUES(start_time), duration_min = VALUES(duration_min)"));
				ps->setInt(1, id);
				ps->setString(2, e.subject);
				ps->setString(3, e.room);
				ps->setString(4, timeToSql(e.hour, e.minute));
				ps->setInt(5, e.durationMin);
				ps->executeUpdate();
			}
			else {
				std::unique_ptr<sql::PreparedStatement> ps(
					con.prepareStatement("DELETE FROM exam WHERE note_id=?"));
				ps->setInt(1, id);
				ps->executeUpdate();
			}
			con.commit();
			con.setAutoCommit(true);
		}
		catch (const sql::SQLException& ex) {
			try {
				con.rollback();
				con.setAutoCommit(true);
			}
			catch (...) {}
			throw std::runtime_error(std::string("Greska pri izmeni beleske: ") + ex.what());
		}
	
}

std::vector<Note> Notes::getUpcomingNotes() const {
	std::vector<Note> result;
	try {
		sql::Connection& con = db.connection();
		std::unique_ptr<sql::PreparedStatement> ps(
			con.prepareStatement(std::string(SELECT_NOTES)
				+ "WHERE n.note_date >= CURDATE() ORDER BY n.note_date, n.id"));
		std::unique_ptr<sql::ResultSet> rs(ps->executeQuery());
		while (rs->next())
			result.push_back(rowToNote(*rs));
	}
	catch (const sql::SQLException& e) {
		throw std::runtime_error(std::string("Greska pri citanju beleski: ") + e.what());
	}
	return result;
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
	std::vector<Note> result;
	try {
		sql::Connection& con = db.connection();
		std::unique_ptr<sql::PreparedStatement> ps(
			con.prepareStatement(std::string(SELECT_NOTES) +
				"WHERE e.note_id IS NOT NULL AND n.note_date >= CURDATE() "
				"ORDER BY n.note_date, e.start_time")
		);
		std::unique_ptr<sql::ResultSet> rs(ps->executeQuery());
		while (rs->next())
			result.push_back(rowToNote(*rs));
	}
	catch (const sql::SQLException& e) {
		throw std::runtime_error(std::string("Greska pri citanju ispita: ") + e.what());
	}
	return result;
}

std::vector<Note> Notes::searchByText(const std::string& query) const {
	std::vector<Note> result;
	std::string escaped;
	for (char c : query) {
		if (c == '\\' || c == '%' || c == '_')
			escaped += '\\';
		escaped += c;
	}
	try {
		sql::Connection& con = db.connection();
		std::unique_ptr<sql::PreparedStatement> ps(
			con.prepareStatement(std::string(SELECT_NOTES) +
				"WHERE n.text LIKE ? ORDER BY n.note_date, n.id")
		);
		ps->setString(1, "%" + escaped + "%");
		std::unique_ptr<sql::ResultSet> rs(ps->executeQuery());
		while (rs->next())
			result.push_back(rowToNote(*rs));
	}
	catch (const sql::SQLException& e) {
		throw std::runtime_error(std::string("Greska pri pretrazi: ") + e.what());
	}
	return result;
}



std::string Notes::dateToSql(const std::tm& date) {
	std::ostringstream ss;
	ss << std::put_time(&date, "%Y-%m-%d");
	return ss.str();
}

std::tm Notes::sqlToDate(const std::string& s) {
	std::tm date{};
	std::istringstream ss(s);
	ss >> std::get_time(&date, "%Y-%m-%d");
	if (ss.fail())
		throw std::runtime_error("Neispravan datum iz baze: " + s);
	return date;
}

Note Notes::rowToNote(sql::ResultSet& rs) {
	Note n;
	n.id = rs.getInt("id");
	n.date = sqlToDate(rs.getString("note_date"));
	n.text = rs.getString("text");

	if (!rs.isNull("subject")) {
		ExamInfo e;
		e.subject = rs.getString("subject");
		if (!rs.isNull("room"))
			e.room = rs.getString("room");
		std::string t = rs.getString("start_time");
		e.hour = std::stoi(t.substr(0,2));
		e.minute = std::stoi(t.substr(3, 2));
		e.durationMin = rs.getInt("duration_min");

		n.exam = e;
	}
	return n;
}

std::string Notes::timeToSql(int hour, int minute) {
	std::ostringstream t;
	t << std::setfill('0') << std::setw(2) << hour << ":" << std::setw(2) << minute << ":00";
	return t.str();
}

std::vector<Note> Notes::getExamsWithin(int days)const {
	std::vector<Note> result;
	try {
		sql::Connection& con = db.connection();
		std::unique_ptr<sql::PreparedStatement> ps(con.prepareStatement(
			std::string(SELECT_NOTES)
			+ "WHERE e.note_id IS NOT NULL "
			"AND n.note_date BETWEEN CURDATE() AND DATE_ADD(CURDATE(), INTERVAL ? DAY) "
			"ORDER BY n.note_date, e.start_time"
		));
		ps->setInt(1, days);
		std::unique_ptr<sql::ResultSet> rs(ps->executeQuery());
		while (rs->next())
			result.push_back(rowToNote(*rs));
	}
	catch (const sql::SQLException& e) {
		throw std::runtime_error(std::string("Greska pri citanju podsetnika: ") + e.what());
	}
	return result;

}