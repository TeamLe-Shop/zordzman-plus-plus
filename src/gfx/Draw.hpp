/* Draw.hpp should take care of drawing with spritesheets.
 * I'll be reusing some of the original Zordzman source code
 * for this. - JavaCakess/vividMario52
 */

#ifndef DRAW_HPP
#define DRAW_HPP

#include <stdio.h>
#include <SDL_opengl.h>
#include "Texture.hpp" // The original Zordzman used a custom SpriteSheet class.
					   // We'll probably enhance Texture.hpp and use that as 
				       // a spritesheet.

// TODO: Add more shit

class Drawer {
	
public:
	static void beginDraw();
	static void draw(Texture spritesheet, int tx, int ty, float x, float y, float w, float h);
	static void endDraw();
};


#endif /* DRAW_HPP */
