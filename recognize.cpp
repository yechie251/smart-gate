#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


#include <iostream>
#include <ctime>
#include <fstream>
#include <map>


// define the namespaces
using namespace cv;
using namespace cv::face;
using namespace std;


// main program
int main(){

  // define camera settings
  Mat image ;
  VideoCapture cap(0);
  cap.set(CAP_PROP_FRAME_WIDTH, 320);//Setting the width of the video
  cap.set(CAP_PROP_FRAME_HEIGHT, 240);//Setting the height of the video//
  if (!cap.isOpened()) {cout << "cannot open camera";}

  cap >> image;
  map<int, string> labels;


  // read labels(classes) from text file
  ifstream infile("./utils/labels.txt");

  int a;
  string b;
  while (infile >> a >> b){
    labels[a] = b;
  }

  // define the face detector using cascade classifier
  CascadeClassifier classifier;
  classifier.load("./utils/lbpcascade_frontalface.xml");

  // define the face recognitizer using LBPH algorithm
  Ptr<LBPHFaceRecognizer> recognizer =  LBPHFaceRecognizer::create(2, 2, 7, 7, 17);
  recognizer->read("./utils/embeddings.xml");

  Mat windowFrame;
  namedWindow("Security System", 1);
  int numframes = 0;
  time_t timer_begin,timer_end;
  time ( &timer_begin );

  for(;;){

    // capture frame from camera and convert to grayscale
    Mat frame;
    cap >> frame;
    cvtColor(frame, windowFrame, COLOR_BGR2GRAY);
    vector<Rect> faces;

    // detect faces int the frame
    classifier.detectMultiScale(frame, faces, 1.2, 5);

    // recignize each face in the frame
    for(size_t i = 0; i < faces.size(); i++){
      
      Mat face = windowFrame(faces[i]);

      // zero the confidence
      double confidence = 0.0;

      // predict 
      int predicted = recognizer->predict(face);
      recognizer->predict(face, predicted, confidence);

      // if confidence is low:
      if(labels.find(predicted) == labels.end() || confidence < 25){
         putText(frame, "Unknown", Point(faces[i].x ,faces[i].y - 5), FONT_HERSHEY_DUPLEX, 1, Scalar(0,0,255), 1);
         rectangle(frame, faces[i], Scalar(0, 0, 255));
      }

      // if confidence is high (face recognized):
      else{
        putText(frame, labels[predicted], Point(faces[i].x ,faces[i].y - 5), FONT_HERSHEY_DUPLEX, 1, Scalar(0,255,0), 1);
        rectangle(frame, faces[i], Scalar(0, 255, 0));
        }

      cout << "ID: " << predicted << " | Confidence: " << confidence << endl;
    }

    // show the captured frame with faces detected, label and confidence 
    imshow("Security System", frame);
    numframes++;

    //  if 'q' is pressed, close the program
    if(waitKey(30) >=0) break;
  }

  time ( &timer_end );

  // calculate the FPS
  double secondsElapsed = difftime ( timer_end,timer_begin );
  cout << "FPS:" << numframes / secondsElapsed << endl;
  return 0;
}