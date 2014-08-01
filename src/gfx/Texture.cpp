#include "Texture.hpp"

#include <SDL_image.h>
#include <stdexcept>
#include <iostream>
#include <cassert>


void Texture::loadFromFile(std::string const &filename) {
	load_image_to_tex(filename.c_str(), &m_handle);
}

	
bool Texture::load_image_to_tex ( char const * const filename, GLuint * tex ) {
     
	SDL_Surface *surface;   // This surface will tell us the details of the image
	GLenum texture_format;
	GLint  nOfColors;

	surface = IMG_Load(filename);

	if(surface) {
    	// get the number of channels in the SDL surface
    	nOfColors = surface->format->BytesPerPixel;
	
		if(nOfColors == 4) {
	
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
	
			else
				texture_format = GL_BGRA_EXT;
		}
	
		else if(nOfColors == 3) {
	
				if (surface->format->Rmask == 0x000000ff)
	
						texture_format = GL_RGB;
	
				else
	
						texture_format = GL_BGR_EXT;
		}
	
		else return false;
	
		glGenTextures(1, tex);
	
		glBindTexture(GL_TEXTURE_2D, *tex);
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
		glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
			texture_format, GL_UNSIGNED_BYTE, surface->pixels);

	}
	else return false;
	
	if(surface) {
		SDL_FreeSurface(surface);
	}
	std::cout << "lal\n";
	return true;
}
	


std::string Texture::getPath() const { return path; }

int Texture::getWidth() const { return m_width; }

int Texture::getHeight() const { return m_height; }

int Texture::getTileSize() const { return tile_size; }

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, m_handle); }

Texture::~Texture() { glDeleteTextures(1, &m_handle); }
