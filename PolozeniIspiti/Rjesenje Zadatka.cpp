#include <iostream>
using namespace std;


/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
6. ZA POTREBE TESTIRANJA, UNUTAR MAIN FUNKCIJE MOZETE DODAVATI NOVE TESTNE PODATKE
****************************************************************************/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char * crt = "\n----------------------------------------------------\n";
class Datum {
	int * _dan, *_mjesec, *_godina;
public:
	Datum(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	Datum(const Datum &d) {
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum &operator = (const Datum &d) {
		*_dan = *d._dan;
		*_mjesec = *d._mjesec;
		*_godina = *d._godina;
		return *this;
	}

	friend ostream & operator<<(ostream & COUT, Datum obj) {
		COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina;
		return COUT;
	}
};

template<class T, int velicina>
class FITArray {
	T _niz[velicina];
	int _trenutno;
	bool _dozvoliDupliciranje;
public:
	//OSTALE FUNKCIJE CLANICE DEFINISATI VAN TEMPLATE KLASE
	FITArray(bool dozvoliDupliciranje = true) { _dozvoliDupliciranje = dozvoliDupliciranje; _trenutno = 0; }
	bool operator += (T el);
	bool operator -= (T el);
	FITArray &operator = (const FITArray &f);
	FITArray operator()(int v1, int v2);
	int GetTrenutno() { return _trenutno; }
	T * GetNiz() { return _niz; }
	friend ostream &operator << <>(ostream &o, const FITArray &f);

};
template<class T, int velicina>
bool FITArray<T, velicina>::operator+= (T el) {
	if (_trenutno == velicina) return false;
	if (!_dozvoliDupliciranje) {
		for (size_t i = 0; i < _trenutno; i++)
		{
			if (_niz[i] == el) return false;
		}
	}
	_niz[_trenutno++] = el;
	return true;
}
template<class T, int velicina>
bool FITArray<T, velicina>::operator-=(T el) {
	for (size_t i = 0; i < _trenutno; i++)
	{
		if (_niz[i] == el) {
			_niz[i] = _niz[--_trenutno];
			return true;
		}
	}
	return false;
}
template<class T, int velicina>
ostream &operator <<(ostream &o, const FITArray<T, velicina> &f) {
	for (size_t i = 0; i < f._trenutno; i++)
	{
		o << f._niz[i] << " ";
	}
	return o;
}
template<class T, int velicina>
FITArray<T, velicina> FITArray<T, velicina>::operator()(int v1, int v2) {
	FITArray<T, velicina> noviNiz;
	if (_trenutno < v2) v2 = _trenutno;
	for (v1; v1 <= v2; v1++)
		noviNiz += _niz[v1];
	return noviNiz;
}
template<class T, int velicina>
FITArray<T, velicina> &FITArray<T, velicina>::operator=(const FITArray &f) {
	_trenutno = f._trenutno;
	_dozvoliDupliciranje = f._dozvoliDupliciranje;
	for (size_t i = 0; i < f._trenutno; i++)
		_niz[i] = f._niz[i];
	return *this;
}


class PolozeniPredmet {
	char * _naziv;
	int _ocjena;
	Datum _datumPolaganja;
public:
	PolozeniPredmet():_ocjena(5), _datumPolaganja(1,1,2000) {
		_naziv = nullptr;
	}
	PolozeniPredmet(Datum datum, const char *naziv, int ocjena):_datumPolaganja(datum), _ocjena(ocjena){
		int size = strlen(naziv) + 1;
		_naziv = new char[size];
		strcpy_s(_naziv, size, naziv);
	}
	PolozeniPredmet(const PolozeniPredmet &p):_ocjena(p._ocjena), _datumPolaganja(p._datumPolaganja) {
		int size = strlen(p._naziv) + 1;
		_naziv = new char[size];
		strcpy_s(_naziv, size, p._naziv);
	}
	PolozeniPredmet &operator = (const PolozeniPredmet &p) {
		if (this != &p) {
			delete[] _naziv;
			int size = strlen(p._naziv) + 1;
			_naziv = new char[size];
			strcpy_s(_naziv, size, p._naziv);
			_ocjena = p._ocjena;
			_datumPolaganja = p._datumPolaganja;
		}
		return *this;
	}
	bool operator == (const PolozeniPredmet &p) {
		return strcmp(_naziv, p._naziv) == 0;
	}
	~PolozeniPredmet() { delete[] _naziv; _naziv = nullptr; }
	int GetOcjena()const { return _ocjena; }
	friend ostream & operator<<(ostream & COUT,const PolozeniPredmet & obj) {
		COUT << obj._naziv << "(" << obj._ocjena << ")" << obj._datumPolaganja << endl;
		return COUT;
	}
};

class Student {
	static int _indeksCounter; //sluzi za odredjivanje narednog broja indeksa koji ce biti dodijeljen studentu, pocevsi od 160000
	const int _indeks;
	char * _imePrezime;
	FITArray<PolozeniPredmet, 40> _polozeniPredmeti;
public:
	Student(const char *imePrezime):_indeks(_indeksCounter++) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
	}
	~Student() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	Student &operator = (const Student &s) {
		if (this != &s) {
			delete[] _imePrezime;
			int size = strlen(s._imePrezime) + 1;
			_imePrezime = new char[size];
			strcpy_s(_imePrezime, size, s._imePrezime);
			_polozeniPredmeti = s._polozeniPredmeti;
		}
		return *this;
	}
	bool DodajPredmet(PolozeniPredmet p) {
		return _polozeniPredmeti += p;
	}
	float GetProsjek() {
		float prosjek = 0;

		if (_polozeniPredmeti.GetTrenutno() == 0) return 0;

		for (size_t i = 0; i < _polozeniPredmeti.GetTrenutno(); i++)
		{
			prosjek += _polozeniPredmeti.GetNiz()[i].GetOcjena();
		}
		prosjek /= _polozeniPredmeti.GetTrenutno();
		return prosjek;
	}
	friend ostream & operator<<(ostream & COUT, Student & obj) {
		COUT << obj._indeks << " " << obj._imePrezime << endl << obj._polozeniPredmeti;
		return COUT;
	}
};
int Student::_indeksCounter = 160000;
void main() {
	const int max = 20;

	Datum jucer(20, 11, 2017), danas(21, 11, 2017), sutra(22, 11, 2017);
	cout << jucer << danas << sutra << endl;
	jucer = danas;
	cout << jucer << danas << sutra << endl;

	//parametar tipa bool odredjuje da li je u nizu dozvoljeno dupliciranje elemenata
	FITArray<int, max> nizIntegera(false);

	for (size_t i = 0; i < max - 1; i++)
		nizIntegera += i;//dodaje novog clana niza
	
	if (!(nizIntegera += 6))//pokusavamo dodati dupli clan niza
		cout << "Element nije dodan u niz" << endl;

	cout << crt << nizIntegera << crt; //ispisuje sve clanove niza
	nizIntegera -= 9; //uklanja clan niza sa vrijednoscu 9
	nizIntegera -= 17;

	FITArray<int, max> noviNizIntegera(nizIntegera);

	cout << crt << noviNizIntegera << crt;
	
	/*parametri odredjuju lokacije (indekse u nizu) elemenata OD - DO koje je potrebno vratiti. u slucaju da u nizu ne postoji trazeni broj elemenata funkcija treba da vrati sve element od lokacije OD pa do posljednje dostupnog elementa */
	cout << "Clanovi niza od lokacije 2 do lokacije 5 su -> " << noviNizIntegera(2, 5) << crt;

	Student adel("Adel Handzic"), jasmin("Jasmin Azemovic");

	PolozeniPredmet prII(jucer, "PRII", 8), prIII(sutra, "PRIII", 7);

	adel.DodajPredmet(prII);
	adel.DodajPredmet(prIII);
	cout << adel << endl;
	//vraca prosjecnu ocjenu studenta
	cout << "prosjecna ocjena -> " << adel.GetProsjek() << crt;

	jasmin = adel;
	cout << jasmin << endl;
	cout << "Prosjecna ocjena -> " << jasmin.GetProsjek() << crt;
	getchar();
}