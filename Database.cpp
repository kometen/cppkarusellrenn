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
                    {"racestart_at", c[3].as<std::string>()},
                    {"interval", c[4].as<std::string>()}
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
    std::string racestart_at = json["racestart_at"];
    std::string interval = json["interval"];
    try {
        pqxx::connection C("dbname=races user=claus hostaddr=127.0.0.1 port=5432");
        if (!C.is_open()) {
            std::cout << "Unable to connect to database" << std::endl;
        } else {
            pqxx::work W(C);

            std::string query = "insert into races (location, racename, racestart_at, interval) values ";
            query += "('" + location + "', '" + racename + "', '" + racestart_at + "', " + interval + ")";

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

nlohmann::json Database::get_participants(nlohmann::json json) {
    int status = EXIT_FAILURE;
    participants["type"] = "participants";
    participants["participants"] = { };
    std::string location = json["location"];
    std::string racename = json["racename"];
    std::string racestart_at = json["racestart_at"];

    try {
        pqxx::connection C("dbname=races user=claus hostaddr=127.0.0.1 port=5432");
        if (!C.is_open()) {
            std::cout << "Unable to connect to database" << std::endl;
        } else {
            std::string query = "select * from participants";
            query += " where location = '" + location + "'";
            query += " and racename = '" + racename + "'";
            query += " and racestart_at = '" + racestart_at + "'";
            query += " order by racestart_at desc";

            pqxx::nontransaction N(C);
            pqxx::result R(N.exec(query));

            for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
                participants["participants"] += { \
                    {"id", c[0].as<std::string>()},
                    {"location", c[1].as<std::string>()},
                    {"racename", c[2].as<std::string>()},
                    {"racestart_at", c[3].as<std::string>()},
                    {"name", c[4].as<std::string>()},
                    {"gender", c[5].as<std::string>()},
                    {"born", c[6].as<std::string>()},
                    {"club", c[7].as<std::string>()},
                    {"start_at", static_cast<bool>(c[8].size()) ? c[8].as<std::string>() : ""},
                    {"ended_at", static_cast<bool>(c[9].size()) ? c[9].as<std::string>() : ""},
                    {"racetime", static_cast<bool>(c[10].size()) ? c[10].as<std::string>() : ""}
                };
            }
            status = EXIT_SUCCESS;
        }
    } catch (const std::exception& e) {
        participants["participants"] = {{"status", status}, {"reason", "unable to read table"}};
        std::cerr << e.what() << std::endl;
    }

    return participants;
}

int Database::add_participant(nlohmann::json json) {
    int status = EXIT_FAILURE;
    std::string location = json["location"];
    std::string racename = json["racename"];
    std::string racestart_at = json["racestart_at"];
    std::string name = json["name"];
    std::string gender = json["gender"];
    std::string born = json["born"];
    std::string club = json["club"];

    try {
        pqxx::connection C("dbname=races user=claus hostaddr=127.0.0.1 port=5432");
        if (!C.is_open()) {
            std::cout << "Unable to connect to database" << std::endl;
        } else {
            pqxx::work W(C);

            std::string query = "insert into participants (location, racename, racestart_at, name, gender, born, club) values ";
            query += "('" + location + "', '" + racename + "', '" + racestart_at + "', '";
            query += name + "', '" + gender + "', '" + born + "', '" + club + "')";

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
