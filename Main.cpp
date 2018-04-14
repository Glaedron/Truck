//g++ -std=c++0x -Wall -pedantic /home/pi/Truck/Main.cpp -o /home/pi/Truck/Truck -lwiringPi -lpthread `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lopencv_imgproc -lopencv_highgui -lopencv_core -lopencv_videoio


#include <iostream>
#include "Logfile.h"

Logfile Crashlog ("Crashlog");

#include "Mainloop.h"

int main()
{
  wiringPiSetup();

  Mainloop LegoTruck;

  LegoTruck.Init ();

  //LegoTruck.Test ();

  Timer FPS, FPSLimitation;

  while (LegoTruck.Run)
  {
    Crashlog.Reset ();

    LegoTruck.Input ();

    Crashlog.Write ("MainloopInput");

    LegoTruck.Update ();

    Crashlog.Write ("MainloopUpdate");

    LegoTruck.Render ();

    Crashlog.Write ("MainloopRender");

    FPSLimitation.Update ();
    
    SDL_Delay ((1000 / 20) - FPSLimitation.GetElapsedMilliSeconds ());

    FPS.Update ();
    //std::cout << 1000 / FPS.GetElapsedMilliSeconds () << std::endl;
  }

}
