#include "Draw.hpp"

// TODO: Add more shit

void Drawer::beginDraw() { glBegin(GL_QUADS); }

void Drawer::draw(Texture spritesheet, int tx, int ty, float x, float y, float w, float h) {
	int ts = spritesheet.getTileSize();
	int ss_w = spritesheet.getWidth();
	int ss_h = spritesheet.getHeight();
	/* If it's out of the bounds, print a warning message and return. */
	if (tx < 0 || ty < 0 || tx > ss_w / ts || ty > ss_h / ts)
	{
		printf("[WARNING] Tried to access tile (%d, %d) in spritesheet %s, out of bounds", tx, ty,
			spritesheet.getPath().c_str());
		return;
	}
	
	/* Correct the tx and ty values. */
	
	float tx_;
	float ty_;
	float ts_ = 0.25f / (float) ts;
	
	glColor3f(1, 1, 1);
	
	/* Avoid dividing by zero */
	if ((float)(tx * ts) < (ts_))
		tx_ = 0;
	else
		tx_ = 0.25f / (float)(tx * ts);
		
	if ((float)(ty * ts) < (ts_))
		ty_ = 0;
	else
		ty_ = 0.25f / (float)(ty * ts);
	
	/* Bind the spritesheet texture... */
	spritesheet.bind();
	
	/* The meat of the draw() method. */
	printf("%f %f %f\n", tx_, ty_, ts_);
	glTexCoord2f(tx_, ty_);
	glVertex2f(x, y);
	glTexCoord2f(tx_ + ts_, ty_);
	glVertex2f(x + w, y);
	glTexCoord2f(tx_ + ts_, ty_ + ts_);
	glVertex2f(x + w, y + h);
	glTexCoord2f(tx_, ty_ + ts_);
	glVertex2f(x, y + h);
	
	/* ... unbind the texture. */
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Drawer::endDraw() { glEnd(); }