#include "Texture.h"

Texture::Texture() {
    width = 0;
    height = 0;
    textureID = 0;
    type = 0;
    imageData = NULL;
    bpp = 0;
}

/* Constructor to load and intialize the texture all at once */
Texture::Texture(const char *filename) {
    createTexture(filename);
}

Texture::~Texture() { }


/*
 * loadUncompressedTGA(FILE *TGAfile)
 * Open and test the file to make sure it is a valid TGA file
 */
int Texture::loadUncompressedTGA(FILE *TGAfile) // Load an uncompressed TGA
{												// (based on NeHe's TGA loading code)
    GLubyte temp;
    GLuint cswap;
    TGA tga;			// TGA image data

    if(fread(tga.header, sizeof(tga.header), 1, TGAfile) == 0)		// Read TGA header
    {
        fprintf(stderr, "Could not read info header.\n");		// Display error
        if(TGAfile != NULL)										// if file is still open
        {
            fclose(TGAfile);									// Close it
        }
        return GL_FALSE;										// Exit with failure
    }

    this->width  = tga.header[1] * 256 + tga.header[0];		// Determine the TGA Width	(highbyte*256+lowbyte)
    this->height = tga.header[3] * 256 + tga.header[2];		// Determine the TGA Height	(highbyte*256+lowbyte)
    this->bpp	= tga.header[4];							        // Determine the bits per pixel
    tga.width		= this->width;							// Copy width into local structure
    tga.height		= this->height;							// Copy height into local structure
    tga.bpp			= this->bpp;								    // Copy BPP into local structure

    if((this->width <= 0) || (this->height <= 0)
       || ((this->bpp != 24) && (this->bpp !=32)))		// Make sure all information is valid
    {
        fprintf(stderr, "Invalid texture information.\n");		// Display Error
        if(TGAfile != NULL)										// Check if file is still open
        {
            fclose(TGAfile);										// If so, close it
        }
        return GL_FALSE;										// Return "failure"
    }

    if(bpp == 24)										// If the the image is 24 BPP
    {
        this->type	= GL_RGB;								// Set image type to GL_RGB
        printf("Texture type is GL_RGB\n");
    }
    else														// Else it's 32 BPP
    {
        this->type	= GL_RGBA;								// Set image type to GL_RGBA
        printf("Texture type is GL_RGBA\n");
    }

    tga.bytesPerPixel	= (tga.bpp / 8);						// Compute the number of BYTES per pixel
    tga.imageSize		= (tga.bytesPerPixel * tga.width * tga.height);	// Compute the total amount of memory needed
    this->imageData = new GLubyte[tga.imageSize];				        // Allocate that much memory

    if(this->imageData == NULL)										// If no space was allocated
    {
        fprintf(stderr, "Could not allocate memory for image.\n");		// Display Error
        fclose(TGAfile);													// Close the file
        return GL_FALSE;												// Return "failure"
    }

    if(fread(this->imageData, 1, tga.imageSize, TGAfile) != tga.imageSize)	// Attempt to read image data
    {
        fprintf(stderr, "Could not read image data.\n");					// Display error
        if(this->imageData != NULL)										// If image data was allocated
        {
            delete[] this->imageData;										// Deallocate that data
        }
        fclose(TGAfile);														// Close file
        return GL_FALSE;													// Return "failure"
    }

    // stegu 2013-11-19: Stupid, slow and outdated in-place XOR byte swapping code removed. Ugh.
    // Swap the BGR(A) byte order in the TGA file to the standard RGB(A) byte order for OpenGL.
    for(cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
    {
        temp = this->imageData[cswap];
        this->imageData[cswap] = this->imageData[cswap+2];
        this->imageData[cswap+2] = temp;
    }

    fclose(TGAfile);			// Close file
    return GL_TRUE;			// Return success
}

/*
 * loadTGA(char * filename)
 * Open and test the file to make sure it is a valid TGA file
 */
int Texture::loadTGA(const char *filename)
{
    FILE * TGAfile;
    TGAHeader tgaheader;

    GLubyte uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0}; // Uncompressed TGA Header
    GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0}; // RLE Compressed TGA Header

    TGAfile = fopen(filename, "rb");

    if(TGAfile == NULL) // If the file didn't open...
    {
        fprintf(stderr, "Could not open texture file.\n");	// Display an error message
        return GL_FALSE;									// Exit function with "failure"
    }

    if(fread(&tgaheader, sizeof(TGAHeader), 1, TGAfile) == 0) // Attempt to read 12 byte file header
    {
        fprintf(stderr, "Could not read file header.\n");	// If it fails, display an error message
        if(TGAfile != NULL)									// Check to see if file is still open
        {
            fclose(TGAfile);									// If it is, close it
        }
        return GL_FALSE;									// Exit with failure
    }

    if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)	// See if header matches the predefined header of
    {															// an Uncompressed TGA image
        this->loadUncompressedTGA(TGAfile);		                // If so, jump to Uncompressed TGA loading code
    }
    else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0) // See if header matches the predefined header of
    {																 // an RLE compressed TGA image
        fprintf(stderr, "RLE compressed TGA files are not supported.\n");
        fclose(TGAfile);
        return GL_FALSE;											// Exit with failure
    }
    else															// If header matches neither type
    {
        fprintf(stderr, "Unsupported image file format.\n");		// Unknown file type, or unknown TGA version
        fclose(TGAfile);
        return GL_FALSE;											// Exit with failure
    }
    return GL_TRUE;													// All is well, return "success"
}

/*
 * Load and activate a 2D texture from a TGA file
 */
void Texture::createTexture(const char *filename) {

    this->loadTGA(filename); // Private method, reads this->imageData from TGA file

    glEnable(GL_TEXTURE_2D); // Required for glBuildMipmap() to work (!)
    glGenTextures(1, &(this->textureID));     // Create The texture ID
    glBindTexture ( GL_TEXTURE_2D , this->textureID );
    // Set parameters to determine how the texture is resized
    glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
    // Set parameters to determine how the texture wraps at edges
    glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
    glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
    // Read the texture data from file and upload it to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0,
                 this->type, GL_UNSIGNED_BYTE, this->imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] this->imageData; // Image data was copied to the GPU, so we can delete it
}
