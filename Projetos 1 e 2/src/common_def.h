#ifndef COMMON_DEF
#define COMMON_DEF

#define MAX_IMAGE_SIZE_PER_PACKAGE 500.0
#define MAX_REGISTRY_PER_PACKAGE 10

/** @brief Request operation types.
* 
*   REGISTER: Creates new registry. Doesn't returns data.
*   LIST_BY_COURSE: List all registries filtering by course. Returns data.
*   LIST_BY_SKILL: List all registries filtering by skill. Returns data. 
*   LIST_BY_YEAR: List all registries filtering by year. Returns data.
*   LIST_ALL: List all registries. Returns data.
*   GET_BY_MAIL: List all registries filtering by mail. Returns data.
*   REMOVE_BY_MAIL: Remove a registry by mail. Doesn't return data.
*   SEND_IMAGE: Send a registry image. Doesn't return data.
*   GET_IMAGE_BY_MAIL:  registry image by mail. Return data. Not implemented with TCP. 
*
*/
enum OperationType
{
    REGISTER,
    LIST_BY_COURSE,
    LIST_BY_SKILL, 
    LIST_BY_YEAR,
    LIST_ALL,
    GET_BY_MAIL,
    REMOVE_BY_MAIL,
    SEND_IMAGE,
    GET_IMAGE_BY_MAIL
};

/// @brief Stores a profile registry
struct Registry
{
    char mail[25];
    char name[25];
    char surname[50];
    char city[25];
    char course[50];
    int graduationYear;
    char skills[100];
};

/// @brief Stores a registry image
struct RegistryImage
{
    int imageSize;
    char mail[25];
    char image[];
};

typedef enum OperationType OperationType;
typedef struct Registry Registry;
typedef struct RegistryImage RegistryImage;

struct ImageFrag
{
    int size;
    int maxSizePerPackage;
    int nPackages;
    int packageIndex;
    int imageSize;
    char mail[25];
    char imageFrag[];
};

typedef struct ImageFrag ImageFrag;

union ImageOrFrag
{
    RegistryImage image;
    ImageFrag frag;
};

typedef union ImageOrFrag ImageOrFrag; 

struct FragList
{
    int size;
    int code;
    ImageFrag** frags;
};

typedef struct FragList FragList; 

struct Image
{
    int isFragIfOne;
    ImageOrFrag image;
};

typedef struct Image Image;

ImageFrag* getImageFrag(int nPackages, int imageSize, int maxSizePerPackage, char* image, char* mail, int packageIndex);
RegistryImage* restoreImage(ImageFrag** frags);


#endif