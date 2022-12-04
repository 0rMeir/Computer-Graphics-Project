Computer Graphics 203.3710 Assignment #1
Wireframe Viewer
Simple Mesh Display

1.
![1](https://user-images.githubusercontent.com/100144150/205489131-863b1c5e-bed4-4aa0-bccf-8d4dde1327a2.JPG)


2.
   Scale and translate - At first I simply duplicated each vector in An integer to scale the sizes, but after sometime and a bit of searching online I found there are
   useful tools in the glm library, such as scale and translate matrices, so Ive included <glm/gtx/transform.hpp> in my MeshModel header and started using them.
   so, for scaling I used the the scale function which transforms a matrix with a scale 4 * 4 matrix created from a vector of 3 components, and for translation I used
   the translate functoin that Builds a translation 4 * 4 matrix created from 3 scalars. 

