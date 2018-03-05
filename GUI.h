#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2//SDL_ttf.h>
#include <sstream>

class GUI
{
  public:

  GUI ();
  ~GUI ();

  void Event ();
  void Display (long double a, long double  b,long double  c,long double d,long double e);
  void RenderText (long double Dis, int x, int y);
  void RenderTexture (SDL_Texture* Tex, int x, int y);

  private:

  std::stringstream ss;
  static bool init;

  SDL_Event _Event;
  SDL_DisplayMode current;
  SDL_Window *_Window = nullptr;
  SDL_Renderer *_Renderer = nullptr;
  TTF_Font* _Font = nullptr;
  SDL_Rect Message_rect;
  SDL_Rect _TruckRect;
  SDL_Surface* text;
  SDL_Texture* Message;
  SDL_Texture* _Truck;
  SDL_Color textColor = {255, 127, 31};
};

bool GUI::init = 0;

GUI::GUI ()
{
  if (init == 0)
  {
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
    {
      std::cout <<" error vid/timer"<< std::endl;
    }

    if (IMG_Init (IMG_INIT_PNG) == -1)
    {
      std::cout <<" error png"<< std::endl;
    }

    if (TTF_Init() == -1)
    {
      std::cout <<" error ttf"<< std::endl;
    }

    _Font = TTF_OpenFont ("FreeSans.ttf", 30);

    if(!_Font)
    {
      std::cout <<" error ttf2"<< std::endl;
    }

    init = 1;
  }

  for (int i = 0; i < SDL_GetNumVideoDisplays (); i++)
  {
    if (SDL_GetCurrentDisplayMode (i, &current) == 0)
    {
      _Window = SDL_CreateWindow ("*TRUCK*", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, current.w, current.h, SDL_WINDOW_RESIZABLE);
    }
  }
	
  _Renderer = SDL_CreateRenderer (_Window, -1, SDL_RENDERER_ACCELERATED);

  _Truck = IMG_LoadTexture (_Renderer,"Truck.png");
  SDL_QueryTexture (_Truck, 0, 0, &_TruckRect.w, &_TruckRect.h);
}

GUI::~GUI ()
{
  SDL_DestroyWindow (_Window);
  SDL_DestroyRenderer (_Renderer);
}

void GUI::Event ()
{
  SDL_PollEvent (&_Event);

  switch (_Event.type)
  {
    case SDL_QUIT:
    {
      SDL_Quit ();

      break;
    }
  }
}

void GUI::Display (long double a, long double b, long double c, long double d, long double e)
{
  Event ();

  SDL_SetRenderDrawColor (_Renderer, 0, 0, 200, 255);
	
  SDL_RenderClear (_Renderer);

  SDL_SetRenderDrawColor (_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

  SDL_RenderDrawLine (_Renderer, (current.w / 2), 0, (current.w / 2), current.h);
  RenderTexture (_Truck, ((current.w / 4) * 3) - (_TruckRect.w / 2), (current.h - _TruckRect.h));
  RenderText (a, 0, 0);
  RenderText (b, 0, 50);
  RenderText (c, 0, 100);
  RenderText (d, 0, 150);
  RenderText (e, 0, 200);

  SDL_RenderPresent (_Renderer);
}

void GUI::RenderText (long double Dis, int x, int y)
{
    ss << Dis;

    text = TTF_RenderText_Solid (_Font, ss.str ().c_str (), textColor);

    Message = SDL_CreateTextureFromSurface (_Renderer, text);

    ss.str ("");

    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = text -> w;
    Message_rect.h = text -> h;

    SDL_RenderCopy(_Renderer, Message, NULL, &Message_rect);
      
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(text);
}

void GUI::RenderTexture (SDL_Texture* Tex, int x, int y)
{
  _TruckRect.x = x;
  _TruckRect.y = y;

  SDL_RenderCopy(_Renderer, Tex, NULL, &_TruckRect);

  //SDL_DestroyTexture(Tex);
}