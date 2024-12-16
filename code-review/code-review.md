# Informal Code Review: Software Design and Engineering Artifact

## Introduction

The artifact I have chosen for this review is a 3D modeling project that I initially created as part of a prior course in the Computer Science program. This project involves developing a basic 3D rendering engine using C++ and OpenGL. Although the project was functional, it lacked several important enhancements that could improve both its functionality and design. Revisiting this project provided me with an opportunity to enhance the code and demonstrate my skills in software design, with a focus on improving the structure, readability, and performance of the existing implementation.

## Existing Functionality

The current project consists of several files including `scenemanager.h`, `scenemanager.cpp`, `viewmanager.h`, `viewmanager.cpp`, `shapemeshes.cpp`, `shadermanager.cpp`, and `maincode.cpp`. These components work together to form a basic 3D rendering engine. The core functionality includes:

1. **Rendering 3D Objects**: The program uses **OpenGL shaders and meshes** to render 3D objects in a scene.
2. **Scene Management**: The `scenemanager.cpp` file handles the initialization of objects, textures, and shaders.
3. **Camera Controls**: The `viewmanager.cpp` file handles basic camera movement, allowing users to navigate the 3D space via keyboard and mouse input.
4. **Shader Management**: The `shadermanager.cpp` file loads and compiles shaders necessary for rendering.
5. **Object Management**: The `shapemeshes.cpp` class defines the geometries and textures of 3D objects.

While the system functions, it is still limited by several aspects that can be improved, such as the lack of error handling, scalability issues due to fixed-size arrays, and underdeveloped user interactions.

## Code Analysis

### Areas for Improvement

The existing codebase demonstrates foundational concepts, but there are several areas that could be improved to make the system more robust, maintainable, and scalable:

1. **Structure and Modularity**:
   - The current codebase has some tightly coupled components, making it harder to maintain and extend. For instance, the `scenemanager.cpp` file handles both scene setup and object management. Separating these responsibilities could improve code clarity and modularity.
   - There is also an opportunity to improve the separation of concerns by creating more reusable functions and classes to handle specific tasks (e.g., error handling, resource loading).

2. **Efficiency**:
   - The project uses fixed-size arrays like `m_textureIDs[16]`, which limit the scalability of the application. This could be replaced with dynamic containers like `std::vector` to allow for more flexibility in managing resources such as textures.
   - Shader and texture loading are inefficient and could be optimized to reduce overhead.

3. **Error Handling**:
   - The current code does not handle errors effectively. For example, invalid file paths or failed OpenGL bindings are not properly managed, leading to potential crashes.
   - Adding robust error handling, such as exception handling or proper return value checks, would increase the stability of the application.

4. **Functionality and Usability**:
   - The camera controls are basic and can be enhanced to allow for smoother, more intuitive movement. Currently, the camera controls can feel abrupt, and there is no way to adjust the camera's speed dynamically based on user input.
   - The system lacks features such as toggling between different projection views (e.g., perspective vs. orthographic), which could be added to improve the user experience.

5. **Commenting and Documentation**:
   - The code lacks sufficient comments and documentation. While some areas have basic inline comments, many sections lack descriptions of what the functions and classes do. Adding detailed comments throughout the code will improve its readability and maintainability, making it easier for other developers (and future myself) to understand the code.

## Enhancement Plans

Based on the analysis of the existing code, the following enhancements are planned to improve the overall functionality, performance, and maintainability of the project:

### 1. **Refactor Code for Modularity and Maintainability**:
   - **Break down large classes and functions**: For example, separating scene management and object handling into different classes will make the codebase more modular and easier to maintain.
   - **Refactor camera controls**: Improve the `viewmanager.cpp` class to include smoother camera movement with dynamic speed adjustments based on user input.

### 2. **Replace Fixed-size Arrays with Dynamic Containers**:
   - Replace the `m_textureIDs[16]` array with `std::vector` to handle dynamic amounts of texture data more effectively. This will make the project more scalable and flexible as the number of textures grows.

### 3. **Implement Robust Error Handling**:
   - Add proper **error checks** in critical sections, such as when loading files (shaders, textures) or initializing OpenGL contexts. This will prevent the program from crashing when these operations fail and provide better user feedback.

### 4. **Enhance Camera Functionality**:
   - Implement **smooth mouse-driven camera movement** and allow for dynamic adjustments of camera speed based on user input (e.g., keyboard events for increasing/decreasing camera speed).
   - Add **projection switching** between perspective and orthographic views to provide the user with more flexibility when viewing the 3D scene.

### 5. **Improve Documentation and Commenting**:
   - Increase the **amount of inline comments** and provide more **descriptive function headers** to explain the purpose and functionality of key components. Additionally, update the `README.md` file to describe the project setup and how to use the application.

## Alignment with Course Outcomes

The planned enhancements align well with several course outcomes, particularly in the areas of software design, algorithmic problem-solving, and usability:

1. **Software Design**: Refactoring the code and implementing modular components demonstrates my ability to design maintainable and scalable systems.
2. **Algorithmic Problem-Solving**: By improving the camera controls and projection switching, I will apply algorithmic thinking to solve real-world user interaction problems.
3. **Performance Optimization**: Replacing fixed-size arrays with `std::vector` and optimizing resource loading will improve the performance and scalability of the system.
4. **Testing**: The improvements will be validated through extensive testing, ensuring that new features (like dynamic camera speed and error handling) work seamlessly.
5. **Documentation**: Improving comments and documentation throughout the project aligns with the course's emphasis on writing clear, maintainable code.

## Conclusion

In this code review, I have identified several areas for improvement in my 3D modeling project, including code structure, error handling, performance, and functionality. The planned enhancements, such as refactoring for modularity, replacing fixed-size arrays with dynamic containers, and improving camera controls, will address these issues while demonstrating my ability to apply best practices in software design and development. I look forward to implementing these changes and continuing to refine my skills in C++ and OpenGL.
