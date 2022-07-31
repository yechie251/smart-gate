#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <unistd.h>
#include <chrono>


using namespace std;
using namespace cv;


    


Mat Read_Embeddings(string path, int num_samples) {
    FileStorage fs(path, FileStorage::READ);

     
     Mat R;
     for (int i = 0; i < num_samples; ++i) {
        Mat T;
        fs["_"+to_string(i)] >> T;
        R.push_back(T);
        }
     fs.release();  
     Mat OUT = R; 
     return OUT;
  // code block to be executed  
}


int main(){

    // init timers

    chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();
    chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    int is_first = 1;

    cout<<"start serial com to arduino"<<endl;
    int fd;
    if(wiringPiSetup() < 0)return 1;
    if((fd = serialOpen("/dev/ttyACM0",9600)) < 0)return 1;
    serialPutchar(fd,'0');
    cout<<"arduino connected!"<<endl;

    cout<<"start file"<<endl;
    // path to person 1 embedings
    string Avraham_path = "avraham_embeddings.xml";
    string Yechiel_path = "yechiel_embeddings.xml";

    cout<<"read embeddings"<<endl;
    // get the embeddings matrix
    Mat Embeddings_Avraham = Read_Embeddings(Avraham_path, 2);
    Mat Embeddings_Yechiel = Read_Embeddings(Avraham_path, 2);


    cout<<"done read embeddings"<<endl;
    // print embeddings size
    cout << Embeddings_Avraham.size();
    cout << Embeddings_Yechiel.size();

    // path to dnn weights
    String fd_modelPath = "./models/face_detection_yunet_2022mar.onnx";
    String fr_modelPath = "./models/face_recognition_sface_2021dec.onnx";

    // parameters
    float scoreThreshold = 0.9;
    float nmsThreshold = 0.3;
    int topK = 5000;
    double cosine_similar_thresh = 0.363;
    double l2norm_similar_thresh = 1.128;

    Ptr<FaceDetectorYN> detector = FaceDetectorYN::create(fd_modelPath, "", Size(320, 240), scoreThreshold, nmsThreshold, topK);
    Ptr<FaceRecognizerSF> faceRecognizer = FaceRecognizerSF::create(fr_modelPath, "");



    // init the camera
    VideoCapture cap(0, CAP_V4L2);
    cap.set(CAP_PROP_FRAME_WIDTH, 320);//Setting the width of the video
    cap.set(CAP_PROP_FRAME_HEIGHT, 240);//Setting the height of the video//
    if (!cap.isOpened()) {cout << "cannot open camera";};
    Mat frame;
    Mat aligned_face;
    Mat faces;
    Mat feature;
    bool aouth;
    // real time cap frames
    for(;;){

    // capture frame from camera and convert to grayscale
    
    
    cap >> frame;
    detector->detect(frame, faces);
    if (faces.rows > 0){
        for(size_t i = 0; i < faces.rows; i++){

        // extract features
        faceRecognizer->alignCrop(frame, faces.row(0), aligned_face);
        faceRecognizer->feature(aligned_face, feature);

        double cos_score1 = faceRecognizer->match(feature, Embeddings_Avraham.row(0), FaceRecognizerSF::DisType::FR_COSINE);
        double L2_score1 = faceRecognizer->match(feature, Embeddings_Avraham.row(0), FaceRecognizerSF::DisType::FR_NORM_L2);
        

        double cos_score2 = faceRecognizer->match(feature, Embeddings_Yechiel.row(0), FaceRecognizerSF::DisType::FR_COSINE);
        double L2_score2 = faceRecognizer->match(feature, Embeddings_Yechiel.row(0), FaceRecognizerSF::DisType::FR_NORM_L2);
        
        if ((cos_score1 >= cosine_similar_thresh) && (L2_score1 <= l2norm_similar_thresh)){
            rectangle(frame, Rect2i(int(faces.at<float>(i, 0)), int(faces.at<float>(i, 1)), int(faces.at<float>(i, 2)), int(faces.at<float>(i, 3))), Scalar(0, 255, 0), 1);
            putText(frame, "Avraham", Point(int(faces.at<float>(i, 0)), int(faces.at<float>(i, 1))), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
            aouth = 1;
            now = std::chrono::steady_clock::now();

        }
        else if ((cos_score2 >= cosine_similar_thresh) && (L2_score2 <= l2norm_similar_thresh)){
        
            rectangle(frame, Rect2i(int(faces.at<float>(i, 0)), int(faces.at<float>(i, 1)), int(faces.at<float>(i, 2)), int(faces.at<float>(i, 3))), Scalar(0, 255, 0), 1);
            putText(frame, "Yechiel", Point(int(faces.at<float>(i, 0)), int(faces.at<float>(i, 1))), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
            aouth = 1;
            now = std::chrono::steady_clock::now();
        }

        else{
            rectangle(frame, Rect2i(int(faces.at<float>(i, 0)), int(faces.at<float>(i, 1)), int(faces.at<float>(i, 2)), int(faces.at<float>(i, 3))), Scalar(0, 0, 255), 1);
            putText(frame, "Unknown", Point(int(faces.at<float>(i, 0)), int(faces.at<float>(i, 1))), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
            aouth = 0;
        }
        if (aouth == 1){
            if ((chrono::duration_cast<std::chrono::microseconds>(now - prev).count()) >= 10000000){
                serialPutchar(fd,'1');
                serialPutchar(fd,'0');
                prev = now;
                aouth = 0;
                }

        }
        
        }
        
    }
    
    imshow("Live", frame);
    //  if 'q' is pressed, close the program
    if(waitKey(30) >=0) {
        break;
        serialClose(fd);
    }
    
    }
    return 0;
}
