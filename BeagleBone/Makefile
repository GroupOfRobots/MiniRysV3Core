CXX=g++
CXXFLAGS=-Wall -Wextra
LFLAGS=-lpthread

OBJS=build/battery.o
OBJS+= build/controller.o
# OBJS+= build/newserial.o
OBJS+= build/MPU6050.o
OBJS+= build/IMU.o
OBJS+= build/I2Cdev.o
OBJS+= build/Motors.o
OBJS+= build/TestApplication.o
OBJS+= build/sonar.o

all: build/testApp

build/testApp: $(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) -o build/testApp

build/battery.o: Battery/battery.cpp Battery/battery.h
	$(CXX) $(CXXFLAGS) -c Battery/battery.cpp -o build/battery.o

build/controller.o: Controller/controller.cpp Controller/controller.h
	$(CXX) $(CXXFLAGS) -c Controller/controller.cpp -o build/controller.o

# build/newserial.o: newserial.cpp newserial.h
# 	$(CXX) $(CXXFLAGS) -c newserial.cpp -o build/newserial.o

build/MPU6050.o: IMU/MPU6050.cpp IMU/MPU6050.h
	$(CXX) $(CXXFLAGS) -c IMU/MPU6050.cpp -o build/MPU6050.o

build/IMU.o: IMU/IMU.cpp IMU/IMU.h
	$(CXX) $(CXXFLAGS) -c IMU/IMU.cpp -o build/IMU.o

build/I2Cdev.o: IMU/I2Cdev.cpp IMU/I2Cdev.h
	$(CXX) $(CXXFLAGS) -c IMU/I2Cdev.cpp -o build/I2Cdev.o

build/Motors.o: Motors/Motors.cpp Motors/Motors.h
	$(CXX) $(CXXFLAGS) -c Motors/Motors.cpp -o build/Motors.o

build/TestApplication.o: examples/TestApplication.cpp
	$(CXX) $(CXXFLAGS) -c examples/TestApplication.cpp -o build/TestApplication.o

build/sonar.o: Sonar/sonar.cpp Sonar/sonar.h
	$(CXX) $(CXXFLAGS) -c Sonar/sonar.cpp -o build/sonar.o

clean:
	rm build/*

.PHONY: all clean
