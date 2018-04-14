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
    
    Timer _RunningTime;
    Sprite _Time;
};

bool Mainloop::init = 0;

Mainloop::Mainloop ()
{
}

Mainloop::~Mainloop ()
{
  SDL_DestroyWindow (_Window);
  SDL_DestroyRenderer (_Renderer);
  SDL_GameControllerClose (_Controller);

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
      std::cout <<" error vid/timer"<< std::endl;

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
      _Window = SDL_CreateWindow ("*TRUCK*", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _Current.w, _Current.h, SDL_WINDOW_RESIZABLE);
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

  //std::cout <<SDL_GameControllerAddMappingsFromFile ("/home/pi/Truck/Unterlagen/Mappings.txt")<< std::endl;

  _LegoTruck = new Truck (_Renderer, _Controller);
  _Time = Sprite (_Renderer);

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

          _LegoTruck -> SetModeControlled ();
          _LegoTruck -> Stop ();

          break;
        }
      }

      break;
    }

    case SDL_CONTROLLERBUTTONDOWN:
    {
      switch (_Event.cbutton.button)
      {
        case SDL_CONTROLLER_BUTTON_X:
        {
          Run = false;

          break;
        }
      }

      break;
    }
  }
}

void Mainloop::Update ()
{
  Crashlog.Write ("MainloopUpdate1");

  _LegoTruck -> SetPos ((_Current.w / 4) - (_LegoTruck -> GetRect ().w / 2), (_Current.h / 2) - (_LegoTruck -> GetRect ().h / 2));
  _LegoTruck -> Update ();

  Crashlog.Write ("MainloopUpdate2");

  _RunningTime.Update ();

  Crashlog.Write ("MainloopUpdate3");

  RunningTime += _RunningTime.GetElapsedMilliSeconds ();

  _Time.SetTextPos (100, 100);

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

  Crashlog.Write ("MainloopUpdate4");
}

void Mainloop::Render ()
{
  Crashlog.Write ("MainloopRender1");

  SDL_SetRenderDrawColor (_Renderer, 0, 0, 200, 255);
  SDL_RenderClear (_Renderer);

  Crashlog.Write ("MainloopRender2");

  SDL_SetRenderDrawColor (_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine (_Renderer, (_Current.w / 2), 0, (_Current.w / 2), _Current.h);

  Crashlog.Write ("MainloopRender3");

  _LegoTruck -> Render ();

  Crashlog.Write ("MainloopRender4");

  _Time.RenderText (Time);

  Crashlog.Write ("MainloopRender5");

  SDL_RenderPresent (_Renderer);

  Crashlog.Write ("MainloopRender6");
}