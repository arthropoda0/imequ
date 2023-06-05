#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <iostream>


double calculatePSNR(const char* filename1, const char* filename2,int channel) {
    // Load the original and generated images
    cv::Mat original_image = cv::imread(filename1, cv::IMREAD_COLOR);
    cv::Mat generated_image = cv::imread(filename2, cv::IMREAD_COLOR);

    if (original_image.empty() || generated_image.empty()) {
        printf("Error: Failed to read the images\n");
        return -1.0;
    }

    // Split the original and generated images into BGR channels
    std::vector<cv::Mat> original_channels, generated_channels;
    cv::split(original_image, original_channels);
    cv::split(generated_image, generated_channels);

    // Calculate the mean squared error (MSE) for the red channel
    cv::Mat red_diff = original_channels[channel] - generated_channels[channel]; //2 for Red, 1 for GREEN, 0 for RED
    double mse = cv::mean(red_diff.mul(red_diff))[0];

    // Calculate the maximum pixel value
    double maxPixelValue = 255.0;

    // Calculate the PSNR
    double psnr;
    if (mse == 0) {
        psnr = INFINITY;
    } else {
        psnr = 20.0 * log10(maxPixelValue / sqrt(mse));
    }

    return psnr;
}




int main() {
    const char* filename1 = "/Users/arthropoda/projects/c/homework/mandrill.ppm";
    const char* filename2 = "/Users/arthropoda/projects/c/homework/build/ycbcr.ppm";

    double psnr_red = calculatePSNR(filename1, filename2, 2); // Red channel
    printf("red : %.5f\n", psnr_red);
    double psnr_green = calculatePSNR(filename1, filename2, 1); // Green channel
    printf("green : %.5f\n", psnr_green);
    double psnr_blue = calculatePSNR(filename1, filename2, 0); // Blue channel
    printf("blue : %.5f\n", psnr_blue);

    double avg = (psnr_red + psnr_green+ psnr_blue) / 3;
    printf("avg : %.5f\n", avg);

    return 0;
}
