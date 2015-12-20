#include <iostream>
#include <bandit/bandit.h>

#include "WebsocketServer.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Blåbærsyltetøj!" << endl;

    WebsocketServer server;
    server.run(9003);
    return bandit::run(argc, argv);
    //return 0;
}