Computer Graphics Assignment #1c
Wireframe Viewer, Cameras and others

1. A short video showing View Volume Changes through a Slider:

https://user-images.githubusercontent.com/100144150/209354398-9e6f0f12-f8de-4555-9e3a-3a069201b35e.mp4



2. I chose to implement the Camera's tranformation matrices to create the "View" matrix.
   So in the multiplicatoin order (viewPort * normalized Projection * View * WorldModel * LocalModel * MODEL VERTICES) the view matrix,
   contained the transformations of the camera, so at the end of calculating each transformations, I inverted the matrix and this is actually the matrix that I sent 
   to "Render".
   Also, Gui wise, I used (again) ImGui::Slider to enable the user to change the camera's location.


https://user-images.githubusercontent.com/100144150/209365368-fdf52128-43f9-47aa-8486-c60637b9ac18.mp4


3. In order to keep the same proportions after changing the window size, I updated the "ViewPort" Matrix according to the window size.
   So, inside of "RenderFrame", everytime the window size is changing I updated the Viewport matrix accordingly by sending the new width and height.
   in the sanity check I adjusted the view volume to contain alot more of "the world" in a way that disform the model radicly.
    Then I simply reduced the window, so that if the proportions were not preserved, the model (or part of it) would not be displayed on the screen.
    
 
