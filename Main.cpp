#include <iostream>
#include "Mainloop.h"

int main()
{
  wiringPiSetup();

  Mainloop LegoTruck;

  LegoTruck.Init ();

  while (LegoTruck.Run)
  {
    LegoTruck.Input ();
    LegoTruck.Update ();
    LegoTruck.Render ();
  }
}
