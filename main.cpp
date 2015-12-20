#include <iostream>
#include <bandit/bandit.h>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Hello, World!" << endl;

    return bandit::run(argc, argv);
    //return 0;
}