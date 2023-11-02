# rayjack
Our beloved raytracing engine is mainly based on Peter Shirley's [Fundamentals of Computer Graphics](https://www.amazon.com/Fundamentals-Computer-Graphics-Steve-Marschner/dp/1482229390) and his wonderful [tutorials.](https://raytracing.github.io/)
<br> <br>
This project basically consists of three parts: **rayjack**, **application** and **external**. rayjack is the source code of the raytracing library, application is a simple app where we create sample scenes using rayjack and external is the third party libraries that we use in rayjack.

### What is Raytracing ?
Raytracing is simply a reverse simulation of the real-life image formation mechanism. In reality, we see and perceive our 3-dimensional world in a 2-dimensional way thanks to the rays coming to our eyes or camera from various ray sources. The rays we care about in this scenario are the rays that fall into our eye/camera among billions of rays. With raytracing, we can simulate this scenario in reverse by simply sending rays from our camera (instead of detecting which rays among all the possible rays will fall on our eye/camera). In other words, we simulate the journey of all the rays that will come back to our camera one by one and calculate their individual contributions (color contribution) to the final image. 

### Features we have implemented so far:
- Sphere object & ray intersection
- Antialiasing
- Ray diffusion
- Reflection and refraction
- Lambertian Material
- Metal Material
- Dielectric Material
- Positionable and rotatable camera
- Depth of field (defocus blur)

### How to build ?
We wrote the project as a cross-platfrom cross-compiler project using C++17 and you can easily build it using CMake after cloning the project to your locale. Just remember to clone it with `--recurse-submodules` command because we manage external libraries (i.e glm) with submodules. After building and compiling the project, you will have an executable called `rayjack-app` and a static library called `rayjack-lib`. Actually `rayjack-app` is just a simple application that uses `rayjack-lib`. You can run `rayjack-app` to see the sample scene and render different scenes by playing with the source code (application.cpp). Or you can link `rayjack-lib` directly to your own project and use it. 

