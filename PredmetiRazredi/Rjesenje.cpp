#include <iostream>
#include <string>
#include <regex>
#include<vector>
using namespace std;

enum Predmeti { Matematika, Historija, Geografia, Hemija, Biologija, Engleski, Muzicko, Fizicko };
enum Razredi { I1, I2, I3, II1, II2, II3, III1, III2, III3, IV1, IV2, IV3 };
char * ispisPredmeti[] = { "Matematika", "Historija", "Geografia", "Hemija", "Biologija", "Engleski", "Muzicko", "Fizicko" };
char * ispisRazredi[] = { "I1", "I2", "I3", "II1", "II2", "II3", "III1", "III2", "III3", "IV1", "IV2", "IV3" };
char *crt = "\n-----------------------------\n";
bool ProvjeriValidnostBrojaTelefona(string telefon){//globalna funkcija
	/*
	broj telefona treba biti u formatu: +387(61)-222-333
	*/
	return regex_match(telefon, regex("(\\+)(\\d{3})(\\()(\\d{2})(\\))(\\-)(\\d{3})(\\-)(\\d{3})"));
}
ostream &operator << (ostream &output, Predmeti &p){
	if (p == Matematika) output << "Matematika";
	else if (p == Historija) output << "Historija";
	else if (p == Geografia) output << "Geografia";
	else if (p == Hemija) output << "Hemija";
	else if (p == Biologija) output << "Biologija";
	else if (p == Engleski) output << "Engleski";
	else if (p == Muzicko) output << "Engleski";
	else if (p == Fizicko) output << "Engleski";
	return output;
}
ostream &operator << (ostream &output, Razredi &r){
	if (r == I1) output << "I1";
	else if (r == I2) output << "I2";
	else if (r == I3) output << "I3";
	else if (r == II1) output << "II1";
	else if (r == II2) output << "II2";
	else if (r == II3) output << "II3";
	else if (r == III1) output << "III1";
	else if (r == III2) output << "III2";
	else if (r == III3) output << "III3";
	else if (r == IV1) output << "IV1";
	else if (r == IV2) output << "IV2";
	else if (r == IV3) output << "IV3";
	return output;
}
template<class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutno;
public:
	Kolekcija()
	{
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	Kolekcija(const Kolekcija &k){
		_trenutno = k._trenutno;
		_elementi1 = new T1[k._trenutno];
		_elementi2 = new T2[k._trenutno];
		for (size_t i = 0; i < k._trenutno; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}
	Kolekcija &operator= (const Kolekcija &k){
		if (this != &k){
			delete[] _elementi1;
			delete[] _elementi2;
			_trenutno = k._trenutno;
			_elementi1 = new T1[k._trenutno];
			_elementi2 = new T2[k._trenutno];
			for (size_t i = 0; i < k._trenutno; i++)
			{
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];
			}
		}
		return *this;
	}
	void AddElement(T1 el1, T2 el2){
		T1 *temp1 = new T1[_trenutno + 1];
		T2 *temp2 = new T2[_trenutno + 1];
		for (size_t i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;
		temp1[_trenutno] = el1;
		temp2[_trenutno] = el2;
		_elementi1 = temp1; temp1 = nullptr;
		_elementi2 = temp2; temp2 = nullptr;
		_trenutno++;
	}
	void RemoveElement(int i){
		for (size_t j = i; j < _trenutno - 1; j++)
		{
			_elementi1[j] = _elementi1[j + 1];
			_elementi2[j] = _elementi2[j + 1];
		}
		_trenutno--;
	}
	int GetTrenutno()const{ return _trenutno; }
	T1 GetElement1(int i)const{ return _elementi1[i]; }
	T2 GetElement2(int i)const{ return _elementi2[i]; }
	~Kolekcija()
	{
		delete[]_elementi1;
		delete[]_elementi2;
	}
	friend ostream &operator <<(ostream &output, const Kolekcija &k){
		for (size_t i = 0; i < k.GetTrenutno(); i++)
			output << k.GetElement1(i) << " " << k.GetElement2(i) << endl;
		return output;
	}
};
class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}

	Datum(const Datum&obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}

	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}

	Datum &operator = (const Datum &d){
		*_dan = *d._dan;
		*_mjesec = *d._mjesec;
		*_godina = *d._godina;
		return *this;
	}
	friend ostream& operator<< (ostream &COUT, const Datum &obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}
};

