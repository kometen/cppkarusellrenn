//
// Created by Claus Guttesen on 22/12/2015.
//

#ifndef CPPKARUSELLRENN_DATABASE_H
#define CPPKARUSELLRENN_DATABASE_H

#include <json.hpp>
#include <pqxx/pqxx>

class Database {
private:
    nlohmann::json races;
    nlohmann::json participants;

public:
    Database();
    nlohmann::json get_races();
    int add_race(nlohmann::json);
    nlohmann::json get_participants(nlohmann::json);
    int add_participant(nlohmann::json);
};


#endif //CPPKARUSELLRENN_DATABASE_H
