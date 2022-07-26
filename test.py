# import the opencv library
import cv2
  
  
# define a video capture object
vid = cv2.VideoCapture(0)
vid.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
vid.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)
recognizer = cv2.face.LBPHFaceRecognizer_create(2, 2, 7, 7) # 2, 2, 7, 7
recognizer.read("utils/embeddings.xml")
classifier = cv2.CascadeClassifier()
classifier.load("utils/lbpcascade_frontalface.xml")
#recognizer.load()

labels = ["yechiel", "avraham"]
i = 0
while(True):
      
    # Capture the video frame
    # by frame
    ret, frame = vid.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    face = classifier.detectMultiScale(gray, 1.2, 5)
    if len(face) > 0:
        (x, y, w, h) = face[0]
        
        cropped_image = gray[y:y+h, x:x+w]

        #cv2.imwrite("dataset/avraham/img_{}.jpg".format(i), frame)
        #i += 1
        label, conf = recognizer.predict(cropped_image)
        print(label, conf)
        if conf > 25:
            text = "{}: {}".format(labels[label], conf)
            cv2.rectangle(frame, (x, y),(x+w, y+h), (0,255,0))
            cv2.putText(frame, text, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 1,(0,255,0), 2)
        else:
            cv2.rectangle(frame, (x, y),(x+w, y+h), (0,0,255))

    # Display the resulting frame
    cv2.imshow('frame', frame)
      
    # the 'q' button is set as the
    # quitting button you may use any
    # desired button of your choice
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
  
# After the loop release the cap object
vid.release()
# Destroy all the windows
cv2.destroyAllWindows()