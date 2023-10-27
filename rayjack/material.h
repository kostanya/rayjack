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

    bool scatter(const Ray& rayIn, const HitRecord& rec, color& attenuation, Ray& scattered) const override {
        vec3 scatterDirection = rec.normal + randomUnitVector();

        // Eliminate zero vector case
        if (isNearZero(scatterDirection))
            scatterDirection = rec.normal;

        scattered = Ray(rec.p, scatterDirection);
        attenuation = m_albedo;
        return true;
    }

private:
    color m_albedo; // Fraction of light that a surface reflects. Different percentages for r,g,b values.
};

class Metal : public Material {
public:
    Metal(const color& albedo, float fuzz) : m_albedo(albedo), m_fuzz(fuzz < 1.0f ? fuzz : 1.0f) {}

    bool scatter(const Ray& rayIn, const HitRecord& rec, color& attenuation, Ray& scattered) const override {
        vec3 reflectDirection = reflect(glm::normalize(rayIn.direction()), rec.normal);
        scattered = Ray(rec.p, reflectDirection + m_fuzz * randomUnitVector());
        attenuation = m_albedo;
        return (glm::dot(scattered.direction(), rec.normal) > 0.0f);
    }

private:
    color m_albedo; // Fraction of light that a surface reflects. Different percentages for r,g,b values.
    float m_fuzz;   // Measure of fuzziness of the metal. Higher the fuzzier.
};
