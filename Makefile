all:
	g++ motion_detection.cpp -o hw4_exe -lopencv_highgui -lopencv_core -lstdc++ -lopencv_imgproc -lopencv_video 



clean:
	rm motion_detection_exe
