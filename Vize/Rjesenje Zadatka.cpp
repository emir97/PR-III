#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <thread>
#include <mutex>
#include <regex>
using namespace std;
const char *crt = "\n-----------------------------------------------\n";

enum Status { ZAHTJEV_ZAPRIMLJEN, ODUSTAO, ODBIJENA, IZDATA, ISTEKLA, PONISTENA };
enum Sortiranje { RASTUCI, OPADAJUCI };
enum Drzava { SAD, AUSTRALIJA, ENGLESKA };

mutex m;

ostream &operator <<(ostream &o, Status s) {
	if (s == Status::ZAHTJEV_ZAPRIMLJEN) o << "ZAHTJEV_ZAPRIMLJEN";
	else if (s == Status::ODUSTAO) o << "ODUSTAO";
	else if (s == Status::ODBIJENA) o << "ODBIJENA";
	else if (s == Status::IZDATA) o << "IZDATA";
	else if (s == Status::ISTEKLA) o << "ISTEKLA";
	else if (s == Status::PONISTENA) o << "PONISTENA";
	return o;
}

class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
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
	Datum &operator++() {
		(*_dan)++;
		if ((*_mjesec == 1 || *_mjesec == 3 || *_mjesec == 5 || *_mjesec == 7 || *_mjesec == 8 || *_mjesec == 10 || *_mjesec == 12) && *_dan > 31) {
			*_dan = 1;
			(*_mjesec)++;
		}
		else if ((*_mjesec == 1 || *_mjesec == 3 || *_mjesec == 5 || *_mjesec == 7 || *_mjesec == 8 || *_mjesec == 10 || *_mjesec == 12) && *_dan > 30) {
			*_dan = 1;
			*_mjesec = *_mjesec + 1;
		}
		else if (*_mjesec == 2 && *_dan > 29 && (*_godina % 4 == 0 && *_godina % 100 != 0 || *_godina % 400 == 0)) {
			*_dan = 1;
			*_mjesec = *_mjesec + 1;
		}
		else if (*_mjesec == 2 && *_dan > 28 && !(*_godina % 4 == 0 && *_godina % 100 != 0 || *_godina % 400 == 0)) {
			*_dan = 1;
			*_mjesec = *_mjesec + 1;
		}

		if (*_mjesec > 12) {
			*_mjesec = 1;
			*_godina = *_godina + 1;
		}
		return *this;
	}
	Datum &operator = (const Datum &d) {
		*_dan = *d._dan;
		*_mjesec = *d._mjesec;
		*_godina = *d._godina;
		return *this;
	}
	bool operator ==(const Datum &d) {
		return *_dan == *d._dan && *_mjesec == *d._mjesec && *_godina == *d._godina;
	}
	bool operator >(const Datum &d) {
		return (*_dan + *_mjesec * 30 + *_godina * 365) > (*d._dan + *d._mjesec * 30 + *d._godina * 365);
	}
	friend ostream& operator<< (ostream &COUT, Datum &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}

	int GetYear() { return *_godina; }
};

