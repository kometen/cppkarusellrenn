// http://banditcpp.org/writingtests.html
//
// Created by Claus Guttesen on 20/12/2015.
//

#include <bandit/bandit.h>
#include "WebsocketServer.hpp"

using namespace bandit;

go_bandit([]() {

    describe("First test", []() {
        it("equal", [&]() {
            AssertThat(1, Equals(1));
        });
    });

});