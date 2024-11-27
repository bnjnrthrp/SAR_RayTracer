#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "pdf.h"
#include "texture.h"

class scatter_record {
public: 
    color attenuation;
    shared_ptr<pdf> pdf_ptr;
    bool skip_pdf;
    ray skip_pdf_ray;
};


class material {
public:
    virtual ~material() = default;

    virtual color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const {
        return color(0., 0., 0.);
    }

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, scatter_record& srec
    ) const {
        return false;
    }

    virtual double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const { return 0; }
};

class lambertian : public material {
public: 
    lambertian(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
    lambertian(shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(
        const ray& r_in, const hit_record& rec, scatter_record& srec
    ) const override {
        srec.attenuation = tex->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
        srec.skip_pdf = false;
        return true;
    }

    double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const override { 
        double cos_theta = dot(rec.normal, unit_vector(scattered.direction()));
        return cos_theta < 0 ? 0 : cos_theta / pi;
    }

private:
    shared_ptr<texture> tex;
};


class metal : public material {
public:
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(
        const ray& r_in, const hit_record& rec, scatter_record& srec
    ) const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    
        srec.attenuation = albedo;
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
        srec.skip_pdf_ray = ray(rec.p, reflected, r_in.time());

        return true;
    }

private:
    color albedo;
    double fuzz;
};

class glossy : public material {
public:
    // Fuzz texture interpreted as the magnitude of the fuzz texture.
    glossy(shared_ptr<texture>& a, shared_ptr<texture>& f) : albedo(a), fuzz(f) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, scatter_record& srec
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);

        srec.skip_pdf = true;
        double fuzz_factor = (fuzz->value(rec.u, rec.v, rec.p)).length();
        srec.skip_pdf_ray = ray(rec.p, reflected + fuzz_factor * random_on_unit_sphere(), r_in.time());

        srec.pdf_ptr = 0;
        return true;
    }
public:
    shared_ptr<texture> albedo, fuzz;
};

class dielectric : public material {
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(
        const ray& r_in, const hit_record& rec, scatter_record& srec
    ) const  override {
        srec.attenuation = color(1.0, 1.0, 1.0);
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);
        
        srec.skip_pdf_ray = ray(rec.p, direction, r_in.time());
        return true;
    }

    

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refraction_index;

    /*Schlick approximation of reflectance*/
    static double reflectance(double cosine, double refraction_index) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

class diffuse_light : public material {
public:
    diffuse_light(shared_ptr<texture> tex) : tex(tex) {}
    diffuse_light(const color& emit) : tex(make_shared<solid_color>(emit)) {}

    color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const override {
        if (!rec.front_face)
            return color(0., 0., 0.);
        return tex->value(u, v, p);
    }

private:
    shared_ptr<texture> tex;
};

class isotropic : public material {
public:
    isotropic(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}

    isotropic(shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(
        const ray& r_in, const hit_record& rec, scatter_record& srec
    ) const override {
        srec.attenuation = tex->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<sphere_pdf>();
        srec.skip_pdf = false;
        return true;
    }

    double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const override {
        return 1 / (4 * pi);
    }

private: 
    shared_ptr<texture> tex;
};


// See https://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
// The MTL format is based on the Phong shading model, so this uses a bit of reinterpretation
// See https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF , 
// and https://www.psychopy.org/api/visual/phongmaterial.html , and http://vr.cs.uiuc.edu/node198.html
// There are a few properties, which we allow to vary based on textures: 
// diffuse color: albedo for lambertian 
// specular color: albedo for metal
// emissive color: emissive :)
//
// sharpness map: remapped to fuzz := 1-log_10(sharpness)/4, sharpness clamped to [1, 10000]
//
class mtl_material : public material {
public:
    mtl_material(
        shared_ptr<texture> diffuse_a,
        shared_ptr<texture> specular_a,
        shared_ptr<texture> emissive_a,
        shared_ptr<texture> transparency_map,
        shared_ptr<texture> sharpness_map,
        int illum) :
        emissive_text(emissive_a),
        diffuse_text(diffuse_a),
        specular_text(specular_a),
        transparency_text(transparency_map),
        roughness_text(make_shared<roughness_from_sharpness_texture>(sharpness_map, 1, 10000))
    {
        diffuse_mat = make_shared<lambertian>(diffuse_text);
        specular_mat = make_shared<glossy>(specular_text, roughness_text);
        emissive_mat = make_shared<diffuse_light>(emissive_text);
    }

    virtual bool scatter(
        const ray& ray_in, const hit_record& rec, scatter_record& srec
    ) const override {
        double transp_prob = transparency_prob(rec.u, rec.v, rec.p);
        if (transp_prob > random_double()) {

            srec.attenuation = transparency_text->value(rec.u, rec.v, rec.p);
            srec.skip_pdf = true;
            // Continue in the same direction, starting from hitpoint
            srec.skip_pdf_ray = ray(rec.p, ray_in.direction(), ray_in.time());

            return false;
        }
        return choose_mat(rec.u, rec.v, rec.p)->scatter(ray_in, rec, srec);
    }

    virtual color emitted(
        const ray& r_in, const hit_record& rec, double u, double v, const point3& p
    ) const override {
        return emissive_mat->emitted(r_in, rec, u, v, p);
    }

    virtual double scattering_pdf(
        const ray& r_in, const hit_record& rec, const ray& scattered) const override {
        // We don't need to care about the transparent case, this only integrates over scattered rays (note specular are scatterd, but not diffuse)
        double diff_prob = diffuse_prob(rec.u, rec.v, rec.p);
        return diff_prob * (diffuse_mat->scattering_pdf(r_in, rec, scattered))
            + (1 - diff_prob) * specular_mat->scattering_pdf(r_in, rec, scattered);
    }
public:
    shared_ptr<texture> emissive_text, diffuse_text, specular_text, transparency_text, roughness_text;
private:
    shared_ptr<material> emissive_mat, diffuse_mat, specular_mat;
    inline double transparency_prob(double u, double v, const point3& p) const {
        double diff = diffuse_text->value(u, v, p).length();
        double spec = specular_text->value(u, v, p).length();
        double transp = transparency_text->value(u, v, p).length();
        return transp / (transp + diff + spec + 0.00001);
    }

    inline double diffuse_prob(double u, double v, const point3& p) const {
        double diff = diffuse_text->value(u, v, p).length();
        double spec = specular_text->value(u, v, p).length();
        return diff / (diff + spec + 0.00001);
    }

    inline shared_ptr<material> choose_mat(double u, double v, const point3& p) const {
        if (diffuse_prob(u, v, p) > random_double()) {
            return diffuse_mat;
        }
        else {
            return specular_mat;
        }
    }
};


#endif // MATERIAL_H