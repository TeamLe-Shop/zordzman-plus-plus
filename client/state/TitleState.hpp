#pragma once

#include "state/State.hpp"

namespace client {

class Client;

class TitleState : public State {
public:
    TitleState();
    void render(client::Client * c);
    void input();

private:
    unsigned int tick;
};

} // namespace client
