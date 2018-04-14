class Engine
{
  public:

  Engine ();
  Engine (int PinForward, int PinBackward, int PinPWM, SDL_Renderer* renderer);

  void Forward (float speed);
  void Backward (float speed);
  void Stop ();

  void Render ();

  void SetPos (int x, int y);

  private:

  SDL_Renderer* _Renderer;

  Sprite _Engine;

  float _Speed;

  int _PinForward;
  int _PinBackward;
  int _PinPWM;
};

Engine::Engine ()
{
}

Engine::Engine (int PinForward, int PinBackward, int PinPWM, SDL_Renderer* renderer)
{
  _Renderer = renderer;

  _Engine = Sprite (_Renderer);

  _PinForward = PinForward;
  _PinBackward = PinBackward;
  _PinPWM = PinPWM;

  pinMode (_PinForward, OUTPUT);
  pinMode (_PinBackward, OUTPUT);

  softPwmCreate (_PinPWM, 100, 100);
}

void Engine::Forward (float speed)
{
  _Speed = speed;
  softPwmWrite (_PinPWM, _Speed);

  digitalWrite (_PinForward,1);
  digitalWrite (_PinBackward,0);
}

void Engine::Backward (float speed)
{
  _Speed = speed;
  softPwmWrite (_PinPWM, _Speed);

  digitalWrite (_PinForward,0);
  digitalWrite (_PinBackward,1);
}

void Engine::Stop ()
{
  _Speed = 0;
  softPwmWrite (_PinPWM, _Speed);

  digitalWrite (_PinForward,0);
  digitalWrite (_PinBackward,0);
}

void Engine::Render ()
{
  _Engine.RenderText (_Speed);
}

void Engine::SetPos (int x, int y)
{
  _Engine.SetTextPos (x, y);
}
