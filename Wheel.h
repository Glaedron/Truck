class Wheel
{
	public:

        Wheel ();
	Wheel (int PinLeft, int PinRight, int PinRelais);
	
	void Left ();
	void Right ();
	void Middle ();
	void Stop ();
	
	private:
	
	int _PinLeft;
	int _PinRight;
	int _PinRelais;
	
	bool _Left, _Right, _Middle;
};

Wheel::Wheel ()
{
}

Wheel::Wheel (int PinLeft, int PinRight, int PinRelais)
{
	_PinLeft = PinLeft;
	_PinRight = PinRight;
	_PinRelais = PinRelais;
	
	pinMode (_PinLeft, OUTPUT);
	pinMode (_PinRight, OUTPUT);
	pinMode (_PinRelais,OUTPUT);
}

void Wheel::Left ()
{
	if (_Right == true && _Middle == false)
	{
		Middle ();
	}
	
	if (!_Left)
	{
		digitalWrite (_PinRight,0);
		digitalWrite (_PinLeft,1);
		digitalWrite (_PinRelais,1);
		
		delay (1000);
		
		digitalWrite (_PinRight,0);
		digitalWrite (_PinLeft,0);
		digitalWrite (_PinRelais,0);
		
		_Left = true;
		_Right = false;
		_Middle = false;
	}
}

void Wheel::Right ()
{
	if (_Left == true && _Middle == false)
	{
		Middle ();
	}
	
	if (!_Right)
	{
		digitalWrite (_PinRight,1);
		digitalWrite (_PinLeft,0);
		digitalWrite (_PinRelais,1);
		
		delay (1000);
		
		digitalWrite (_PinRight,0);
		digitalWrite (_PinLeft,0);
		digitalWrite (_PinRelais,0);
		
		_Left = false;
		_Right = true;
		_Middle = false;
	}
}

void Wheel::Middle ()
{
	if (!_Middle)
	{
		digitalWrite (_PinRight,0);
		digitalWrite (_PinLeft,0);
		digitalWrite (_PinRelais,1);

        delay (1000);

        digitalWrite (_PinRelais,0);
    
        _Left = false;
        _Right = false;
        _Middle = true;
	}
}

void Wheel::Stop ()
{
	digitalWrite (_PinRight,0);
    digitalWrite (_PinLeft,0);
    digitalWrite (_PinRelais,0);
}
