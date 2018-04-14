#include <fstream>

using namespace std;

class Logfile
{
	public:
	
	Logfile (string sName);
	void Write (string sMeldung);
	void Write (string sMeldung, float fWert);
	void Write (string sMeldung, string sWert);
	void Reset ();
	
	private:
	
	string m_sName;
};

Logfile::Logfile (string sName)
{
	m_sName = sName + ".txt";
}

void Logfile::Write (string sMeldung)
{
	ofstream file (m_sName.c_str (),ios::app);
	file << sMeldung << endl;
	file.close ();
}

void Logfile::Write (string sMeldung, float fWert)
{
	ofstream file (m_sName.c_str (),ios::app);
	file << sMeldung <<": "<< fWert << endl;
	file.close ();
}

void Logfile::Write (string sMeldung, string sWert)
{
	ofstream file (m_sName.c_str (),ios::app);
	file << sMeldung <<": "<< sWert << endl;
	file.close ();
}

void Logfile::Reset ()
{
	ofstream file (m_sName.c_str (),ios::trunc);
	file.close ();
}