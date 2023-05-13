#ifndef SQL_H
#define SQL_H

#include <string>
#include <vector>
#include <map>
#include "log.h"
#include <exception>

#include "sqlite3.h"
#include "../lib/sqleet-v0.31.1/sqleet.h"

using sqlArray = std::vector<std::map <std::string, std::string>>;
using sqlParams = std::vector <std::string>;

class Sql
{
public:
    static Sql & connect (std::string dbName = "starsurvival.dat");

    ~Sql();

    //NOTE request like "INSERT INTO Projects (name, totalRub) VALUES ( ?, ? );"
    //parameters send in replacement (2nd argument).
    //'?' may be only for values (not for name of Tables or Columns)
    int request(std::string request,
        const sqlParams & replacement
        = sqlParams()) const;

    sqlArray getData(std::string request,
        const sqlParams & replacement
        = sqlParams()) const;

    int countData(std::string request,
        const sqlParams & replacement
        = sqlParams()) const;

private:
    sqlite3 *db;
    Sql(std::string dbName);
    std::string password;

    static bool dbChecked;
    void setup() const;

    void dbOpen(const std::string & dbName);
    void dbClose();

    void errorHandler(const std::string & errorMessage, const std::string & exeptionMessage) const;

};

class SqlException: public std::exception
{
public:
    SqlException(std::string error);

    const char* what() const noexcept;

private:
    std::string m_error;
};

#endif // SQL_H
