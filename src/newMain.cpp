#include <functional>
#include <iostream>
#include <thread>

#include "../include/common.h"
#include "../include/bvh.h"
#include "../include/camera.h"
#include "../include/constant_medium.h"
#include "../include/hittable.h"
#include "../include/hittable_list.h"
#include "../include/material.h"
#include "../include/triangle.h"
#include "../include/quad.h"
#include "../include/sphere.h"
#include "../include/model.h"
#include "../include/texture.h"
#include "../include/obj_loader.h"


/*
void bouncing_spheres() {

    hittable_list world;

    std::shared_ptr<checker_texture> checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9 ,.9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    color albedo = color::random() * color::random();
                    std::shared_ptr<lambertian> sphere_material = make_shared<lambertian>(albedo);
                    point3 center2 = center + vec3(0, random_double(0, .5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    color albedo = color::random(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    std::shared_ptr<metal> sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    std::shared_ptr<dielectric> sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    std::shared_ptr<dielectric> material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    std::shared_ptr<lambertian> material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    std::shared_ptr<metal> material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.7, 0.8, 1.0);

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);
}


void checkered_spheres() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.7, 0.8, 1.0);

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void earth() {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.7, 0.8, 1.0);

    cam.vfov = 20;
    cam.lookfrom = point3(0, 0, 12);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}

void perlin_spheres() {
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.7, 0.8, 1.0);

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void quads() {
    hittable_list world;

    // Materials
    auto left_red = make_shared<lambertian>(color(1.0, .2, .2));
    auto back_green = make_shared<lambertian>(color(.2, 1., .2));
    auto right_blue = make_shared<lambertian>(color(.2, .2, 1.));
    auto upper_orange = make_shared<lambertian>(color(1., .5, 0.));
    auto lower_teal = make_shared<lambertian>(color(.2, .8, .8));

    // Quads
    world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.7, 0.8, 1.0);

    cam.vfov = 80;
    cam.lookfrom = point3(0, 0, 9);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void simple_light() {
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    world.add(make_shared<sphere>(point3(0, 7, 0), 2, difflight));
    world.add(make_shared<quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 20;
    cam.lookfrom = point3(26, 3, 6);
    cam.lookat = point3(0, 2, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}
*/
void cornell_box() {
    hittable_list world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

    // Box 1
    // Aluminum option
    /*shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), aluminum);*/

    // Normal box
    shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_xyz>(box1, 0, 15, 0);
    box1 = make_shared<scale>(box1, vec3(.5, .5, .5));
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    /*shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_xyz>(box1, 0, -18, 0);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    world.add(box2);*/

    // Glass sphere
    auto glass = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(190, 90, 190), 90, glass));

    // Light Sources
    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    lights.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), empty_material));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 10;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world, lights);
}

void cornell_smoke() {
    hittable_list world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    world.add(make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
    world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

    shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_xyz>(box1, 0, 15, 0);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));

    shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_xyz>(box1, 0, -18, 0);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));



    world.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    world.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    // Light Sources
    auto empty_material = shared_ptr<material>();
    quad lights(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), empty_material);


    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world, lights);
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list world;

    world.add(make_shared<bvh_node>(boxes1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    world.add(make_shared<quad>(point3(123, 554, 147), vec3(300, 0, 0), vec3(0, 0, 265), light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

    world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));
    world.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.2);
    world.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    world.add(make_shared<translate>(
        make_shared<rotate_xyz>(
            make_shared<bvh_node>(boxes2), 0, 15, 0),
        vec3(-100, 270, 395)
    )
    );

    // Light Sources
    auto empty_material = shared_ptr<material>();
    quad lights(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), empty_material);


    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = image_width;
    cam.samples_per_pixel = samples_per_pixel;
    cam.max_depth = max_depth;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(478, 278, -600);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);
 
    cam.defocus_angle = 0;

    cam.render(world, lights);
}

void simple_triangle() {
    hittable_list world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto green = make_shared<lambertian>(color(.12, .45, .15));

    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    world.add(make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
    world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

    world.add(make_shared<triangle>(point3(265, 100, 295),  point3(300, 100, 295), point3(265, 200, 350), green));
    world.add(make_shared<triangle>(point3(100, 100, 150), point3(550, 100, 550), point3(250, 400, 250), red));
    /*triangle simple(point3(100, 100, 150), point3(550, 100, 550), point3(250, 400, 250), red);
    vec3 dummy(1, 1, 1);
    std::clog << "Simple triangle \n";
    simple.print(std::clog, dummy, dummy, 1.0);*/
    // Light Sources
    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    lights.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), empty_material));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 10;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world, lights);
}

