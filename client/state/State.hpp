#pragma once

namespace client {

class Client;

namespace state {

class State {
public:
    State();
    virtual ~State();

    virtual void render(client::Client * c);
    virtual void input();
};

} // namespace state

} // namespace client
