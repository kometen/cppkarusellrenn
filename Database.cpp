//
// Created by Claus Guttesen on 22/12/2015.
//

#include "Database.h"

Database::Database() { }

nlohmann::json Database::get_races() {
    races.clear();
    races["type"] = "races";
    try {
        pqxx::connection C("dbname=races user=claus hostaddr=127.0.0.1 port=5432");
        if (!C.is_open()) {
            std::cout << "Unable to connect to database" << std::endl;
        } else {
            std::string query = "select * from races order by racestart_at desc";
            pqxx::nontransaction N(C);
            pqxx::result R(N.exec(query));

            for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
                races["races"] += { \
                    {"id", c[0].as<std::string>()},
                    {"location", c[1].as<std::string>()},
                    {"racename", c[2].as<std::string>()},
                    {"racestart_at", c[3].as<std::string>()}
                };
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return races;
}

int Database::add_race(nlohmann::json json) {
    int status = EXIT_FAILURE;
    std::string location = json["location"];
    std::string racename = json["racename"];
    std::string date = json["date"];
    try {
        pqxx::connection C("dbname=races user=claus hostaddr=127.0.0.1 port=5432");
        if (!C.is_open()) {
            std::cout << "Unable to connect to database" << std::endl;
        } else {
            pqxx::work W(C);

            std::string query = "insert into races (location, racename, racestart_at) values ";
            query += "('" + location + "', '" + racename + "', '" + date + "')";

            W.exec(query);
            W.commit();
            C.disconnect();
            status = EXIT_SUCCESS;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return status;
}
