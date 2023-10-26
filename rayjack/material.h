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
private:
    color m_albedo;

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
};

class Metal : public Material {
private:
    color m_albedo;

public:
    Metal(const color& albedo) : m_albedo(albedo) {}

    bool scatter(const Ray& rayIn, const HitRecord& rec, color& attenuation, Ray& scattered) const override {
        vec3 reflected = reflect(glm::normalize(rayIn.direction()), rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = m_albedo;
        return true;
    }
};
