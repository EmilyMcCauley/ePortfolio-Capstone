# Software Design and Engineering Artifact Narrative

## Artifact Description
The artifact I selected for this milestone is a 3D modeling project initially created as part of a prior course in the Computer Science program. The project involved developing a basic 3D rendering engine using OpenGL, featuring key components such as `scenemanager.h`, `scenemanager.cpp`, `viewmanager.h`, `viewmanager.cpp`, `shapemeshes.cpp`, `shadermanager.cpp`, and `maincode.cpp`. While functional, the original project was incomplete, and I was dissatisfied with its final state. Reassessing the project provided an opportunity to enhance its functionality and design while refining my skills in C++ and OpenGL. My goal was to create a more polished and maintainable artifact for inclusion in my ePortfolio.

## Justification for Inclusion in the ePortfolio
I selected this artifact because it showcases my ability to tackle complex software development topics, such as 3D graphics, OpenGL, and advanced C++ programming. This project demonstrates my proficiency in software design and my capability to improve and optimize an existing system. Through this enhancement, I addressed performance, maintainability, and scalability, which are critical components of robust software systems.

Key improvements include:
- **Code Readability**: I added detailed comments across primary files, such as `scenemanager.cpp`, `viewmanager.cpp`, `shapemeshes.cpp`, and `maincode.cpp`, to make the codebase more maintainable and understandable for future developers.
- **Error Handling**: I implemented robust error handling in `scenemanager.cpp`, which addressed issues like invalid file paths and failed OpenGL bindings. Similar error checks were added to `maincode.cpp` and `viewmanager.cpp` to ensure a more stable and user-friendly experience.
- **Dynamic Data Structures**: I replaced the static array `m_textureIDs[16]` with a dynamic container, specifically `std::vector`. This change improved the flexibility of the program, allowing it to handle varying amounts of texture data without being constrained by a fixed array size.

These enhancements reflect my ability to make strategic design decisions that improve performance, scalability, and reliability, while demonstrating advanced software engineering practices.

## Reflection on the Enhancement Process
Enhancing this artifact was a valuable learning experience, presenting challenges that helped deepen my understanding of software design and engineering.

### Key Learnings:
- **Refactoring for Scalability**: Replacing static arrays with dynamic containers required a thorough understanding of the project’s structure and C++ memory management principles.
- **Error Handling**: Anticipating and handling OpenGL-related errors was particularly challenging due to the complexity of diagnosing and addressing these issues. This improvement reinforced the importance of robust error-checking mechanisms in software systems.
- **Code Maintainability**: Adding detailed comments and improving code readability taught me the importance of creating maintainable code for both myself and other developers who might work on the project in the future.

### Challenges:
- **Dynamic Memory Management**: Refactoring the codebase to use `std::vector` instead of static arrays introduced challenges in understanding how dynamic memory interacts with existing components.
- **Error Handling in OpenGL**: OpenGL bindings and file path validation were unpredictable and difficult to debug, but incorporating comprehensive error-checking mechanisms significantly improved the artifact's stability.

### Course Outcomes Met:
- **Software Design and Engineering**: I enhanced the project by addressing performance issues, improving maintainability, and implementing robust error-handling mechanisms.
- **Algorithmic Principles**: Refactoring the code to use dynamic memory management required the application of algorithmic principles to ensure efficient and stable performance.
- **System Robustness and Security**: While security was not a primary focus, the improvements in error handling indirectly contributed to system reliability and robustness by preventing crashes and unpredictable behavior.

## Conclusion
This project demonstrates my growing proficiency in software development and engineering. By improving the artifact’s code readability, error handling, and scalability, I addressed critical areas for improvement while reinforcing best practices in software design. The enhancement process provided valuable experience in refactoring code, managing dynamic memory, and implementing robust solutions to real-world problems.

As an addition to my ePortfolio, this artifact showcases my technical skills, problem-solving abilities, and growth as a software developer. It is a testament to my ability to analyze, enhance, and optimize software systems, making it an essential representation of my capabilities in the field of computer science.
