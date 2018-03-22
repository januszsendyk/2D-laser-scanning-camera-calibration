#include "opencv2/opencv.hpp"
#include <iostream>


using namespace cv;
using namespace std;

int main(int, char**)
{
	Mat temp;
	Mat img[20];
	string nazwa;
	vector<vector<Point2f>> wek;
	vector<Point3f> dobra;
	vector<vector<Point3f>> dobra_wek;

	namedWindow("image");
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened()) // check if we succeeded
		return -1;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 6; j++)
		{
			dobra.push_back(Point3f(j, i, 0));
		}
	while (1)
	{
		cap >> temp;
		imshow("image", temp);
		if (waitKey(30) >= 0) break;
	}
	//cout << dobra;
	for (int i = 0; i < 20; )
	{
		
		
		//cap >> img[i];
		temp.copyTo(img[i]);

		Size patternsize(6, 8);
		vector<Point2f> corners;

		bool patternfound = findChessboardCorners(img[i], patternsize, corners,
			CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
			+ CALIB_CB_FAST_CHECK);

		if (patternfound)
		{
			Mat subpixel_BW;
			cvtColor(img[i], subpixel_BW, CV_BGR2GRAY);
			cornerSubPix(subpixel_BW, corners, Size(11, 11), Size(-1, -1),
				TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			//if (i == 0)
				//cout << corners;
			wek.push_back(corners);
			dobra_wek.push_back(dobra);

			namedWindow("Chessboard");
			drawChessboardCorners(img[i], patternsize, Mat(corners), patternfound);
			imshow("Chessboard", img[i]);
			i++;
		}

		while (1)
		{
			cap >> temp;
			imshow("image", temp);
			if (waitKey(30) >= 0) break;
		}

	}


	Mat cameraMatrix, distCoeffs;
	vector <Mat>rv, tv;
	cout << dobra_wek.size() << endl;
	cout << wek.size() << endl;
	calibrateCamera(dobra_wek, wek,img[0].size(), cameraMatrix, distCoeffs,rv,tv);


	FileStorage file("cam_param.yml", cv::FileStorage::WRITE);
	//Mat cameraMatrix = (Mat_<double>(3, 3) << 1000, 0, 320, 0, 1000, 240, 0, 0, 1);
	//Mat distCoeffs = (Mat_<double>(5, 1) << 0.1, 0.01, -0.001, 0, 0);
	file << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;

	file.release();


	/*FileStorage fs2("cam_param.yml", FileStorage::READ);

	Mat cameraMatrix2, distCoeffs2;
	fs2["cameraMatrix"] >> cameraMatrix2;
	fs2["distCoeffs"] >> distCoeffs2;

	fs2.release();


	Mat tmp;
	cap >> temp;
	undistort(temp, tmp, cameraMatrix2, distCoeffs2);*/

	Mat tmp;
	undistort(img[0], tmp, cameraMatrix, distCoeffs);
	for (;;)
	{
		imshow("First Img undistorted", tmp);

		if (waitKey(1) >= 0) break;
		
	}
	return 0;
}
