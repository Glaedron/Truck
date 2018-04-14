class Timer
{
  public:
	
    Timer ();
    void Update ();
    void FPS (int fps);
    int GetFPS ();
	
    float GetElapsedSeconds ();
    float GetElapsedMilliSeconds ();

    float Time;
	
  private:
	
    float _Elapsed;
    float _Current;
    float _Last;

    float _FPSStart;
    float _FPSEnd;
};

Timer::Timer ()
{
  _Elapsed = 0;
  _Current = 0;
  _Last = SDL_GetTicks ();
}

void Timer::Update ()
{
  _Current = SDL_GetTicks ();
  _Elapsed = _Current - _Last;
  _Last = _Current;
}

void Timer::FPS (int fps)
{
  _FPSStart = _Last;

  Update ();

  SDL_Delay ((1000 / fps) - _Elapsed);
}

int Timer::GetFPS ()
{
  Update ();

  _FPSEnd = _Current;

  int elapsed = _FPSEnd - _FPSStart;

  return 1000 / elapsed;
}

float Timer::GetElapsedSeconds ()
{
  return (_Elapsed / 1000);
}

float Timer::GetElapsedMilliSeconds ()
{
  return (_Elapsed);
}