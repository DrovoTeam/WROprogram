import cv2
import numpy as np 
from picamera.array import PiRGBArray
from picamera import PiCamera
import colorama
import sys

def nothing(x):
    pass

cv2.namedWindow("image")
############ create trackbars for color change
cv2.createTrackbar('HMin','image',0,179,nothing) # Hue is from 0-179 for Opencv
cv2.createTrackbar('SMin','image',0,255,nothing)
cv2.createTrackbar('VMin','image',0,255,nothing)
cv2.createTrackbar('HMax','image',0,179,nothing)
cv2.createTrackbar('SMax','image',0,255,nothing)
cv2.createTrackbar('VMax','image',0,255,nothing)

############ Set default value for MAX HSV trackbars.
cv2.setTrackbarPos('HMax', 'image', 179)
cv2.setTrackbarPos('SMax', 'image', 255)
cv2.setTrackbarPos('VMax', 'image', 255)

############Initialize to check if HSV min/max value changes
hMin = sMin = vMin = hMax = sMax = vMax = 0
phMin = psMin = pvMin = phMax = psMax = pvMax = 0

camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 20
camera.brightness= 50
rawCapture = PiRGBArray(camera, size=(640, 480))

for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
	vid = frame.array
	rows, cols, _ = vid.shape
	#############print("Rows", rows)
	#############print("Cols", cols)
	image = vid[285: 480, 0: 640]
	capRight = vid[285: 480, 320: 640]
	capLeft = vid[285: 480, 0: 319]

	gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
	#hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
	hsvR = cv2.cvtColor(capRight, cv2.COLOR_BGR2HSV)
	hsvL = cv2.cvtColor(capLeft, cv2.COLOR_BGR2HSV)
	############# get current positions of all trackbars
	hMin = cv2.getTrackbarPos('HMin','image')
	sMin = cv2.getTrackbarPos('SMin','image')
	vMin = cv2.getTrackbarPos('VMin','image')
	
	hMax = cv2.getTrackbarPos('HMax','image')
	sMax = cv2.getTrackbarPos('SMax','image')
	vMax = cv2.getTrackbarPos('VMax','image')
	
	############# Set minimum and max HSV values to display
	#lowerR = np.array([hMin, sMin, vMin])         #this used for calibration    /////////////////////////////////////////////////////////////////////////////////////
	#upperR = np.array([hMax, sMax, vMax])                                       /////////////////////////////////////////////////////////////////////////////////////
	lowerR = np.array([158, 117, 42])                                            #/////////////////////////////////////////////////////////////////////////////////////
	upperR = np.array([179, 255, 168])
	
	############# Create HSV Image and threshold into a range.
	#hsvth = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
	hsvthR = cv2.cvtColor(capRight, cv2.COLOR_BGR2HSV)
	hsvthL = cv2.cvtColor(capLeft, cv2.COLOR_BGR2HSV)
	maskRR = cv2.inRange(hsvthR, lowerR, upperR)                 
	maskRL = cv2.inRange(hsvthL, lowerR, upperR)
	outputRR = cv2.bitwise_and(capRight,capRight, mask= maskRR)
	outputRL = cv2.bitwise_and(capLeft,capLeft, mask= maskRL)
	#######################
	          
	red = np.uint8([[[141, 130, 250]]])
	hsvRed = cv2.cvtColor(red,cv2.COLOR_BGR2HSV)
	
	maskRR = cv2.inRange(hsvR, lowerR, upperR)
	red_mask = cv2.bitwise_and(capRight	, capRight	, mask=maskRR)
	
	
	maskRL = cv2.inRange(hsvL, lowerR, upperR)
	red_mask2 = cv2.bitwise_and(capLeft	, capLeft	, mask=maskRL)
	
	contoursRR, _ = cv2.findContours(maskRR, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
	contoursRL, _ = cv2.findContours(maskRL, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
	
	for contour in contoursRR:
         RareaR= cv2.contourArea(contour)
         #############print(Rarea)
         if RareaR > 13000 and RareaR < 25000:    #near
             
             cv2.drawContours(capRight, contour, -1, (0, 255 , 0), 3)
             D = print("near")
             print("right red")
                
         elif RareaR > 1400 and RareaR < 4000:
             
             cv2.drawContours(capRight, contour, -1, (0, 0 , 255), 3)
             
             Q = print("not too close")
             print("right red",Q)
             
         elif RareaR > 400  and RareaR < 1000:
             
             cv2.drawContours(capRight, contour, -1, (255, 0 , 0), 3)
             W = print("far")
             print("right green",W)   
                     
	for contour in contoursRL:
         RareaL= cv2.contourArea(contour)
         #############print(Rarea)
         if RareaL > 13000 and RareaL < 25000:    #near
             
             cv2.drawContours(capLeft, contour, -1, (0, 255 , 0), 3)
             D = print("near")
         
         elif RareaL > 1400 and RareaL < 4000:
             
             cv2.drawContours(capLeft, contour, -1, (0, 0 , 255), 3)
             Q = print("not too close")
             print('LEFT RED', Q)
             
            
         elif RareaL > 400  and RareaL < 1000:
             
             cv2.drawContours(capLeft, contour, -1, (255, 0 , 0), 3)
             W = print("far")
             print('LEFT RED',W)
             
            
	#lowerG = np.array([hMin, sMin, vMin])     #  /////////////////////////////////////////////////////////////////////////////////////
	#upperG = np.array([hMax, sMax, vMax])       # /////////////////////////////////////////////////////////////////////////////////////
	lowerG = np.array([69, 224, 33])            ## /////////////////////////////////////////////////////////////////////////////////////
	upperG = np.array([162, 255, 110])
	
	#hsvth = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
	hsvthGR = cv2.cvtColor(capRight, cv2.COLOR_BGR2HSV)
	hsvthGL = cv2.cvtColor(capLeft, cv2.COLOR_BGR2HSV)
	###maskG = cv2.inRange(hsvth, lowerG, upperG)
	maskGR = cv2.inRange(hsvthGR, lowerG, upperG)
	maskGL = cv2.inRange(hsvthGL, lowerG, upperG)
	###outputG = cv2.bitwise_and(image,image, mask= maskG)
	outputGR = cv2.bitwise_and(capRight,capRight, mask= maskGR)
	outputGL = cv2.bitwise_and(capLeft,capLeft, mask= maskGL)
	#######################
	          
	green = np.uint8([[[31, 127, 28]]])
	hsvGreen = cv2.cvtColor(green,cv2.COLOR_BGR2HSV)
	
	maskGR = cv2.inRange(hsvR, lowerG, upperG)
	green_mask = cv2.bitwise_and(capRight	,capRight	, mask=maskGR)
	maskGL = cv2.inRange(hsvL, lowerG, upperG)
	green_mask2 = cv2.bitwise_and(capLeft	, capLeft	, mask=maskGL)
	
	contoursGR, _ = cv2.findContours(maskGR, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
	contoursGL, _ = cv2.findContours(maskGL, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
	
	for contour in contoursGR:
         GareaR= cv2.contourArea(contour)
         #############print(Garea)
         if GareaR > 13000 and GareaR < 25000:
             print("GO LEFT")
             cv2.drawContours(capRight, contour, -1, (0, 255 , 0), 3)
             D = print("near")
             print('RIGHT GREEN', D)
           
         elif GareaR > 1400 and GareaR < 4000:
             cv2.drawContours(capRight, contour, -1, (0, 0 , 255), 3)
             Q = print("not too close")
             print('RIGHT GREEN', Q)
            
         elif GareaR > 400  and GareaR < 1000:
             cv2.drawContours(capRight, contour, -1, (255, 0 , 0), 3)
             W = print("far")
             print('RIGHT GREEN', W)
            
	for contour in contoursGL:
         GareaL= cv2.contourArea(contour)
         #############print(Garea)
         if GareaL > 13000 and GareaL < 25000:
             print("GO LEFT")
             cv2.drawContours(capLeft, contour, -1, (0, 255 , 0), 3)
             D = print("near")
             print('LEFT GREEN', D)
           
         elif GareaL > 1400 and GareaL < 4000:
             cv2.drawContours(capLeft, contour, -1, (0, 0 , 255), 3)
             Q = print("not too close")
             print('LEFT GREEN', Q)
          
         elif GareaL > 400  and GareaL < 1000:
             cv2.drawContours(capLeft, contour, -1, (255, 0 , 0), 3)
             W = print("far")
             print('LEFT GREEN', W)
             
	cv2.imshow("LIVE", image)
	cv2.imshow("red.R", red_mask)
	cv2.imshow("green.L", green_mask2)
	cv2.imshow("red.L", red_mask2)
	cv2.imshow("green.R", green_mask)
	
	cv2.imshow("OUTPUT.R.L", outputRL)
	cv2.imshow("OUTPUT.G.R", outputGR)
	cv2.imshow("OUTPUT.R.R", outputRR)
	cv2.imshow("OUTPUT.G.L", outputGL)
	cv2.imshow("cap left", capLeft)
	cv2.imshow("cap right", capRight)
	key = cv2.waitKey(1)
	rawCapture.truncate(0)
	if key == 27:
		break

cv2.destroyAllWindows()






