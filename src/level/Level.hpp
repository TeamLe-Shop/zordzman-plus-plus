#pragma once
#include <string>
#include <vector>

typedef unsigned char byte;

class Level {
public:
    Level(std::string const &levelname);
    void setWidth(int width);
    void setHeight(int height);
    int getWidth() const;
    int getHeight() const;
    byte tileAt(int x, int y);
    void setTileAt(int x, int y, byte t);

private:
    std::vector<byte> m_tiles;
    int m_width, m_height;
};
