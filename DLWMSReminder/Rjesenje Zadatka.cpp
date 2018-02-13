#include <iostream>
#include <regex>
#include <vector>
#include <iterator>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;
const char *crt = "\n----------------------------------------------------\n";
mutex m;
class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum &dat) {
		_dan = new int(*dat._dan);
		_mjesec = new int(*dat._mjesec);
		_godina = new int(*dat._godina);
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
	bool operator > (const Datum &d) {
		return (*_dan + *_mjesec * 30 + *_godina * 365) > (*d._dan + *d._mjesec * 30 + *d._godina * 365);
	}
	int GetDani() { return *_dan + *_mjesec * 30 + *_godina * 365; }
	friend ostream &operator<<(ostream &COUT, const Datum &obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}

};
class Izuzetak : public exception {
	string _funkcija;
public :
	Izuzetak(string opis, string funkcija) :exception(opis.c_str()), _funkcija(funkcija) {}
	friend ostream &operator <<(ostream &o, const Izuzetak &i) {
		o << i.what() << endl << "Funkcija -> " << i._funkcija;
		return o;
	}
};

template <class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutnoElemenata;
	bool _omoguciDupliranjeElemenata;
public:
	Kolekcija(bool omoguciDupliranjeElemenata = false) {
		_trenutnoElemenata = 0;
		_omoguciDupliranjeElemenata = omoguciDupliranjeElemenata;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	Kolekcija(const Kolekcija &k):_trenutnoElemenata(k._trenutnoElemenata),
		_omoguciDupliranjeElemenata(k._omoguciDupliranjeElemenata), _elementi1(new T1[k._trenutnoElemenata]), _elementi2(new T2[k._trenutnoElemenata]) {
		for (size_t i = 0; i < k._trenutnoElemenata; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}
	~Kolekcija() {
		delete[]_elementi2; _elementi2 = nullptr;
		delete[]_elementi1; _elementi1 = nullptr;
		_trenutnoElemenata = 0;
	}
	
	int GetTrenutno() const { return _trenutnoElemenata; }
	T1& GetElement1(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi1[lokacija];
	}

	T2& GetElement2(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi2[lokacija];
	}
	void SetElement2(int lokacija, T2 el2) {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		_elementi2[lokacija] = el2;
	}
	bool AddElement(T1 el1, T2 el2) {
		if (!_omoguciDupliranjeElemenata) {
			for (size_t i = 0; i < _trenutnoElemenata; i++)
			{
				if (_elementi1[i] == el1 && _elementi2[i] == el2)
					return false;
			}
		}
		T1 *temp1 = new T1[_trenutnoElemenata + 1];
		T2 *temp2 = new T2[_trenutnoElemenata + 1];

		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;
		temp1[_trenutnoElemenata] = el1;
		temp2[_trenutnoElemenata] = el2;
		_elementi1 = temp1; temp1 = nullptr;
		_elementi2 = temp2; temp2 = nullptr;
		_trenutnoElemenata++;
		return true;
	}
	bool RemoveElement(T1 el1) {
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			if (_elementi1[i] == el1) {
				T1 *temp1 = new T1[_trenutnoElemenata - 1];
				T2 *temp2 = new T2[_trenutnoElemenata - 1];
				int counter = 0;
				for (size_t j = 0; j < _trenutnoElemenata; j++)
				{
					if (i != j) {
						temp1[counter] = _elementi1[j];
						temp2[counter++] = _elementi2[j];
					}
				}
				delete[] _elementi1;
				delete[] _elementi2;
				_elementi1 = temp1; temp1 = nullptr;
				_elementi2 = temp2; temp2 = nullptr;
				_trenutnoElemenata--;
				return true;
			}
		}
		return false;
	}
	Kolekcija &operator = (const Kolekcija &k) {
		if (this != &k) {
			_omoguciDupliranjeElemenata = k._omoguciDupliranjeElemenata;
			_trenutnoElemenata = k._trenutnoElemenata;
			delete[] _elementi1; _elementi1 = nullptr;
			delete[] _elementi2; _elementi2 = nullptr;
			_elementi1 = new T1[k._trenutnoElemenata];
			_elementi2 = new T2[k._trenutnoElemenata];
			for (size_t i = 0; i < k._trenutnoElemenata; i++)
			{
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];
			}
		}
		return *this;
	}
	friend ostream &operator<<(ostream &COUT, const Kolekcija &obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
return COUT;
	}
};

