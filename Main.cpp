//g++ -std=c++0x -Wall -pedantic /home/pi/Truck/Main.cpp -o /home/pi/Truck/Truck -lwiringPi -lpthread `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lopencv_imgproc -lopencv_highgui -lopencv_core -lopencv_videoio


#include <iostream>
#include "Logfile.h"
#include "Mainloop.h"

int main()
{
  wiringPiSetup();

  Mainloop LegoTruck;

  LegoTruck.Init ();

  //LegoTruck.Test ();

  while (LegoTruck.Run)
  {
    LegoTruck.Input ();
    LegoTruck.Update ();
    LegoTruck.Render ();
  }
}
