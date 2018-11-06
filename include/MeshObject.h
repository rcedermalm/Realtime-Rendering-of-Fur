//
// Created by rebec on 2018-10-31.
//

#ifndef OPENGL_TEMPLATE_MESHOBJECT_H
#define OPENGL_TEMPLATE_MESHOBJECT_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>   // To use OpenGL datatypes

#include <cstdio>  // For C-style file input in readOBJ()
#include <cmath>   // For sin() and cos() in soupCreateSphere()
#include <cstring> // For strcmp() - a leftover from the C version

// Some <cmath> headers define M_PI, some don't. Make sure we have it.
#ifndef M_PI
#define M_PI (3.14159265359)
#endif // M_PI

/* A struct to hold geometry data and send it off for rendering */
class MeshObject {

public:

    /* Constructor: initialize a triangleSoup object to all zeros */
    MeshObject();

    /* Destructor: clean up allocated data in a triangleSoup object */
    ~MeshObject();

    /* Clean up allocated data in a triangleSoup object */
    void clean();

    /* Create a very simple demo mesh with a single triangle */
    void createTriangle();

    /* Create a simple box geometry */
    void createBox(float xsize, float ysize, float zsize);

    /* Create a sphere (approximated by polygon segments) */
    void createSphere(float radius, int segments);

    /* Load geometry from an OBJ file */
    void readOBJ(const char* filename);

    /* Print data from a triangleSoup object, for debugging purposes */
    void print();

    /* Print information about a triangleSoup object (stats and extents) */
    void printInfo();

    /* Render the geometry in a triangleSoup object */
    void render();

private:
    // All data members are private. They are accessed only by methods in the class.
    GLuint vao;          // Vertex array object, the main handle for geometry
    int nverts; // Number of vertices in the vertex array
    int ntris;  // Number of triangles in the index array (may be zero)
    GLuint vertexbuffer; // Buffer ID to bind to GL_ARRAY_BUFFER
    GLuint indexbuffer;  // Buffer ID to bind to GL_ELEMENT_ARRAY_BUFFER
    GLfloat *vertexarray; // Vertex array on interleaved format: x y z nx ny nz s t
    GLuint *indexarray;   // Element index array

    void printError(const char *errtype, const char *errmsg);

};


#endif //OPENGL_TEMPLATE_MESHOBJECT_H
