Computer Graphics 203.3710 Assignment #1
Wireframe Viewer
Simple Mesh Display

1.
![1](https://user-images.githubusercontent.com/100144150/205489131-863b1c5e-bed4-4aa0-bccf-8d4dde1327a2.JPG)


2.
   Scale and translate - At first I simply duplicated each vector in An integer to scale the sizes, but after sometime and a bit of searching online I found there are
   useful tools in the glm library, such as scale and translate matrices, so Ive included <glm/gtx/transform.hpp> in my MeshModel header and started using them.
   so, for scaling I used the the scale function which transforms a matrix with a scale 4 * 4 matrix created from a vector of 3 components, and for translation I used
   the translate function that Builds a translation 4 * 4 matrix created from 3 scalars.
   
3.
![3](https://user-images.githubusercontent.com/100144150/205493823-c00265d1-6e59-4303-a759-5cb1c6cb3412.JPG)

4. the multiplication order:                      
   world Scale Matrix * World Translate Matrix * World Rotation Matrix *  local Scale Matrix * local Translate Matrix * local Rotation Matrix
 
 ![6](https://user-images.githubusercontent.com/100144150/205494149-84cc4c53-341d-47e7-a314-83322287953c.JPG)

 
 
 
                            

