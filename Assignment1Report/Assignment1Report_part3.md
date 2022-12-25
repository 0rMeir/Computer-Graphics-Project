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
   in the sanity check I adjusted the view volume to contain alot less from "the world" in a way that change the model proportions radicly.
   Then I simply reduced the window, so that if the proportions were not preserved, the model (or part of it) would not be displayed on the screen.
   
   

https://user-images.githubusercontent.com/100144150/209473504-129b35d8-0241-4f95-8604-31d956229070.mp4


   
   
4.    


https://user-images.githubusercontent.com/100144150/209473088-7c8f66bc-31ef-4bbe-8ba2-e060ac00f79b.mp4



5. As you can see in the video, the activation of the first transformation illustrates a rotation of the camera around the model, with the model remaining more or less in the same place in the frame (but in each frame, a different part of it will be visible, as happens in such a situation).
But, when we activate the second transformation, we will get a situation where the camera actually rotates in place, so it can be clearly seen that the model radically changes its position in the frame, just as it would happen in such a situation.
In general, since activating the transformations on the camera is done by activating transformations on the model but in the opposite way (by inverting the matrix) we will get exactly the desired result.


https://user-images.githubusercontent.com/100144150/209473962-a77b834c-ff60-4cbb-b5da-c34063c3afb9.mp4




6.

https://user-images.githubusercontent.com/100144150/209471644-5302fc0a-862e-40dd-9dd5-8d38cecead0a.mp4

7.


https://user-images.githubusercontent.com/100144150/209471691-0ee7d3cf-49aa-43de-a3e8-550e7e3e5b82.mp4

8. orthogonal Vs perspective.
   By setting the camera at the same position, the difference bwtween the two projection is clear.
   The orthogonal projection on all its characteristics is clearly visible as well as the perspective projection which presents us with a realistic approach with a
    correct view in terms of the distance of the object on its many parts


https://user-images.githubusercontent.com/100144150/209471754-8ea795a5-3266-4a74-bcd1-fe3f5368e59d.mp4

13.![gui](https://user-images.githubusercontent.com/100144150/209474384-7b0a74a1-6412-4269-9de8-d2586d1bee17.JPG)

14. I havent implemented any additional features except what was asked in the assiignment.

