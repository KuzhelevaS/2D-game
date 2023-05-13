#include "sql.h"
#include "catch.hpp"

TEST_CASE("SQL") {
    remove("testSQL.db");
    Sql::connect("testSQL.db");
    int count = Sql::connect().countData("SELECT * FROM Profile;");
    REQUIRE(count == 1);

    sqlParams v1;
    v1.push_back("22");
    int lastId = Sql::connect().request("INSERT INTO Profile (gameScreen) VALUES (? );", v1);
    REQUIRE(lastId == 2);
    sqlArray v1_2;
    v1_2 = Sql::connect().getData("SELECT * FROM Profile;");
    REQUIRE(v1_2[1]["id"] == "2");
    REQUIRE(v1_2[1]["gameScreen"] == "22");

    sqlParams v2;
    v2.push_back("2");
    Sql::connect().request("UPDATE Profile SET gameScreen='55' WHERE id=?", v2);
    v1_2 = Sql::connect().getData("SELECT * FROM Profile;");
    REQUIRE(v1_2[1]["id"] == "2");
    REQUIRE(v1_2[1]["gameScreen"] == "55");

    count = Sql::connect().countData("SELECT * FROM Profile WHERE gameScreen = '55';");
    REQUIRE(count == 1);

    Sql::connect().request("INSERT INTO Profile (gameScreen) VALUES (?);", v1);
    count = Sql::connect().countData("SELECT * FROM Profile;");
    REQUIRE(count == 3);

    int exceptionRealCount = 0;
    int exceptionPlanCount = 0;
    try {
        exceptionPlanCount++;
        count = Sql::connect().countData("SELECT * FROM Profile WHERE name = test;");
    } catch (std::exception &exception) {
        exceptionRealCount++;
    }
    REQUIRE(exceptionRealCount == exceptionPlanCount);
}

