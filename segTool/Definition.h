#pragma once
#include <opencv2\opencv.hpp>

#define LRB 3    //Left, right, both
#define FusedLRB 1
//#define CombineGallery    //If defined. Combined gallery is used. 
#ifdef CombineGallery
	#define Gallery_num 1
#endif
#ifndef CombineGallery
	#define Gallery_num 5
#endif

#define Posture_num 370
#define feature_dimension 1764
#define MaxKeyFrameNumber 6
#define delay2
#define iSIZE 64
#define ThreForOnline 0.5
#define MarryThre 0.0
#define topXValue   5   //The allowed top class index to be considered in doMatche.
#define trajectoryFactor 0
//#define DrawInOpenGL
//#define DrawSpeed
#define keyFrameGap 10
#define top2Tore   0.08   //used in function "doMatch" in Matching class. 
#define pairSumPenalty 0.02
#define finalResultThre 0//0.58 is for posture only.
#define allowedTraFrameSize 30

#define WordsNum 370       //手语词汇个数
//#define heightThres	370
#define consistThres	5

#define MaxKeyPostureNum 10
#define Theda  0.0000001


//////////////////////////////////////////////////////////////////////////
const int F=512;
const int num=5;
const int dim=3;//3D
const int probenum=0;
const double PI= 3.1415;
const float eps=0.0001;
const int rsnum=200;


//////////////////////////////////////////////////////////////////////////
/// @author xu zhihao
/// @struct _Vector2i
/// @brief skeleton data transformed data
//////////////////////////////////////////////////////////////////////////
struct _Vector2i
{
	int x;
	int y;
};

//////////////////////////////////////////////////////////////////////////
/// @author xu zhihao
/// @struct SLR_ST_Skeleton
/// @brief skeleton data  real data
//////////////////////////////////////////////////////////////////////////
struct _Vector4f
{
	float x;
	float y;
	float z;
	float w;
};
//////////////////////////////////////////////////////////////////////////
/// @author xu zhihao
/// @struct SLR_ST_Skeleton
/// @brief skeleton data  real data and transformed data
//////////////////////////////////////////////////////////////////////////
struct SLR_ST_Skeleton
{
	_Vector4f _3dPoint[20];    ///< real point
	_Vector2i _2dPoint[20];    ///< pix in color image
}; 
struct KeyFrameUnit
{
	int StartFrame;
	int EndFrame;
};
struct HandSegment
{
	int keyframe_num;
	int* keyframe_no;
	IplImage*** keyframe_pic;
	int** keyframe_ID;
	int** keyframe_coor;
};
enum HANDTYPE
{
	LEFT,
	RIGHT,
	BOTH,
};

//////////////////////////////////////////////////////////////////////////
/// @author xu zhihao
/// @struct ColorModel
/// @brief skin color model, mean and variance of cr cb
//////////////////////////////////////////////////////////////////////////
struct ColorModel
{
	double mean_cr;     ///< mean of cr
	double mean_cb;     ///< mean of cb
	double d_cr;        ///< variance of cr
	double d_cb;        ///< variance of cb
	ColorModel():mean_cr(0),mean_cb(0),d_cr(0),d_cb(0){};
};

struct KeyFrameSegment
{
	int BeginFrameID;		//关键片段开始帧号
	int EndFrameID;			//关键片段jie'shu帧号

	int BothLabel;			//0-无	1-有
	int LeftLabel;			//0-无	1-有
	int RightLabel;			//0-无	1-有

	int BothNum;			//双手图片数目
	int LeftNum;			//左手图片数目
	int RightNum;			//右手图片数目

	int* BothID;			//每张双手分割图片的帧号
	int* LeftID;			//每张左手分割图片的帧号
	int* RightID;			//每张右手分割图片的帧号

	int* BothCoor;			//每张双手分割图片左上角点坐标
	int* LeftCoor;			//每张左手分割图片左上角点坐标
	int* RightCoor;			//每张右手分割图片左上角点坐标

	IplImage** BothImages;	//双手图片
	IplImage** LeftImages;	//双手图片
	IplImage** RightImages;	//双手图片
};