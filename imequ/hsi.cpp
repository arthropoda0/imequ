#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


cv::Mat convertHSIToPPM(const cv::Mat& hsiImage) {
    cv::Mat ppmImage(hsiImage.size(), CV_8UC3);

    for (int i = 0; i < hsiImage.rows; ++i) {
        for (int j = 0; j < hsiImage.cols; ++j) {
            // Get pixel values in HSI color space
            cv::Vec3b hsiPixel = hsiImage.at<cv::Vec3b>(i, j);

            // Convert HSI to BGR
            float hue = hsiPixel[0] * CV_PI / 180.0;
            float saturation = hsiPixel[1] / 255.0;
            float intensity = hsiPixel[2] / 255.0;

            float b, g, r;

            if (hue >= 0 && hue < 2 * CV_PI / 3) {
                b = intensity * (1 - saturation);
                r = intensity * (1 + (saturation * cos(hue)) / cos(CV_PI / 3 - hue));
                g = 3 * intensity - (r + b);
            } else if (hue >= 2 * CV_PI / 3 && hue < 4 * CV_PI / 3) {
                hue -= 2 * CV_PI / 3;
                r = intensity * (1 - saturation);
                g = intensity * (1 + (saturation * cos(hue)) / cos(CV_PI / 3 - hue));
                b = 3 * intensity - (r + g);
            } else {
                hue -= 4 * CV_PI / 3;
                g = intensity * (1 - saturation);
                b = intensity * (1 + (saturation * cos(hue)) / cos(CV_PI / 3 - hue));
                r = 3 * intensity - (g + b);
            }

            // Convert BGR values to the range [0, 255]
            int bVal = static_cast<int>(b * 255);
            int gVal = static_cast<int>(g * 255);
            int rVal = static_cast<int>(r * 255);

            // Set pixel values in the PPM image
            ppmImage.at<cv::Vec3b>(i, j) = cv::Vec3b(bVal, gVal, rVal);
        }
    }

    return ppmImage;
}

int main()
{

  Mat src = imread("/Users/arthropoda/projects/c/homework/mandrill.ppm", 1);

  if(src.empty())
    cerr << "Error: Loading image" << endl;
  Mat hsi(src.rows, src.cols, src.type());

  float r, g, b, h, s, in;

  for(int i = 0; i < src.rows; i++)
    {
      for(int j = 0; j < src.cols; j++)
        {
          b = src.at<Vec3b>(i, j)[0];
          g = src.at<Vec3b>(i, j)[1];
          r = src.at<Vec3b>(i, j)[2];

          in = (b + g + r) / 3;

          int min_val = 0;
          min_val = std::min(r, std::min(b,g));

          s = 1 - 3*(min_val/(b + g + r));
          if(s < 0.00001)
            {
                  s = 0;
            }else if(s > 0.99999){
                  s = 1;
            }

          if(s != 0)
            {
              h = 0.5 * ((r - g) + (r - b)) / sqrt(((r - g)*(r - g)) + ((r - b)*(g - b)));
              h = acos(h);

              if(b <= g)
                {
                  h = h;
                } else{
                  h = ((360 * 3.14159265) / 180.0) - h;
                }
            }

          hsi.at<Vec3b>(i, j)[0] = (h * 180) / 3.14159265;
          hsi.at<Vec3b>(i, j)[1] = s*100;
          hsi.at<Vec3b>(i, j)[2] = in;
        }
    }
     
     // Split HSI image into individual channels
    std::vector<cv::Mat> channels;
    cv::split(hsi, channels);

    
    cv::equalizeHist(channels[2], channels[2]);

    // Merge the channels back into an HSI image
    cv::merge(channels, hsi);

    // Convert the HSI image back to BGR color space
   // cv::cvtColor(hsi, hsi, cv::COLOR_HSV2BGR);
    hsi = convertHSIToPPM(hsi);

    namedWindow("RGB image", WINDOW_NORMAL);
    namedWindow("HSI image", WINDOW_NORMAL);


    
    imshow("RGB image", src);
    imshow("HSI image", hsi);


    waitKey(0);

    imwrite("hsi.ppm",hsi);

    return 0;
  }