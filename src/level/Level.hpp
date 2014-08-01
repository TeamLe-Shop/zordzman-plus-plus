#pragma once

// TODO: Actually add shit

typedef unsigned char byte;

class Level {
public:
    Level(int width, int height);
    void setWidth(int w);
    void setHeight(int h);

private:
    byte **map;
    int width, height;
};
