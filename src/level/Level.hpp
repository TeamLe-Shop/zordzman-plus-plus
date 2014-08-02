#pragma once
#include <string>

typedef unsigned char byte;

class Level {
public:
    Level(std::string const &levelname);
    void setWidth(int width);
    void setHeight(int height);
    byte tileAt(int x, int y);
    void setTileAt(int x, int y, byte t);

private:
    byte *map;
    int m_width, m_height;
};
