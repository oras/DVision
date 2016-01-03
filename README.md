# DVision

This is a research project in computer vision to deteced events of interest using APMplanner 2.0. and OpenCV computer vision library. First we will focus on fire detection and alarm using UAV and ground control app. This will include new Controls and Settings GUI operator screen.

# APMplanner 2.0 Events Detection Version Installation process

look for the orginal APMplanner installation process on their wiki page:
(!!!Just change the name of the repository name when doing the clone request command!!!)

https://github.com/diydrones/apm_planner/blob/master/README.md

# Opencv Installation Process (Linux):

http://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html

# Setting up OpenCV in QT:

We need to add the following lines to the "qgroundcontrol.pro" file inorder to set the QT linker modify OpenCV LIBS:

INCLUDEPATH += /usr/local/include/opencv

LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui




