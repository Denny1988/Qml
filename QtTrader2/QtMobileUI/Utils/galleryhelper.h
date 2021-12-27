#ifndef UTILS_GALLERYHELPER_H
#define UTILS_GALLERYHELPER_H
#include <QString>

class GalleryHelper
{
public:
    GalleryHelper();
    static QString getGalleryImagePath();
    static bool saveImageToGalleryByPath(const QString& imgPath);
};

#endif // UTILS_GALLERYHELPER_H
