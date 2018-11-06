#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h> // For OpenGL typedefs

#include <cstdio>  // For file I/O
#include <cstring> // For memcmp() - a remnant from the C code


class Texture {

public:

    GLuint	width;	    // Image width
    GLuint	height;	    // Image height
    GLuint	textureID;  // Texture ID for OpenGL
    GLuint	type;	    // Image type (3 bytes per pixel: GL_RGB, 4 bytes: GL_RGBA)

    Texture();

    /* Constructor to load and intialize the texture all at once */
    Texture(const char *filename);

    /* Destructor */
    ~Texture();

    // The external entry point for loading a texture from a TGA file
    void createTexture(const char *filename); // Load GL texture from file

private:
    GLubyte	*imageData;	// Image data (3 or 4 bytes per pixel)
    GLuint	bpp;		// Image color depth in bits per pixel

    // Internal "private" funtions, called internally by createTexture()
    int loadUncompressedTGA(FILE *tgafile); // Load data from an uncompressed TGA file
    int loadTGA(const char *filename);		    // Open, check and load a TGA file

};

// Two convenient structs to read and interpret TGA file information
typedef struct
{
    GLubyte Header[12];	// TGA File Header
} TGAHeader;

typedef struct
{
    GLubyte		header[6];								// First 6 useful bytes from the header
    GLuint		bytesPerPixel;							// Holds number of bytes per pixel used in the TGA file
    GLuint		imageSize;								// Data size in bytes
    GLuint		type;
    GLuint		height;									//Height of Image
    GLuint		width;									//Width ofImage
    GLuint		bpp;									// Bits Per Pixel
} TGA;
