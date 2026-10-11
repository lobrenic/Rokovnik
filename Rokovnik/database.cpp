#include "database.h"
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/exception.h>
#include <stdexcept>
Database::Database(const std::string& host,
    const std::string& user,
    const std::string& password,
    const std::string& schema) :host(host), user(user), password(password), schema(schema) {

}
Database::~Database() {
    close();
}
void Database::open() {
    if (con) return;
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
        std::unique_ptr<sql::Connection> temp(
            driver->connect("tcp://" + host + ":3306", user, password)
        );
        temp->setSchema(schema);
        std::unique_ptr<sql::Statement> st(temp->createStatement());
        st->execute("SET NAMES utf8mb4");

        con = std::move(temp);
    }
    catch (const sql::SQLException& e) {
        throw std::runtime_error(std::string("Greska pri povezivanju sa bazom: ") + e.what());
    }

}
void Database::close() {
    con.reset();
}
bool Database::isOpen() const{
    return con != nullptr;
}
sql::Connection& Database::connection() const {
    if (!con)
        throw std::runtime_error("Veza sa bazom nije otvorena");
    return *con;
}