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

private:
    unsigned int tick;
};

} // namespace state

} // namespace client
