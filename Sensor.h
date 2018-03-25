class Sensor
{
  public:

    Sensor ();

    Sensor (int PinTrigger, int PinEcho, SDL_Renderer* renderer);

    bool Update ();
    void Render ();

    void SetPos (int x, int y);

    long double GetVal ()
    {
      return _Val;
    }

  private:

    SDL_Rect _MessageRect;
    SDL_Rect _TextboxRect;

    SDL_Surface* _Text;
    SDL_Texture* _Message;

    TTF_Font* _Font = nullptr;
    SDL_Color _TextColor = {0, 0, 0};

    std::stringstream _StrStr;

    SDL_Renderer* _Renderer = nullptr;


    int _PinTrigger;
    int _PinEcho;

    timeval _Start;
    timeval _End;

    timeval _CheckStart;
    timeval _CheckEnd;

    long double _TimeDiff;
    long double _Check;

    long double _Preval;
    long double _Val;
};

Sensor::Sensor ()
{
}

Sensor::Sensor (int PinTrigger, int PinEcho, SDL_Renderer* renderer)
{
  _PinTrigger = PinTrigger;
  _PinEcho = PinEcho;

  pinMode (_PinTrigger, OUTPUT);
  pinMode (_PinEcho, INPUT);
  
  _Renderer = renderer;

  _Font = TTF_OpenFont ("/home/pi/Truck/Unterlagen/TTFs/sfd/FreeSans.ttf", 30);

  if(!_Font)
  {
    std::cout <<" error ttf2"<< std::endl;
  }
}

bool Sensor::Update ()
{
  digitalWrite (_PinTrigger, 1);
  delay (1);
  digitalWrite (_PinTrigger, 0);
  delay (0.01);
  digitalWrite (_PinTrigger, 1);

  _TimeDiff = 0;


  gettimeofday (&_CheckStart, 0);

  while (digitalRead (_PinEcho) == 0)
  {
    gettimeofday (&_CheckEnd, 0);
    _Check = _CheckEnd.tv_usec - _CheckStart.tv_usec;

    if (_Check > 500)
    {
      std::cout << _Check <<" Mikrosekunden; Timeout bzw. Sensorproblem "<< std::endl;
      return 0;
    }
  }




  gettimeofday (&_CheckStart, 0);
  gettimeofday (&_Start, 0);

  while (digitalRead (_PinEcho) == 1)
  {
    gettimeofday (&_CheckEnd, 0);
    _Check = _CheckEnd.tv_usec - _CheckStart.tv_usec;

    if (_Check > 200000)
    {
      return 0;
    }
  }

  gettimeofday (&_End, 0);




  _TimeDiff = _End.tv_usec - _Start.tv_usec;

  _Preval = (((_TimeDiff * 0.001) * 34.3) / 2);

  if (_Preval >= 0 && _Preval < 500)
  {
    _Val = _Preval;
  }

  //std::cout << _TimeDiff * 0.001 <<" Millisekunden "<< std::endl;
  //std::cout << _TimeDiff <<" Mikrosekunden "<< std::endl;

  return 1;
}

void Sensor::Render ()
{
  _StrStr << _Val;

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

void Sensor::SetPos (int x, int y)
{
  _TextboxRect.x = x;
  _TextboxRect.y = y;
}