class Sprite
{
  public:

    Sprite ();
    Sprite (SDL_Renderer* renderer);

    SDL_Rect GetSpriteRect ()
    {
      return _TextureRect;
    }

    SDL_Rect GetTextRect ()
    {
      return _TextboxRect;
    }

    void SetSpritePos (int x, int y);
    void SetTextPos (int x, int y);
    void Load (std::string path);
    
    void RenderSprite ();
    void RenderText (std::string text);
    void RenderText (long double text);

  private:

    SDL_Rect _MessageRect;
    SDL_Rect _TextboxRect;

    SDL_Surface* _Text;
    SDL_Texture* _Message;

    SDL_Texture* _Texture;
    SDL_Rect _TextureRect;

    TTF_Font* _Font = nullptr;
    SDL_Color _TextColor = {0, 0, 0};

    std::stringstream _StrStr;

    SDL_Renderer* _Renderer = nullptr;
};

Sprite::Sprite ()
{
}

Sprite::Sprite (SDL_Renderer* renderer)
{
  _Renderer = renderer;

  _Font = TTF_OpenFont ("/home/pi/Truck/Unterlagen/TTFs/sfd/FreeSans.ttf", 30);

  if(!_Font)
  {
    std::cout <<" error ttf2"<< std::endl;
  }
}

void Sprite::SetSpritePos (int x, int y)
{
  _TextureRect.x = x;
  _TextureRect.y = y;
}

void Sprite::SetTextPos (int x, int y)
{
  _TextboxRect.x = x;
  _TextboxRect.y = y;
}

void Sprite::Load (std::string path)
{
  _Texture = IMG_LoadTexture (_Renderer, path.c_str ());
  SDL_QueryTexture (_Texture, 0, 0, &_TextureRect.w, &_TextureRect.h);
}

void Sprite::RenderSprite ()
{
  SDL_RenderCopy (_Renderer, _Texture, NULL, &_TextureRect);
}

void Sprite::RenderText (long double text)
{
  _StrStr << text;

  _Text = TTF_RenderText_Solid (_Font, _StrStr.str ().c_str (), _TextColor);

  _Message = SDL_CreateTextureFromSurface (_Renderer, _Text);

  _StrStr.str ("");

  _TextboxRect.x -= ((_Text->w + 10) / 2);
  _TextboxRect.y -= ((_Text->h + 10) / 2);
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
      
  SDL_FreeSurface (_Text);
}

void Sprite::RenderText (std::string text)
{
  _Text = TTF_RenderText_Solid (_Font, text.c_str (), _TextColor);

  _Message = SDL_CreateTextureFromSurface (_Renderer, _Text);

  _TextboxRect.x -= ((_Text->w + 10) / 2);
  _TextboxRect.y -= ((_Text->h + 10) / 2);
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
      
  SDL_FreeSurface (_Text);
}