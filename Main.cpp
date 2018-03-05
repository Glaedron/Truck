#include <iostream>
#include "Truck.h"

int main()
{
  wiringPiSetup();

  Truck LegoTruck;

  LegoTruck.DisplayTest ();

//  LegoTruck.Test ();

//  LegoTruck.PWMTest ();

//  LegoTruck.Drive();
}
