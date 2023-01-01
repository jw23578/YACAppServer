#include "opencvwrapper.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "utils/base64.h"

OpenCVWrapper::OpenCVWrapper()
{

}

bool OpenCVWrapper::resizeImageBase64(const std::string &input,
                                      std::string &output,
                                      int targetWidth,
                                      int targetHeight,
                                      std::string &errorMessage)
{
    std::vector<char> binInput;
    bin_base64_decode(input, binInput);
    cv::InputArray ia(binInput);
    cv::Mat original(cv::imdecode(ia, cv::IMREAD_UNCHANGED));
    int calcedWidth(original.cols * targetHeight / original.rows);
    int calcedHeight(original.rows * targetWidth / original.cols);
    if (calcedWidth > targetWidth)
    {
        targetHeight = calcedHeight;
    }
    if (calcedHeight > targetHeight)
    {
        targetWidth = calcedWidth;
    }
    cv::Mat resized;
    cv::resize(original, resized, cv::Size(targetWidth, targetHeight), cv::INTER_AREA );
    std::vector<unsigned char> result;
    std::vector<int> compression_params;
    cv::imencode(".jpg", resized, result, compression_params);
    output = base64_encode(result.data(), result.size());
    return true;
}

bool OpenCVWrapper::resizeImage(std::basic_string<std::byte> &input,
                                std::basic_string<std::byte> &output,
                                int targetWidth,
                                int targetHeight,
                                std::string &errorMessage)
{
    cv::Mat data(1, input.size(), CV_8UC1, input.data());
    cv::Mat original(cv::imdecode(data, cv::IMREAD_UNCHANGED));
    int calcedWidth(original.cols * targetHeight / original.rows);
    int calcedHeight(original.rows * targetWidth / original.cols);
    if (calcedWidth > targetWidth)
    {
        targetHeight = calcedHeight;
    }
    if (calcedHeight > targetHeight)
    {
        targetWidth = calcedWidth;
    }
    cv::Mat resized;
    cv::resize(original, resized, cv::Size(targetWidth, targetHeight), cv::INTER_AREA );
    std::vector<unsigned char> result;
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(75);
    cv::imencode(".jpg", resized, result, compression_params);
    output.append(reinterpret_cast<std::byte*>(result.data()), result.size());
    return true;
}

bool OpenCVWrapper::resizeImage(std::basic_string<std::byte> &inout, int targetWidth, int targetHeight, std::string &errorMessage)
{
    cv::Mat data(1, inout.size(), CV_8UC1, inout.data());
    data = cv::imdecode(data, cv::IMREAD_UNCHANGED);
    if (data.rows == targetHeight && data.cols == targetWidth)
    {
        return true;
    }
    int calcedWidth(data.cols * targetHeight / data.rows);
    int calcedHeight(data.rows * targetWidth / data.cols);
    if (calcedWidth > targetWidth)
    {
        targetHeight = calcedHeight;
    }
    if (calcedHeight > targetHeight)
    {
        targetWidth = calcedWidth;
    }
    cv::resize(data, data, cv::Size(targetWidth, targetHeight), cv::INTER_AREA );
    std::vector<unsigned char> result;
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(75);
    cv::imencode(".jpg", data, result, compression_params);
    inout.clear();
    inout.append(reinterpret_cast<std::byte*>(result.data()), result.size());
    return true;
}
