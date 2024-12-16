# Informal Code Review: ePortfolio Artifacts

## Introduction

For my ePortfolio, I am presenting two key artifacts that showcase my growth as a Computer Science student: a **3D Modeling Project** and a **MongoDB Database Project**. The first artifact demonstrates my understanding of **Software Design** and **Algorithms**, while the second artifact highlights my skills in **Databases**. In this code review, I will provide an analysis of the current functionality, areas for improvement, and planned enhancements for both artifacts, aligning with the course outcomes.

### Artifacts:
1. **3D Modeling Project** (Software Design and Algorithms)
2. **MongoDB Database Project** (Databases)

## 1. 3D Modeling Project (Software Design and Algorithms)

### Existing Functionality

The **3D Modeling Project** involves developing a basic 3D rendering engine using **OpenGL** and **C++**. The project includes key files such as `scenemanager.h`, `scenemanager.cpp`, `viewmanager.h`, `viewmanager.cpp`, `shapemeshes.cpp`, `shadermanager.cpp`, and `maincode.cpp`. The existing functionality includes:

1. **Scene Management**: The scene manager initializes 3D objects, textures, and shaders, and handles the creation and destruction of objects in the scene.
   
2. **3D Rendering**: OpenGL is used for rendering objects like meshes, applying shaders and textures to 3D models.
   
3. **Camera Movement**: The `viewmanager.cpp` handles basic camera controls via keyboard and mouse. Movement is functional but limited and lacks smooth transitions.

4. **Shader Management**: The `shadermanager.cpp` file manages shader loading, compiling, and binding for rendering objects.

5. **Main Program Logic**: The `maincode.cpp` handles OpenGL initialization, window setup, and the main rendering loop.

### Code Analysis

The **current codebase** has several areas for improvement:

- **Camera Controls**: The existing camera system uses basic controls and can be **jerky and unresponsive**. Additionally, there is no support for smooth transitions or dynamic speed adjustments.
  
  **Improvement Plan**: I will enhance the camera controls to make them **smoother** and **more responsive**. This will include **mouse-driven camera movement** and the ability to dynamically adjust the camera speed based on user input. Additionally, I plan to introduce **projection switching** between perspective and orthographic views to provide more flexibility in viewing 3D scenes.

- **Efficiency**: The project uses **static arrays** (e.g., `m_textureIDs[16]`), which limits scalability and flexibility.

  **Improvement Plan**: I will refactor the code to use **dynamic containers** like `std::vector`, allowing the program to handle varying amounts of texture data without the limitations of a fixed array.

- **Error Handling**: The project lacks sufficient error handling, particularly for issues related to file loading, OpenGL initialization, and resource management.

  **Improvement Plan**: I will implement **robust error handling** throughout the codebase to ensure that failures in texture loading, shader compilation, and OpenGL bindings do not crash the program.

- **Documentation**: The code lacks comprehensive **comments** and **documentation**, making it difficult for others to understand or maintain.

  **Improvement Plan**: I will add detailed comments throughout the key files, particularly the scene manager, view manager, and shader manager, to improve readability and maintainability.

### Enhancement Plan

The planned enhancements will focus on:

- Improving **camera controls** and **movement** algorithms (Algorithms and Data Structures).
- Refactoring the scene management and texture handling code to replace static arrays with dynamic containers (Data Structures).
- Implementing better **error handling** and **logging** (Software Design).
- Adding **comments** and improving **documentation** (Software Design).

These enhancements will demonstrate my ability to improve **software design**, apply **algorithmic principles** to solve real-world problems, and increase the **efficiency and functionality** of a complex system.

---

## 2. MongoDB Database Project (Databases)

### Existing Functionality

The **MongoDB Database Project** is a database system for tracking student grades, courses, and scores. It uses **MongoDB** for data storage and **JavaScript** for querying the database. The existing functionality includes:

1. **Data Modeling**: The database schema defines collections for students, courses, and scores, with relationships between them using MongoDB’s document structure.

2. **Querying**: Users can query the database to retrieve student grades, calculate averages, and list course enrollments.

3. **Basic CRUD Operations**: The project includes basic **Create**, **Read**, **Update**, and **Delete** operations to manipulate student records, courses, and scores.

### Code Analysis

The current **MongoDB project** has several areas that need improvement:

- **Handling Missing Data**: The database does not handle **missing values** (e.g., NULL scores) efficiently, which can lead to incomplete or incorrect query results.

  **Improvement Plan**: I plan to implement a **COALESCE function** or similar logic to handle NULL or missing data by providing fallback values or calculating averages based on available data.

- **Query Efficiency**: Some of the queries are not optimized for performance, especially when dealing with large datasets.

  **Improvement Plan**: I will focus on **query optimization** by utilizing **indexes** and restructuring queries to reduce the time complexity. This will make the database more scalable for larger datasets.

- **Data Integrity**: There is limited validation of data before it is entered into the database, which could lead to inconsistencies.

  **Improvement Plan**: I will add **validation checks** to ensure that the data being inserted into the database adheres to the expected format (e.g., valid scores, course IDs).

- **Security and Access Control**: The project currently lacks **authentication** and **authorization** mechanisms for users accessing the database.

  **Improvement Plan**: I will implement **role-based access control** (RBAC) and other security measures to ensure that sensitive data is protected and access is restricted to authorized users.

- **Documentation**: As with the 3D project, there is a lack of **comments** and **documentation**, making the code difficult to understand and maintain.

  **Improvement Plan**: I will add **comments** and improve the **README** file to explain the structure and use of the database, including instructions for setup and querying.

### Enhancement Plan

The enhancements for the **MongoDB project** will focus on:

- Improving **data handling** and **query optimization** (Databases).
- Adding **data validation** and **error handling** (Software Design).
- Implementing **security** measures such as authentication and access control (Software Design).
- Adding detailed **comments** and improving **documentation** (Software Design).

These changes will address key areas in **database management**, **query optimization**, **data integrity**, and **security**, and will showcase my ability to design and optimize database systems that are scalable, reliable, and secure.

---

## Conclusion

In this code review, I have presented a detailed analysis of two key artifacts in my ePortfolio: the **3D Modeling Project** and the **MongoDB Database Project**. For each artifact, I have identified the existing functionality, pinpointed areas for improvement, and outlined a detailed plan for enhancements. These enhancements will align with the course outcomes across **Software Design**, **Algorithms and Data Structures**, and **Databases**, demonstrating my ability to improve and optimize complex systems while adhering to best practices in software development.

By making these improvements, I will not only enhance the functionality and usability of each project but also showcase my growth and proficiency in **database management**, **algorithmic problem solving**, and **software design**.