class Dogadjaj
{
	Datum _datumOdrzavanja;
	Kolekcija<string, bool> * _obaveze; //cuva informaciju o obavezama koje je potrebno ispuniti prije samog dogadjaja, string se odnosi na opis, a bool na izvrsenje te obaveze (da li je zavrsena ili ne)

	char *_naziv;
	int _notificirajPrije; //oznacava broj dana prije samog dogadjaja kada ce krenuti notifikacija/podsjetnik
	bool _rekurzivnaNotifikacija; //ako je vrijednost true onda se korisnik notificira svaki dan do _datumaOdrzavanja dogadjaja, a pocevsi prije dogadjaja za _brojDanaZaNotifikaciju

	bool ProvjeriValidnost(string s) {
		return regex_search(s, regex("UBITI|MRZITI|ubiti|mrziti|TUCI|tuci"));
	}
public:
	Dogadjaj(Datum datumOdrzavanja, const char *naziv, int brojDana = 1,
		bool rekurzivnaNotifikacija = false) : _datumOdrzavanja(datumOdrzavanja)
	{
		_naziv = new char[strlen(naziv) + 1];
		strcpy_s(_naziv, strlen(naziv) + 1, naziv);

		_notificirajPrije = brojDana;
		_rekurzivnaNotifikacija = rekurzivnaNotifikacija;
		_obaveze = nullptr;
	}

	Dogadjaj(const Dogadjaj &obj) : _datumOdrzavanja(obj._datumOdrzavanja)
	{
		_naziv = new char[strlen(obj._naziv) + 1];
		strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);

		_notificirajPrije = obj._notificirajPrije;
		_rekurzivnaNotifikacija = obj._rekurzivnaNotifikacija;
		_obaveze = new Kolekcija<string, bool>;
		*_obaveze = *obj._obaveze;
	}
	~Dogadjaj()
	{
		delete[] _naziv;
		_naziv = nullptr;
		delete _obaveze;
		_obaveze = nullptr;
	}

	bool AddObavezu(string obaveza) {
		if (_obaveze == nullptr)
			_obaveze = new Kolekcija<string, bool>;
		if (ProvjeriValidnost(obaveza)) throw Izuzetak("U textu obaveze postoji zabranjena rijec", __FUNCTION__);
		return _obaveze->AddElement(obaveza, false);
	}
	Datum GetDatumObaveze()const { return _datumOdrzavanja; }
	char *GetNaziv() { return _naziv; }
	Kolekcija<string, bool> *GetObaveze() { return _obaveze; }
	bool operator == (const Dogadjaj &d) {
		return strcmp(_naziv, d._naziv) == 0;
	}
};

class Student
{
	int _indeks;
	string _imePrezime;
	vector<Dogadjaj> _dogadjaji;
public:

	Student(int indeks, string imePrezime) : _indeks(indeks), _imePrezime(imePrezime) {}

	int GetIndeks() const { return _indeks; }
	string GetImePrezime()const { return _imePrezime; }
	vector<Dogadjaj>& GetDogadjaji() { return _dogadjaji; }

	bool AddDogadjaj(Dogadjaj d) {
		for (vector<Dogadjaj>::iterator i = _dogadjaji.begin(); i != _dogadjaji.end(); i++)
			if (*i == d)
				return false;
		_dogadjaji.push_back(d);
		return true;
	}
	friend ostream &operator<<(ostream &COUT, const Student &obj)
	{
		COUT << obj._imePrezime << " (" << obj._indeks << ")" << endl;
		return COUT;
	}
};

class DLWMSReminder
{
	vector<Student> _reminiderList;

