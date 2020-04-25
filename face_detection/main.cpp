#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/ocl.hpp>

using namespace cv;
using namespace std;
using std::cout;
using std::cerr;
using std::vector;
using std::string;

using cv::Mat;
using cv::Point2f;
using cv::KeyPoint;
using cv::Scalar;
using cv::Ptr;

using cv::FastFeatureDetector;
using cv::SimpleBlobDetector;

using cv::DMatch;
using cv::BFMatcher;
using cv::DrawMatchesFlags;
using cv::Feature2D;
using cv::ORB;
using cv::BRISK;
using cv::AKAZE;
using cv::KAZE;

using cv::xfeatures2d::BriefDescriptorExtractor;
using cv::xfeatures2d::SURF;
using cv::xfeatures2d::SIFT;
using cv::xfeatures2d::DAISY;
using cv::xfeatures2d::FREAK;

using namespace cv;
using namespace std;
using namespace cv::dnn;

const double th = 0.2;
const double kDistanceCoef = 2.0;
const int kMaxMatchingSize = 1000;
const string desc_type = "sift";
const string match_type = "bf";
const string img_file1 = "/Users/augustinecha/local/opencv_test/opencv_test/models/image.jpg";
//const string img_file1 = "/Users/augustinecha/local/opencv_test/opencv_test/models/image2.png";


inline void detect_and_compute(string type, Mat& img, vector<KeyPoint>& kpts, Mat& desc) {
    if (type.find("fast") == 0) {
        type = type.substr(4);
        Ptr<FastFeatureDetector> detector = FastFeatureDetector::create(10, true);
        detector->detect(img, kpts);
    }
    if (type.find("blob") == 0) {
        type = type.substr(4);
        Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create();
        detector->detect(img, kpts);
    }
    if (type == "surf") {
        Ptr<Feature2D> surf = SURF::create(800.0);
        surf->detectAndCompute(img, Mat(), kpts, desc);
    }
    if (type == "sift") {
        Ptr<Feature2D> sift = SIFT::create();
        sift->detectAndCompute(img, Mat(), kpts, desc);
    }
    if (type == "orb") {
        Ptr<ORB> orb = ORB::create();
        orb->detectAndCompute(img, Mat(), kpts, desc);
    }
    if (type == "brisk") {
        Ptr<BRISK> brisk = BRISK::create();
        brisk->detectAndCompute(img, Mat(), kpts, desc);
    }
    if (type == "kaze") {
        Ptr<KAZE> kaze = KAZE::create();
        kaze->detectAndCompute(img, Mat(), kpts, desc);
    }
    if (type == "akaze") {
        Ptr<AKAZE> akaze = AKAZE::create();
        akaze->detectAndCompute(img, Mat(), kpts, desc);
    }
}

inline void match(string type, Mat& desc1, Mat& desc2, vector<DMatch>& matches) {
    matches.clear();
    if (type == "bf") {
        BFMatcher desc_matcher(cv::NORM_L2, true);
        desc_matcher.match(desc1, desc2, matches, Mat());
    }
    if (type == "knn") {
        BFMatcher desc_matcher(cv::NORM_L2, true);
        vector< vector<DMatch> > vmatches;
        desc_matcher.knnMatch(desc1, desc2, vmatches, 1);
        for (int i = 0; i < static_cast<int>(vmatches.size()); ++i) {
            if (!vmatches[i].size()) {
                continue;
            }
            matches.push_back(vmatches[i][0]);
        }
    }
    std::sort(matches.begin(), matches.end());
    float sz = matches.size();
    float cnt = 0.0;
    while (matches.front().distance * kDistanceCoef < matches.back().distance) {
        matches.pop_back();
        cnt += 1;
    }
    while (matches.size() > kMaxMatchingSize) {
        matches.pop_back();
    }
    float rate = (sz - cnt) / sz;
    if(rate > th)
        cout << "identified" << endl;
    else
        cout << "failed" << endl;
}

