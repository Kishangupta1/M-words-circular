#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <aruco.hpp>
#include <fstream>
#include <string>
#include <cmath>

using namespace cv;

const int part = 2;//part of circle to paint mkr
const int numDivision = 6;
const int nCols = 3; // Number of cols of markers
const int nRows = numDivision; // Number of rows of markers
const int prod = nRows*nCols;
int nHalfPixelsPerMarker = 50;
//const int nPixelsPerMarker = 2*nHalfPixelsPerMarker; // #pixels per marker side
int nPixelsPerMarker = 100;
int nbufferPixels = 50; // #buffer pixels

const double PI = 4.*(std::atan(1.))/part;
const double Rin = 800;  
double d = static_cast<double>(nPixelsPerMarker+nbufferPixels);
//const double Rout = (Rin + nCols*d + 100.);
double Rout = (Rin + nCols*d + (nPixelsPerMarker/6));
double incrad = 1.;

int main()
{
  // TODO make marker size a multiple of the number of bits+border
  
  // Size of the image
  // const int Ny = Rin + nCols*d* + (nPixelsPerMarker/6);//
  const int Ny = 2.2*Rout;// nCols*nPixelsPerMarker+(nCols+1)*nbufferPixels;
  const int Nx = 2.2*Rout;// nRows*nPixelsPerMarker+(nRows+1)*nbufferPixels;

  // Output image
  cv::Mat_<uchar> img(Nx,Ny,static_cast<uchar>(255));

  // Create a dictionary
  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_1000);

  
  //std::cout<<"\nMaxR:"<<Rout;	
  // Create markers and paint the image
  for(int i=0; i<nRows; ++i)
    for(int j=0; j<nCols; ++j)
      {
	//int incrad=0;// = 10;
	//if(j==2) incrad = 50;
	double r = Rin + j*d*incrad + nHalfPixelsPerMarker + (nPixelsPerMarker*2/6);
	incrad = r/Rin;
	
	//	std::cout<<"\nincrad: "<<incrad<<" "<<"Rad: "<<r;
	//std::cout<<"\nRout: "<<Rout<<" "<<"HalfPixel :"<<nHalfPixelsPerMarker;
	
	double incTheta = 2.*PI/(static_cast<double>(numDivision));
	double Theta = i*incTheta;
	double center[2];
	center[0] = r*(std::cos(Theta)) + Rout;
	center[1] = r*(std::sin(Theta))+ Rout;
	
	/*	std::cout << "\n\nFor row number: " << (i+1) << " & col number: " << (j+1) ;
	std::cout << "\nthe value of center[0] is :" << center[0] <<" "<<center[0]-Rout;
	std::cout << "\nthe value of center[1] is :" << center[1] <<" "<<center[1]-Rout ;
	*/
	
	// Generate the marker (i,j)
	int mnum = nCols*i+j;
	//	std::cout<<mnum;
	cv::Mat marker;
	dictionary->drawMarker(mnum, nPixelsPerMarker*incrad, marker, 1);
	int kstart = static_cast<int>(center[0]-(nHalfPixelsPerMarker*incrad));
	int Lstart = static_cast<int>(center[1]-(nHalfPixelsPerMarker*incrad));
	
	// Paint the image with the marker
	for(int k=kstart; k<kstart+nPixelsPerMarker*incrad; ++k)
	  for(int L=Lstart; L<Lstart+nPixelsPerMarker*incrad; ++L)
	  img.at<uchar>(k,L) = marker.at<uchar>(k-kstart, L-Lstart);
      }

  //cv::imwrite("circle.jpg", img);
  std::cout << "\n\nTotal number of marker is "<< prod << "\n\n"<<std::flush;

  //make inner and outer circles
  cv::circle(img,cv::Point(Nx/2,Ny/2), Rin, cv::Scalar(0),1, 8, 0);
  cv::circle(img,cv::Point(Nx/2,Ny/2), Rout, cv::Scalar(0),1, 8, 0);
  cv::imwrite("circle.jpg", img);

}

