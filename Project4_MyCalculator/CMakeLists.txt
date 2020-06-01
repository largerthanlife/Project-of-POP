cmake_minimum_required(VERSION 3.16)
#project name
PROJECT(mycalculator)

#head file path
INCLUDE_DIRECTORIES(
)

#source directory
AUX_SOURCE_DIRECTORY(./source/ DIR_SRSC)

#add sub directory
#ADD_SUBDIRECTORY(source)

#set extern libraries
#SET(PROJECT_PATH ${src})

#add excutable file
ADD_EXECUTABLE(mycal ${DIR_SRSC})

#add link library
#TARGET_LINK_LIBRARIES()

#TARGET_LINK_LIBRARIES(retinaface /usr/local/lib/libreadline.so /usr/local/lib/libhissrctory.so)
TARGET_LINK_LIBRARIES(mycal /usr/local/lib/libreadline.so -lncurses) 
