#pragma once

#include "hit_record.h"
#include "ray.h"
#include "utils.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& rayIn, const HitRecord& rec, color& attenuation,
                         Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const color& albedo) : m_albedo(albedo) {}

    bool scatter(const Ray& rayIn, const HitRecord& rec, color& attenuation, Ray& scattered) const override;

private:
    color m_albedo; // Fraction of light that a surface reflects. Different percentages for r,g,b values.
};

class Metal : public Material {
public:
    Metal(const color& albedo, float fuzz) : m_albedo(albedo), m_fuzz(fuzz < 1.0f ? fuzz : 1.0f) {}

    bool scatter(const Ray& rayIn, const HitRecord& rec, color& attenuation, Ray& scattered) const override;

private:
    color m_albedo; // Fraction of light that a surface reflects. Different percentages for r,g,b values.
    float m_fuzz;   // Measure of fuzziness of the metal. Higher the fuzzier.
};

// Dielectric: insulating (yalitkan) material or a very poor conductor of electric current.
// When a light ray hits them, it splits into a reflected ray and a refracted (transmitted) ray.
// We’ll handle that by randomly choosing between reflection and refraction, only generating one scattered ray
// per interaction.
class Dielectric : public Material {
public:
    Dielectric(float refractiveIndex) : m_refractiveIndex(refractiveIndex) {}

    bool scatter(const Ray& rayIn, const HitRecord& rec, color& attenuation, Ray& scattered) const override;

private:
    float m_refractiveIndex; // Refractive index

    // In reality, reflectivity varies with angle. Schlick's approximation addresses this issue.
    static float reflectance(float cosine, float refIdx);
};
