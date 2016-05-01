#pragma once

#include <SDL.h>

namespace client {

class Client;

class State {
public:
    State();
    virtual ~State();

    virtual void render(client::Client * c);
    virtual void input(SDL_Event event);
};

} // namespace client
