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
#include <cctype>
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

bool crop = false;
bool size = false;

int customwidth = 0;
int customheight = 0;

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

inline bool operator== (const Rect& _lhs, const Rect& _rhs) { return (_lhs.GetSize() == _rhs.GetSize()); }
inline bool operator!= (const Rect& _lhs, const Rect& _rhs) { return !(_lhs == _rhs); }
inline bool operator<  (const Rect& _lhs, const Rect& _rhs) { return (_lhs.GetSize() < _rhs.GetSize()); }
inline bool operator>  (const Rect& _lhs, const Rect& _rhs) { return (_rhs < _lhs); }
inline bool operator<= (const Rect& _lhs, const Rect& _rhs) { return !(_lhs > _rhs); }
inline bool operator>= (const Rect& _lhs, const Rect& _rhs) { return !(_lhs < _rhs); }

std::ostream& operator<< (std::ostream& _os, const Rect& _rect)
{
	_os << _rect.x << " " << _rect.y << " " << _rect.w << " " << _rect.h;
	return _os;
}

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

		if (crop) {
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
		}
		else { content = { 0, 0, width, height }; }

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

inline std::vector<Rect> PackTexture (std::vector<Image>& _images, int& _texturewidth, int& _textureheight, int& _power)
{
	// used to store the positions (and sizes) of each image in the packed texture
	std::vector<Rect> texturedata;
	texturedata.reserve(_images.size());

	if (!size) {
		// determines the smallest power of two that fits the largest image
		int maximgside = std::max(_images.at(0).content.w, _images.at(0).content.h);
		for (/* NULL */; _texturewidth < maximgside; ++_power) { _texturewidth = std::pow(2, _power); }
		_textureheight = _texturewidth; // keeps the image square
	}
	else {
		// sets the custom width and height the user asked for
		_texturewidth = customwidth;
		_textureheight = customheight;
	}

	// the largest image will always be stored at position 0,0 in the texture
	texturedata.push_back(Rect { 0, 0, _images.at(0).content.w, _images.at(0).content.h });

	// goes through all remaining images, packing them as best as possible
	if (_images.size() > 1) {
		// a separate vector to sort the bounds by height (smallest to largest)
		std::vector<Rect> heights;
		heights.reserve(_images.size());
		heights.push_back(texturedata.back());
		// goes through all the remaining images, packing them appropriately
		for (auto image = _images.begin()+1; image != _images.end(); ++image) {
			// sets up for the image packing process
			std::cout << "Packing '" << image->filename << "'... ";
			unsigned int smallest = 0;
			bool badplace = true;
			Rect bounds = { 0, 0, image->content.w, image->content.h };
			// keep searching for a valid spot on the texture until one is found
			while (badplace) {
				// check to see if the image intersects with any currently packed images...
				for (auto packedimg: texturedata) {
					badplace = false;
					if (bounds.Intersects(packedimg)) {
						// ...if it does then it is moved along to the right
						badplace = true;
						bounds.x += packedimg.w;
						break;
					}
				}
				// repeat the process until the image does not intersect with any others
				if (badplace) { continue; }
				// if the image is out of bounds (horizontally)...
				if (bounds.x + bounds.w > _texturewidth) {
					// ...then move it back to the left and downwards for continued searching
					badplace = true;
					bounds.x = 0, bounds.y = heights.at(smallest).y + heights.at(smallest).h;
					smallest++;
					// if the image is out of bounds (vertically)...
					if (bounds.y + bounds.h > _textureheight) {
						// ...then resize the texture as there is no valid spot currently available
						_texturewidth = std::pow(2, _power++);
						_textureheight = _texturewidth;
						bounds.x = 0, bounds.y = 0;
						smallest = 0;
					}
				}
			}
			// pack the image into the texture
			texturedata.push_back(bounds);
			// add its bounds to the height-sorted vector, for use in future packing
			heights.push_back(bounds);
			// the height must be sorted smallest to largest to work effectively
			std::stable_sort(
				heights.begin(), heights.end(),
				[] (const Rect& _lhs, const Rect& _rhs)->bool
				{ return ((_lhs.y + _lhs.h) < (_rhs.y + _rhs.h)); }
			);
			// the packing for the image was successful
			std::cout << "successful!" << std::endl;
		}
	}

	// returns the positions (and sizes) of the images in the packed texture
	return texturedata;
}

