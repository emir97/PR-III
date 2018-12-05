#include <iostream>
/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOCNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
6. ZA POTREBE TESTIRANJA, UNUTAR MAIN FUNKCIJE MOZETE DODAVATI NOVE TESTNE PODATKE
****************************************************************************/
using namespace std;
const char * crt = "\n------------------------------------\n";
char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum &d):Datum(*d._dan, *d._mjesec, *d._godina){}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, const Datum &obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}
	int operator - (const Datum &d) {
		return  (*_dan + *_mjesec * 30 + *_godina * 365) - (*d._dan + *d._mjesec * 30 + *d._godina * 365);
	}
};

template<class T>
class Kolekcija {
	T * _niz;
	int _trenutno;
public:
	Kolekcija() : _trenutno(0), _niz(nullptr) {}
	~Kolekcija() { delete[] _niz; _niz = nullptr; _trenutno = 0; }
	T * GetNiz() { return _niz; }
	int GetTrenutno() { return _trenutno; }
	//implementirati potrebne funkcije
	Kolekcija(const Kolekcija &k) {
		_trenutno = k._trenutno;
		_niz = new T[k._trenutno];
		for (size_t i = 0; i < k._trenutno; i++)
		{
			_niz[i] = k._niz[i];
		}
	}
	void Add(const T &t) {
		T *temp = new T[_trenutno + 1];
		for (size_t i = 0; i < _trenutno; i++)
		{
			temp[i] = _niz[i];
		}
		delete[] _niz;
		temp[_trenutno] = t;
		_niz = temp;
		_trenutno++;
	}
	friend ostream &operator<< (ostream &o, const Kolekcija &k) {
		for (size_t i = 0; i < k._trenutno; i++)
		{
			o << k._niz[i] << endl;
		}
		o << endl;
		return o;
	}
	T& operator[] (int i) {
		return _niz[i];
	}
};

class Usluga {
	char* _naziv;
	double _cijena;//cijena usluge je izrazena po danu koristenja
public:
	Usluga() : _naziv(nullptr), _cijena(0) {}
	Usluga(const char* naziv, double cijena) : _cijena(cijena) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	Usluga(const Usluga &u) : Usluga(u._naziv, u._cijena) { }
	Usluga(Usluga &&u):_naziv(move(u._naziv)), _cijena(move(u._cijena)){ }
	// Move constructor takodjer moze i se implementirati i na ovaj nacin
	/*
	Usluga(Usluga &&u) {
		_naziv = u._naziv;
		u._naziv = nullptr;
		_cijena = u._cijena;
	*/
	bool operator == (const Usluga &u) { return strcmp(_naziv, u._naziv) == 0; }
	Usluga &operator = (const Usluga &u) {
		if (&u != this) {
			delete[] _naziv;
			_naziv = AlocirajNizKaraktera(u._naziv);
			_cijena = u._cijena;
		}
		return *this;
	}
	~Usluga() { delete[] _naziv; _naziv = nullptr; }
	double GetCijena() { return _cijena; }
	const char * GetNaziv() { return _naziv; }
	friend ostream& operator<< (ostream &COUT, const Usluga &obj);
};
ostream& operator<< (ostream &COUT, const Usluga &obj) {
	COUT << obj._naziv << " -> \t\t" << obj._cijena << " KM" << endl;
	return COUT;
}

class Rezervacija {
	static int _rezervacijeIDBrojac;
	const int _rezervacijaID; ///broj rezervacije krece od 160000
	Datum _rezervacijaOD;
	Datum _rezervacijaDO;
	double _cijenaPoDanu; //cijena osnovnih usluga koja ne ukljucuje dodatne usluge
	Kolekcija<Usluga> _dodatneUsluge; //dodatne usluge mogu biti: sauna, sportski tereni, kuglanje, masaza i sl.
public:
	//implementirati potrebne funkcije
	Rezervacija(const Datum &OD, const Datum &DO, double cijenaPoDanu) : 
		_rezervacijaID(_rezervacijeIDBrojac++), 
		_rezervacijaOD(OD), 
		_rezervacijaDO(DO), 
		_cijenaPoDanu(cijenaPoDanu) { }
	
	Rezervacija(const Rezervacija &r) :
		_rezervacijaID(r._rezervacijaID),
		_rezervacijaOD(r._rezervacijaOD),
		_rezervacijaDO(r._rezervacijaDO),
		_cijenaPoDanu(r._cijenaPoDanu),
		_dodatneUsluge(r._dodatneUsluge) { }

