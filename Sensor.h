class Sensor {
	public:

        Sensor ();
	Sensor (int PinTrigger, int PinEcho);

	long double GetDistance ();

	private:

	long double _Value;
	int _PinTrigger;
	int _PinEcho;

	timeval _Start;
	timeval _End;
};

Sensor::Sensor ()
{
}

Sensor::Sensor (int PinTrigger, int PinEcho)
{
  _PinTrigger = PinTrigger;
  _PinEcho = PinEcho;
}

long double Sensor::GetDistance ()
{
  int w;
  int y;
  long double x;

  pinMode (_PinTrigger, OUTPUT);
  pinMode (_PinEcho, INPUT);
  digitalWrite (_PinTrigger, 1);
  delay (1);
  digitalWrite (_PinTrigger, 0);
  delay (0.01);
  digitalWrite (_PinTrigger, 1);

  //cout <<"start"<< endl;
  w = 1;
  y = 1;
  x = 0;

  while (w == 1)
  {
    if (digitalRead (_PinEcho) == 1)
    {
      gettimeofday (&_Start, 0);
      w = 0;
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
  _Value = (((x * 0.000001) * 34300) / 2);


  //cout <<x*0.001<<" Millisekunden "<< endl;
  //cout << x << endl;

  return _Value;
}
