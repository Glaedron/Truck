#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2//SDL_ttf.h>
#include <sstream>
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

    void Textbox (long double Dis, int x, int y);
    void Pic (SDL_Texture* Tex, int x, int y);

    std::stringstream _StrStr;
    static bool init;

    Truck LegoTruck;

    SDL_Color _TextColor = {0, 0, 0};
    SDL_Event _Event;
    SDL_DisplayMode _Current;

    SDL_GameController *_Controller = nullptr;
    SDL_Window *_Window = nullptr;
    SDL_Renderer *_Renderer = nullptr;
    TTF_Font* _Font = nullptr;
  
    SDL_Rect _MessageRect;
    SDL_Rect _TruckRect;
    SDL_Rect _TextboxRect;

    SDL_Surface* _Text;
    SDL_Texture* _Message;
    SDL_Texture* _Truck;
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

  _Font = TTF_OpenFont ("/home/pi/Truck/Unterlagen/TTFs/sfd/FreeSans.ttf", 30);

  if(!_Font)
  {
    std::cout <<" error ttf2"<< std::endl;

    return 0;
  }

  for (int i = 0; i < SDL_GetNumVideoDisplays (); i++)
  {
    if (SDL_GetCurrentDisplayMode (i, &_Current) == 0)
    {
      _Window = SDL_CreateWindow ("*TRUCK*", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _Current.w, _Current.h, SDL_WINDOW_RESIZABLE);
    }
  }
	
  _Renderer = SDL_CreateRenderer (_Window, -1, SDL_RENDERER_ACCELERATED);

  _Truck = IMG_LoadTexture (_Renderer, "Unterlagen/Truck.png");
  SDL_QueryTexture (_Truck, 0, 0, &_TruckRect.w, &_TruckRect.h);

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

  Run = true;

  return 1;
}

void Mainloop::Test ()
{
//  LegoTruck.DisplayTest ();

//  LegoTruck.Test ();

//  LegoTruck.PWMTest ();

//  LegoTruck.Drive();
}

void Mainloop::Input ()
{
  SDL_PollEvent (&_Event);

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

    case SDL_CONTROLLERBUTTONDOWN:
    {
      switch (_Event.cbutton.button)
      {
        case SDL_CONTROLLER_BUTTON_B:
        {
          LegoTruck.Stop ();
        }

        case SDL_CONTROLLER_BUTTON_A:
        {
        }

        case SDL_CONTROLLER_BUTTON_X:
        {
        }

        case SDL_CONTROLLER_BUTTON_Y:
        {
        }
      }

      break;
    }
  }
}

void Mainloop::Update ()
{
  LegoTruck.Measure ();
  //LegoTruck.Drive ();
}

void Mainloop::Render ()
{
  //_Stream = _Cam.GetFrame ();

  SDL_SetRenderDrawColor (_Renderer, 0, 0, 200, 255);
  SDL_RenderClear (_Renderer);

  SDL_SetRenderDrawColor (_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine (_Renderer, (_Current.w / 2), 0, (_Current.w / 2), _Current.h);

  Pic (_Truck, (_Current.w / 4) - (_TruckRect.w / 2), (_Current.h / 2) - (_TruckRect.h / 2));

  Textbox (LegoTruck._ValLeft, (_TruckRect.x), 400);
  Textbox (LegoTruck._ValFrontLeft, (_TruckRect.x), _TruckRect.y);
  Textbox (LegoTruck._ValFront, (_TruckRect.x) + (_TruckRect.w / 2), _TruckRect.y);
  Textbox (LegoTruck._ValFrontRight, (_TruckRect.x) + (_TruckRect.w), _TruckRect.y);
  Textbox (LegoTruck._ValRight, (_TruckRect.x) + (_TruckRect.w), 400);

  SDL_RenderPresent (_Renderer);
}





void Mainloop::Textbox (long double Dis, int x, int y)
{
    _StrStr << Dis;

    _Text = TTF_RenderText_Solid (_Font, _StrStr.str ().c_str (), _TextColor);

    _Message = SDL_CreateTextureFromSurface (_Renderer, _Text);

    _StrStr.str ("");

    _TextboxRect.x = x - ((_Text->w + 10) / 2);
    _TextboxRect.y = y - ((_Text->h + 10) / 2);
    _TextboxRect.w = _Text->w + 10;
    _TextboxRect.h = _Text->h + 10;

    _MessageRect.w = _Text -> w;
    _MessageRect.h = _Text -> h;
    _MessageRect.x = (_TextboxRect.x - (_MessageRect.w / 2)) + (_TextboxRect.w / 2);
    _MessageRect.y = (_TextboxRect.y - (_MessageRect.h / 2)) + (_TextboxRect.h / 2);


    SDL_SetRenderDrawColor (_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    SDL_RenderDrawRect (_Renderer, &_TextboxRect);
    SDL_RenderFillRect (_Renderer, &_TextboxRect);

    SDL_RenderCopy (_Renderer, _Message, NULL, &_MessageRect);
      
    SDL_DestroyTexture (_Message);
    SDL_FreeSurface (_Text);
}

void Mainloop::Pic (SDL_Texture* Tex, int x, int y)
{
  _TruckRect.x = x;
  _TruckRect.y = y;

  SDL_RenderCopy (_Renderer, Tex, NULL, &_TruckRect);
}