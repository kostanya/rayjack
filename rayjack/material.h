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

// Dielectric: insulating (yalitkan) material or a very poor conductor of electric current.
// When a light ray hits them, it splits into a reflected ray and a refracted (transmitted) ray.
// We’ll handle that by randomly choosing between reflection and refraction, only generating one scattered ray
// per interaction.
class Dielectric : public Material {
public:
    Dielectric(float refractiveIndex) : m_refractiveIndex(refractiveIndex) {}

    bool scatter(const Ray& rayIn, const HitRecord& rec, color& attenuation, Ray& scattered) const override {
        // If the Ray hits the object with dielectric material from the front face,
        // as the Ray refracts from the air into the object,
        // refractionRatio = airRefractiveIndex / this.refractiveIndex
        // Else, as the Ray refracts from the object into the air,
        // refractionRatio = this.refractiveIndex / airRefractiveIndex
        const float airRefractiveIndex = 1.0f;
        float refractionRatio = rec.frontFace ? (airRefractiveIndex / m_refractiveIndex)
                                              : (m_refractiveIndex / airRefractiveIndex);

        vec3 rayInUnitDirection = glm::normalize(rayIn.direction());
        // float cosTheta = std::fmin(glm::dot(-rayInUnitDirection, rec.normal), 1.0f);
        float cosTheta = glm::dot(-rayInUnitDirection, rec.normal);
        float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

        bool cannotRefract = refractionRatio * sinTheta > 1.0f;
        vec3 direction;
        if (cannotRefract)
            direction = reflect(rayInUnitDirection, rec.normal);
        else
            direction = refract(rayInUnitDirection, rec.normal, refractionRatio);

        scattered = Ray(rec.p, direction);
        attenuation = color(1.0f, 1.0f, 1.0f);
        return true;
    }

private:
    float m_refractiveIndex; // Refractive index
};
