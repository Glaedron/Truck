#include "Truck.h"

class Mainloop
{
  public:

    Mainloop ();
    ~Mainloop ();

    bool Init ();
    void Test ();

    void Input ();
    void Update ();
    void Render ();

    bool Run = false;

  private:

    static bool init;

    SDL_Event _Event;
    SDL_DisplayMode _Current;

    SDL_GameController* _Controller = nullptr;
    SDL_Window* _Window = nullptr;
    SDL_Renderer* _Renderer = nullptr;

    Truck* _LegoTruck = nullptr;

    std::stringstream _StrStr;
    std::string Time;
    int sec = 0;
    int min = 0;
    int h = 0;
    long double RunningTime = 0;
    
    bool _ControllerConnected = false;

    Timer _RunningTime;
    Timer _GameTime;
    Sprite _Time;
    Sprite _FPS;
    Sprite _GameController;
};

bool Mainloop::init = 0;

Mainloop::Mainloop ()
{
}

Mainloop::~Mainloop ()
{
  SDL_DestroyWindow (_Window);
  SDL_DestroyRenderer (_Renderer);

  if (_Controller)
  {
    SDL_GameControllerClose (_Controller);
  }

  _LegoTruck -> Stop ();

  delete _LegoTruck;

  SDL_Quit ();
}

bool Mainloop::Init ()
{
  if (init == 0)
  {
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) == -1)
    {
      std::cout <<" error vid/timer/controller"<< std::endl;

      return 0;
    }

    if (IMG_Init (IMG_INIT_PNG) == -1)
    {
      std::cout <<" error png"<< std::endl;

      return 0;
    }

    if (TTF_Init() == -1)
    {
      std::cout <<" error ttf"<< std::endl;

      return 0;
    }

    init = 1;
  }

  for (int i = 0; i < SDL_GetNumVideoDisplays (); i++)
  {
    if (SDL_GetCurrentDisplayMode (i, &_Current) == 0)
    {
      _Current.h -= 70;
      _Window = SDL_CreateWindow ("*TRUCK*", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _Current.w, _Current.h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    }
  }
	
  _Renderer = SDL_CreateRenderer (_Window, -1, SDL_RENDERER_ACCELERATED);

  for (int i = 0; i < SDL_NumJoysticks (); ++i)
  {
    if (SDL_IsGameController (i))
    {
      _Controller = SDL_GameControllerOpen(i);

      if (_Controller)
      {
        break;
      }

      else
      {
        std::cout <<"Could not open gamecontroller " << std::endl;

        return 0;
      }
    }
  }

  _LegoTruck = new Truck (_Renderer);
  _Time = Sprite (_Renderer);
  _FPS = Sprite (_Renderer);
  _GameController = Sprite (_Renderer);

  Run = true;

  return 1;
}

void Mainloop::Test ()
{
  _LegoTruck -> SetModeTest ();
}

void Mainloop::Input ()
{
  SDL_PollEvent (&_Event);

  _LegoTruck -> Input (_Event);

  switch (_Event.type)
  {
    case SDL_QUIT:
    {
      Run = false;

      _LegoTruck -> SetModeControlled ();
      _LegoTruck -> Stop ();

      break;
    }

    case SDL_KEYDOWN:
    {
      switch (_Event.key.keysym.sym)
      {
        case SDLK_ESCAPE:
        {
          Run = false;

          std::cout <<"ESC"<< std::endl;

          _LegoTruck -> SetModeControlled ();
          _LegoTruck -> Stop ();

          break;
        }
      }

      break;
    }

    case SDL_CONTROLLERDEVICEADDED:
    {
      break;
    }

    case SDL_CONTROLLERDEVICEREMOVED:
    {
      break;
    }

    case SDL_CONTROLLERBUTTONDOWN:
    {
      switch (_Event.cbutton.button)
      {
        case SDL_CONTROLLER_BUTTON_B:
        {
          Run = false;

          _LegoTruck -> SetModeControlled ();
          _LegoTruck -> Stop ();

          break;
        }
      }

      break;
    }
  }
}

void Mainloop::Update ()
{
  _Time.SetTextPos (140, 100);
  _FPS.SetTextPos (140, 200);
  _GameController.SetTextPos (140, 400);
  _LegoTruck -> SetPos ((_Current.w / 4) - (_LegoTruck -> GetRect ().w / 2), (_Current.h / 2) - (_LegoTruck -> GetRect ().h / 2));
  _LegoTruck -> Update ();

  _RunningTime.Update ();

  RunningTime += _RunningTime.GetElapsedMilliSeconds ();

  if (RunningTime >= 1000)
  {
    RunningTime -= 1000;
    sec += 1;
  }

  if (sec > 59)
  {
    sec -= 60;
    min += 1;
  }

  if (min > 59)
  {
    min -= 60;
    h += 1;
  }

  _StrStr << h <<":"<< min <<":"<< sec;

  Time = _StrStr.str ();

  _StrStr.str ("");
}

void Mainloop::Render ()
{
  SDL_SetRenderDrawColor (_Renderer, 0, 0, 200, 255);
  SDL_RenderClear (_Renderer);

  SDL_SetRenderDrawColor (_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine (_Renderer, (_Current.w / 2), 0, (_Current.w / 2), _Current.h);

  _LegoTruck -> Render ();

  _Time.RenderText (Time);

  if (_LegoTruck -> GetModeSelf () == true)
  {
    _GameTime.FPS (15);
  }

  else if (_LegoTruck -> GetModeControlled () == true)
  {
    _GameTime.FPS (60);
  }

  _FPS.RenderText (_GameTime.GetFPS ());

  if (_Controller)
  {
    _GameController.RenderText ("GC: added");
  }

  else if (!_Controller)
  {
    _GameController.RenderText ("GC: removed");
  }

  else 
  {
    _GameController.RenderText ("GC: undefined");
  }

  SDL_RenderPresent (_Renderer);
}