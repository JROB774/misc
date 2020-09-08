/*******************************************************************************

The process for packing is as follows:

- Sort the images from largest to smallest before attempting to pack them, this
  will make for a better/tighter pack.
- Create a texture with power of 2 dimensions, just large enough to fit the
  largest image passed into the program.
- Pack the largest image at pos (0,0), then the process for packing all other
  images begins below:
- Place the image at pos (0,0) and continue moving it to the right until it does
  not intersect with any other already packed images.
- If this is not the case, then the image is moved downwards (by the height of
  the smallest image currently packed) and the process repeats once more. Each
  time it will choose the next smallest image to move down by.
- If the whole image is scanned and no valid position is found, then the image
  is expanded (by the next power of 2) and this whole process repeats until a
  valid position is successfully obtained.
- Once this is done for all images, the data for how to effectively pack the
  images is complete and can be used to save a completed, packed texture.

*******************************************************************************/

#include <cstdlib>
#include <cstdint>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

struct Rect
{
	int x, y, w, h;

	inline bool Intersects (const Rect& _rect) const
	{
		return ((x < (_rect.x + _rect.w)) && ((x + w) > _rect.x) &&
				(y < (_rect.y + _rect.h)) && ((y + h) > _rect.y));
	}

	inline int GetSize () const
	{
		return w * h;
	}
};

struct Image
{
	static const int BPP = 4; // bytes-per-pixel, not bits

	int width, height;
	int pitch; // size of each row in bytes (also referred to as stride)
	Rect content; // the bounds of useful data (not transparent)
	std::uint8_t* pixels;
	std::string filename;

	Image (): width(0), height(0), pitch(0), content({}), pixels(nullptr) {}
	Image (const char* _filename) { Load(_filename); }

	// requires copy construction + assignment to be in a std::vector
	Image (const Image& _img) { Load(_img.filename); }
	Image& operator= (const Image& _rhs) { Load(_rhs.filename); return *this; }

	// requires move construction + assignment to be used by std::sort
	Image (Image&& _img) { *this = _img; }
	Image& operator= (Image&& _rhs)
	{
		width = _rhs.width, height = _rhs.height;
		pitch = _rhs.pitch;
		content = _rhs.content;
		pixels = _rhs.pixels;
		filename = _rhs.filename;

		_rhs.pixels = nullptr;
		_rhs.Destroy();

		return *this;
	}

	~Image () { Destroy(); }

	bool Load (std::string _filename)
	{
		// free-up old memory, if neccessary (avoids need for manual clean-up)
		if (pixels) { Destroy(); }

		// load the image file as raw pixel data
		pixels = stbi_load(_filename.c_str(), &width, &height, nullptr, BPP);
		if (!pixels) { return false; }
		pitch = width * BPP;
		filename = _filename;

		// set to max values so that content boundaries can be conditioned
		content = { width, height, 0, 0 };

		// go through each pixel of the image to detemrine the content bounds
		for (int iy = 0; iy < height; ++iy) {
			for (int ix = 0; ix < pitch; ix += BPP) {
				// if the current pixel is not transparent...
				if (pixels[iy * pitch + (ix + (BPP - 1))] != 0x00) {
					// ...attempt to see if it is valid for a content boundary
					if ((ix / BPP) < content.x) { content.x = ix / BPP; }
					if ((ix / BPP) > content.w) { content.w = ix / BPP; }
					if (iy < content.y) { content.y = iy; }
					if (iy > content.h) { content.h = iy; }
				}
			}
		}

		// converts the use of content w and h from acting as a second position
		// to being the width and height instead (this made the previous code
		// easier to create but will not work for future use so must be changed)
		content.w -= (content.x - 1);
		if (content.w < 0) { content.w = 0; }
		content.h -= (content.y - 1);
		if (content.h < 0) { content.h = 0; }

		// successful loading of an image file into memory
		return true;
	}

	void Destroy ()
	{
		if (pixels) { stbi_image_free(pixels); pixels = nullptr; }
		width = 0, height = 0;
		pitch = 0;
		content = { 0, 0, 0, 0 };
		filename.clear();
	}
};

int main (int _argc, char* _argv[])
{
	// cannot operate if no image data is passed to the program
	if (_argc <= 1) {
		std::cout << "No image files given!" << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}

	// sets up the dynamic array for storing the input image data 
	std::vector<Image> images;
	images.reserve(_argc - 1);

	// loads in all the images passed to the program (cropping them)
	std::cout << "Beginning texture loading..." << std::endl;
	for (int i = 1; i < _argc; ++i) {
		std::cout << "Loading '" << _argv[i] << "... ";
		images.push_back(Image());
		if (!images.back().Load(_argv[i])) {
			// handles failure to load an image (terminates)
			std::cout << "failed!" << std::endl;
			std::cin.get();
			return EXIT_FAILURE;
		}
		std::cout << "successful!" << std::endl;
	}
	std::cout << "Texture loading complete!" << std::endl;

	// saves each image individually
	std::cout << "Beginning texture saving..." << std::endl;
	for (auto& image: images) {
		std::cout << "Saving '" << image.filename << "'... ";
		// create a buffer to store useful data
		std::vector<std::uint8_t> buffer;
		buffer.resize((image.content.w * image.content.h) * Image::BPP);
		// fill the buffer with the useful data from the image
		int texturepitch = image.content.w * Image::BPP;
		// writes the data of each image into the buffer (at the correct pos)
		Rect bounds = { 0, 0, image.content.w, image.content.h };
		int boundspitch = (bounds.x * Image::BPP);
		int boundssize = (bounds.x + bounds.w) * Image::BPP;
		int imgy = image.content.y;
		for (int iy = bounds.y; iy < (bounds.y + bounds.h); ++iy) {
			int imgx = image.content.x * Image::BPP;
			for (int ix = boundspitch; ix < boundssize; ix += Image::BPP) {
				buffer.at(iy * texturepitch + (ix+0)) = image.pixels[imgy * image.pitch + imgx+0];
				buffer.at(iy * texturepitch + (ix+1)) = image.pixels[imgy * image.pitch + imgx+1];
				buffer.at(iy * texturepitch + (ix+2)) = image.pixels[imgy * image.pitch + imgx+2];
				buffer.at(iy * texturepitch + (ix+3)) = image.pixels[imgy * image.pitch + imgx+3];
				imgx += Image::BPP;
			}
			imgy++;
		}
		// saves out the PNG image itself
		stbi_write_png(
			std::string("output\\" + image.filename).c_str(),
			image.content.w, image.content.h,
			Image::BPP,
			&buffer[0],
			texturepitch
		);
		std::cout << "successful!" << std::endl;
	}
	std::cout << "Texture saving cmplete!" << std::endl;

	// terminate the program successfully
	std::cout << "\nAUTOPACKER HAS SUCCESSFULLY FINISHED!" << std::endl;
	std::cin.get();
	return EXIT_SUCCESS;
}