int main (int _argc, char* _argv[])
{
	// cannot operate if no image data is passed to the program
	if (_argc <= 1) {
		std::cout << "No image files given!" << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}

	// ask user if they want images cropped or not
	while (true) {
		char cropinput = 0;
		std::cout << "Crop images (Y/N): ";
		std::cin >> cropinput;
		std::cin.ignore();
		std::cin.clear();
		if (std::toupper(cropinput) == 'Y') { crop = true; break; }
		else if (std::toupper(cropinput) == 'N') { break; }
		else { std::cout << "Invalid input!" << std::endl; } 
	}

	// ask user if they want a certain width and height or not
	while (true) {
		char sizeinput = 0;
		std::cout << "Custom width and height (Y/N): ";
		std::cin >> sizeinput;
		std::cin.ignore();
		std::cin.clear();
		if (std::toupper(sizeinput) == 'Y') { size = true; break; }
		else if (std::toupper(sizeinput) == 'N') { break; }
		else { std::cout << "Invalid input!" << std::endl; } 
	}

	// ask for the two size values
	if (size) {
		std::cout << "Enter image width: ";
		std::cin >> customwidth;
		std::cin.ignore();
		std::cin.clear();
		std::cout << "Enter image height: ";
		std::cin >> customheight;
		std::cin.ignore();
		std::cin.clear();
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

	// sorts all the images based on size (largest to smallest)
	std::cout << "Beginning texture sorting..." << std::endl;
	std::stable_sort(
		images.begin(), images.end(),
		[] (const Image& _lhs, const Image& _rhs)->bool
		{ return (_rhs.content.GetSize() < _lhs.content.GetSize()); }
	);
	std::cout << "Texture sorting complete!" << std::endl;

	// calculates the way to pack the different images together
	std::cout << "Beginning texture packing..." << std::endl;
	int texturewidth = 0, textureheight = 0;
	int power = 0;
	auto texturedata = PackTexture(images, texturewidth, textureheight, power);
	std::cout << "Texture packing complete!" << std::endl;

	// produces the final image made up of all input images together
	std::cout << "Beginning texture creation..." << std::endl;
	// allocates a buffer to store the pixel data of the texture
	std::vector<std::uint8_t> buffer;
	buffer.resize((texturewidth * textureheight) * Image::BPP, 0);
	// stores the texture pitch (saves on performing the calculation repeatedly)
	int texturepitch = texturewidth * Image::BPP;
	// writes the data of each image into the buffer (at the correct pos)
	for (unsigned int i = 0; i < images.size(); ++i) {
		auto& image = images.at(i);
		Rect bounds = texturedata.at(i);
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
	}
	std::cout << "Texture creation complete!" << std::endl;

	// lets the user enter the name for the ouput file
	std::cout << "Enter a name for the output texture: ";
	std::string outputname;
	std::getline(std::cin, outputname);

	// saves the image data in the buffer as a packed PNG texture
	std::cout << "Beginning texture saving (this can take a while)..." << std::endl;
	stbi_write_png(
		(outputname + ".png").c_str(),
		texturewidth, textureheight,
		Image::BPP,
		&buffer[0],
		texturewidth * Image::BPP
	);
	std::cout << "Texture saving complete!" << std::endl;

	// produces the reference file that stores each image's bounds
	std::cout << "Beginning reference creation..." << std::endl;
	std::ofstream reference(outputname + ".ref", std::ios::trunc);
	// handles failure to create the reference file (terminates)
	if (!reference.is_open()) {
		std::cout << "Reference creaion failed!" << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}
	// begins writing data to the output stream
	for (unsigned int i = 0; i < images.size(); ++i) {
		// creates a unique identifier for each image reference
		std::string::size_type startpos = images.at(i).filename.find_last_of("/\\");
		std::string refname = images.at(i).filename.substr(startpos+1);
		// writes the identifier, boundary data, and offset data, to the file (in plain-text ASCII)
		reference << refname << " " << texturedata.at(i) << " " << images.at(i).content.x << " " << images.at(i).content.y << "\n";
	}
	// closes the stream after use
	reference.close();
	std::cout << "Reference creation complete!" << std::endl;

	// terminate the program successfully
	std::cout << "\nTEXPACKER HAS SUCCESSFULLY FINISHED!" << std::endl;
	std::cin.get();
	return EXIT_SUCCESS;
}