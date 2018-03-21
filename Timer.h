class Timer
{
  public:
	
    Timer ();
    void Update ();
	
    float GetElapsedSeconds ();
    float GetElapsedMilliSeconds ();

    float Time;
	
  private:
	
    float _Elapsed;
    float _Current;
    float _Last;
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

float Timer::GetElapsedSeconds ()
{
  return (_Elapsed / 1000);
}

float Timer::GetElapsedMilliSeconds ()
{
  return (_Elapsed);
}