class Osoba {
protected:
	char * _imePrezime;
	Datum * _datumRodjenja;
	string _brojTelefona;
public:
	Osoba(const char * imePrezime, Datum datum) {
		_imePrezime = new char[strlen(imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
		_datumRodjenja = new Datum(datum);
		_brojTelefona = "";

	}
	Osoba(const Osoba & obj) :_brojTelefona(obj._brojTelefona) {
		_imePrezime = new char[strlen(obj._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
		_datumRodjenja = new Datum(*obj._datumRodjenja);
	}
	~Osoba() { delete[]_imePrezime; delete _datumRodjenja; }

	bool setBrojTelefona(string brojTelefona){
		if (ProvjeriValidnostBrojaTelefona(brojTelefona)){
			_brojTelefona = brojTelefona;
			return true;
		}
		return false;
	}
	bool operator ==(const Osoba &o){ return strcmp(_imePrezime, o._imePrezime) == 0; }
	Osoba operator=(const Osoba &o){
		if (this != &o){
			delete[] _imePrezime;
			_brojTelefona = o._brojTelefona;
			_imePrezime = new char[strlen(o._imePrezime) + 1];
			strcpy_s(_imePrezime, strlen(o._imePrezime) + 1, o._imePrezime);
			*_datumRodjenja = *o._datumRodjenja;
		}
		return *this;
	}
	friend ostream &operator <<(ostream &o, const Osoba &u){
		o << "Ime i prezime " << u._imePrezime << " Broj telefona " << u._brojTelefona << "Datum rodjenja " << *u._datumRodjenja;
		return o;
	}
};

class Nastavnik :public Osoba {
	Kolekcija<Predmeti, Razredi> * _predmetiRazredi;
public:
	Nastavnik(const char* ime_prezime, Datum d) :Osoba(ime_prezime, d), _predmetiRazredi(new Kolekcija<Predmeti, Razredi>){}
	Nastavnik(const Nastavnik &n) :Osoba(n), _predmetiRazredi(new Kolekcija<Predmeti, Razredi>(*n._predmetiRazredi)){}
	~Nastavnik(){ delete _predmetiRazredi; }
	bool AddPredmetRazred(Predmeti p, Razredi r){
		int broj_ponavljanja = 0;
		for (size_t i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
		{
			if (_predmetiRazredi->GetElement1(i) == p && _predmetiRazredi->GetElement2(i) == r) return false;
			if (_predmetiRazredi->GetElement1(i) == p) broj_ponavljanja++;
		}
		if (broj_ponavljanja == 2) return false;
		_predmetiRazredi->AddElement(p, r);
		return true;
	}
	bool operator ==(const Nastavnik &n){
		return static_cast<Osoba>(n) == static_cast<Osoba>(*this);
	}
	bool RemovePredmetRazred(Predmeti p){
		for (size_t i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
		{
			if (_predmetiRazredi->GetElement1(i) == p){
				_predmetiRazredi->RemoveElement(i);
				return true;
			}
		}
		return false;
	}
	Nastavnik &operator = (const Nastavnik &n){
		Osoba::operator=(n);
		*_predmetiRazredi = *n._predmetiRazredi;
		return *this;
	}
	Kolekcija<Predmeti, Razredi> &getPredmetiRazredi(){ return *_predmetiRazredi; }
};

class Ucenik : public Osoba {
	int _brojUDnevniku;
	Razredi _razred;
	vector<Nastavnik *> _nastavnici;
	Kolekcija<Predmeti, int> _predmetiOcjene;
public:
	Ucenik(const char *ime_prezime, Datum d, int brojUDnevniku, Razredi r) :Osoba(ime_prezime, d), _brojUDnevniku(brojUDnevniku), _razred(r){}
	Ucenik(const Ucenik &u) :Osoba(u), _brojUDnevniku(u._brojUDnevniku), _razred(u._razred), _nastavnici(u._nastavnici), _predmetiOcjene(u._predmetiOcjene){}
	bool AddNastavnik(Nastavnik &n){
		bool flag = true;
		for (size_t i = 0; i < n.getPredmetiRazredi().GetTrenutno(); i++)
		if (n.getPredmetiRazredi().GetElement2(i) == _razred) flag = false;
		if (flag) return false;
		_nastavnici.push_back(&n);
		return true;
	}
	bool AddPredmetOcjenu(Predmeti p, int ocjena){
		bool flag = true;
		for (size_t i = 0; i < _nastavnici.size(); i++)
		if (_nastavnici[i]->getPredmetiRazredi().GetElement1(i) == p) flag = false;
		if (flag) return false;
		_predmetiOcjene.AddElement(p, ocjena);
		return true;
	}
	int GetProsjekByNastavnik(const char *nastavnik){
		float prosjek = 0;
		int brojac = 0;
		Nastavnik n(nastavnik, Datum());
		for (size_t i = 0; i < _nastavnici.size(); i++)
		{
			if (*_nastavnici[i] == n){
				for (size_t j = 0; j < _predmetiOcjene.GetTrenutno(); j++)
				{
					for (size_t k = 0; k < _nastavnici[i]->getPredmetiRazredi().GetTrenutno(); k++)
					{
						if (_predmetiOcjene.GetElement1(j) == _nastavnici[i]->getPredmetiRazredi().GetElement1(k)){
							prosjek += _predmetiOcjene.GetElement2(j);
							brojac++;
						}
					}
				}
			}
		}
		if (brojac == 0) return 0;
		return prosjek / brojac;
	}
	vector<Nastavnik *> &getNastavnici(){ return _nastavnici; }
	Kolekcija<Predmeti, int> &getPredmetiOcjene(){ return _predmetiOcjene; }
	friend ostream &operator <<(ostream &o, const Ucenik &u){
		o << static_cast<Osoba>(u) << endl;
		o << "Broj u dnevniku " << u._brojUDnevniku << endl;
		o << "Predmeti Ocjene" << endl;
		o << u._predmetiOcjene << endl;
		return o;
	}
};

void main(){

	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_.TXT
	5. NAZIV I LISTA PARAMETARA FUNKCIJA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.
	6. NIJE POTREBNO BACATI IZUZETKE. SVE FUNKCIJE VRACAJU TRUE ILI FALSE
	****************************************************************************/

#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region TestiranjeKolekcije
	Kolekcija<int, int> kolekcija1;
	int brojElemenata = 15;
	for (size_t i = 0; i < brojElemenata; i++)
		kolekcija1.AddElement(i, i);
	kolekcija1.RemoveElement(0);//uklanja element na osnovu lokacije
	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
	cout << kolekcija1 << endl;
	Kolekcija<int, int> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;
	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;
	Kolekcija<int, int> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
#pragma endregion
#pragma region TestiranjeNastavnika
	Nastavnik jasmin("Jasmin Azemovic", Datum(15, 2, 1980));
	Nastavnik adel("Adel Handzic", Datum(15, 10, 1981));
	/*NASTAVNIK MOZE PREDAVATI ISTI PREDMET, ALI TO MORA BITI U RAZLICITIM ODJELJENJIMA I NAJVISE U DVA RAZREDA (NA DVIJE RAZLICITE GODINE)*/
	if (jasmin.AddPredmetRazred(Matematika, I1))
		cout << "Predmeti dodan" << endl;
	if (jasmin.AddPredmetRazred(Historija, I1))
		cout << "Predmeti dodan" << endl;

	if (jasmin.AddPredmetRazred(Geografia, I1))
		cout << "Predmeti dodan" << endl;

	if (jasmin.AddPredmetRazred(Geografia, I2))
		cout << "Predmeti dodan" << endl;

	if (adel.AddPredmetRazred(Historija, III1))
		cout << "Predmeti dodan" << endl;

	if (adel.RemovePredmetRazred(Historija))
		cout << "Predmet uklonjen!" << endl;

	Nastavnik goran(adel);
	if (goran == adel)
		cout << "Nastavnici identicni" << endl;
	goran = jasmin;
	if (goran == jasmin)
		cout << "Nastavnici identicni" << endl;

	Kolekcija<Predmeti, Razredi> & predmetiRazredi = jasmin.getPredmetiRazredi();

	if (!jasmin.setBrojTelefona("+38761111222"))
		cout << "Broj telefona nije dodan!" << endl;
	if (!jasmin.setBrojTelefona("+387-61-111-222"))
		cout << "Broj telefona nije dodan!" << endl;
	if (jasmin.setBrojTelefona("+387(61)-111-222"))
		cout << "Broj telefona uspjesno dodan!" << endl;
#pragma endregion

#pragma region TestiranjeUcenika
	Ucenik denis("Denis Music", Datum(8, 10, 1990), 3, III1);
	Ucenik elmin("Elmin Sudic", Datum(15, 6, 1993), 1, I1);
	Ucenik adil("Adil Joldic", Datum(13, 8, 1992), 2, II1);
	/*VODITI RACUNA DA SE DODAJU REFERENCE NA NASTAVNIKE KAKO BI SVI UCENICI DIJELILI ISTE INFORMACIJE O NASTAVNICIMA U SLUCAJU DA SE NASTAVNIKU DODA NEKI PREDMET*/
	/*ZA USPJESNO DODAVANJE, NASTAVNIK MORA POSJEDOVATI (PREDAVATI) NAJMANJE JEDAN PREDMET U TOM RAZREDU */
	if (denis.AddNastavnik(jasmin))
		cout << "Nastavnik uspjesno dodan" << endl;
	/*ONEMOGUCITI DODAVANJE ISTIH NASTAVNIKA; POJAM ISTI PODRAZUMIJEVA IDENTICNE VRIJEDNOSTI SVIH ATRIBUTA*/
	if (denis.AddNastavnik(adel))
		cout << "Nastavnik uspjesno dodan" << endl;
	/*DA BI SE DODALA OCJENA ZA ODREDJENI PREDMET UCENIK MORA POSJEDOVATI NASTAVNIKA KOJI PREDAJE TAJ PREDMET*/
	if (denis.AddPredmetOcjenu(Matematika, 3))
		cout << "Ocjena uspjesno dodana" << endl;
	if (denis.AddPredmetOcjenu(Historija, 4))
		cout << "Ocjena uspjesno dodana" << endl;

	vector<Nastavnik*> & nastavnici = denis.getNastavnici();
	cout << "Ucenik Denis ima dodijeljena " << nastavnici.size() << " nastavnika" << endl;

	Kolekcija<Predmeti, int> & uspjeh = denis.getPredmetiOcjene();
	cout << "Denis ima " << uspjeh.GetTrenutno() << " polozenih predmeta" << endl;
	cout << "Prosjecna ocjena kod nastavnika Jasmin Azemovic je " << denis.GetProsjekByNastavnik("Jasmin Azemovic") << endl;

	Ucenik denis2(denis);
	cout << denis2 << endl;

#pragma endregion
	system("pause");
}