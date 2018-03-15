#include <wiringPi.h>
#include <softPwm.h>
#include <sys/time.h>
#include "Sensor.h"
#include "Engine.h"
#include "Wheel.h"
#include "Switch.h"
#include "Interface.h"

class Truck
{
  public:

    Truck ();
    ~Truck ();

    void Drive();
    void Stop();
    void DisplayTest();
    void Test();
    void PWMTest();

  private:

    long double _ValRight = 0;
    long double _ValFrontRight = 0;
    long double _ValFront = 0;
    long double _ValFrontLeft = 0;
    long double _ValLeft = 0;


    Sensor _Right;
    Sensor _FrontRight;
    Sensor _Front;
    Sensor _FrontLeft;
    Sensor _Left;


    Wheel _Wheel;
    Engine _Engine;
    Engine _PWMEngine;
    Switch _Power;
    Interface *_View = nullptr;

    int _count;
};

Truck::Truck ()
{
  _Right = Sensor (28, 27, &_ValRight);
  _FrontRight = Sensor (11, 31, &_ValFrontRight);
  _Front = Sensor (10, 6, &_ValFront);
  _FrontLeft = Sensor (5, 4, &_ValFrontLeft);
  _Left = Sensor (16, 15, &_ValLeft);

  _Wheel = Wheel (24, 25, 26);
  _Engine = Engine (22, 23);
  _PWMEngine = Engine (22, 23, 21);
  _Power = Switch (0);
  _View = new Interface (&_ValLeft, &_ValFrontLeft, &_ValFront, &_ValFrontRight, &_ValRight);
}

Truck::~Truck ()
{
  delete _View;
}

void Truck::Drive()
{
  while (1)
  {
    while (_Power.GetState() == true)
    {
      _Right.Measure ();
      _FrontRight.Measure ();
      _Front.Measure ();
      _FrontLeft.Measure ();
      _Left.Measure ();
     
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
  //for (int counter = 0; counter < 1000; counter++)

  while (_View -> Run == true)
  {
    _Left.Measure ();
    _FrontLeft.Measure ();
    _Front.Measure ();
    _FrontRight.Measure ();
    _Right.Measure ();

    _View -> Display ();

    //std::cout << _ValRight << std::endl;

    delay (250);
  }

  delete _View;
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