template<class T1, class T2, int max>
class FITKolekcija {
	T1 * _elementi1[max];
	T2 * _elementi2[max];
	int _trenutno;
public:
	FITKolekcija() {
		for (size_t i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}
	FITKolekcija(const FITKolekcija &k) {
		if (max >= k.GetMax()) {
			_trenutno = k._trenutno;
			for (size_t i = 0; i < k._trenutno; i++)
			{
				_elementi1[i] = new T1(*k._elementi1[i]);
				_elementi2[i] = new T2(*k._elementi2[i]);
			}
		}
	}
	~FITKolekcija() {
		for (size_t i = 0; i < max; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	T1 ** GetT1() { return _elementi1; }
	T2 ** GetT2() { return _elementi2; }
	int GetTrenutno() { return _trenutno; }
	int GetMax()const { return max; }
	void Dodaj(T1 el1, T2 el2) {
		if (_trenutno == max) throw exception("Kolekcija je puna.");
		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno++] = new T2(el2);
	}
	void Sortiraj(Sortiranje s) {
		if (s == RASTUCI) {
			bool promjena = true;
			while (promjena)
			{
				promjena = false;
				for (size_t i = 0; i < _trenutno - 1; i++)
				{
					if (*_elementi1[i] > *_elementi1[i + 1]) {
						T1 temp1 = *_elementi1[i];
						T2 temp2 = *_elementi2[i];
						*_elementi1[i] = *_elementi1[i + 1];
						*_elementi2[i] = *_elementi2[i + 1];
						*_elementi1[i + 1] = temp1;
						*_elementi2[i + 1] = temp2;
						promjena = true;
					}
				}
			}
		}
		else if (s == OPADAJUCI) {
			bool promjena = true;
			while (promjena)
			{
				promjena = false;
				for (size_t i = 0; i < _trenutno - 1; i++)
				{
					if (*_elementi1[i] < *_elementi1[i + 1]) {
						T1 temp1 = *_elementi1[i];
						T2 temp2 = *_elementi2[i];
						*_elementi1[i] = *_elementi1[i + 1];
						*_elementi2[i] = *_elementi2[i + 1];
						*_elementi1[i + 1] = temp1;
						*_elementi2[i + 1] = temp2;
						promjena = true;
					}
				}
			}
		}
	}
	friend ostream& operator<< (ostream &COUT, FITKolekcija &obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << *obj._elementi1[i] << " " << *obj._elementi2[i] << endl;
		return COUT;
	}
};


class Viza {
protected:
	Drzava _drzava;
	Datum _vazenjeOD;
	Datum _vazenjeDO;
	char * _napomena;
	// string se odnosi na razlog promjene statusa, a moguće je samo 10 izmjena statusa
	FITKolekcija<Status, string, 10> _statusi;
public:
	Viza(Drzava drzava, Datum _OD, Datum _DO, char * napomena = nullptr) :_vazenjeDO(_DO), _vazenjeOD(_OD)
	{
		if (napomena != nullptr) {
			_napomena = new char[strlen(napomena) + 1];
			strcpy_s(_napomena, strlen(napomena) + 1, napomena);
		}
		else
			_napomena = nullptr;
		_drzava = drzava;
	}
	Viza(const Viza &v) :_drzava(v._drzava), _vazenjeOD(v._vazenjeOD), _vazenjeDO(v._vazenjeDO), _statusi(v._statusi) {
		if (v._napomena != nullptr) {
			int size = strlen(v._napomena) + 1;
			_napomena = new char[size];
			strcpy_s(_napomena, size, v._napomena);
		}
	}
	~Viza() { delete[] _napomena; _napomena = nullptr; }
	Datum GetVazenjeOD()const { return _vazenjeOD; }
	Datum GetVazenjeDO()const { return _vazenjeDO; }
	char * GetNapomena()const { return _napomena; }
	Drzava GetDrzava()const { return _drzava; }
	FITKolekcija<Status, string, 10> & GetStatuse() { return _statusi; }
	virtual void Info() = 0;

	bool operator==(const Viza &v) {
		return _drzava == v.GetDrzava() && _vazenjeDO == v.GetVazenjeDO() && _vazenjeOD == v.GetVazenjeOD();
	}
	void DodajStatus(Status s, string poruka) {
		_statusi.Dodaj(s, poruka);
	}
	void SetNapomena(const char *napomena) {
		delete[] _napomena;
		int size = strlen(napomena) + 1;
		_napomena = new char[size];
		strcpy_s(_napomena, size, napomena);
	}
};

class PoslovnaViza :public Viza {
	string _kompanija; //naziv kompanije koja potvrđuje da posluje sa aplikantom
public:
	PoslovnaViza(Drzava drzava, Datum _OD, Datum _DO, string kompanija, char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_kompanija = kompanija;
	}
	friend ostream & operator << (ostream & COUT, PoslovnaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._kompanija << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;

	}
	void Info() {
		cout << *this << endl;
	}
};
class TuristickaViza : public Viza {
	string _adresa; //adresa hotele ili drugog smještaja na kojoj će aplikant boraviti
public:
	TuristickaViza(Drzava drzava, Datum _OD, Datum _DO, string adresa, char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_adresa = adresa;
	}
	friend ostream & operator << (ostream & COUT, TuristickaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._adresa << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;
	}
	void Info() {
		cout << *this << endl;
	}
};

class Aplikant {
	char * _imePrezime;
	Datum * _datumRodjenja;
	char * _brojPasosa;
	vector<Viza *> _vize;
public:
	Aplikant(const char * imePrezime, Datum datumRodjenja, const char * brPasosa) {
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		vel = strlen(brPasosa) + 1;
		_brojPasosa = new char[vel];
		strcpy_s(_brojPasosa, vel, brPasosa);
		_datumRodjenja = new Datum(datumRodjenja);
	}
	~Aplikant() {
		delete[]_imePrezime; _imePrezime = nullptr;
		delete[]_brojPasosa; _brojPasosa = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			delete _vize[i]; _vize[i] = nullptr;
		}
	}
	friend ostream & operator << (ostream & COUT, Aplikant & obj) {
		COUT << obj._imePrezime << " " << *obj._datumRodjenja << " " << *obj._brojPasosa << endl;
		COUT << "VIZE -> " << endl;
		for (size_t i = 0; i < obj._vize.size(); i++)
			obj._vize[i]->Info();
		return COUT;
	}

	vector<Viza * > GetVize() { return _vize; }

	bool DodajVizu(Viza *v) {
		if ((2018 - _datumRodjenja->GetYear()) < 18) return false;
		for (size_t i = 0; i < v->GetStatuse().GetTrenutno(); i++)
		{
			if (*v->GetStatuse().GetT1()[i] == Status::ISTEKLA || *v->GetStatuse().GetT1()[i] == Status::PONISTENA)
				return false;
		}
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (_vize[i]->GetDrzava() == Drzava::SAD) {
				for (size_t j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++)
				{
					if (*_vize[i]->GetStatuse().GetT1()[j] == Status::PONISTENA)
						return false;
				}
			}
		}
		if (dynamic_cast<PoslovnaViza*>(v) != nullptr) {
			_vize.push_back(new PoslovnaViza(*dynamic_cast<PoslovnaViza*>(v)));
		}
		else if (dynamic_cast<TuristickaViza*>(v) != nullptr) {
			_vize.push_back(new TuristickaViza(*dynamic_cast<TuristickaViza*>(v)));
		}
		return true;
	}
	bool DodajStatus(Viza *v, Status s, string poruka = "") {
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (*_vize[i] == *v) {
				bool zahtjevZaprimljen = false;
				bool izdata = false;
				for (size_t j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++)
				{
					if (*_vize[i]->GetStatuse().GetT1()[j] == Status::ZAHTJEV_ZAPRIMLJEN) {
						zahtjevZaprimljen = true;
					}
					if (*_vize[i]->GetStatuse().GetT1()[j] == Status::IZDATA) {
						izdata = true;
					}
				}
				if (s == Status::ZAHTJEV_ZAPRIMLJEN) {
					_vize[i]->DodajStatus(s, poruka);
					return true;
				}
				else if ((s == Status::ODUSTAO || s == Status::ODBIJENA || s == Status::IZDATA) && zahtjevZaprimljen) {
					_vize[i]->DodajStatus(s, poruka);
					return true;
				}
				else if ((s == Status::PONISTENA || s == Status::ISTEKLA) && izdata) {
					_vize[i]->DodajStatus(s, poruka);
					return true;
				}
			}
		}
		return false;
	}
	void PrintajVizu(Viza *v) {
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (*v == *_vize[i]) {
				thread t([](Viza *v) {
					m.lock();
					v->Info();
					this_thread::sleep_for(chrono::seconds(5));
					m.unlock();
				}, _vize[i]);
				t.join();
			}
		}
	}
	vector<Viza*> GetVizeByDrzava(Drzava d, Status s) {
		vector<Viza*> temp;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (_vize[i]->GetDrzava() == d) {
				bool status = false;
				for (size_t j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++)
				{
					if (*_vize[i]->GetStatuse().GetT1()[j] == s)
						status = true;
				}
				if (status && *_vize[i]->GetStatuse().GetT1()[_vize[i]->GetStatuse().GetTrenutno() - 1] == Status::IZDATA)
					temp.push_back(_vize[i]);
			}
		}
		return temp;
	}
	void ZamijeniRijec(string rijec, string subrijec) {
		regex r("(\\b" + rijec + "\\b)|(\\B" + rijec + "\\B)");
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (_vize[i]->GetNapomena() != nullptr) {
				string zamijeni(_vize[i]->GetNapomena());
				string zamijenjenaRijec = regex_replace(zamijeni, r, subrijec);
				_vize[i]->SetNapomena(zamijenjenaRijec.c_str());
			}
		}
	}
};

