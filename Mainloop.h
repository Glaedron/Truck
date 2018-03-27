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

  _LegoTruck = new Truck (_Renderer, _Controller);

  Run = true;

  return 1;
}

void Mainloop::Test ()
{
//  _LegoTruck -> Test ();

//  _LegoTruck -> PWMTest ();

  _LegoTruck -> WheelTest ();
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

      break;
    }

    case SDL_KEYDOWN:
    {
      switch (_Event.key.keysym.sym)
      {
        case SDLK_ESCAPE:
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
  _LegoTruck -> SetPos ((_Current.w / 4) - (_LegoTruck -> GetRect ().w / 2), (_Current.h / 2) - (_LegoTruck -> GetRect ().h / 2));
  _LegoTruck -> Update ();
}

void Mainloop::Render ()
{
  SDL_SetRenderDrawColor (_Renderer, 0, 0, 200, 255);
  SDL_RenderClear (_Renderer);

  SDL_SetRenderDrawColor (_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine (_Renderer, (_Current.w / 2), 0, (_Current.w / 2), _Current.h);

  _LegoTruck -> Render ();

  SDL_RenderPresent (_Renderer);
}