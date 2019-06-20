#include <opencv2/opencv.hpp>
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

class Tclass {
	Mat* imgSrc1;
	Mat* imgSrc2;
public:
	Tclass(int size, Mat* src1, Mat* src2=nullptr);
	~Tclass() {}
	int getSliderPos() { return slider_pos; }
	static void on_TrackBar(int slider_value, void* userData);
	Mat* getSrc1() { return imgSrc1; }
	Mat* getSrc2() { return imgSrc2; }
	int slider_pos;
	int slider_max;
};

Tclass::Tclass(int size, Mat* src1, Mat* src2) : slider_max(size), imgSrc1(src1), imgSrc2(src2) {

}

void Tclass::on_TrackBar(int slider_value, void* userData)
{
	Tclass * pTclass = reinterpret_cast<Tclass*>(userData);
	float alpha = static_cast<float>(slider_value) / pTclass->slider_max;
	float beta = (1.0 - alpha);
	Mat dst;
	addWeighted(*pTclass->getSrc1(), alpha, *pTclass->getSrc2(), beta, 0.0, dst);
	imshow("Linear Blend", dst);
}

int main(int argc, char** argv)
{
	// Read the image file
	Mat image = imread("C:/yliu/opencv/images/eagle.jpg");
	Mat grey = imread("C:/yliu/opencv/images/eagle.jpg", IMREAD_GRAYSCALE);
	cout << grey.channels() << endl;

	Mat dst;
	equalizeHist(grey, dst);

	int cols = image.cols;
	int rows = image.rows;

	if (image.empty()) // Check for failure
	{
		cout << "Could not open or find the image" << endl;
		system("pause"); //wait for any key press
		return -1;
	}

	Mat hsv, v;
	cvtColor(image, hsv, COLOR_BGR2HSV);

	string trackBarName = "Alpha control";

	Tclass* pTAlpha = new Tclass(100, &grey, &dst);

	int alpha_slider = 0;
	int alpha_slider_max = 100;

	//createTrackbar(trackBarName, "Linear Blend", &alpha_slider, alpha_slider_max, Tclass::on_TrackBar, pTAlpha);

	float alpha = 0.9f;
	float beta = (1.0 - alpha);
	addWeighted(grey, alpha, dst, beta, 0.0, v);

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			Vec3b & color = hsv.at<Vec3b>(Point(x, y));
			uchar & g = v.at<uchar>(Point(x, y));
			color[2] = g;
		}
	}

	cvtColor(hsv, hsv, COLOR_HSV2BGR);

//	Mat image_blurred_with_3x3_kernel;
//	GaussianBlur(image, image_blurred_with_3x3_kernel, Size(3, 3), 0);
//	Mat image_blurred_with_adaptive;
//	bilateralFilter(image, image_blurred_with_adaptive, 5, 25, 25);

	//Erode the image with 5x5 kernel
//	Mat image_eroded_with_5x5_kernel;
//	erode(image, image_eroded_with_5x5_kernel, getStructuringElement(MORPH_RECT, Size(5, 5)));

	String windowName = "My HelloWorld Window"; //Name of the window
//	String windowNameGauss = "My Gaussian Window"; //Name of the window
//	String windowNameAdaptiveGauss = "My Adaptive Gaussian Window"; //Name of the window
//
//	String erode = "Erode Window"; //Name of the window
	String greyW = "Grey Window"; //Name of the window
	String histW = "Hist Window"; //Name of the window

	String blendW = "Linear Blend";

	namedWindow(windowName); // Create a window
//	namedWindow(windowNameGauss); // Create a window
//	namedWindow(windowNameAdaptiveGauss); // Create a window
//
//	namedWindow(erode); // Create a window

	namedWindow(greyW);

	namedWindow(histW);

	namedWindow(blendW);

	createTrackbar(trackBarName, "Linear Blend", &alpha_slider, alpha_slider_max, Tclass::on_TrackBar, pTAlpha);

	imshow(windowName, image); // Show our image inside the created window.
//	imshow(windowNameGauss, image_blurred_with_3x3_kernel); // Show our image inside the created window.
//	imshow(windowNameAdaptiveGauss, image_blurred_with_adaptive); // Show our image inside the created window.
//
//	imshow(erode, image_eroded_with_5x5_kernel);

	imshow(greyW, grey);
	imshow(histW, hsv);


	waitKey(0); // Wait for any keystroke in the window

//	destroyWindow(windowName); //destroy the created window
//	destroyWindow(windowNameGauss);
//	destroyWindow(windowNameAdaptiveGauss);
	destroyAllWindows();
	delete pTAlpha;
	return 0;
}