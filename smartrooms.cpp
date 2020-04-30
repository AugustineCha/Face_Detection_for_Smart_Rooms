#include "smartrooms.h"
#include "ui_smartrooms.h"
#include <qcommonstyle.h>
#include <QMessageBox>
#include <string>
#include <vector>
#include <global.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
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
using namespace std;

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

bool match(string type, Mat& desc1, Mat& desc2, vector<DMatch>& matches) {
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
//        cout << "identified" << endl;
        return true;
    else
//        cout << "failed" << endl;
        return false;
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

Mat detectFaceOpenCVDNN(Net net, Mat &frameOpenCVDNN){
    int frameHeight = frameOpenCVDNN.rows;
    int frameWidth = frameOpenCVDNN.cols;

    cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, false, false);
//#else
//    cv::Mat inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, true, false);
//#endif

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

SmartRooms::SmartRooms(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SmartRooms)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/rahimai/Documents/University/Spring 2020/CPP/SmartRooms/Face_Detection_for_Smart_Rooms/home.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    this->setFixedSize(800,600);

    ui->pushButton->setFlat(true);
    ui->pushButton_2->setFlat(true);
    ui->pushButton_3->setFlat(true);

    QCommonStyle style;
    ui->homeButton->setIcon(style.standardIcon(QStyle::SP_ArrowBack));
    ui->homeButton_2->setIcon(style.standardIcon(QStyle::SP_ArrowBack));
    ui->homeButton_3->setIcon(style.standardIcon(QStyle::SP_ArrowBack));

}

SmartRooms::~SmartRooms()
{
    delete ui;
}

void SmartRooms::on_pushButton_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(1);
}

void SmartRooms::on_pushButton_2_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(2);
    ui -> comboBox -> clear();
    for (const auto& kv: area_map){
        QString name = QString::fromStdString(kv.first);
        ui -> comboBox -> addItem(name);
    }
}

void SmartRooms::on_pushButton_3_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(3);
}


void SmartRooms::on_homeButton_clicked()
{
    ui -> stackedWidget ->setCurrentIndex(0);
}

void SmartRooms::on_homeButton_2_clicked()
{
    ui -> stackedWidget ->setCurrentIndex(0);
}


void SmartRooms::on_homeButton_3_clicked()
{
    ui -> stackedWidget ->setCurrentIndex(0);
}

//Button for adding Persons
void SmartRooms::on_pushButton_5_clicked()
{
    //Reading in user input
    QString firstname_q = ui -> lineEditRoom_3 -> text();
    QString lastname_q = ui -> lineEditRoom_4 -> text();
    QString id_q = ui -> lineEditRoom_5 -> text();
    QString img_name_q = ui -> lineEditRoom_6 -> text();

    //Converting user input from QString to std::string or int
    string firstname = firstname_q.toStdString();
    string lastname = lastname_q.toStdString();
    string img_name = img_name_q.toStdString();
    int id = stoi(id_q.toStdString());


    //Check if key already exists
    if (person_ids.find(id) != person_ids.end()){

        QMessageBox messageBox;
        messageBox.critical(0,"Error 401","ID already exists!");
        messageBox.setFixedSize(500,200);

    }
    else{

        //Create image path
        string img_path = "pathtoworkingdirectory" + img_name;

        //Save input both temporarily and permanently
        vector<string> names = {firstname, lastname};

        fstream data_ostream;
        data_ostream.open("person_ids.txt", std::ios::app);

        if (!data_ostream.good()){

            std::cout << "Error: Loading data failed.\n";
            data_ostream.close();

        }
        else{

            person_ids[id] = names;
            data_ostream << firstname << "," << lastname << "," << id << "," << img_path << endl;

            }
        data_ostream.close();

    }

    //Clear fields
    ui -> lineEditRoom_3 -> clear();
    ui -> lineEditRoom_4 -> clear();
    ui -> lineEditRoom_5 -> clear();

}

//This function splits a string by a given delimiter
void splitbydelimiter(const string& str, vector<int>& cont,
              char delim = ',')
{
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(stoi(str.substr(previous, current - previous)));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(stoi(str.substr(previous, current - previous)));
}


//Button for adding areas
void SmartRooms::on_pushButton_4_clicked()
{
    //Reading in user input
    QString areaname_q = ui -> lineEditRoom -> text();
    QString idlist_q = ui -> lineEditRoom_2 -> text();

    //Converting user input from QString to std::string
    string areaname = areaname_q.toStdString();
    string idlist = idlist_q.toStdString();

    //Split user input ids
    vector<int> authorized_ids;
    splitbydelimiter(idlist, authorized_ids);

    //Save input both temporarily and permanently
    fstream dataar_ostream;
    dataar_ostream.open("areas.txt", std::ios::app);

    if (!dataar_ostream.good()){

        std::cout << "Error: Loading data failed.\n";
        dataar_ostream.close();

    }
    else{

        area_map[areaname] = authorized_ids;

        dataar_ostream << areaname << ",";
        for(auto x: authorized_ids){
            dataar_ostream << x << ",";
        }
        dataar_ostream << "#";

    }

    //Clear fields
    ui -> lineEditRoom -> clear();
    ui -> lineEditRoom_2 -> clear();
}

void SmartRooms::on_pushButton_6_clicked()
{
    QString accessed_area_q = ui -> comboBox -> currentText();
    string accessed_area = accessed_area_q.toStdString();
    vector<int> authorized_ids = area_map[accessed_area]; //RAII



    //Open webcam and take picture

    //Call all ids authorized to enter specific room

    //Compare with DB (use hashmap for each authorized id and check if swift accepts it)

    // Set a boolean, either access or denial

    Net net = cv::dnn::readNetFromCaffe(caffeConfigFile, caffeWeightFile);

    VideoCapture source;
    source.open(0);
    Mat frame;

    double tt_opencvDNN = 0;
    double fpsOpencvDNN = 0;

    Mat img1 = cv::imread(img_file1, 1);
    // Multiple images
    vector<Mat> images;
    images.push_back(img1);
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
//        detect_and_compute(desc_type, img1, kpts2, desc2);

//        match(match_type, desc1, desc2, matches);

//        vector<char> match_mask(matches.size(), 1);
//        findKeyPointsHomography(kpts1, kpts2, matches, match_mask);

//        Mat res;
//        cv::drawMatches(in, kpts1, img1, kpts2, matches, res, Scalar::all(-1),
//                        Scalar::all(-1), match_mask, DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

        Mat res;
        bool identified = false;
        for(int i=0; i<images.size(); ++i){
            detect_and_compute(desc_type, images[i], kpts2, desc2);

            identified = match(match_type, desc1, desc2, matches);

            vector<char> match_mask(matches.size(), 1);
            findKeyPointsHomography(kpts1, kpts2, matches, match_mask);

//            cv::drawMatches(in, kpts1, img1, kpts2, matches, res, Scalar::all(-1),
//                            Scalar::all(-1), match_mask, DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
            if (identified)
                break;
        }


//        cv::imshow("result", res);
        if(identified){
//            destroyAllWindows();
            cout << "identified" << endl;
            break;
        }
        int k = waitKey(5);
        if(k == 27)
        {
            destroyAllWindows();
            break;
        }
    }
}
