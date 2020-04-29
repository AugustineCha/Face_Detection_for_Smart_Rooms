// mode: c++
#ifndef CAPTURE_THREAD_H
#define CAPTURE_THREAD_H

#include <QString>
#include <QThread>
#include <QMutex>

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face/facemark.hpp"
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


class CaptureThread : public QThread
{
    Q_OBJECT
public:
    CaptureThread(int camera, QMutex *lock);
    CaptureThread(QString videoPath, QMutex *lock);
    ~CaptureThread();
    void setRunning(bool run) {running = run; };
    void startCalcFPS() {fps_calculating = true; };
    enum VideoSavingStatus {
                            STARTING,
                            STARTED,
                            STOPPING,
                            STOPPED
    };

    void setVideoSavingStatus(VideoSavingStatus status) {video_saving_status = status; };
    void setMotionDetectingStatus(bool status) {
        motion_detecting_status = status;
        motion_detected = false;
        if(video_saving_status != STOPPED) video_saving_status = STOPPING;
    };
    void findKeyPointsHomography(vector<KeyPoint>& kpts1, vector<KeyPoint>& kpts2,
                                 vector<DMatch>& matches, vector<char>& match_mask);


protected:
    void run() override;

signals:
    void frameCaptured(cv::Mat *data);
    void fpsChanged(float fps);
    void videoSaved(QString name);

private:
    void calculateFPS(cv::VideoCapture &cap);
    void startSavingVideo(cv::Mat &firstFrame);
    void stopSavingVideo();
    void motionDetect(cv::Mat &frame);
    void detectFaces(cv::Mat &frame);
    void detect_and_compute(string type, Mat& img, vector<KeyPoint>& kpts, Mat& desc);
    void match(string type, Mat& desc1, Mat& desc2, vector<DMatch>& matches);

private:
    bool running;
    int cameraID;
    QString videoPath;
    QMutex *data_lock;
    cv::Mat frame;

    // FPS calculating
    bool fps_calculating;
    float fps;

    // video saving
    int frame_width, frame_height;
    VideoSavingStatus video_saving_status;
    QString saved_video_name;
    cv::VideoWriter *video_writer;

    // motion analysis
    bool motion_detecting_status;
    bool motion_detected;
    cv::Ptr<cv::BackgroundSubtractorMOG2> segmentor;
    // face detection
    cv::CascadeClassifier *classifier;
};

#endif // CAPTURE_THREAD_H
