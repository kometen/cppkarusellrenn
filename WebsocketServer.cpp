//
// Created by Claus Guttesen on 20/12/2015.
//

#include "WebsocketServer.h"

void WebsocketServer::on_open(connection_hdl hdl) {
    connection_ptr con = m_server.get_con_from_hdl(hdl);

    con->session_id = ++m_next_session_id;
    std::cout << "Opening connection with connection id " << con->session_id << std::endl;
    m_connections.insert(hdl);
}

void WebsocketServer::on_close(connection_hdl hdl) {
    connection_ptr con = m_server.get_con_from_hdl(hdl);

    std::cout << "Closing connection " << con->name << " with session id " << con->session_id << std::endl;
    m_connections.erase(hdl);
}

void WebsocketServer::on_message(connection_hdl hdl, server::message_ptr msg) {
    Database database {};
    connection_ptr con = m_server.get_con_from_hdl(hdl);

    payload = msg->get_payload ();
    try {
        jdata.clear ();
        jdata = nlohmann::json::parse(payload);

        if (jdata["type"] == "validate") {
            message.clear();
            message["type"] = "status";
            message["status"] = "OK";
            msg->set_payload(message.dump());
            m_server.send(hdl, msg);
            std::cout << "Connected." << std::endl;
        }

        if (jdata["type"] == "get races") {
            message.clear();
            message = database.get_races();
            msg->set_payload(message.dump());
            m_server.send(hdl, msg);
        }

        if (jdata["type"] == "add race") {
            int status = database.add_race(jdata);
            message.clear();
            message = database.get_races();
            msg->set_payload(message.dump());
            for (auto it : m_connections) {
                m_server.send(it, msg);
            }
        }

        // Select race with radio button and get participants.
        if (jdata["type"] == "get participants") {
            message.clear();
            message = database.get_participants(jdata);
            msg->set_payload(message.dump());
            for (auto it : m_connections) {
                m_server.send(it, msg);
            }
        }

        if (jdata["type"] == "add participant") {
            int status = database.add_participant(jdata);
            message.clear();
            message = database.get_participants(jdata);
            msg->set_payload(message.dump());
            for (auto it : m_connections) {
                m_server.send(it, msg);
            }
        }

        if (jdata["type"] == "update participant") {
            int status = database.update_participant(jdata);
            message.clear();
            message = database.get_participants(jdata);
            msg->set_payload(message.dump());
            for (auto it : m_connections) {
                m_server.send(it, msg);
            }
        }

        if (jdata["type"] == "delete participant") {
            int status = database.delete_participant(jdata);
            message.clear();
            message = database.get_participants(jdata);
            msg->set_payload(message.dump());
            for (auto it : m_connections) {
                m_server.send(it, msg);
            }
        }

    } catch (const std::exception& e) {
        std::string what_e;
        what_e = e.what();
        jerror["type"] = "error";
        jerror["error_type"] = "Unable to parse json: " + what_e;
        msg->set_payload(jerror.dump());
        m_server.send(hdl, msg);
    }

}

void WebsocketServer::run(uint16_t port) {
    m_server.listen(websocketpp::lib::asio::ip::tcp::v4(), port);
    m_server.start_accept();
    m_server.run();
}
