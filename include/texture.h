#ifndef TEXTURE_H
#define TEXTURE_H

#include <map>
#include "perlin.h"
#include "rtw_stb_image.h"

enum TEXTURE_ROUGHNESS {SMOOTH, SLIGHTLY_ROUGH, ROUGH};

// X- 3cm C- 6cm L- 23cm
// X- Smooth = .1cm, Rough = 1.5cm
// C- Smooth = .2cm, Rough = 3.0cm
// L- Smooth = .8cm, Rough = 11.5cm
static std::map<std::string, double> tex_map = {
	{"Stone", .017},
	{"Grass", .016},
	{"Dirt", .02},
	{"Cobblestone", .01},
	{"Wooden_Plank", .08},
	{"Sand", .05},
	{"Gravel", .021},
	{"Iron_Ore", .017},
	{"Coal_Ore", .017},
	{"Log", .018},
	{"Leaves", .023},
	{"Sandstone", .011},
	{"Bed", .04},
	{"Tall_Grass", .05},
	{"Dandelion", .06},
	{"Rose", .06},
	{"Iron_Block", .008},
	{"Double_Slab", .008},
	{"Stone_Slab", .1},
	{"Bookshelf", .008},
	{"Torch", .06},
	{"Wooden_Stairs", .08},
	{"Chest", .1},
	{"Wooden_Door", .005},
	{"Ladder", .02},
	{"Cobblestone_Stairs", .011},
	{"Wall_Sign", .001},
	{"Lever", .02},
	{"Iron_Door", .0003},
	{"Wooden_Plate", .02},
	{"Redstone_Torch_(on)", .03},
	{"Stone_Button", .01},
	{"Fence", .013},
	{"Glowstone", .017},
	{"Stone_Brick", .01},
	{"Iron_Bars", .0004},
	{"Glass_Pane", .00001},
	{"Stone_Brick_Stairs", .02},
	{"White_Wool", .017},
	{"Red_Wool", .017},
	{"Sapling", .017},
	{"Stationary_Water", .00005},
	{"Stationary_Lava", .02},
	{"Glass", .00005},
	{"Dispenser", .023},
	{"Piston", .012},
	{"Piston_Head", .012},
	{"Brown_Mushroom", .03},
	{"Red_Mushroom", .03},
	{"Gold_Block", .007},
	{"Brick", .01},
	{"Moss_Stone", .016},
	{"Obsidian", .01},
	{"Fire", .04},
	{"Monster_Spawner", .1},
	{"Redstone_Wire", .019},
	{"Crafting_Table", .019},
	{"Crops", .4},
	{"Farmland", .3},
	{"Furnace", .01},
	{"Sign_Post", .001},
	{"Rail", .03},
	{"Stone_Plate", .009},
	{"Redstone_Torch_(off)", .017},
	{"Clay", .010},
	{"Sugar_Cane", .4},
	{"Netherrack", .04},
	{"Jack-O-Lantern", .04},
	{"Cake", .02},
	{"Repeater_(on)", .02},
	{"Trapdoor", .011},
	{"Vines", .02},
	{"Fence_Gate", .012},
	{"Brick_Stairs", .018},
	{"Lily_Pad", .017},
	{"Enchantment_Table", .011},
	{"Orange_Wool", .017},
	{"Light_Blue_Wool", .017},
	{"Gray_Wool", .017},
	{"Light_Gray_Wool", .017},
	{"Cyan_Wool", .017},
	{"Brown_Wool", .017},
	{"Green_Wool", .017},
	{"Black_Wool", .017},
	{"Main", .0012},
	{"Material.001", .002},
	{"Material.002", .0015}
};

//static std::map<std::string, TEXTURE_ROUGHNESS> tex_map = {
//	{"Stone", rough},
//	{"Grass", rough},
//	{"Dirt", rough},
//	{"Cobblestone", medium},
//	{"Wooden_Plank", smooth},
//	{"Sand", smooth},
//	{"Gravel", rough},
//	{"Iron_Ore", rough},
//	{"Coal_Ore", rough},
//	{"Log", rough},
//	{"Leaves", rough},
//	{"Sandstone", medium},
//	{"Bed", rough},
//	{"Tall_Grass", rough},
//	{"Dandelion", rough},
//	{"Rose", rough},
//	{"Iron_Block", smooth},
//	{"Double_Slab", smooth},
//	{"Stone_Slab", medium},
//	{"Bookshelf", smooth},
//	{"Torch", rough},
//	{"Wooden_Stairs", rough},
//	{"Chest", rough},
//	{"Wooden_Door", smooth},
//	{"Ladder", rough},
//	{"Cobblestone_Stairs", medium},
//	{"Wall_Sign", smooth},
//	{"Lever", rough},
//	{"Iron_Door", smooth},
//	{"Wooden_Plate", rough},
//	{"Redstone_Torch_(on)", rough},
//	{"Stone_Button", medium},
//	{"Fence", medium},
//	{"Glowstone", rough},
//	{"Stone_Brick", medium},
//	{"Iron_Bars", smooth},
//	{"Glass_Pane", smooth},
//	{"Stone_Brick_Stairs", rough},
//	{"White_Wool", rough},
//	{"Red_Wool", rough},
//	{"Sapling", rough},
//	{"Stationary_Water", smooth},
//	{"Stationary_Lava", rough},
//	{"Glass", smooth},
//	{"Dispenser", rough},
//	{"Piston", medium},
//	{"Piston_Head", medium},
//	{"Brown_Mushroom", rough},
//	{"Red_Mushroom", rough},
//	{"Gold_Block", smooth},
//	{"Brick", medium},
//	{"Moss_Stone", rough},
//	{"Obsidian", medium},
//	{"Fire", rough},
//	{"Monster_Spawner", rough},
//	{"Redstone_Wire", rough},
//	{"Crafting_Table", rough},
//	{"Crops", rough},
//	{"Farmland", rough},
//	{"Furnace", medium},
//	{"Sign_Post", smooth},
//	{"Rail", rough},
//	{"Stone_Plate", medium},
//	{"Redstone_Torch_(off)", rough},
//	{"Clay", medium},
//	{"Sugar_Cane", rough},
//	{"Netherrack", rough},
//	{"Jack-O-Lantern", rough},
//	{"Cake", rough},
//	{"Repeater_(on)", rough},
//	{"Trapdoor", medium},
//	{"Vines", rough},
//	{"Fence_Gate", medium},
//	{"Brick_Stairs", rough},
//	{"Lily_Pad", rough},
//	{"Enchantment_Table", medium},
//	{"Orange_Wool", rough},
//	{"Light_Blue_Wool", rough},
//	{"Gray_Wool", rough},
//	{"Light_Gray_Wool", rough},
//	{"Cyan_Wool", rough},
//	{"Brown_Wool", rough},
//	{"Green_Wool", rough},
//	{"Black_Wool", rough}
//};

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