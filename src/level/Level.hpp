#pragma once

// TODO: Actually add shit

typedef unsigned char byte;

class Level {
public:
    Level(int width, int height);
    void setWidth(int width);
    void setHeight(int height);

private:
    byte **map;
    int m_width, m_height;
};
