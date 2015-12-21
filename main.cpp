#include <iostream>
#include <bandit/bandit.h>

#include "WebsocketServer.h"

using namespace std;

int main(int argc, char* argv[]) {
    int test_status;
    cout << "Blåbærsyltetøj!" << endl;

    WebsocketServer server;
    test_status = bandit::run(argc, argv);
    if (test_status == 0) {
        server.run (9003);
    }
    //return 0;
}