class Engine
{
  public:

  Engine ();
  Engine (int PinForward, int PinBackward);
  Engine (int PinForward, int PinBackward, int PinPWM);

  void Forward ();
  void Backward ();
  void SetSpeed (float Speed);
  void Stop ();

  private:

  int _PinForward;
  int _PinBackward;
  int _PinPWM;
};

Engine::Engine ()
{
}

Engine::Engine (int PinForward, int PinBackward)
{
  _PinForward = PinForward;
  _PinBackward = PinBackward;

  pinMode (_PinForward, OUTPUT);
  pinMode (_PinBackward, OUTPUT);
}

Engine::Engine (int PinForward, int PinBackward, int PinPWM)
{
  _PinForward = PinForward;
  _PinBackward = PinBackward;
  _PinPWM = PinPWM;

  pinMode (_PinForward, OUTPUT);
  pinMode (_PinBackward, OUTPUT);

  softPwmCreate (_PinPWM, 100, 100);
}

void Engine::Forward ()
{
  digitalWrite (_PinForward,1);
  digitalWrite (_PinBackward,0);
}

void Engine::Backward ()
{
  digitalWrite (_PinForward,0);
  digitalWrite (_PinBackward,1);
}

void Engine::SetSpeed (float Speed)
{
  softPwmWrite (_PinPWM, Speed);
}

void Engine::Stop ()
{
  SetSpeed (0);

  digitalWrite (_PinForward,0);
  digitalWrite (_PinBackward,0);
}
