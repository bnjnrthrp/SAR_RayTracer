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

void cornell_box() {

    hittable_list world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    // Cornell box sides
    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 0, 555), vec3(0, 555, 0), green));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(0, 0, -555), vec3(0, 555, 0), red));
    world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 0, -555), white));
    world.add(make_shared<quad>(point3(555, 0, 555), vec3(-555, 0, 0), vec3(0, 555, 0), white));

    // Light
    world.add(make_shared<quad>(point3(213, 554, 227), vec3(130, 0, 0), vec3(0, 0, 105), light));

    // Box
    shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_xyz>(box1, 0, 15, 0);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_xyz>(box2, 0, -18, 0);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    world.add(box2);

    // Light Sources
    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    lights.add(
        make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), empty_material));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 30;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.initialize();
    cam.render(world, lights);
}

void cornell_SAR() {
    hittable_list world;

    auto med_gray = make_shared<solid_color>(color(.45));
    auto med_fuzz = make_shared<solid_color>(color(.5));

    auto rough = make_shared<lambertian>(color(.2));
    auto slightly_rough = make_shared<medium>(med_gray, med_fuzz, .7);
    auto smooth = make_shared<lambertian>(color(.9));
    auto light = make_shared<diffuse_light>(color(7));

    world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), slightly_rough)); // right wall
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), slightly_rough)); // left wall
    world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), smooth)); // Ceiling
    world.add(make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), slightly_rough)); // ceiling
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), rough)); // floor
    world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), rough)); // back wall
    //world.add(make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));

    

    // Box
    shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), smooth);
    box1 = make_shared<rotate_xyz>(box1, 0, 15, 0);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), slightly_rough);
    box2 = make_shared<rotate_xyz>(box2, 0, -18, 0);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    world.add(box2);

    camera radar;
    double wavelength = SPECTRAL_MAP.find(X)->second;

    radar.aspect_ratio = 1.0;
    radar.image_width = 400;
    radar.samples_per_pixel = 30;
    radar.max_depth = 50;
    radar.background = color(0, 0, 0);

    radar.vfov = 40;
    radar.lookfrom = point3(278, 278, -800);
    radar.lookat = point3(278, 278, 0);
    radar.vup = vec3(0, 1, 0);

    radar.defocus_angle = 0;

    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    vec3 offset = (radar.lookat - radar.lookfrom) * 0.01;

    radar.initialize();
    radar.colocate_light(world, lights, light);

    radar.render(world, lights);
}


void eiffel_SAR() {
    hittable_list world;

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto white = make_shared<lambertian>(color(.1, .1, .1));

    // Choose from frequency bands {Visible, X-, C-, L-}
    double wavelength = SPECTRAL_MAP.find(X)->second;

    auto tower = load_model_from_file("./models/eiffel.obj", white, wavelength);
    world.add(tower);
    

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 30;
    cam.max_depth = 5;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    point3 center = world.bounding_box().get_center() * vec3(1, 0, 1); // View the center of the world at 0 elevation
    point3 near(center.x(), 500, -center.z() * 3);
    point3 far(center.x(), 2000, -center.z() * 30);

    cam.lookfrom = point3(0, 800, -800);
    
    //cam.lookfrom = far;
    // cam.lookfrom = wherever_you_are
    //cam.lookat = point3(278, 278, 0);
    cam.lookat = vec3(0, 0, -200);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;


    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    vec3 offset = (cam.lookat - cam.lookfrom) * 0.01;

    cam.initialize();
    //lights.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), empty_material));
    cam.colocate_light(world, lights, light);
    //world.add(make_shared<quad>(cam.lookfrom - offset, vec3(600, 0, 0), vec3(0, 600, 0), light));
    //lights.add(make_shared<quad>(cam.lookfrom - offset, vec3(-600, 0, 0), vec3(0, -600, 0), empty_material));
    cam.render(world, lights);
}




void house_ray() {
    hittable_list world;

    auto red = make_shared<lambertian>(color(200, 10, 10));
    auto grey = make_shared<lambertian>(color(.05, .05, .05));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));


    world.add(make_shared<quad>(point3(-200, -1, -200), vec3(555, 0, 0), vec3(0, 0, 555), grey)); // floor

    double wavelength = SPECTRAL_MAP.find(VISIBLE)->second;
    std::shared_ptr<hittable> house = load_model_from_file("./models/house.obj", red, 0.0);
    house = make_shared<scale>(house, vec3(200, 200, 200));
    house = make_shared<translate>(house, vec3(0, 0, 100));
    world.add(house);

    world.add(make_shared<quad>(point3(100, 800, 0), vec3(330, 0, 0), vec3(0, 0, 305), light));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 30;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 50;
    point3 center = world.bounding_box().get_center() * vec3(1, 0, 1); // View the center of the world at 0 elevation
    point3 near(center.x(), 500, -center.z() * 3);
    point3 far(center.x(), 2000, -center.z() * 30);

    cam.lookfrom = point3(center.x(), 800, -800);
    cam.lookfrom = near;
    //cam.lookfrom = far;
    //cam.lookfrom = wherever_you_are
    //cam.lookat = point3(278, 278, 0);
    cam.lookat = world.bounding_box().get_center() * vec3(1, 0, 1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;


    // Light Sources
    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    lights.add(
        make_shared<quad>(point3(430, 800, 305), vec3(-330, 0, 0), vec3(0, 0, -305), empty_material));

    cam.initialize();

    cam.render(world, lights);
}

