//
// Created by Claus Guttesen on 22/12/2015.
//

#include "Database.hpp"

// pqxx::result::field
std::string column_content(pqxx::result::field a) {
    if (static_cast<bool>(a.size())) {
        return a.as<std::string>();
    } else {
        return "";
    }
}

Database::Database(const unsigned int connections) {
    for (int i = 0; i < connections; ++i) {
        try {
            auto dbconn = std::make_shared<pqxx::connection>(connectionString);
            dbpool.emplace(dbconn);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }

    }
    std::cout << "stack size: " << dbpool.size() << std::endl;
}

nlohmann::json Database::get_races() {
    races.clear();
    races["type"] = "races";

    auto D = dbpool.top();
    dbpool.pop();

    std::string query = "select * from races order by racestart_at desc";
    pqxx::nontransaction N(*D);
    pqxx::result R(N.exec(query));

    for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
        races["races"] += { \
                    {"id", column_content(c[0]) },
                    {"location", column_content(c[1]) },
                    {"racename", column_content(c[2]) },
                    {"racestart_at", column_content(c[3]) },
                    {"interval", column_content(c[4]) }
        };
    }

    dbpool.push(D);

    return races;
}

int Database::add_race(const nlohmann::json json) {
    int status = EXIT_FAILURE;
    std::string location = json["location"];
    std::string racename = json["racename"];
    std::string racestart_at = json["racestart_at"];
    std::string interval = json["interval"];

    auto D = dbpool.top();
    dbpool.pop();

    pqxx::work W(*D);

    std::string query = "insert into races (location, racename, racestart_at, interval) values ";
    query += "('" + location + "', '" + racename + "', '" + racestart_at + "', " + interval + ")";

    W.exec(query);
    W.commit();
    status = EXIT_SUCCESS;

    dbpool.push(D);

    return status;
}

nlohmann::json Database::get_participants(const nlohmann::json json) {
    int status = EXIT_FAILURE;
    participants["type"] = "participants";
    participants["participants"] = { };
    std::string location = json["location"];
    std::string racename = json["racename"];
    std::string racestart_at = json["racestart_at"];

    std::string query = "select * from participants";
    query += " where location = '" + location + "'";
    query += " and racename = '" + racename + "'";
    query += " and racestart_at = '" + racestart_at + "'";
    query += " order by born, gender";

    auto D = dbpool.top();
    pqxx::nontransaction N(*D);
    pqxx::result R(N.exec(query));

    for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
        participants["participants"] += { \
                    {"id", column_content(c[0]) },
                    {"location", column_content(c[1]) },
                    {"racename", column_content(c[2]) },
                    {"racestart_at", column_content(c[3]) },
                    {"name", column_content(c[4]) },
                    {"gender", column_content(c[5]) },
                    {"born", column_content(c[6]) },
                    {"club", column_content(c[7]) },
                    {"start_at", column_content(c[8]) },
                    {"ended_at", column_content(c[9]) },
                    {"racetime", column_content(c[10]) },
                    {"raceid", column_content(c[11]) }
        };
    }
    status = EXIT_SUCCESS;

    dbpool.push(D);

    return participants;
}

int Database::add_participant(const nlohmann::json json) {
    int status = EXIT_FAILURE;
    std::string location = json["location"];
    std::string racename = json["racename"];
    std::string racestart_at = json["racestart_at"];
    std::string name = json["name"];
    std::string gender = json["gender"];
    std::string born = json["born"];
    std::string club = json["club"];

    auto D = dbpool.top();
    dbpool.pop();
    pqxx::work W(*D);

    std::string query = "insert into participants (location, racename, racestart_at, name, gender, born, club) values ";
    query += "('" + location + "', '" + racename + "', '" + racestart_at + "', '";
    query += name + "', '" + gender + "', '" + born + "', '" + club + "')";

    W.exec(query);
    W.commit();
    status = EXIT_SUCCESS;

    dbpool.push(D);

    return status;
}

int Database::update_participant(const nlohmann::json json) {
    int status = EXIT_FAILURE;
    std::string id = json["id"];
    std::string name = json["name"];
    std::string gender = json["gender"];
    std::string born = json["born"];
    std::string club = json["club"];

    auto D = dbpool.top();
    dbpool.pop();
    pqxx::work W(*D);

    std::string query = "update participants set ";
    query += "name = '" + name + "', gender = '" + gender + "', born = '" + born + "', club = '" + club + "'";
    query += " where id = " + id;

    W.exec(query);
    W.commit();
    status = EXIT_SUCCESS;

    dbpool.push(D);

    return status;
}

int Database::delete_participant(const nlohmann::json json) {
    int status = EXIT_FAILURE;
    std::string location = json["location"];
    std::string racename = json["racename"];
    std::string racestart_at = json["racestart_at"];
    std::string name = json["name"];
    std::string gender = json["gender"];
    std::string born = json["born"];
    std::string club = json["club"];

    auto D = dbpool.top();
    dbpool.pop();
    pqxx::work W(*D);

    std::string query = "delete from participants where ";
    query += "location = '" + location + "' and racename = '" + racename + "' and racestart_at =  '" + racestart_at + "'";
    query += "and name = '" + name + "' and gender = '" + gender + "' and born = '" + born + "' and club = '" + club + "'";

    W.exec(query);
    W.commit();
    status = EXIT_SUCCESS;

    dbpool.push(D);

    return status;
}
