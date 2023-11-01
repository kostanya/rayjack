#include "material.h"

bool Lambertian::scatter(const Ray& rayIn, const HitRecord& rec, color& attenuation, Ray& scattered) const {
    vec3 scatterDirection = rec.normal + randomUnitVector();

    // Eliminate zero vector case
    if (isNearZero(scatterDirection))
        scatterDirection = rec.normal;

    scattered = Ray(rec.p, scatterDirection);
    attenuation = m_albedo;
    return true;
}

bool Metal::scatter(const Ray& rayIn, const HitRecord& rec, color& attenuation, Ray& scattered) const {
    vec3 reflectDirection = reflect(glm::normalize(rayIn.direction()), rec.normal);
    scattered = Ray(rec.p, reflectDirection + m_fuzz * randomUnitVector());
    attenuation = m_albedo;
    return (glm::dot(scattered.direction(), rec.normal) > 0.0f);
}

bool Dielectric::scatter(const Ray& rayIn, const HitRecord& rec, color& attenuation, Ray& scattered) const {
    // If the Ray hits the object with dielectric material from the front face,
    // as the Ray refracts from the air into the object,
    // refractionRatio = airRefractiveIndex / this.refractiveIndex
    // Else, as the Ray refracts from the object into the air,
    // refractionRatio = this.refractiveIndex / airRefractiveIndex
    const float airRefractiveIndex = 1.0f;
    float refractionRatio =
        rec.frontFace ? (airRefractiveIndex / m_refractiveIndex) : (m_refractiveIndex / airRefractiveIndex);

    vec3 rayInUnitDirection = glm::normalize(rayIn.direction());
    // float cosTheta = std::fmin(glm::dot(-rayInUnitDirection, rec.normal), 1.0f);
    float cosTheta = glm::dot(-rayInUnitDirection, rec.normal);
    float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

    bool cannotRefract = refractionRatio * sinTheta > 1.0f;
    vec3 direction;
    if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomFloat())
        direction = reflect(rayInUnitDirection, rec.normal);
    else
        direction = refract(rayInUnitDirection, rec.normal, refractionRatio);

    scattered = Ray(rec.p, direction);
    attenuation = color(1.0f, 1.0f, 1.0f);
    return true;
}

// In reality, reflectivity varies with angle. Schlick's approximation addresses this issue.
float Dielectric::reflectance(float cosine, float refIdx) {
    float r0 = (1.0f - refIdx) / (1.0f + refIdx);
    r0 *= r0;
    return r0 + (1.0f - r0) * static_cast<float>(std::pow((1.0f - cosine), 5));
}