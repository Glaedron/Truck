class Textbox
{
  public:

  Textbox ();
  Textbox (SDL_Renderer *renderer);

  void Render (long double MSG, int x, int y);

  private:

  std::stringstream _StrStr;
  static bool init;

  SDL_Renderer *_Renderer = nullptr;
  TTF_Font* _Font = nullptr;

  SDL_Rect _MessageRect;
  SDL_Rect _TextboxRect;

  SDL_Surface* _Text;
  SDL_Texture* _Message;
  SDL_Color _TextColor = {0, 0, 0};
};

bool Textbox::init = 0;

Textbox::Textbox ()
{
}

Textbox::Textbox (SDL_Renderer *renderer)
{
  _Renderer = renderer;

  if (init == 0)
  {
    if (TTF_Init() == -1)
    {
      std::cout <<" error ttf"<< std::endl;
    }

    _Font = TTF_OpenFont ("/home/pi/Truck/TTFs/sfd/FreeSans.ttf", 30);

    if(!_Font)
    {
      std::cout <<" error ttf2"<< std::endl;
    }

    init = 1;
  }
}

void Textbox::Render (long double MSG, int x, int y)
{
  _StrStr << MSG;

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

  SDL_RenderCopy(_Renderer, _Message, NULL, &_MessageRect);
}