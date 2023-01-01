#ifndef OPENCVWRAPPER_H
#define OPENCVWRAPPER_H

#include <string>

class OpenCVWrapper
{
public:
    OpenCVWrapper();

    bool resizeImageBase64(std::string const &input,
                           std::string &output,
                           int targetWidth,
                           int targetHeight,
                           std::string &errorMessage);

    bool resizeImage(std::basic_string<std::byte> &input,
                     std::basic_string<std::byte> &output,
                     int targetWidth,
                     int targetHeight,
                     std::string &errorMessage);

    bool resizeImage(std::basic_string<std::byte> &inout,
                     int targetWidth,
                     int targetHeight,
                     std::string &errorMessage);
};

#endif // OPENCVWRAPPER_H
