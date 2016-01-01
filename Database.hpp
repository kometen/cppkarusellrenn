//
// Created by Claus Guttesen on 22/12/2015.
//

#ifndef CPPKARUSELLRENN_DATABASE_H
#define CPPKARUSELLRENN_DATABASE_H

#include <stack>
#include <json.hpp>
#include <pqxx/pqxx>

class Database {
private:
    nlohmann::json races;
    nlohmann::json participants;
    std::string connectionString = "dbname=races user=claus hostaddr=127.0.0.1 port=5432";
    std::stack<pqxx::connection*> dbpool;

public:
    Database(const unsigned int);
    nlohmann::json get_races();
    int add_race(const nlohmann::json);
    nlohmann::json get_participants(const nlohmann::json);
    int add_participant(const nlohmann::json);
    int update_participant(const nlohmann::json);
    int delete_participant(const nlohmann::json);
};


#endif //CPPKARUSELLRENN_DATABASE_H
