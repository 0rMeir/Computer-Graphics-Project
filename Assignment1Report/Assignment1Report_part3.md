Computer Graphics Assignment #1c
Wireframe Viewer, Cameras and others

1. A short video showing View Volume Changes through a Slider:

https://user-images.githubusercontent.com/100144150/209354398-9e6f0f12-f8de-4555-9e3a-3a069201b35e.mp4


2. I chose to implement the Camera's tranformation matrices to create the "View" matrix.
   So in the multiplicatoin order (viewPort * normalized Projection * View * WorldModel * LocalModel * MODEL VERTICES) the view matrix,
   contained the transformations of the camera, so at the end of calculating the transformations, I inverted the matrix and this is actually the matrix that I sent 
   to "Render".
   Also, Gui wise, I used (again) ImGui::Slider to enable the user to change the camera's location.
