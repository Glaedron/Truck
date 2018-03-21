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

        long double _TimeDiff;
        long double _Check;
        long double _Preval;
};

Sensor::Sensor ()
{
}

Sensor::Sensor (int PinTrigger, int PinEcho, long double *val)
{
  _PinTrigger = PinTrigger;
  _PinEcho = PinEcho;

  pinMode (_PinTrigger, OUTPUT);
  pinMode (_PinEcho, INPUT);
  
  _Value = val;
}

bool Sensor::Measure ()
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
    *_Value = _Preval;
  }

  //std::cout << _TimeDiff * 0.001 <<" Millisekunden "<< std::endl;
  //std::cout << _TimeDiff <<" Mikrosekunden "<< std::endl;

  return 1;
}
