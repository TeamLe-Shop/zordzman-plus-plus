#pragma once

#include "state/State.hpp"

namespace client {

class Client;

class TitleState : public State {
public:
    TitleState();
    void render(client::Client * c);
    void input(SDL_Event event);

private:
    unsigned int tick;

};

} // namespace client
