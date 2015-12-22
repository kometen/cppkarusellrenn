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

public:
    Database();
    nlohmann::json get_races();
};


#endif //CPPKARUSELLRENN_DATABASE_H