	void PosaljiPoruku(string s) {
		m.lock();
		cout << s << endl;
		m.unlock();
	}
public:
	void AddStudent(Student s) {
		for (vector<Student>::iterator i = _reminiderList.begin(); i != _reminiderList.end(); i++)
		{
			if (i->GetIndeks() == s.GetIndeks())
				throw Izuzetak("Student sa tim brojem indexa " + to_string(s.GetIndeks()) + " vec postoji.", __FUNCTION__);
		}
		_reminiderList.push_back(s);
	}
	bool OznaciObavezuKaoZavrsenu(int index, string nazivDogadjaja, string nazivObaveze) {
		for (vector<Student>::iterator i = _reminiderList.begin(); i != _reminiderList.end(); i++)
		{
			if (i->GetIndeks() == index) {
				for (vector<Dogadjaj>::iterator j = i->GetDogadjaji().begin(); j != i->GetDogadjaji().end(); j++)
				{
					if (j->GetNaziv() == nazivDogadjaja && j->GetObaveze() != nullptr) {
						for (size_t k = 0; k < j->GetObaveze()->GetTrenutno(); k++)
						{
							if (j->GetObaveze()->GetElement1(k) == nazivObaveze) {
								j->GetObaveze()->SetElement2(k, true);
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}
	int PosaljiNotifikacije(Datum d) {

		int brojac = 0;

		for (vector<Student>::iterator i = _reminiderList.begin(); i != _reminiderList.end(); i++)
		{
			for (vector<Dogadjaj>::iterator j = i->GetDogadjaji().begin(); j != i->GetDogadjaji().end(); j++)
			{
				if (j->GetDatumObaveze() > d) {
					brojac++;
					thread t(
						&DLWMSReminder::PosaljiPoruku,
						this,
						"Postovani " + i->GetImePrezime() + "\n" +
						"Dogadjaj " + j->GetNaziv() + "je zakazan za "+to_string(j->GetDatumObaveze().GetDani() - d.GetDani()) +", "+
						"a do sada ste ispunili " + 
						to_string([j]()->float {
						int brojac = 0;
						for (size_t i = 0; i < j->GetObaveze()->GetTrenutno(); i++)
						{
							if (j->GetObaveze()->GetElement2(i)) brojac++;
						}
						return ((float)brojac / j->GetObaveze()->GetTrenutno())*100;
						}()) + 
						"% svojih obaveza. " +
						"Neispunjene obaveze su: \n " + 
						[j]()->string {
							int brojac = 1;
							string s = "";
							for (size_t i = 0; i < j->GetObaveze()->GetTrenutno(); i++)
							{
								if (!j->GetObaveze()->GetElement2(i)) {
									s += to_string(brojac) + ". " + j->GetObaveze()->GetElement1(i) + "\n";
									brojac++;
								}
							}
							return s;
						}() +
						"Predlazemo Vam da ispunite i ostale planirane obaveze.\n" +
						"FIT Reminder\n" + crt
					);
					t.join();
				}
			}
		}
		return brojac;
	}
};

void main() {

	/**************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_ParcijalniII.TXT
	5. NAZIVI FUNKCIJA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI PO ŽELJI.
	****************************************************************************/

#pragma region Datum

	Datum danas(28, 1, 2018), sutra(29, 1, 2018);
	Datum datumIspitaPRIII(30, 1, 2018), datumIspitBPII(31, 1, 2018);
	Datum prekosutra(danas);
	prekosutra = danas;
	cout << danas << endl
		<< sutra << endl
		<< prekosutra << crt;

#pragma endregion

#pragma region Kolekcija

	/*
	AddElement :: omogucava dodavanje novog elementa u kolekciju. Ukoliko je moguce, osigurati automatsko prosiranje kolekcije prilikom dodavanja svakog novog elementa, te onemoguciti ponavljanje elemenata
	RemoveElement :: na osnovu parametra tipa T1 uklanja elemente iz kolekcije i ukoliko je moguce smanjuje velicinu niza/kolekcije. Prilikom uklanjanja elemenata ocuvati redoslijed njihovog dodavanja
	*/
	const int brElemenata = 10;
	Kolekcija<int, float> kolekcija1;
	for (size_t i = 0; i < brElemenata; i++)
		if (!kolekcija1.AddElement(i, i + (0.6 * i)))
			cout << "Elementi " << i << " i " << i + (0.6 * i) << " nisu dodati u kolekciju" << endl;

	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
	cout << kolekcija1 << endl;

	kolekcija1.RemoveElement(1);

	Kolekcija<int, float> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;

	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;

	Kolekcija<int, float> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;

#pragma endregion

#pragma region Dogadjaj

	Dogadjaj ispitPRIII(datumIspitaPRIII, "Ispit iz PRIII", 5, true),

		ispitBPII(datumIspitBPII, "Ispit iz BPII", 7, true);
	/*po vlasitom izboru definisati listu zabranjenih rijeci koje ce onemoguciti dodavanje odredjene obaveze. Prilikom provjere koristiti regex*/
	if (ispitPRIII.AddObavezu("Preraditi pdf materijale"))cout << "Obaveza dodana!" << endl;
	//onemoguciti dupliranje obaveza
	if (!ispitPRIII.AddObavezu("Preraditi pdf materijale"))cout << "Obaveza nije dodana!" << endl;
	if (ispitPRIII.AddObavezu("Pregledati video materijale"))cout << "Obaveza dodana!" << endl;
	if (ispitPRIII.AddObavezu("Preraditi ispitne zadatke"))cout << "Obaveza dodana!" << endl;
	if (ispitPRIII.AddObavezu("Samostalno vjezbati"))cout << "Obaveza dodana!" << endl;

	if (ispitBPII.AddObavezu("Preraditi knjigu SQL za 24 h"))cout << "Obaveza dodana!" << endl;
	if (ispitBPII.AddObavezu("Pregledati video materijale"))cout << "Obaveza dodana!" << endl;
	if (ispitBPII.AddObavezu("Napraviti bazu za konkretnu aplikaciju"))cout << "Obaveza dodana!" << endl;

	Student jasmin(150051, "Jasmin Azemovic"), adel(160061, "Adel Handzic");

	if (jasmin.AddDogadjaj(ispitPRIII) && jasmin.AddDogadjaj(ispitBPII))
		cout << "Dogadjaj uspjesno dodan!" << endl;

	if (adel.AddDogadjaj(ispitPRIII) && adel.AddDogadjaj(ispitBPII))
		cout << "Dogadjaj uspjesno dodan!" << endl;
	//onemoguciti dupliranje dogadjaja
	if (!adel.AddDogadjaj(ispitPRIII))
		cout << "Dogadjaj nije uspjesno dodan!" << endl;

	DLWMSReminder reminder;

	try
	{
		reminder.AddStudent(jasmin);
		reminder.AddStudent(adel);
		//u slucaju dupliranja studenata funkcija baca izuzetak tipa Izuzetak
		reminder.AddStudent(jasmin);
	}
	catch (Izuzetak &err)
	{
		//ispisati sve informacije o nastalom izuzetku
		cout << err << endl;
	}

	//da bi bila oznacena kao zavrsena, obaveza mora postojati i mora biti oznacena kao nezavrsena (false)
	if (reminder.OznaciObavezuKaoZavrsenu(150051, "Ispit iz PRIII", "Pregledati video materijale"))
		cout << "Obaveza oznacena kao zavrsena" << endl;

	/*metodi PosaljiNotifikacije se salje trenutni datum na osnovu cega ona pretrazuje sve studente koje treba podsjetiti/notoficirati o dogadjajima koji se priblizavaju.
	Koristeci multithread-ing, svim studentima se salju notifikacije sa sljedecim sadrzajem:
	-------------------------------------------------------------------------
	Postovani Jasmin Azemovic,
	Dogadjaj Ispit iz PRIII je zakazan za 3 dana, a do sada ste obavili 56% obaveza vezanih za ovaj dogadjaj. Neispunjene obaveze su:
	1.Preraditi ispitne zadatke
	2.Samostalno vjezbati
	Predlazemo Vam da ispunite i ostale planirane obaveze.
	FIT Reminder
	-------------------------------------------------------------------------
	Dakle, notifikacije ce biti poslane svim studentima koji su dodali dogadjaj za 30.01.2018. godine i oznacili da zele da budu podsjecani ponovo/rekurzivno najmanje 2 dana prije samog dogadjaja (podaci se odnose na konkretan dogadjaj: Ispit iz PRIII)

	*/
	int poslato = 0;
	//funkcija vraca broj poslatih podsjetnika/notifikacija
	poslato = reminder.PosaljiNotifikacije(danas);
	cout << "Za " << danas << " poslato ukupno " << poslato << " podsjetnika!" << endl;
	poslato = reminder.PosaljiNotifikacije(sutra);
	cout << "Za " << sutra << " poslato ukupno " << poslato << " podsjetnika!" << endl;

#pragma endregion

	system("pause");
}