	bool operator += (const Usluga &u) {
		for (size_t i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
		{
			if (_dodatneUsluge.GetNiz()[i] == u) return false;
		}
		_dodatneUsluge.Add(u);
		return true;
	}
	void PrintRacun() {
		double sumaDodatniUsluga = 0;
		int brojDana = _rezervacijaDO - _rezervacijaOD;
		for (size_t i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
		{
			sumaDodatniUsluga += brojDana * _dodatneUsluge.GetNiz()[i].GetCijena();
		}
		cout << crt <<
			"R.ID: " << _rezervacijaID << endl << 
			"OD: " << _rezervacijaOD << endl << 
			"DO: " << _rezervacijaDO << endl << 
			crt << 
			"DODATNE USLUGE:" << endl << 
			crt << 
			_dodatneUsluge << endl << 
			crt << 
			"Osnovne usluge -> " << brojDana * _cijenaPoDanu << 
			crt << 
			"Dodatne usluge -> " << sumaDodatniUsluga <<
			crt << 
			"Ukupno -> " << brojDana * _cijenaPoDanu + sumaDodatniUsluga << 
			crt;
	}
	Usluga * operator[](const char *naziv) {
		for (size_t i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
		{
			if (strcmp(_dodatneUsluge.GetNiz()[i].GetNaziv(), naziv) == 0) return _dodatneUsluge.GetNiz() + i;
		}
		return nullptr;
	}
	Kolekcija<Usluga> operator()(int i, int j) {
		Kolekcija<Usluga> k;
		for (size_t i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
		{
			if (_dodatneUsluge.GetNiz()[i].GetCijena() >= i && _dodatneUsluge.GetNiz()[i].GetCijena() <= j)
				k.Add(_dodatneUsluge.GetNiz()[i]);
		}
		return k;
	}
};
int Rezervacija::_rezervacijeIDBrojac = 160000;


void main() {
	cout << crt << "TRAJANJE ISPITA -> 90 minuta " << crt;
	Kolekcija<int> brojevi;
	for (int i = 0; i < 15; i++)
		brojevi.Add(i);
	cout << brojevi << crt;
	//u okviru klase Usluga implementirati MOVE konstruktor
	Usluga Parking("Parking", 5), Dorucak("Dorucak", 10), Rucak("Rucak", 15), Veseraj("Veseraj", 10);
	//rezervacijaOD, rezervacijaDO, cijenaPoDanu
	Rezervacija rezervacija(Datum(25, 10, 2018), Datum(30, 10, 2018), 59.9);
	//dodaje novu uslugu u rezervaciju. onemoguciti ponavljanje usluga
	if (rezervacija += Parking)
		cout << "Usluga uspjesno dodana!" << endl;
	if (rezervacija += Dorucak)
		cout << "Usluga uspjesno dodana!" << endl;
	if (rezervacija += Rucak)
		cout << "Usluga uspjesno dodana!" << endl;
	if (rezervacija += Veseraj)
		cout << "Usluga uspjesno dodana!" << endl;
	if (!(rezervacija += Dorucak))
		cout << "Usluga vec postoji!" << endl;
	/*prikazuje racun formatiran na nacin prikazan na slici. prilikom izracunavanja iznosa koristiti cijene osnovnih i dodatnih usluga, kao i broj dana za koje je napravljena rezervacija (napomena: duzina trajanja rezervacije je maksimalno 30 dana)*/
	rezervacija.PrintRacun();
	//vraca pokazivac na uslugu sa proslijedjenim nazivom
	Usluga * p = rezervacija["Dorucak"];
	if (p != nullptr)
		cout << *p << crt;
	//vraca sve dodatne usluge koje imaju cijenu u opsegu vrijednosti parametara
	Kolekcija<Usluga> uslugePoCijeni = rezervacija(8, 12);
	for (size_t i = 0; i < uslugePoCijeni.GetTrenutno(); i++)
		cout << uslugePoCijeni[i] << crt;
	cin.get();
	
}



/*
------------------------------------
R.ID: 160000
OD: 25 10 2018
DO: 30 10 2018
------------------------------------
DODATNE USLUGE:
------------------------------------
Parking ->              5 KM
Dorucak ->             10 KM
Rucak ->               15 KM
Veseraj ->             10 KM
------------------------------------
Osnovne usluge->        299.5 KM
------------------------------------
Dodatne usluge->        200   KM
------------------------------------
Ukupno ->               499.5 KM
------------------------------------
*/

