class Wheel
{
  public:

    Wheel ();
    Wheel (int PinLeft, int PinRight, int PinPWM);
	
    void Left (int position);
    void Right (int position);
    void Middle ();
	
  private:
	
    int _PinLeft;
    int _PinRight;
    int _PinPWM;

    double _Range = 10.204;
    double _Positions [8] = {
                             0,
                             0.258 * _Range,
                             0.380 * _Range,
                             0.501 * _Range,
                             0.635 * _Range,
                             0.760 * _Range,
                             0.882 * _Range,
                             1.000 * _Range
                           };
};

Wheel::Wheel ()
{
}

Wheel::Wheel (int PinLeft, int PinRight, int PinPWM)
{
  _PinLeft = PinLeft;
  _PinRight = PinRight;
  _PinPWM = PinPWM;

  pinMode (_PinLeft, OUTPUT);
  pinMode (_PinRight, OUTPUT);
  softPwmCreate (_PinPWM, 0, _Range);
}

void Wheel::Left (int position)
{
  if (position > 7)
  {
    position = 7;
  }

  if (position < 0)
  {
    position = 0;
  }

  softPwmWrite (_PinPWM, _Positions [position]);
  digitalWrite (_PinLeft,1);
  digitalWrite (_PinRight,0);
}

void Wheel::Right (int position)
{
  if (position > 7)
  {
    position = 7;
  }

  if (position < 0)
  {
    position = 0;
  }

  softPwmWrite (_PinPWM, _Positions [position]);
  digitalWrite (_PinLeft,0);
  digitalWrite (_PinRight,1);
}

void Wheel::Middle ()
{
  softPwmWrite (_PinPWM, 0);
  digitalWrite (_PinLeft,0);
  digitalWrite (_PinRight,0);
}
