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

    Truck (SDL_Renderer *renderer, SDL_GameController* controller);
    ~Truck ();

    void Stop ();

    void SetPos (int x, int y);

    SDL_Rect GetRect ()
    {
      return _TruckRect;
    }

    void SetModeSelf ()
    {
      ModeTest = 0;
      ModeControlled = 0;
      ModeSelf = 1;

      Stop ();
    }

    void SetModeControlled ()
    {
      ModeTest = 0;
      ModeSelf = 0;
      ModeControlled = 1;

      Stop ();
    }

    void SetModeTest ()
    {
      ModeControlled = 0;
      ModeSelf = 0;
      ModeTest = 1;

      Stop ();
    }

    void Input (SDL_Event event);
    void Update ();
    void Render ();

    void Test();
    void PWMTest();
    void WheelTest ();

  private:

    void Drive ();

    SDL_GameController* _Controller = nullptr;
    SDL_Joystick* _Joystick = nullptr;

    SDL_Texture* _Truck;
    SDL_Rect _TruckRect;

    SDL_Renderer *_Renderer = nullptr;

    Sensor _Right;
    Sensor _FrontRight;
    Sensor _Front;
    Sensor _FrontLeft;
    Sensor _Left;

    Wheel _Wheel;
    Engine _PWMEngine;
    Engine _Engine;
    Switch _Power;

    bool ModeSelf = 0;
    bool ModeControlled = 0;
    bool ModeTest = 0;
};

Truck::Truck (SDL_Renderer *renderer, SDL_GameController* controller)
{
  _Renderer = renderer;
  _Controller = controller;

  _Joystick = SDL_GameControllerGetJoystick (_Controller);

  _Right = Sensor (28, 27, _Renderer);
  _FrontRight = Sensor (11, 31, _Renderer);
  _Front = Sensor (10, 6, _Renderer);
  _FrontLeft = Sensor (5, 4, _Renderer);
  _Left = Sensor (16, 15, _Renderer);

  _Wheel = Wheel (12, 14, 25);
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
    if (_Front.GetVal () > 60 && _Right.GetVal () > 10 && _Left.GetVal () > 10)
    {
      _Engine.Forward();
      _Wheel.Middle();
    }

    if ((_Front.GetVal () > 20 && _Front.GetVal () < 60 && _Right.GetVal () > _Left.GetVal ()) || (_Front.GetVal () > 20 && _Left.GetVal () < 10))
    {
      _Engine.Forward();
      _Wheel.Right(7);
    }

    if ((_Front.GetVal () > 20 && _Front.GetVal () < 60 && _Left.GetVal () > _Right.GetVal ()) || (_Front.GetVal () > 20 && _Right.GetVal () < 10))
    {
      _Engine.Forward();
      _Wheel.Left(7);
    }
/*
    if (_Front.GetVal () < 20)
    {
      while (_Front.GetVal () <= 40)
      {
        if (_Front.GetVal () < 40 && _Right.GetVal () > _Left.GetVal ())
	{
	  _Engine.Backward();
	  _Wheel.Right(7);
	}

        if (_Front.GetVal () < 40 && _Left.GetVal () > _Right.GetVal ())
	{
	  _Engine.Backward();
	  _Wheel.Left(7);
	}
      }
    }
*/
  }
}

void Truck::Stop()
{
  _Wheel.Middle ();
  _Engine.Stop ();
  _PWMEngine.Stop ();
}

void Truck::SetPos (int x, int y)
{
  _TruckRect.x = x;
  _TruckRect.y = y;
}

void Truck::Input (SDL_Event event)
{
  switch (event.type)
  {
    case SDL_CONTROLLERBUTTONDOWN:
    {
      switch (event.cbutton.button)
      {
        case SDL_CONTROLLER_BUTTON_A:
        {
        }

        case SDL_CONTROLLER_BUTTON_B:
        {
        }

        case SDL_CONTROLLER_BUTTON_X:
        {
          SetModeSelf ();

          break;
        }

        case SDL_CONTROLLER_BUTTON_Y:
        {
          SetModeControlled ();

          break;
        }
      }

      break;
    }

  }
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
    Drive ();
  }

  else if (ModeControlled == 1)
  {
  }

  else if (ModeTest == 1)
  {
    Test ();

    PWMTest ();

    WheelTest ();

    SetModeSelf ();
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

  delay (1000);

  _Engine.Backward ();

  delay (1000);

  Stop ();

  _Wheel.Right (7);

  delay (1000);

  _Wheel.Middle ();

  delay (1000);

  _Wheel.Left (7);

  delay (1000);

  _Wheel.Middle ();

  delay (1000);

  Stop ();
}

void Truck::PWMTest()
{
  for (int forward = 15; forward < 100; forward++)
  {
    _PWMEngine.Forward ();
    _PWMEngine.SetSpeed (forward);

    delay (100);
  }

  Stop ();
  delay (1000);

  for (int backward = 15; backward < 100; backward++)
  {
    _PWMEngine.Backward ();
    _PWMEngine.SetSpeed (backward);

    delay (100);
  }

  Stop ();
}

void Truck::WheelTest ()
{
  for (int counterL = 0; counterL <= 7; counterL++)
  {
    _Wheel.Left (counterL);
    delay (500);
  }

  _Wheel.Middle ();

  delay (500);

  for (int counterR = 0; counterR <= 7; counterR++)
  {
    _Wheel.Right (counterR);
    delay (500);
  }

  Stop ();
}