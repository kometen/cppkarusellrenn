//
// Created by Claus Guttesen on 20/12/2015.
//

#ifndef CPPKARUSELLRENN_SERVER_H
#define CPPKARUSELLRENN_SERVER_H

#include <json.hpp>
#include <unordered_map>
#include <pqxx/pqxx>
#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

struct connection_data {
    unsigned int session_id;
    std::string name;
};

struct custom_config : public websocketpp::config::asio {
    // Pull default settings.
    typedef websocketpp::config::asio core;

    typedef core::concurrency_type concurrency_type;
    typedef core::request_type request_type;
    typedef core::response_type response_type;
    typedef core::message_type message_type;
    typedef core::con_msg_manager_type con_msg_manager_type;
    typedef core::endpoint_msg_manager_type endpoint_msg_manager_type;
    typedef core::alog_type alog_type;
    typedef core::elog_type elog_type;
    typedef core::rng_type rng_type;
    typedef core::transport_type transport_type;
    typedef core::endpoint_base endpoint_base;

    // Set a custom connection_base class.
    typedef connection_data connection_base;
};

typedef websocketpp::server<custom_config> server;
typedef server::connection_ptr connection_ptr;


using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


class WebsocketServer {
private:
    unsigned int m_next_session_id;
    server m_server;

    typedef std::set<connection_hdl, std::owner_less<connection_hdl>> con_list;
    con_list m_connections;

    std::string payload;
    nlohmann::json jdata;
    nlohmann::json message;
    nlohmann::json jerror;

public:
    WebsocketServer() : m_next_session_id(1) {
        m_server.init_asio();

        m_server.set_open_handler(bind(&WebsocketServer::on_open, this, ::_1));
        m_server.set_close_handler(bind(&WebsocketServer::on_close, this, ::_1));
        m_server.set_message_handler(bind(&WebsocketServer::on_message, this, ::_1, ::_2));
    }

    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void on_message(connection_hdl hdl, server::message_ptr msg);

    void run(uint16_t port);
};


#endif //CPPKARUSELLRENN_SERVER_H
