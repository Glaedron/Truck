#include <iostream>
#include "Mainloop.h"
#include "Timer.h"

int main()
{
  wiringPiSetup();

  Mainloop LegoTruck;

  LegoTruck.Init ();

  //LegoTruck.Test ();

  Timer FPS, FPSLimitation;

  while (LegoTruck.Run)
  {
    LegoTruck.Input ();
    LegoTruck.Update ();
    LegoTruck.Render ();

    FPSLimitation.Update ();
    
    //SDL_Delay ((1000 / 20) - FPSLimitation.GetElapsedMilliSeconds ());

    FPS.Update ();
    //std::cout << 1000 / FPS.GetElapsedMilliSeconds () << std::endl;
  }

}
