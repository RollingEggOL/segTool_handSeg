// segTool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Readvideo.h"
#include <string>
#include <atlstr.h>
#include <opencv2\opencv.hpp>
#include "windows.h"
#include <math.h>
#include <direct.h>
using namespace std;
using namespace cv;

int _tmain()
{
	BOOL videoFindFlag;
	CFileFind videoFileFind;
	const bool b_recordVide = false;  //Record the color, depth, skeleton or not.
	CString label[18]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r'};
	
	for (int l=0; l<18; l++)
	{
		CString folderName;
		folderName.Format("..\\output\\%02d", l);
		mkdir(folderName);

		CString fileFolderName;
		fileFolderName.Format("E:\\handShapeData\\%s\\*.oni", label[l]);
		videoFindFlag = TRUE;
		videoFindFlag = videoFileFind.FindFile(fileFolderName);
		int nSample = 0;
		while(videoFindFlag)
		{ 
			// Find the sign video
			videoFindFlag = videoFileFind.FindNextFile();
			CString videoFileName = videoFileFind.GetFilePath();
			Readvideo myReadVideo;
			CString personID = videoFileName.Right(24).Left(2);

			// Initial the video recording
			VideoWriter writer_depth;
			VideoWriter writer_skeleton; 
			if (b_recordVide)
			{
				CString addName = videoFileName.Right(25).Left(10);
				string depthName = "..\\data\\" + addName + "_depth.avi";
				string skeletonName = "..\\data\\" + addName + "_skeleton.avi";
				CString colorName;
				colorName.Format("..\\data\\%s_color.avi", addName);
				writer_depth.open(depthName, CV_FOURCC('D', 'I', 'V', 'X'), 25.0, Size(r_width, r_heigth));
				writer_skeleton.open(skeletonName, CV_FOURCC('D', 'I', 'V', 'X'), 25.0, Size(r_width, r_heigth));
				CString colorOriName;
				colorOriName.Format("%s\\color.avi",videoFileName);
				CopyFile(colorOriName,colorName,TRUE);
			}

			// Read the sign video
			string  s   =   (LPCTSTR)videoFileName;
			myReadVideo.readvideo(s);
			int frameSize = myReadVideo.vColorData.size();
			cvNamedWindow("color_skeleton",1);
			cout<<"The total frame is: "<<frameSize<<endl;

			// Process each frame
			for (int i=0; i<frameSize; i++)
			{
				// File name of images to be saved. 
				CString fileName;
				fileName.Format("..\\output\\%02d\\c%02d_p%s_%05d_color.jpg", l, l, personID, nSample);
				CString fileName_depth;
				fileName_depth.Format("..\\output\\%02d\\c%02d_p%s_%05d_depth.jpg", l, l, personID, nSample);
				nSample++;

				// color and depth images to be saved. 
				IplImage* showImage;
				IplImage* showImage_depth;
				// Make color frame as the showImage
				showImage = myReadVideo.vColorData[i];
				// Make depth frame as the showImage
				Mat depthMat = myReadVideo.retrieveGrayDepth(myReadVideo.vDepthData[i]);
				IplImage* depthImage = &(IplImage)depthMat;
				showImage_depth = cvCreateImage(cvGetSize(depthImage), 8, 1);
				for (int rows=0; rows<depthImage->height; rows++)
				{
					uchar* src_ptr = (uchar*)(depthImage->imageData + rows*depthImage->widthStep);
					uchar* des_ptr = (uchar*)(showImage_depth->imageData + rows*showImage_depth->widthStep);
					for (int cols=0; cols<depthImage->width; cols++)
					{
						if (src_ptr[3*cols] < 1)
						{
							des_ptr[cols] = 255;
						}
						else
						{
							des_ptr[cols] = src_ptr[3*cols];
						}
					}
				}
				

				//----------------------------------------
				//Record the sign postures
				CvRect hand_rect;
				int len_box = 64;
				hand_rect.x = myReadVideo.vSkeletonData[i]._2dPoint[11].x - len_box/2;
				hand_rect.y = myReadVideo.vSkeletonData[i]._2dPoint[11].y - len_box/2;
				hand_rect.height = len_box;
				hand_rect.width = len_box;

				cvSetImageROI(showImage, hand_rect);
				cvSaveImage(fileName, showImage);
				cvResetImageROI(showImage);

				cvSetImageROI(showImage_depth, hand_rect);
				cvSaveImage(fileName_depth, showImage_depth);
				cvResetImageROI(showImage_depth);

				//----------------------------------------
				// Create black background
				// 			showImage = cvCreateImage(cvSize(width, height),8,3);
				// 			for (int rows=0; rows<height; rows++)
				// 			{
				// 				uchar* src_ptr = (uchar*)(showImage->imageData + rows*showImage->widthStep);
				// 				for (int cols=0; cols<width; cols++)
				// 				{
				// 					src_ptr[3*cols + 0] = 0;
				// 					src_ptr[3*cols + 1] = 0;
				// 					src_ptr[3*cols + 2] = 0;
				// 				}
				// 			}

				// 			for (int j=2; j<=11; j++)
				// 			{
				// 				CvPoint p1;
				// 				p1.x = myReadVideo.vSkeletonData[i]._2dPoint[j].x;
				// 				p1.y = myReadVideo.vSkeletonData[i]._2dPoint[j].y;
				// 				cvCircle(showImage,p1,2,cvScalar(225,0,0),3,8,0);
				// 				if (j!=7 && j!=3 && j!=11)
				// 				{
				// 					CvPoint p2;
				// 					p2.x = myReadVideo.vSkeletonData[i]._2dPoint[j+1].x;
				// 					p2.y = myReadVideo.vSkeletonData[i]._2dPoint[j+1].y;
				// 					cvLine(showImage,p1,p2,cvScalar(0,0,225),3,8,0);
				// 				}
				// 				if (j==2)
				// 				{
				// 					CvPoint p2;
				// 					p2.x = myReadVideo.vSkeletonData[i]._2dPoint[j+2].x;
				// 					p2.y = myReadVideo.vSkeletonData[i]._2dPoint[j+2].y;
				// 					CvPoint p3;
				// 					p3.x = myReadVideo.vSkeletonData[i]._2dPoint[j+6].x;
				// 					p3.y = myReadVideo.vSkeletonData[i]._2dPoint[j+6].y;
				// 					cvLine(showImage,p1,p2,cvScalar(0,0,225),3,8,0);
				// 					cvLine(showImage,p1,p3,cvScalar(0,0,225),3,8,0);
				// 				}
				// 
				// 			}
				// 
				// 
				// 			
				// 
				// 			// Segment right hand
				// 			_Vector4f left_elblow = myReadVideo.vSkeletonData[i]._3dPoint[9];
				// 			_Vector4f left_hand = myReadVideo.vSkeletonData[i]._3dPoint[11];
				// 			_Vector4f left_wrist = myReadVideo.vSkeletonData[i]._3dPoint[10];
				// 			float len_arm = sqrt((float)((left_elblow.x - left_hand.x)*(left_elblow.x - left_hand.x)
				// 				+ (left_elblow.y - left_hand.y)*(left_elblow.y - left_hand.y)
				// 				/*+ (left_elblow.z - left_hand.z)*(left_elblow.z - left_hand.z)*/));
				// 			
				// 
				// 			int len_bound_wid = 60; //len_arm*400;
				// 			int len_bound_hei = 80; //len_arm*500;
				// 
				// 			_Vector2i left_elbow_2D = myReadVideo.vSkeletonData[i]._2dPoint[9];
				// 			_Vector2i left_hand_2D = myReadVideo.vSkeletonData[i]._2dPoint[11];
				// 			_Vector2i left_wrist_2D = myReadVideo.vSkeletonData[i]._2dPoint[10];
				// 
				// 
				// 			int lt[2] = {-len_bound_wid/2, len_bound_hei/2};
				// 			int rt[2] = {len_bound_wid/2, len_bound_hei/2};
				// 			int lb[2] = {-len_bound_wid/2, -len_bound_hei/1.2};
				// 			int rb[2] = {len_bound_wid/2, -len_bound_hei/1.2};
				// 
				// 			float co = (-left_elblow.y + left_wrist.y)/len_arm;
				// 			float si = (-left_elblow.x + left_wrist.x)/len_arm;
				// 
				// 			//int lt_new[2];
				// 			CvPoint lt_new;
				// 			lt_new.x = lt[0]*co + lt[1]*(-si) + left_hand_2D.x;
				// 			lt_new.y = lt[0]*(si) + lt[1]*co + left_hand_2D.y;
				// 
				// 			//int rt_new[2];
				// 			CvPoint rt_new;
				// 			rt_new.x = rt[0]*co + rt[1]*(-si) + left_hand_2D.x;
				// 			rt_new.y = rt[0]*(si) + rt[1]*co + left_hand_2D.y;
				// 
				// 			//int lb_new[2];
				// 			CvPoint lb_new;
				// 			lb_new.x = lb[0]*co + lb[1]*(-si) + left_hand_2D.x;
				// 			lb_new.y = lb[0]*(si) + lb[1]*co + left_hand_2D.y;
				// 
				// 			//int rb_new[2];
				// 			CvPoint rb_new;
				// 			rb_new.x = rb[0]*co + rb[1]*(-si) + left_hand_2D.x;
				// 			rb_new.y = rb[0]*(si) + rb[1]*co + left_hand_2D.y;
				// 
				// 			
				// 			cvLine(showImage,lt_new, rt_new, cvScalar(0,255,0),3,8,0);
				// 			cvLine(showImage,rt_new, rb_new, cvScalar(0,255,0),3,8,0);
				// 			cvLine(showImage,rb_new, lb_new, cvScalar(0,255,0),3,8,0);
				// 			cvLine(showImage,lb_new, lt_new, cvScalar(0,255,0),3,8,0);

				cvShowImage("color_skeleton",showImage);

				if (b_recordVide)
				{
					writer_skeleton << showImage;
					//writer_depth << depthImage; 
				}
				cvReleaseImage(&showImage_depth);

				cvWaitKey(10);
			}

			cvDestroyWindow("color_skeleton");
		}
	}


	cout<<"Done!"<<endl;
	return 0;
}

