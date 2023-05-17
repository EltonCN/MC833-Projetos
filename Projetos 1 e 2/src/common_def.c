#include<string.h>

#include "common_def.h"

char* getImageFrag(int nPackages, int imageSize, int maxSizePerPackage, char* image, char* mail, int packageIndex)
{
    int size = maxSizePerPackage;

    if(packageIndex >= nPackages-1)
    {
        int totalSize = (nPackages-1) * maxSizePerPackage;

        size = imageSize-totalSize;
    }

    int start = (packageIndex-1)*maxSizePerPackage;

    ImageFrag* frag = malloc(sizeof(ImageFrag)+(size*sizeof(char)));

    frag->size = size;
    frag->nPackages = nPackages;
    frag->maxSizePerPackage = maxSizePerPackage;
    frag->packageIndex = packageIndex;
    frag->imageSize = imageSize;

    strcpy(frag->mail, mail);
    memcpy(frag->imageFrag, image[start], size);

    return frag;
}

RegistryImage* restoreImage(ImageFrag** frags)
{
    RegistryImage* image = malloc(sizeof(RegistryImage)+(frags[0]->imageSize*sizeof(char)));

    image->imageSize = frags[0]->imageSize;
    strcpy(image->mail, frags[0]->mail);

    for(int i = 0; i<frags[0]->nPackages; i++)
    {
        int start = frags[i]->packageIndex*frags[0]->maxSizePerPackage;
        
        int size = frags[0]->maxSizePerPackage;
        if(frags[i]->packageIndex >= frags[0]->nPackages-1)
        {
            int totalSize = (frags[0]->nPackages-1) * frags[0]->maxSizePerPackage;

            size = frags[0]->imageSize-totalSize;
        }

        memcpy(&(image->image[start]), frags[i]->imageFrag, size);
    }

    return image;
}