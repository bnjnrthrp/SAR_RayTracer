#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

#ifdef _MSC_VER
	#pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "../external/stb_image.h"

#include <cstdlib>
#include <iostream>

class rtw_image {
public:
	rtw_image() {}

	/*
	Loads image data from specified file. If RTW_IMAGES environment variable is defined, will only search there. 
	Otherwise,it will search for a directory called "images/" in current and parent subdirectories. 
	If unsuccessful, width() and height() will return 0.
	@param image_filename, the name of the file*/
	rtw_image(const char* image_filename) {
		std::string filename = std::string(image_filename);
		auto imagedir = getenv("RTW_IMAGES");

		if (imagedir && load(std::string(imagedir) + "/" + image_filename)) return;
		if (load(filename)) return;
		if (load("images/" + filename)) return;
		if (load("../images/" + filename)) return;
		if (load("../../images/" + filename)) return;
		if (load("../../../images/" + filename)) return;
		if (load("../../../../images/" + filename)) return;
		if (load("../../../../../images/" + filename)) return;

		std::cerr << "ERROR: Could not load image file '" << image_filename << "'." << std::endl;
	}

	~rtw_image() {
		delete[] bdata;
		STBI_FREE(fdata);
	}

	/*
	Loads the linear (gamma = 1) image data from the given file name. Resulting data buffer contains three [0.0, 1.0]
	floating point values for the first pixel (RGB). Pixels are contiguous, going left to right for width of image, followed by the next row.
	@param filename, the name of the file
	@returns True if the load succeeds, otherwise false.
	*/
	bool load(const std::string& filename) {
		int n = bytes_per_pixel;
		fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
		if (fdata == nullptr) return false;

		bytes_per_scanline = image_width * bytes_per_pixel;
		convert_to_bytes();
		return true;
	}

	int width() const { return (fdata == nullptr) ? 0 : image_width; }
	int height() const { return (fdata == nullptr) ? 0 : image_height; }


	/*
	@returns the address of the 3 RGB bytes at that location, otherwise magenta (255, 0, 255)
	*/
	const unsigned char* pixel_data(int x, int y) const {
		static unsigned char magenta[] = { 255, 0, 255 };
		if (bdata == nullptr) return magenta;

		x = clamp(x, 0, image_width);
		y = clamp(y, 0, image_height);
		
		return bdata + (y * bytes_per_scanline) + (x * bytes_per_pixel);
	}

private:
	const int		bytes_per_pixel		= 3;
	float*			fdata				= nullptr;	// Linear floating point pixel data
	unsigned char*	bdata				= nullptr;	// Linear 8-bit pixel data
	int				image_width			= 0;		// Loaded image width
	int				image_height		= 0;		// Loaded image height
	int				bytes_per_scanline	= 0;

	/*Clamps an input to the range [low, high)
	@param x: input value
	@param low: lower limit
	@param high: upper limit*/
	static int clamp(int x, int low, int high) {
		if (x < low) return low;
		if (x < high) return x;
		return high - 1;
	}

	static unsigned char float_to_byte(float value) {
		if (value <= 0.0)
			return 0;
		if (value >= 1.0)
			return 255;
		return static_cast<unsigned char>(256.0 * value);
	}

	/*Convert linear floating point pixel data to bytes. Stores resulting byte data in bdata member*/
	void convert_to_bytes() {
		int total_bytes = image_width * image_height * bytes_per_pixel;
		bdata = new unsigned char[total_bytes];


		// Iteratr through all pixel components, convert [0.0, 1.0] to [0, 255] unsigned byte values
		auto* bptr = bdata;
		auto* fptr = fdata;
		for (auto i = 0; i < total_bytes; i++, fptr++, bptr++)
			*bptr = float_to_byte(*fptr);
	}

};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#endif // RTW_STB_IMAGE_H