#include <wiringPi.h>
#include <softPwm.h>
#include <sys/time.h>
#include "Sensor.h"
#include "Engine.h"
#include "Wheel.h"
#include "Switch.h"
#include "GUI.h"

class Truck
{
  public:

    Truck ();

    void Drive();
    void Stop();
    void DisplayTest();
    void Test();
    void PWMTest();

  private:

    long double _ValRight;
    long double _ValFrontRight;
    long double _ValFront;
    long double _ValFrontLeft;
    long double _ValLeft;


    Sensor _Right;
    Sensor _FrontRight;
    Sensor _Front;
    Sensor _FrontLeft;
    Sensor _Left;


    Wheel _Wheel;
    Engine _Engine;
    Engine _PWMEngine;
    Switch _Power;
    GUI _View;

    void Display ();

    int _count;
};

Truck::Truck ()
{
  _Right = Sensor (11, 12);
  _FrontRight = Sensor (13, 14);
  _Front = Sensor (7, 8);
  _FrontLeft = Sensor (15, 16);
  _Left = Sensor (9, 10);

  _Wheel = Wheel (24, 25, 26);
  _Engine = Engine (22, 23);
  _PWMEngine = Engine (22, 23, 21);
  _Power = Switch (0);
}

void Truck::Drive()
{
  while (1)
  {
    while (_Power.GetState() == true)
    {
      _ValRight = _Right.GetDistance ();
      _FrontRight.GetDistance ();
      _ValFront = _Front.GetDistance ();
      _ValFrontLeft = _FrontLeft.GetDistance ();
      _ValLeft = _Left.GetDistance ();
     
      std::cout <<" run "<< std::endl;

      if (_ValFront > 60 && _ValRight > 10 && _ValLeft > 10)
      {
        _Engine.Forward();
        _Wheel.Middle();
      }

      if ((_ValFront > 20 && _ValFront < 60 && _ValRight > _ValLeft) || (_ValFront > 20 && _ValLeft < 10))
      {
        _Engine.Forward();
	_Wheel.Right();
      }

      if ((_ValFront > 20 && _ValFront < 60 && _ValLeft > _ValRight) || (_ValFront > 20 && _ValRight < 10))
      {
	_Engine.Forward();
	_Wheel.Left();
      }
/*
      if (_ValFront < 20)
      {
        while (_ValFront <= 40)
	{
	  if (_ValFront < 40 && _ValRight > _ValLeft)
	  {
	    _Engine.Backward();
	    _Wheel.Right();
	  }

          if (_ValFront < 40 && _ValLeft > _ValRight)
	  {
	    _Engine.Backward();
	    _Wheel.Left();
	  }
	}
      }*/
    }

    Stop();

    std::cout <<" stop "<< std::endl;
  }
}

void Truck::Stop()
{
  _Wheel.Stop ();
  _Engine.Stop ();
  _PWMEngine.Stop ();
}

void Truck::DisplayTest()
{
  for (int counter = 0; counter < 1000; counter++)
  {
    Display ();
  }
}

void Truck::Test()
{
  _Engine.Forward ();

  delay (5000);

  _Engine.Backward ();

  delay (5000);

  Stop ();

  _Wheel.Right ();

  delay (5000);

  _Wheel.Middle ();

  delay (5000);

  _Wheel.Left ();

  delay (5000);

  _Wheel.Middle ();

  delay (5000);

  Stop ();
}

void Truck::PWMTest()
{
  for (int forward = 15; forward < 100; forward++)
  {
    _PWMEngine.Forward ();
    _PWMEngine.SetSpeed (forward);

    delay (200);
  }

  Stop ();
  delay (1000);

  for (int backward = 15; backward < 100; backward++)
  {
    _PWMEngine.Backward ();
    _PWMEngine.SetSpeed (backward);

    delay (200);
  }

  Stop ();
}

void Truck::Display ()
{
  _View.Display (_count /*_Right.GetDistance ()*/,0 /* _FrontRight.GetDistance ()*/, _Front.GetDistance (),0 /* _FrontLeft.GetDistance ()*/,0 /* _Left.GetDistance ()*/);

  delay (250);

  _count++;
}