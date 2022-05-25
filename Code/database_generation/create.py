import os

import cv2

name=input("Enter Name: ")
os.mkdir(name)
cam = cv2.VideoCapture(0, cv2.CAP_DSHOW) 
img_counter = 0
#cv2.namedWindow("Create")
counter=0
while True:
    ret,img=cam.read()
    cv2.imshow("Image",img)

    k = cv2.waitKey(1)
    if k%256 == 27:
           # ESC pressed
        print("Escape hit, closing...")
        break
    elif k%256 == 32:
            # SPACE pressed
        img_name = "{}_{}.jpg".format(name,img_counter)
        cv2.imwrite(os.path.join(name , img_name), img)
        print("{} written!".format(img_name))
        img_counter += 1

cam.release()

cv2.destroyAllWindows()