void house_SAR() {
    hittable_list world;


    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto white = make_shared<lambertian>(color(.73, .73, .73));

    world.add(make_shared<quad>(point3(-200, -1, -200), vec3(555, 0, 0), vec3(0, 0, 555), white)); // floor

    std::shared_ptr<hittable> house = load_model_from_file("./models/house.obj", white, 0.0);
    house = make_shared<scale>(house, vec3(200, 200, 200));
    house = make_shared<translate>(house, vec3(0, 0, 200));
    world.add(house);

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 30;
    cam.max_depth = 5;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    point3 center = world.bounding_box().get_center() * vec3(1, 0, 1); // View the center of the world at 0 elevation
    point3 near(center.x(), 500, -center.z() * 3);
    point3 far(center.x(), 2000, -center.z() * 30);

    cam.lookfrom = point3(center.x(), 800, -800);
    cam.lookfrom = near;
    //cam.lookfrom = far;
    // cam.lookfrom = wherever_you_are
    //cam.lookat = point3(278, 278, 0);
    cam.lookat = world.bounding_box().get_center() * vec3(1, 0, 1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;


    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    vec3 offset = (cam.lookat - cam.lookfrom) * 0.01;

    cam.initialize();
    //lights.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), empty_material));
    cam.colocate_light(world, lights, light);
    //world.add(make_shared<quad>(cam.lookfrom - offset, vec3(600, 0, 0), vec3(0, 600, 0), light));
    //lights.add(make_shared<quad>(cam.lookfrom - offset, vec3(-600, 0, 0), vec3(0, -600, 0), empty_material));
    cam.render(world, lights);
}

void house_SAR_space() {
    hittable_list world;


    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto white = make_shared<lambertian>(color(.73, .73, .73));

    world.add(make_shared<quad>(point3(-200, -1, -200), vec3(555, 0, 0), vec3(0, 0, 555), white)); // floor

    std::shared_ptr<hittable> house = load_model_from_file("./models/house.obj", white, 0.0);
    house = make_shared<scale>(house, vec3(200, 200, 200));
    house = make_shared<translate>(house, vec3(0, 0, 200));
    world.add(house);

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 5;
    cam.background = color(0, 0, 0);

    cam.vfov = 4;
    point3 center = world.bounding_box().get_center() * vec3(1, 0, 1); // View the center of the world at 0 elevation
    point3 near(center.x(), 500, -center.z() * 3);
    point3 far(center.x(), 2000, -center.z() * 30);

    cam.lookfrom = point3(center.x(), 8000, -800);

    cam.lookat = world.bounding_box().get_center() * vec3(1, 0, 1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;


    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    vec3 offset = (cam.lookat - cam.lookfrom) * 0.01;

    cam.initialize();
    //lights.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), empty_material));
    cam.colocate_light(world, lights, light);
    //world.add(make_shared<quad>(cam.lookfrom - offset, vec3(600, 0, 0), vec3(0, 600, 0), light));
    //lights.add(make_shared<quad>(cam.lookfrom - offset, vec3(-600, 0, 0), vec3(0, -600, 0), empty_material));
    cam.render(world, lights);
}

void rungholt_SAR_plane() {
    hittable_list world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto grey = make_shared<lambertian>(color(.5, .5, .5));

    
    // Choose from frequency bands {Visible, X-, C-, L-}
    double wavelength = SPECTRAL_MAP.find(X)->second;

    auto cube = load_model_from_file("./models/rungholt.obj", red, wavelength);
    cube = make_shared<scale>(cube, vec3(300, 300, 300));
    world.add(cube);
    
    point3 center = world.bounding_box().get_center() * vec3(1, 0, 1); // View the center of the world at 0 elevation
    point3 far(center.x(), 800, -800);

    camera cam;

    cam.aspect_ratio = 1.6;
    cam.image_width = 1600;
    cam.samples_per_pixel = 60;
    cam.max_depth = 10;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = far;
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    vec3 offset = (cam.lookat - cam.lookfrom) * 0.01;

    cam.initialize();
    cam.colocate_light(world, lights, light);
    cam.render(world, lights);
}

void rungholt_SAR_space() {
    hittable_list world;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto grey = make_shared<lambertian>(color(.5, .5, .5));


    // Choose from frequency bands {Visible, X-, C-, L-}
    double wavelength = SPECTRAL_MAP.find(X)->second;

    auto cube = load_model_from_file("./models/rungholt.obj", red, wavelength);
    cube = make_shared<scale>(cube, vec3(300, 300, 300));
    world.add(cube);

    point3 center = world.bounding_box().get_center() * vec3(1, 0, 1); // View the center of the world at 0 elevation
    point3 far(center.x(), 8000, -800);

    camera cam;

    cam.aspect_ratio = 1.6;
    cam.image_width = 1600;
    cam.samples_per_pixel = 60;
    cam.max_depth = 10;
    cam.background = color(0, 0, 0);

    cam.vfov = 4;
    cam.lookfrom = far;
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    vec3 offset = (cam.lookat - cam.lookfrom) * 0.01;

    cam.initialize();
    cam.colocate_light(world, lights, light);
    cam.render(world, lights);
}




int main() {
    switch (3) {
   
    case 1: cornell_box(); break;
    case 2: cornell_SAR(); break;
    case 3: eiffel_SAR(); break;
    case 4: house_ray(); break;
    case 5: house_SAR(); break;
    case 6: house_SAR_space(); break;
    case 7: rungholt_SAR_plane(); break;
    case 8: rungholt_SAR_space(); break;

    default: cornell_box(); break;
    }
}
