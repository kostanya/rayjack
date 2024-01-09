#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "hittable_list.h"
#include "sphere.h"


TEST_CASE("HittableList class tests", "[HittableList]") {
    auto testMaterial = std::make_shared<Lambertian>(color (1.0, 1.0, 1.0));
    HittableList hittableList;
    std::shared_ptr<Sphere> hittable1 = std::make_shared<Sphere>(point3(0, 0, 0), 2.0f, testMaterial);
    std::shared_ptr<Sphere> hittable2 = std::make_shared<Sphere>(point3(2, 0, 0), 2.0f, testMaterial);

    SECTION("Adding objects to HittableList") {

        REQUIRE(hittableList.objects.size() == 0);

        hittableList.add(hittable1);
        REQUIRE(hittableList.objects.size() == 1);

        hittableList.add(hittable2);
        REQUIRE(hittableList.objects.size() == 2);
    }

    SECTION("Clearing objects from HittableList") {

        hittableList.add(hittable1);
        hittableList.add(hittable2);
        REQUIRE(hittableList.objects.size() == 2);

        hittableList.clear();
        REQUIRE(hittableList.objects.size() == 0);
    }
}


TEST_CASE("HittableList hit method tests", "[HittableList::hit]") {
    Interval rayT(0.001, infinity);
    HitRecord rec;
    auto testMaterial = std::make_shared<Lambertian>(color (1.0, 1.0, 1.0));

    SECTION("When there are no objects in HittableList, hitResult should return false") {
        HittableList hittableList;
        Ray r({1, 2, 3}, {4, 5, 6});

        bool hitResult = hittableList.hit(r, rayT, rec);
        REQUIRE(hitResult == false);
    }

    SECTION("Ray hits one object in HittableList") {
        Ray r({0, 0, -5}, {0, 0, 1}); // towards the positive z-axis
        HittableList hittableList;
        std::shared_ptr<Sphere> hittable1 = std::make_shared<Sphere>(point3(0, 0, 0), 2.0f, testMaterial);
        std::shared_ptr<Sphere> hittable2 = std::make_shared<Sphere>(point3(5, 5, 5), 2.0f, testMaterial);

        hittableList.add(hittable1);
        hittableList.add(hittable2);

        bool hitResult = hittableList.hit(r, rayT, rec);
        REQUIRE(hitResult == true);
    }

    SECTION("Ray misses all objects in HittableList") {
        Ray r({0, 0, 0}, {0, 1, 0}); // The ray direction is towards the positive y-axis
        HittableList hittableList;
        std::shared_ptr<Sphere> hittable1 = std::make_shared<Sphere>(point3(0, 0, 5), 2.0f, testMaterial);
        std::shared_ptr<Sphere> hittable2 = std::make_shared<Sphere>(point3(0, 0, 10), 2.0f, testMaterial);

        hittableList.add(hittable1);
        hittableList.add(hittable2);

        bool hitResult = hittableList.hit(r, rayT, rec);
        REQUIRE(hitResult == false);
    }

    SECTION("Ray hits multiple objects in HittableList") {
        Ray r({0, 0, 0}, {0, 0, -1}); // towards the negative z-axis
        std::shared_ptr<Sphere> hittable1 = std::make_shared<Sphere>(point3(0, 0, -5), 2.0f, testMaterial);
        std::shared_ptr<Sphere> hittable2 = std::make_shared<Sphere>(point3(0, 0, -15), 2.0f, testMaterial);

        HittableList hittableList;
        hittableList.add(hittable1);
        hittableList.add(hittable2);

        bool hitResult = hittableList.hit(r, rayT, rec);
        REQUIRE(hitResult == true);
    }

     SECTION("Ray hits closest object first") {
        Ray r({0, 0, 0}, {0, 0, -1}); 
        std::shared_ptr<Sphere> hittable1 = std::make_shared<Sphere>(point3(0, 0, -5), 2.0f, testMaterial);
        std::shared_ptr<Sphere> hittable2 = std::make_shared<Sphere>(point3(0, 0, -15), 4.0f, testMaterial);

        HittableList hittableList;
        hittableList.add(hittable1);
        hittableList.add(hittable2);

        bool hitResult = hittableList.hit(r, rayT, rec);

        REQUIRE(hitResult == true);
    }

    SECTION("Ray hits object with negative t values") {
        Ray r({0, 0, 0}, {0, 0, -1});  
        std::shared_ptr<Sphere> hittable1 = std::make_shared<Sphere>(point3(0, 0, -5), 2.0f, testMaterial);
        HittableList hittableList(hittable1);

        bool hitResult = hittableList.hit(r, rayT, rec);

        REQUIRE(hitResult == true);
    }

    SECTION("Ray hits object with closestSoFar starting at max rayT") {
        Interval rayT(5.0f, 15.0f); // the sphere is located within this interval
        Ray r({0, 0, 0}, {0, 0, 1});  // the ray direction is towards the positive z-axis

        std::shared_ptr<Sphere> hittable1 = std::make_shared<Sphere>(point3(0, 0, 10), 2.0f, testMaterial);
        HittableList hittableList(hittable1);

        bool hitResult = hittableList.hit(r, rayT, rec);

        REQUIRE(hitResult == true);
    }

}