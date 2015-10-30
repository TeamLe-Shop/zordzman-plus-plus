#pragma once

#include "state/State.hpp"

namespace client {

class Client;

namespace state {

class TitleState : public State {
public:
    TitleState();
    void render(client::Client * c);
    void input();
};

} // namespace state

} // namespace client
