#include "sql.h"

SqlException::SqlException(std::string error)
    :m_error(error) {}

const char* SqlException::what() const noexcept {
    return m_error.c_str();
}

bool Sql::dbChecked = false;

Sql & Sql::connect (std::string dbName) {
    #ifdef DEBUG
        dbName = "starsurvival.db";
    #endif
    bool needDbSetup = false;
    if (!dbChecked) {
        std::ifstream dbFile;
        dbFile.open(dbName);
        if (dbFile.is_open()) {
            needDbSetup = false;
        } else {
            needDbSetup = true;
            Log::info(std::string("DataBase not found, start installing DataBase"));
        }
        dbFile.close();
        dbChecked = true;
    }

    static Sql s(dbName);
    if (needDbSetup) {
        s.setup();
    }
    return s;
}


Sql::Sql(std::string dbName):
    password("kr_kimmo-dev2022"){

    #ifdef DEBUG
        password = "";
        Log::info("debug");
    #endif
    #ifdef TEST
        password = "";
        Log::info("test");
    #endif
    dbOpen(dbName);
}

Sql::~Sql() {
    dbClose();
}

void Sql::dbOpen(const std::string &  dbName) {
    int rc = sqlite3_open(dbName.c_str(), &db);
    if( rc ) {
        std::string message = "Sql open error: ";
        message += sqlite3_errmsg(db);
        message += ". Lib v: " + std::string(sqlite3_libversion());
        errorHandler(message, "Sql open error");
    }
    request("PRAGMA key='" + password + "'");
}

void Sql::dbClose() {
    sqlite3_close(db);
}

int Sql::request(std::string request,
    const sqlParams & replacement) const {

    getData(request, replacement);
    return sqlite3_last_insert_rowid(db);
}

sqlArray Sql::getData(std::string request,
    const sqlParams & replacement) const {

    bool isError = false;
    std::string errorMessage;
    std::string lastRequest;
    sqlArray v {};
    sqlite3_stmt* stmt;

    int rc1 = sqlite3_prepare_v2(
        db,
        request.c_str(),
        request.length(),
        &stmt,
        nullptr);
    if (rc1 != SQLITE_OK) {
        isError = true;
        errorMessage = sqlite3_errmsg(db);
    }
    for (size_t i = 0; i < replacement.size(); i++) {
        int rc2 = sqlite3_bind_text(
            stmt,
            i + 1,
            replacement.at(i).c_str(),
            replacement.at(i).length(),
            SQLITE_STATIC);
        if (rc2 != SQLITE_OK) {
            isError = true;
            errorMessage = sqlite3_errmsg(db);
        }
    }
    int code = sqlite3_step (stmt);
    while (code == SQLITE_ROW)
    {
        std::map <std::string, std::string> m;
        int columnCount = sqlite3_data_count(stmt);
        for (int i = 0; i < columnCount; ++i) {
            m[std::string(reinterpret_cast<const char*>(sqlite3_column_name (stmt, i)))] =
                std::string(reinterpret_cast<const char*>(sqlite3_column_text (stmt, i)));
        }
        v.push_back(m);
        code = sqlite3_step (stmt);
    }
    if (code == SQLITE_OK || code == SQLITE_ROW || code == SQLITE_DONE) {
        lastRequest = sqlite3_expanded_sql(stmt);
    } else {
        isError = true;
        lastRequest = request;
        for (size_t i = 0; i < replacement.size(); i++) {
            lastRequest += " / " + replacement.at(i);
        }
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);

    if (isError) {
        errorHandler("Sql send error: " + errorMessage
            + "; request: " + lastRequest, "Sql request error");
    }
    return v;
}

int Sql::countData(std::string request,
    const sqlParams & replacement) const {
    sqlArray v = getData(request, replacement);
    int count = v.size();
    return count;
}

void Sql::errorHandler(const std::string & errorMessage, const std::string & exeptionMessage) const {
    Log::error(errorMessage);
    throw SqlException(exeptionMessage);
}

void Sql::setup() const {
    request("PRAGMA rekey='" + password + "'");

    //Профиль
    request("CREATE TABLE IF NOT EXISTS Profile (id INTEGER PRIMARY KEY AUTOINCREMENT);");
    try {
        request("ALTER TABLE Profile ADD COLUMN gameScreen INTEGER");
    } catch (std::exception &exception) {
        Log::error("SetupSql: " + std::string(exception.what()));
    }
    try {
        request("ALTER TABLE Profile ADD COLUMN hasMap INTEGER DEFAULT '0'");
    } catch (std::exception &exception) {
        Log::error("SetupSql: " + std::string(exception.what()));
    }
    request("INSERT INTO Profile (gameScreen, hasMap) VALUES (0, 0);");

    //Общие настройки
    request("CREATE TABLE IF NOT EXISTS Settings (id INTEGER PRIMARY KEY AUTOINCREMENT);");
    try {
        request("ALTER TABLE Settings ADD COLUMN playerId INTEGER");
    } catch (std::exception &exception) {
        Log::error("SetupSql: " + std::string(exception.what()));
    }
    //try {
    //    request("ALTER TABLE Settings ADD COLUMN devMoveMap INTEGER");
    //} catch (std::exception &exception) {
    //    Log::error("SetupSql: " + std::string(exception.what()));
    //}
    request("INSERT INTO Settings (playerId) VALUES (1);");
    //#ifdef DEBUG
        //request("UPDATE Settings SET devMoveMap = 1 WHERE playerId = 1;");
    //#endif

    //Карта земель
    request("CREATE TABLE IF NOT EXISTS MapGround ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "playerId INTEGER NOT NULL,"
        "x INTEGER NOT NULL,"
        "y INTEGER NOT NULL,"
        "type INTEGER NOT NULL,"
        "isVisited INTEGER NOT NULL DEFAULT '0'"
        ");");

    //Таблица персонажа
    request("CREATE TABLE IF NOT EXISTS Hero ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "playerId INTEGER NOT NULL,"
        "x INTEGER NOT NULL,"
        "y INTEGER NOT NULL,"
        "lookRight INTEGER NOT NULL DEFAULT '1'"
        ");");
}
