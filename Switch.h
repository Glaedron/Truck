class Switch
{
  public:

  Switch ();
  Switch (int Pin);

  bool GetState ();

  private:

  int _Pin;
};

Switch::Switch ()
{
}

Switch::Switch (int Pin)
{
  _Pin = Pin;

  pinMode (_Pin, INPUT);
}

bool Switch::GetState ()
{
  return digitalRead (_Pin);
}
