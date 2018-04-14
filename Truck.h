#include <wiringPi.h>
#include <softPwm.h>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2//SDL_ttf.h>
#include <sstream>
#include "Timer.h"
#include "Sprite.h"
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
      return _Truck.GetSpriteRect ();
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

  private:

    void Self ();
    void Controlled ();

    SDL_GameController* _Controller = nullptr;

    SDL_Renderer *_Renderer = nullptr;

    Sprite _Truck;
    Sprite _Mode;

    Sensor _Right;
    Sensor _FrontRight;
    Sensor _Front;
    Sensor _FrontLeft;
    Sensor _Left;

    Wheel _Wheel;
    Engine _Engine;
    Switch _Power;

    float _SpeedF;
    float _SpeedB;

    int _WheelPosL;
    int _WheelPosR;

    bool ModeSelf = 0;
    bool ModeControlled = 0;
    bool ModeTest = 0;

    std::string Mode;
};

Truck::Truck (SDL_Renderer *renderer, SDL_GameController* controller)
{
  _Renderer = renderer;
  _Controller = controller;

  _Right = Sensor (28, 27, _Renderer);
  _FrontRight = Sensor (11, 31, _Renderer);
  _Front = Sensor (10, 6, _Renderer);
  _FrontLeft = Sensor (5, 4, _Renderer);
  _Left = Sensor (16, 15, _Renderer);

  _Wheel = Wheel (12, 14, 25);
  _Engine = Engine (0, 3, 23);
  _Power = Switch (9);

  _Truck = Sprite (_Renderer);
  _Mode = Sprite (_Renderer);
  _Truck.Load ("Unterlagen/Truck.png");

  if (!ModeTest)
  {
    SetModeSelf ();
  }
}

Truck::~Truck ()
{
}

void Truck::Self ()
{
  Mode = "Selfdriving";

  _Engine.SetSpeed (100);

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

void Truck::Controlled ()
{
  Mode = "Controlled";

  if (_SpeedF > 15 && _SpeedF < 50)
  {
    _SpeedF = 50;
  }

  if (_SpeedF <= 15)
  {
    _SpeedF = 0;
  }

  if (_SpeedF >= 50)
  {
    _Engine.SetSpeed (_SpeedF);
    _Engine.Forward ();
  }

  if (_SpeedF == 0)
  {
    _Engine.Stop ();
  }




  if (_SpeedB > 15 && _SpeedB < 50)
  {
    _SpeedB = 50;
  }

  if (_SpeedB <= 15)
  {
    _SpeedB = 0;
  }

  if (_SpeedB >= 50)
  {
    _Engine.SetSpeed (_SpeedB);
    _Engine.Backward ();
  }

  if (_SpeedB == 0)
  {
    _Engine.Stop ();
  }




  if (_WheelPosL < 3)
  {
    _WheelPosL = 0;
  }

  if (_WheelPosL >= 3)
  {
    _Wheel.Left (_WheelPosL);
  }




  if (_WheelPosR < 3)
  {
    _WheelPosR = 0;
  }

  if (_WheelPosR >= 3)
  {
    _Wheel.Right (_WheelPosR);
  }
}

void Truck::Stop()
{
  _Wheel.Middle ();
  _Engine.Stop ();
}

void Truck::SetPos (int x, int y)
{
  _Truck.SetSpritePos (x, y);
}

void Truck::Input (SDL_Event event)
{
  switch (event.type)
  {
    case SDL_CONTROLLERBUTTONDOWN:
    {
      switch (event.cbutton.button)
      {
        case SDL_CONTROLLER_BUTTON_Y:
        {
          if (ModeControlled == 0 && ModeTest == 0)
          {
            SetModeControlled ();
            std::cout <<" Controlled "<< std::endl;
          }

          else if (ModeSelf == 0 && ModeTest == 0)
          {
            SetModeSelf ();
            std::cout <<" Self "<< std::endl;
          }

          break;
        }
      }

      break;
    }

    case SDL_CONTROLLERAXISMOTION:
    {
      switch (event.caxis.axis)
      {
        case SDL_CONTROLLER_AXIS_LEFTY:
        {
          if (event.caxis.value < 0 && event.caxis.value > -32768)
          {
            _SpeedB = 0;
            _SpeedF = (event.caxis.value * 100) / -32768;
          }

          if (event.caxis.value > 0 && event.caxis.value < 32767)
          {
            _SpeedF = 0;
            _SpeedB = (event.caxis.value * 100) / 32767;
          }

          break;
        }

        case SDL_CONTROLLER_AXIS_RIGHTX:
        {
          if (event.caxis.value < 0 && event.caxis.value > -32768)
          {
            _WheelPosR = 0;
            _WheelPosL = (int)(event.caxis.value * 7) / -32768;
          }

          if (event.caxis.value > 0 && event.caxis.value < 32767)
          {
            _WheelPosL = 0;
            _WheelPosR = (int)(event.caxis.value * 7) / 32767;
          }

          break;
        }
      }

      break;
    }
  }
}

void Truck::Update ()
{
  if (ModeSelf == 1)
  {
    _Left.SetPos (_Truck.GetSpriteRect ().x, (_Truck.GetSpriteRect ().h / 2) + _Truck.GetSpriteRect ().x);
    _FrontLeft.SetPos (_Truck.GetSpriteRect ().x, _Truck.GetSpriteRect ().y);
    _Front.SetPos ((_Truck.GetSpriteRect ().x) + (_Truck.GetSpriteRect ().w / 2), _Truck.GetSpriteRect ().y);
    _FrontRight.SetPos ((_Truck.GetSpriteRect ().x) + (_Truck.GetSpriteRect ().w), _Truck.GetSpriteRect ().y);
    _Right.SetPos ((_Truck.GetSpriteRect ().x) + (_Truck.GetSpriteRect ().w), (_Truck.GetSpriteRect ().h / 2) + _Truck.GetSpriteRect ().x);

    _Left.Update ();
    _FrontLeft.Update ();
    _Front.Update ();
    _FrontRight.Update ();
    _Right.Update ();

    Self ();
  }

  else if (ModeControlled == 1)
  {
    Controlled ();
  }

  else if (ModeTest == 1)
  {
    Test ();

    SetModeSelf ();
  }

  _Mode.SetTextPos (100, 300);
}

void Truck::Render ()
{
  _Truck.RenderSprite ();

  _Mode.RenderText (Mode);

  if (ModeSelf == 1)
  {
    _Left.Render ();
    _FrontLeft.Render ();
    _Front.Render ();
    _FrontRight.Render ();
    _Right.Render ();
  }
}

void Truck::Test()
{
  Mode = "Test";

  for (int forward = 15; forward < 100; forward++)
  {
    _Engine.Forward ();
    _Engine.SetSpeed (forward);

    delay (100);
  }

  Stop ();
  delay (1000);

  for (int backward = 15; backward < 100; backward++)
  {
    _Engine.Backward ();
    _Engine.SetSpeed (backward);

    delay (100);
  }

  Stop ();

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