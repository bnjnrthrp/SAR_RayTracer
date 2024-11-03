#include "../include/common.h"

#include "../include/camera.h"
#include "../include/hittable.h"
#include "../include/hittable_list.h"
#include "../include/material.h"
#include "../include/sphere.h"



int main() {

    // World

    hittable_list world;

    double R = std::cos(pi / 4);

    std::shared_ptr<lambertian> material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    std::shared_ptr<lambertian> material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    std::shared_ptr<dielectric> material_left = make_shared<dielectric>(1.5);
    std::shared_ptr<dielectric> material_bubble = make_shared<dielectric>(1.0 / 1.5);
    std::shared_ptr<metal> material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 10.0;
    cam.focus_dist = 3.4;
    // Render

    cam.render(world);
}