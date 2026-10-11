#pragma once
#include <string>
#include <memory>
namespace sql {
	class Connection;
}

class Database {
private:
	std::string host;
	std::string user;
	std::string password;
	std::string schema;
	std::unique_ptr<sql::Connection> con;

public:
	Database(const std::string& host,
			const std::string& user,
			const std::string& password,
			const std::string& schema);
	~Database();
	Database(const Database&) = delete;
	Database& operator=(const Database&) = delete;

	void open();
	void close();
	bool isOpen() const;

	sql::Connection& connection() const;
};