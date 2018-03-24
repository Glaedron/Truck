#include <iostream>
#include "Mainloop.h"

int main()
{
  wiringPiSetup();

  Mainloop LegoTruck;

  LegoTruck.Init ();

//  LegoTruck.Test ();

  while (LegoTruck.Run)
  {
    LegoTruck.Input ();
    LegoTruck.Update ();
    LegoTruck.Render ();
  }
}
