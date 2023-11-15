#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "hittable_list.cpp"
#include "sphere.h"

#include <memory>
#include <vector>

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

/*
TEST_CASE("HittableList hit method tests", "[HittableList::hit]") {
    Ray r({1, 2, 3}, {4, 5, 6});
    Interval rayT(0.001, infinity);
    HitRecord rec;

    SECTION("When there are no objects in HittableList, hitResult should return false") {
        HittableList hittableList;

        bool hitResult = hittableList.hit(r, rayT, rec);
        REQUIRE(hitResult == false);
    }

    SECTION("Ray hits one object in HittableList") {
        std::shared_ptr<MockHittable> hittableObject = std::make_shared<MockHittable>();
        HittableList hittableList(hittableObject);

        bool hitResult = hittableList.hit(r, rayT, rec);
        REQUIRE(hitResult == true);
    }

    SECTION("Ray misses all objects in HittableList") {
        HittableList hittableList;
        std::shared_ptr<MockHittable> hittableObject1 = std::make_shared<MockHittable>();
        std::shared_ptr<MockHittable> hittableObject2 = std::make_shared<MockHittable>();

        hittableList.add(hittableObject1);
        hittableList.add(hittableObject2);

        Ray r({0, 0, -10.0}, {0, 0, -12});
        Interval rayT(2000.0, 2001.0);
        bool hitResult = hittableList.hit(r, rayT, rec);
        REQUIRE(hitResult == false);
    }

    SECTION("Ray hits multiple objects in HittableList") {
        std::shared_ptr<MockHittable> hittableObject1 = std::make_shared<MockHittable>();
        std::shared_ptr<MockHittable> hittableObject2 = std::make_shared<MockHittable>();

        // Set up the scenario where the ray hits both objects
        // Modify the ray, rayT, or positions of objects to ensure they intersect
        // ...

        HittableList hittableList;
        hittableList.add(hittableObject1);
        hittableList.add(hittableObject2);

        bool hitResult = hittableList.hit(r, rayT, rec);
        REQUIRE(hitResult == true);
    }

     SECTION("Ray hits closest object first") {
        std::shared_ptr<MockHittable> hittableObject1 = std::make_shared<MockHittable>();
        std::shared_ptr<MockHittable> hittableObject2 = std::make_shared<MockHittable>();

        // Adjust ray parameters to ensure hittableObject1 is closer to the ray's origin than hittableObject2
        // ...

        HittableList hittableList;
        hittableList.add(hittableObject1);
        hittableList.add(hittableObject2);

        bool hitResult = hittableList.hit(r, rayT, rec);

        // Assuming the ray hits hittableObject1 first
        REQUIRE(hitResult == true);
        // Add assertions to ensure that hittableObject1 was the closest hit
        // ...
    }

    SECTION("Ray hits object with negative t values") {
        std::shared_ptr<MockHittable> hittableObject = std::make_shared<MockHittable>();

        // Set up the scenario where the ray hits the object but the t values are negative
        // ...

        HittableList hittableList(hittableObject);

        bool hitResult = hittableList.hit(r, rayT, rec);

        // Assuming the ray hits the object but the t values are negative
        REQUIRE(hitResult == true);
        // Add assertions related to the negative t values if required
    }

    SECTION("Ray hits object with closestSoFar starting at max rayT") {
        std::shared_ptr<MockHittable> hittableObject = std::make_shared<MockHittable>();

        // Set up the scenario where the ray hits the object, altering closestSoFar from its initial value
        // ...

        HittableList hittableList(hittableObject);

        bool hitResult = hittableList.hit(r, rayT, rec);

        // Assuming the ray hits the object and alters closestSoFar
        REQUIRE(hitResult == true);
        // Add assertions related to the altered closestSoFar value if required
    }

}*/