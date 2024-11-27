#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"
#include "rtw_stb_image.h"

class texture {
public:
	virtual ~texture() = default;
	virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
	solid_color(const color& albedo) : albedo(albedo) {}
	solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

	color value(double u, double v, const point3& p) const override {
		return albedo;
	}

private:
	color albedo;
};

class checker_texture : public texture {
public:
	checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd) : inv_scale(1.0 / scale), even(even), odd(odd) {}
	checker_texture(double scale, const color& c1, const color& c2) : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)){}

	color value(double u, double v, const point3& p) const override {
		int xInteger = int(std::floor(inv_scale * p.x()));
		int yInteger = int(std::floor(inv_scale * p.y()));
		int zInteger = int(std::floor(inv_scale * p.z()));

		bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

		return isEven ? even->value(u, v, p) : odd->value(u, v, p);
	}

private:
	double inv_scale;
	shared_ptr<texture> even;
	shared_ptr<texture> odd;
};

class image_texture : public texture {
public:
	image_texture(const char* filename) : image(filename) {}

	color value(double u, double v, const point3& p) const override {
		if (image.height() <= 0) return color(0, 1, 1);

		u = interval(0, 1).clamp(u);
		v = 1.0 - interval(0, 1).clamp(v); // Flip V to image coordinates

		int i = int(u * image.width());
		int j = int(v * image.height());
		const unsigned char* pixel = image.pixel_data(i, j);

		double color_scale = 1.0 / 255.0;
		return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
	}
private:
	rtw_image image;
};

class noise_texture : public texture {
public:
	noise_texture(double scale) : scale(scale) {}

	color value(double u, double v, const point3& p) const override {
		// return color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p));
		// return color(1, 1, 1) * noise.turb(p, 7);
		return color(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
	}

private:
	perlin noise;
	double scale;
};

class roughness_from_sharpness_texture : public texture {
public:
	roughness_from_sharpness_texture() {}

	roughness_from_sharpness_texture(shared_ptr<texture> sharpness_map, double min_v, double max_v) : sharpness_text(sharpness_map),
		l_min_val(log(min_v)), l_max_val(log(max_v)) {}

	virtual color value(double u, double v, const point3& p) const override {
		return color(1, 0, 0) * std::clamp(
			log(sharpness_text->value(u, v, p).length() + 0.00001), l_min_val, l_max_val)
			/ (l_max_val - l_min_val);
	}

public:
	shared_ptr<texture> sharpness_text;
private:
	double l_min_val, l_max_val;
};

#endif // TEXTURE_H