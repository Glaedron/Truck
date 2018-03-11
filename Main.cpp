#include <iostream>
#include "Truck.h"
//#include "Camera.h"

int main()
{
  wiringPiSetup();

  Truck LegoTruck;
//  Camera Cam;

//  Cam.Test ();

  LegoTruck.DisplayTest ();

//  LegoTruck.Test ();

//  LegoTruck.PWMTest ();

//  LegoTruck.Drive();
}
