class Sensor {
	public:

        Sensor ();

	Sensor (int PinTrigger, int PinEcho, long double *val);

	bool Measure ();

	private:

	long double *_Value = nullptr;
	int _PinTrigger;
	int _PinEcho;

	timeval _Start;
	timeval _End;

	timeval _CheckStart;
	timeval _CheckEnd;
};

Sensor::Sensor ()
{
}

Sensor::Sensor (int PinTrigger, int PinEcho, long double *val)
{
  _PinTrigger = PinTrigger;
  _PinEcho = PinEcho;
  
  _Value = val;
}

bool Sensor::Measure ()
{
  int w;
  int y;
  long double x;
  long double Check;
  long double preval;

  pinMode (_PinTrigger, OUTPUT);
  pinMode (_PinEcho, INPUT);
  digitalWrite (_PinTrigger, 1);
  delay (1);
  digitalWrite (_PinTrigger, 0);
  delay (0.01);
  digitalWrite (_PinTrigger, 1);

  w = 1;
  y = 1;
  x = 0;

  gettimeofday (&_CheckStart, 0);

  while (w == 1)
  {
    if (digitalRead (_PinEcho) == 1)
    {
      gettimeofday (&_Start, 0);

      w = 0;
    }

    gettimeofday (&_CheckEnd, 0);
    Check = _CheckEnd.tv_usec - _CheckStart.tv_usec;

    if (Check > 500)
    {
      return 0;
    }
  }

  while (y == 1)
  {
    if (digitalRead (_PinEcho) == 0)
    {
      gettimeofday (&_End, 0);

      y = 0;
    }
  }

  x = _End.tv_usec - _Start.tv_usec;

  preval = (((x * 0.000001) * 34300) / 2);

  if (preval >= 0 && preval < 500)
  {
    *_Value = preval;
  }

  //std::cout <<x*0.001<<" Millisekunden "<< std::endl;
  //cout << x << endl;

  return 1;
}
