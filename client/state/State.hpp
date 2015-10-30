#pragma once

namespace client {

class Client;

class State {
public:
    State();
    virtual ~State();

    virtual void render(client::Client * c);
    virtual void input();
};

} // namespace client