inline void findKeyPointsHomography(vector<KeyPoint>& kpts1, vector<KeyPoint>& kpts2,
                                    vector<DMatch>& matches, vector<char>& match_mask) {
    if (static_cast<int>(match_mask.size()) < 3) {
        return;
    }
    vector<Point2f> pts1;
    vector<Point2f> pts2;
    for (int i = 0; i < static_cast<int>(matches.size()); ++i) {
        pts1.push_back(kpts1[matches[i].queryIdx].pt);
        pts2.push_back(kpts2[matches[i].trainIdx].pt);
    }
    findHomography(pts1, pts2, cv::RANSAC, 4, match_mask);
}

const size_t inWidth = 300;
const size_t inHeight = 300;
const double inScaleFactor = 1.0;
const float confidenceThreshold = 0.5;
const cv::Scalar meanVal(104.0, 177.0, 123.0);

#define CAFFE
const std::string pathToModels = "/Users/augustinecha/local/opencv_test/opencv_test";
const std::string caffeConfigFile = pathToModels + "/models/deploy.prototxt";
const std::string caffeWeightFile = pathToModels + "/models/res10_300x300_ssd_iter_140000_fp16.caffemodel";

const std::string tensorflowConfigFile = pathToModels + "/models/opencv_face_detector.pbtxt";
const std::string tensorflowWeightFile = pathToModels + "/models/opencv_face_detector_uint8.pb";

Mat detectFaceOpenCVDNN(Net net, Mat &frameOpenCVDNN)
{
    int frameHeight = frameOpenCVDNN.rows;
    int frameWidth = frameOpenCVDNN.cols;
#ifdef CAFFE
    cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, false, false);
#else
    cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, true, false);
#endif
    
    net.setInput(inputBlob, "data");
    cv::Mat detection = net.forward("detection_out");
    
    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
    Mat cropped_img;
    //    for(int i = 0; i < detectionMat.rows; i++)
    //    {
    int i = 0;
    float confidence = detectionMat.at<float>(i, 2);
    
    if(confidence > confidenceThreshold)
    {
        int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
        int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
        int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
        int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);
        
        cv::rectangle(frameOpenCVDNN, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0),2, 4);
        Mat ROI(frameOpenCVDNN, Rect(x1,y1,x2-x1, y2-y1));
        ROI.copyTo(cropped_img);
    }
    //    }
    return cropped_img;
}

int main( int argc, const char** argv )
{
#ifdef CAFFE
    Net net = cv::dnn::readNetFromCaffe(caffeConfigFile, caffeWeightFile);
#else
    Net net = cv::dnn::readNetFromTensorflow(tensorflowWeightFile, tensorflowConfigFile);
#endif
    
    VideoCapture source;
    if (argc == 1)
        source.open(0);
    else
        source.open(argv[1]);
    Mat frame;
    
    double tt_opencvDNN = 0;
    double fpsOpencvDNN = 0;
        
    Mat img1 = cv::imread(img_file1, 1);
    while(1)
    {
        source >> frame;
        if(frame.empty())
            break;
        double t = cv::getTickCount();
        Mat in = detectFaceOpenCVDNN ( net, frame );
        if(in.empty())
            continue;
        tt_opencvDNN = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
        fpsOpencvDNN = 1/tt_opencvDNN;
        //      putText(frame, format("OpenCV DNN ; FPS = %.2f",fpsOpencvDNN), Point(10, 50), FONT_HERSHEY_SIMPLEX, 1.4, Scalar(0, 0, 255), 4);
        vector<KeyPoint> kpts1;
        vector<KeyPoint> kpts2;
        
        Mat desc1;
        Mat desc2;
        
        vector<DMatch> matches;
        
        detect_and_compute(desc_type, in, kpts1, desc1);
        detect_and_compute(desc_type, img1, kpts2, desc2);
        
        match(match_type, desc1, desc2, matches);
        
        vector<char> match_mask(matches.size(), 1);
        findKeyPointsHomography(kpts1, kpts2, matches, match_mask);
        
        Mat res;
        cv::drawMatches(in, kpts1, img1, kpts2, matches, res, Scalar::all(-1),
                        Scalar::all(-1), match_mask, DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
        
        cv::imshow("result", res);
        int k = waitKey(5);
        if(k == 27)
        {
            destroyAllWindows();
            break;
        }
    }
}
