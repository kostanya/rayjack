# rayjack
Our beloved raytracing engine is mainly based on Peter Shirley's [Fundamentals of Computer Graphics](https://www.amazon.com/Fundamentals-Computer-Graphics-Steve-Marschner/dp/1482229390) and his wonderful [tutorials.](https://raytracing.github.io/)
<br>

### What is Raytracing ?
Raytracing is simply a reverse simulation of the real-life image formation mechanism. In reality, we see and perceive our 3-dimensional world in a 2-dimensional way thanks to the rays coming to our eyes or camera from various ray sources. The rays we care about in this scenario are the rays that fall into our eye/camera among billions of rays. With raytracing, we can simulate this scenario in reverse by simply sending rays from our camera (instead of detecting which rays among all the possible rays will fall on our eye/camera). In other words, we simulate the journey of all the rays that will come back to our camera one by one and calculate their individual contributions (color contribution) to the final image. 

#### Features we have implemented so far:
- 
