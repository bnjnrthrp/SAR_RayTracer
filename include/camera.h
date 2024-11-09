#ifndef CAMERA_H
#define CAMERA_H

/**
* Original code by Peter Shirley (Ray Tracing in a Weekend series)	
*/

#include "hittable.h"
#include "material.h"

class camera {
public: 
	double  aspect_ratio        = 1.0;      // Ratio of image width over height
    int     image_width         = 100;      // Rendered image width in pixel count
    int     samples_per_pixel   = 10;       // Count of random samples per pixel
    int     max_depth           = 10;       // Max number of ray bounces into a scene
    color   background;                     // Scene background color

    double vfov                 = 90;                   // Vertical view angle (field of view)
    point3 lookfrom             = point3(0, 0, 0);      // Point camera is looking from
    point3 lookat               = point3(0, 0, -1);     // Point camera is looking at
    vec3   vup                  = vec3(0, 1, 0);        // Camera-relative "up" direction

    double defocus_angle        = 0;        // Variation angle of rays through each pixel
    double focus_dist           = 10;       // Distance from camera lookfrom point to plane of perfect focus

	void render(const hittable & world) {
		initialize();

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; j++) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; i++) {
                color pixel_color(0.0, 0.0, 0.0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

				write_color(std::cout, pixel_samples_scale * pixel_color);
			}
		}

		std::clog << "\rDone.                 \n";
	}
private:
    int    image_height;            // Rendered image height
    double pixel_samples_scale;     // Color scale factor for a sum of pixel samples
    point3 center;                  // Camera center
    point3 pixel00_loc;             // Location of pixel 0, 0
    vec3   pixel_delta_u;           // Offset to pixel to the right
    vec3   pixel_delta_v;           // Offset to pixel below
    vec3   u, v, w;                 // Camera frame basis vectors
    vec3   defocus_disk_u;          // Defocus disk horizontal radius
    vec3   defocus_disk_v;          // Defocus disk vertical radius

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Determine viewport dimensions.
        double theta = degrees_to_radians(vfov);
        double h = std::tan(theta / 2.0);
        double viewport_height = 2.0 * h * focus_dist;
        double viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate the u, v, w unit basis vectors for the camera coordinate
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        point3 viewport_upper_left = center - (focus_dist * w) - viewport_u/2.0 - viewport_v/2.0;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors
        double defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2.0));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    /*Constructs a camera ray originatin from the origin and directed at pixel i, j*/
    ray get_ray(int i, int j) const {
        vec3 offset = sample_square();
        point3 pixel_sample = pixel00_loc
            + ((i + offset.x()) * pixel_delta_u)
            + ((j + offset.y()) * pixel_delta_v);

        point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        vec3 ray_direction = pixel_sample - ray_origin;
        double ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        point3 p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) {
        if (depth <= 0)
            return color(0, 0, 0);
        
        hit_record rec;

      

        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;
            
        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;
        
        color color_from_scatter = attenuation * ray_color(scattered, depth - 1, world);
        
        return color_from_emission + color_from_scatter;
    }
};





#endif // CAMERA_H