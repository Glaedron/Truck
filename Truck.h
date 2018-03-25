#include <wiringPi.h>
#include <softPwm.h>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2//SDL_ttf.h>
#include <sstream>
#include "Sensor.h"
#include "Engine.h"
#include "Wheel.h"
#include "Switch.h"

class Truck
{
  public:

    Truck (SDL_Renderer *renderer);
    ~Truck ();

    void Stop ();

    void SetPos (int x, int y);

    SDL_Rect GetRect ()
    {
      return _TruckRect;
    }

    void SetModeSelf ()
    {
      ModeControlled = 0;
      ModeSelf = 1;
    }

    void SetModeControlled ()
    {
      ModeSelf = 0;
      ModeControlled = 1;
    }

    void Update ();
    void Render ();

    void Test();
    void PWMTest();

    Wheel _Wheel;
    Engine _PWMEngine;

    long double _ValRight = 0;
    long double _ValFrontRight = 0;
    long double _ValFront = 0;
    long double _ValFrontLeft = 0;
    long double _ValLeft = 0;

  private:

    void Drive ();

    SDL_Texture* _Truck;
    SDL_Rect _TruckRect;

    SDL_Renderer *_Renderer = nullptr;

    Sensor _Right;
    Sensor _FrontRight;
    Sensor _Front;
    Sensor _FrontLeft;
    Sensor _Left;

    Engine _Engine;
    Switch _Power;

    Timer _Timer;
    int _count;

    bool ModeSelf = 0;
    bool ModeControlled = 0;
};

Truck::Truck (SDL_Renderer *renderer)
{
  _Renderer = renderer;

  _Right = Sensor (28, 27, _Renderer);
  _FrontRight = Sensor (11, 31, _Renderer);
  _Front = Sensor (10, 6, _Renderer);
  _FrontLeft = Sensor (5, 4, _Renderer);
  _Left = Sensor (16, 15, _Renderer);

  _Wheel = Wheel (12, 14, 7);
  _Engine = Engine (0, 3);
  _PWMEngine = Engine (0, 3, 23);
  _Power = Switch (9);

  _Truck = IMG_LoadTexture (_Renderer, "Unterlagen/Truck.png");
  SDL_QueryTexture (_Truck, 0, 0, &_TruckRect.w, &_TruckRect.h);
}

Truck::~Truck ()
{
}

void Truck::Drive()
{
  if (_Power.GetState() == true)
  {
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

void Truck::Stop()
{
  _Wheel.Stop ();
  _Engine.Stop ();
  _PWMEngine.Stop ();
}

void Truck::SetPos (int x, int y)
{
  _TruckRect.x = x;
  _TruckRect.y = y;
}

void Truck::Update ()
{
    _Left.SetPos (_TruckRect.x, (_TruckRect.h / 2) + _TruckRect.x);
    _FrontLeft.SetPos (_TruckRect.x, _TruckRect.y);
    _Front.SetPos ((_TruckRect.x) + (_TruckRect.w / 2), _TruckRect.y);
    _FrontRight.SetPos ((_TruckRect.x) + (_TruckRect.w), _TruckRect.y);
    _Right.SetPos ((_TruckRect.x) + (_TruckRect.w), (_TruckRect.h / 2) + _TruckRect.x);

    _Left.Update ();
    _FrontLeft.Update ();
    _Front.Update ();
    _FrontRight.Update ();
    _Right.Update ();

    if (ModeSelf == 1)
    {
      //Drive ();
    }

    else if (ModeControlled == 1)
    {
    }
}

void Truck::Render ()
{
  SDL_RenderCopy (_Renderer, _Truck, NULL, &_TruckRect);

  _Left.Render ();
  _FrontLeft.Render ();
  _Front.Render ();
  _FrontRight.Render ();
  _Right.Render ();
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