void cornell_tetra() {
    hittable_list world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto grey = make_shared<lambertian>(color(.5, .5, .5));

    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    world.add(make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
    world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

    /*std::shared_ptr<hittable> tetrahedron = load_model_from_file("./images/tetrahedron.obj", grey, true);
    shared_ptr<hittable>tetrahedron = make_shared<scale>(tetrahedron, vec3(100, 100, 100), grey);
    tetrahedron = make_shared<rotate_xyz>(tetrahedron, 0.0, 45.0, 0.0);
    tetrahedron = make_shared<translate>(tetrahedron, vec3(265, 300, 200));
    world.add(tetrahedron);*/


    /*std::shared_ptr<hittable> cube = load_model_from_file("./images/cube.obj", red, true);
    cube = make_shared<rotate_xyz>(cube, 0, 15, 0);
    cube = make_shared<scale>(cube, vec3(100, 100, 100));
    cube = make_shared<translate>(cube, vec3(200, 100, 100));
    world.add(cube);*/

    

    auto backpack = load_model_from_file("./backpack/backpack.obj", green, true);
   /* std::clog << "\n\n\n\n\n\n\n\n\nBefore adding/after loading\n";
    backpack->bounding_box().print(std::clog);*/
  
    backpack = make_shared<rotate_xyz>(backpack, 0, 40, 55);
    backpack = make_shared<scale>(backpack, vec3(75, 75, 75));
    backpack = make_shared<translate>(backpack, vec3(150, 200, 100));
    world.add(backpack);
   // std::clog << "\n\n\nAfter loading backpack\n";
   // backpack->bounding_box().print(std::clog);
   // world.bounding_box().print(std::clog);

    /*std::shared_ptr<hittable> cube2 = load_model_from_file("./images/tetrahedron.obj", green, true);
    cube2 = make_shared<rotate_xyz>(cube2, 60, 0, 55);
    cube2 = make_shared<scale>(cube2, vec3(100, 100, 100));
    cube2 = make_shared<translate>(cube2, vec3(300, 200, 400));
    world.add(cube2);*/
    
    /*std::clog << "\n\n\nAfter loading tetrahedron\n";
    world.bounding_box().print(std::clog);*/

    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    lights.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), empty_material));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 50;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world, lights);
}

void rungholt() {
    hittable_list world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto grey = make_shared<lambertian>(color(.5, .5, .5));

    /*world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    world.add(make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
    world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));*/

    /*std::shared_ptr<hittable> tetrahedron = load_model_from_file("./images/tetrahedron.obj", grey, true);
    shared_ptr<hittable>tetrahedron = make_shared<scale>(tetrahedron, vec3(100, 100, 100), grey);
    tetrahedron = make_shared<rotate_xyz>(tetrahedron, 0.0, 45.0, 0.0);
    tetrahedron = make_shared<translate>(tetrahedron, vec3(265, 300, 200));
    world.add(tetrahedron);*/


    auto cube = load_model_from_file("./rungholt/rungholt.obj", red, true);
    cube = make_shared<scale>(cube, vec3(100, 100, 100));
    cube = make_shared<translate>(cube, vec3(200, 100, 100));
    world.add(cube);

   

    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    lights.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), empty_material));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 10;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world, lights);
}

void cornell_SAR() {
    //hittable_list world;
    //auto fuzz = make_shared<noise_texture>(4);
    //auto low_gloss = make_shared<solid_color>(.12, .12, .12);
    //auto med_gloss = make_shared<solid_color>(.65, .65, .65);
    //auto high_gloss = make_shared<solid_color>(.9, .9, .9);

    //auto low_glossy = make_shared<glossy>(low_gloss, fuzz);
    //auto med_glossy = make_shared<glossy>(med_gloss, fuzz);
    //auto high_glossy = make_shared<glossy>(high_gloss, fuzz);
    //auto light = make_shared<diffuse_light>(color(7, 7, 7));
    ////auto absorbant = make_shared<glossy>(color(0, 0, 0), 0);

    //world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), low_glossy));
    //world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), med_glossy));
    //world.add(make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
    //world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), high_glossy));
    //world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), high_glossy));
    //world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), high_glossy));

    ///*std::shared_ptr<hittable> tetrahedron = load_model_from_file("./images/tetrahedron.obj", grey, true);
    //shared_ptr<hittable>tetrahedron = make_shared<scale>(tetrahedron, vec3(100, 100, 100), grey);
    //tetrahedron = make_shared<rotate_xyz>(tetrahedron, 0.0, 45.0, 0.0);
    //tetrahedron = make_shared<translate>(tetrahedron, vec3(265, 300, 200));
    //world.add(tetrahedron);*/


    ////std::shared_ptr<hittable> cube = load_model_from_file("./images/tetrahedron.obj", grey, true);
    //shared_ptr<hittable> cube = tetrahedron(low_glossy);
    //cube = make_shared<scale>(cube, vec3(100, 100, 100));
    //cube = make_shared<rotate_xyz>(cube, 0, 15, 0);
    //cube = make_shared<translate>(cube, vec3(200, 100, 100));
    //world.add(cube);



    ///*std::shared_ptr<hittable> cube2 = load_model_from_file("./images/tetrahedron.obj", green, true);
    //cube2 = make_shared<scale>(cube2, vec3(100, 100, 100));
    //cube2 = make_shared<rotate_xyz>(cube2, 0, 40, 55);
    //cube2 = make_shared<translate>(cube2, vec3(300, 200, 400));
    //world.add(cube2);*/


    //auto empty_material = shared_ptr<material>();
    //hittable_list lights;
    //lights.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), empty_material));

    //camera cam;

    //cam.aspect_ratio = 1.0;
    //cam.image_width = 600;
    //cam.samples_per_pixel = 10;
    //cam.max_depth = 50;
    //cam.background = color(0, 0, 0);

    //cam.vfov = 40;
    //cam.lookfrom = point3(278, 278, -800);
    //cam.lookat = point3(278, 278, 0);
    //cam.vup = vec3(0, 1, 0);

    //cam.defocus_angle = 0;

    //cam.render(world, lights);
}


int main() {
    switch (11) {
    //case 1: bouncing_spheres();  break;
    //case 2: checkered_spheres(); break;
    //case 3: earth(); break;
    //case 4: perlin_spheres(); break;
    //case 5: quads(); break;
    //case 6: simple_light(); break;
    case 7: cornell_box(); break;
    case 8: cornell_smoke(); break;
    case 9: final_scene(800, 10000, 40); break;
    case 10: simple_triangle(); break;
    case 11: cornell_tetra(); break;
    case 12: rungholt(); break;
    case 13: cornell_SAR(); break;

    default: final_scene(400, 250, 4); break;
    }
}