void main() {

	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/

#pragma TestiranjeDatuma

	Datum danas(15, 2, 2017);
	Datum sutra(++danas); // inkrementiranjem se uvecava dan za 1
	Datum prekosutra;
	Datum dat15022017(15, 2, 2017), dat18082017(18, 8, 2017), dat11052018(11, 5, 2018), dat11122017(11, 12, 2017);


	prekosutra = danas;
	if (prekosutra == danas)
		cout << "Datumi su jednaki" << endl;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma end
#pragma TestiranjeKolekcije
	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int, 7> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//NA KOJI NAČIN ĆE SE VRŠITI SORTIRANJE PRVENSTVENO ZAVISI OD VRIJEDNOSTI PARAMETRA, A VRŠI SE NA OSNOVU VRIJEDNOSTI TIPA T1
	brojevi.Sortiraj(RASTUCI);
	cout << brojevi << crt;
	brojevi.Sortiraj(OPADAJUCI);
	cout << brojevi << crt;

	try
	{
		//U SLUČAJU PREKORAČENJA DEFINISANE VELIČINE KOLEKCIJE, POTREBNO JE BACITI IZUZETAK
		brojevi.Dodaj(v13, v196);
	}
	catch (exception & err)
	{
		cout << "Error -> " << err.what() << endl;
	}
#pragma end
#pragma TestiranjeAplikanata
	Aplikant jasmin("Jasmin Azemovic", Datum(15, 5, 1979), "BS6985645");
	PoslovnaViza poslovna(SAD, dat15022017, dat18082017, "Microsoft");

	/*
	NOVA VIZA SE NE MOŽE DODATI UKOLIKO:
	- JE APLIKANT MLAĐI OD 10 GODINA
	- U TOM PERIODU (ODNOSI SE NA PERIOD VIZE OD - DO) NE POSTOJI VAŽEĆA (POJAM VAŽEĆA PODRAZUMIJEVA DA JE POSLJEDNJI STATUS VIZE BIO IZDATA TJ. DA VIZA NIJE PONIŠTENA ILI ISTEKLA) VIZA ZA TU DRŽAVU, BEZ OBZIRA NA NJENU VRSTU.
	- JE NEKA OD RANIJIH VIZA PONIŠTENA (OVO SE ODNOSI SAMO NA VIZE ZA DRŽAVU: SAD)
	*/
	if (jasmin.DodajVizu(&poslovna))
		cout << "Podaci o vizi uspješno zaprimljeni" << endl;
	/*
	STATUSI SE MOGU DODAVATI SAMO U REDOSLIJEDU U KOJEM SE NALAZE U ENUMERACIJI NPR.
	- NIJE MOGUĆE ODBITI VIZU AKO PRETHODNO NIJE IMALA STATUS DA JE ZAHTJEV_ZAPRIMLJEN;
	- NIJE MOGUĆE PONIŠTITI VIZU KOJA PRETHODNO NIJE IZDATA
	*/
	if (jasmin.DodajStatus(&poslovna, ZAHTJEV_ZAPRIMLJEN, "provjeriti navode sa predstavnicima kompanije Microsoft"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;


	if (jasmin.DodajStatus(&poslovna, PONISTENA, "predstavnicima kompanije Microsoft nemaju informaciju o posjeti navedenog aplikanta"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	TuristickaViza turisticka(ENGLESKA, dat18082017, dat11122017, "Whitehall Pl, London SW1A 2BD, UK");
	jasmin.DodajVizu(&turisticka);
	if (jasmin.DodajStatus(&turisticka, ZAHTJEV_ZAPRIMLJEN))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	if (jasmin.DodajStatus(&turisticka, IZDATA)) {
		cout << "Status vize uspijesno promijenjen" << endl;
		//U ZASEBNOJ NITI URADITI PRINTANJE VIZE...KORISTITI METODU SLEEP_FOR
		jasmin.PrintajVizu(&turisticka);
	}
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;
	//VRAĆA PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODREĐENU DRŽAVU, A KOJE SU IMALE ODREĐENI STATUS
	vector<Viza *> jasminVize = jasmin.GetVizeByDrzava(SAD, PONISTENA);

	//UNUTAR NAPOMENE MIJENJA SADRŽAJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
	jasmin.ZamijeniRijec("nemaju informaciju", "su upoznti sa");
#pragma end;
	getchar();
}