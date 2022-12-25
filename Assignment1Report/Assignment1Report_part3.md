Computer Graphics Assignment #1c
Wireframe Viewer, Cameras and others

1. A short video showing View Volume Changes through a Slider:

https://user-images.githubusercontent.com/100144150/209354398-9e6f0f12-f8de-4555-9e3a-3a069201b35e.mp4



2. I chose to implement the Camera's tranformation matrices to create the "View" matrix.
   So in the multiplicatoin order (viewPort * normalized Projection * View * WorldModel * LocalModel * MODEL VERTICES) the view matrix,
   contained the transformations of the camera, so at the end of calculating the transformations, I inverted the matrix and this is actually the matrix that I sent 
   to "Render".
   Also, Gui wise, I used (again) ImGui::Slider to enable the user to change the camera's location.


https://user-images.githubusercontent.com/100144150/209365368-fdf52128-43f9-47aa-8486-c60637b9ac18.mp4


3. In order to keep the same proportions after changing the window size, I updated the "ViewPort" Matrix according to the window size.
   So, inside of "RenderFrame", everytime the window size is changing I updated the Viewport matrix accordingly by sending the new width and height.
   in the sanity check I adjusted the view volume to contain alot more of "the world" in a way that disform the model radicly.
   Then I simply reduced the window, so that if the proportions were not preserved, the model (or part of it) would not be displayed on the screen.
   
   
4.    
https://user-images.githubusercontent.com/100144150/209460876-351d7515-1fd0-4fe5-b875-4e26234c76b0.mp4

5. I chose T1 as translation and T2 as Rotation. So, first I applied T2 in the world frame and then T1 in the model frame. (T1*T2)
the rotation transformation T2 was applied to the entire scene, including the camera and the model. then I also added T1 which shifted the position of the camera in the scene. so the effect was a change in the way that the camera views the scene. it appeared like the camera was looking from a different position.
then I did the opposite, and applied T1 in the world frame and T2 in the model frame.
the translation was applied to the entire scene, including the camera and the model, while the rotation applied to the camera, so it changed the orientation of the camera in the scene. so, again I got a change in the way the camera views the scene.
In general, The camera Transformation is just like the model transformation, only inverse, to mimic the effect of camera view.

6.

https://user-images.githubusercontent.com/100144150/209471644-5302fc0a-862e-40dd-9dd5-8d38cecead0a.mp4

7.


https://user-images.githubusercontent.com/100144150/209471691-0ee7d3cf-49aa-43de-a3e8-550e7e3e5b82.mp4

8. orthogonal Vs perspective.
   By setting the camera at the same position, the difference bwtween the two projection is clear.
   The orthogonal projection on all its characteristics is clearly visible as well as the perspective projection which presents us with a realistic approach with a
    correct view in terms of the distance of the object on its many parts


https://user-images.githubusercontent.com/100144150/209471754-8ea795a5-3266-4a74-bcd1-fe3f5368e59d.mp4


