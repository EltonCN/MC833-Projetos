#include<string.h>

#include "common_def.h"

/// @brief Get a fragment of a image
/// @param nPackages The number of packages/fragments to break the image into
/// @param imageSize The size of the full image
/// @param maxSizePerPackage The maximum size for a package
/// @param image The image
/// @param mail The user mail
/// @param packageIndex The index of the fragment to get
/// @return The image fragment
ImageFrag* getImageFrag(int nPackages, int imageSize, int maxSizePerPackage, char* image, char* mail, int packageIndex)
{
    int size = maxSizePerPackage;

    if(packageIndex >= nPackages-1)
    {
        int totalSize = (nPackages-1) * maxSizePerPackage;

        size = imageSize-totalSize;
    }

    int start = packageIndex*maxSizePerPackage;

    ImageFrag* frag = malloc(sizeof(ImageFrag)+(size));
    
    frag->size = size;
    frag->nPackages = nPackages;
    frag->maxSizePerPackage = maxSizePerPackage;
    frag->packageIndex = packageIndex;
    frag->imageSize = imageSize;

    strcpy(frag->mail, mail);
    memcpy(frag->imageFrag, &image[start], size);

    return frag;
}

/// @brief Restoure the image from fragments
/// @param frags Vector of fragments pointers
/// @return Reconstructed image
RegistryImage* restoreImage(ImageFrag** frags)
{
    RegistryImage* image = malloc(sizeof(RegistryImage)+(frags[0]->imageSize));

    image->imageSize = frags[0]->imageSize;
    strcpy(image->mail, frags[0]->mail);

    for(int i = 0; i<frags[0]->nPackages; i++)
    {
        int start = frags[i]->packageIndex*frags[0]->maxSizePerPackage;
        memcpy(&(image->image[start]), frags[i]->imageFrag, frags[i]->size);
    }

    return image;
}