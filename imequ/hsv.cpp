//Uncomment the following line if you are compiling this code in Visual Studio
//#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // Read the image file
    Mat image = imread("/Users/arthropoda/projects/c/homework/mandrill.ppm");

    // Check for failure
    if (image.empty())
    {
        cout << "Could not open or find the image" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

    //Convert the image from BGR to hsv color space
    Mat hist_equalized_image;
    cvtColor(image, hist_equalized_image, COLOR_BGR2HSV);
    

    vector<Mat> vec_channels;
    split(hist_equalized_image, vec_channels); 

    //Equalize the histogram of only the v channel 
    equalizeHist(vec_channels[2], vec_channels[2]);

    //Merge 3 channels in the vector to form the color image in hsv color space.
    merge(vec_channels, hist_equalized_image); 
   
        
    //Convert the histogram equalized image from hsv to BGR color space again
    cvtColor(hist_equalized_image, hist_equalized_image, COLOR_HSV2BGR);
    imwrite("hsv.ppm",hist_equalized_image);


    //Define the names of windows
    String windowNameOfOriginalImage = "Original Image"; 
    String windowNameOfHistogramEqualized = "Histogram Equalized Color Image";

    // Create windows with the above names
    namedWindow(windowNameOfOriginalImage, WINDOW_NORMAL);
    namedWindow(windowNameOfHistogramEqualized, WINDOW_NORMAL);

    // Show images inside the created windows.
    imshow(windowNameOfOriginalImage, image);
    imshow(windowNameOfHistogramEqualized, hist_equalized_image);

    waitKey(0); // Wait for any keystroke in any one of the windows

    destroyAllWindows(); //Destroy all opened windows

    return 0;
}