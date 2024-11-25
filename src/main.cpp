#include <iostream>
#include <string>
#include <algorithm> // std::min

// Stałe globalne
constexpr double CI = 200;  // Wartość produktu zwiększana przez inżyniera
constexpr int CMag = 200;  // Pojemność magazynu zwiększana przez magazyniera
constexpr int CMkt = 250;  // Popyt zwiększany przez marketera
constexpr int CR = 300;    // Produkcja zwiększana przez robotnika

class Kredyt {
private:
    double wartosc_kredytu = 0;
    double pozostaly_dlug = 0;
    int czas_splaty = 0; // miesiace
    int liczba_pozostalych_rat = 0;
    double procent_kredytu = 0; 
    double miesieczna_splata_kredytu = 0;
    double miesieczne_odsetki = 0; // dodatkowo doplacic do miesiecznej splaty kredytu
    double miesieczna_rata = 0;
    // przyklad: kredyt o wartosci 50000 zl na 12 mies
    // miesieczna splata kredytu = 50000/12 = 4167 zl
    // procent kredytu = czas splaty = 12%
    // miesieczne odsetki = 0.12 * miesieczna rata = 500 zl

public:
    // konstruktor domyslny - nic nie robi, ale potrzebny do dynamicznej alokacji tablicy
    Kredyt() {}
    // konstruujac obiekt podajemy wartosc kredytu i czas splaty w miesiacach
    Kredyt(double wartosc, int czas_splaty) : 
    wartosc_kredytu(wartosc), pozostaly_dlug(wartosc), czas_splaty(czas_splaty), liczba_pozostalych_rat(czas_splaty) {
        obliczenia_miesiecznej_raty(); // liczy miesieczna rate w trakcie konstrukcji
    }

    double get_wartosc_kredytu() {
        return wartosc_kredytu;
    }

    void obliczenia_miesiecznej_raty() {
        procent_kredytu = czas_splaty;
        miesieczna_splata_kredytu = wartosc_kredytu / czas_splaty;
        miesieczne_odsetki = procent_kredytu/100 * miesieczna_splata_kredytu;
        miesieczna_rata = miesieczna_splata_kredytu + miesieczne_odsetki;
    }

    double get_miesieczna_rata() {
        return miesieczna_rata;
    }

    // wywolywana co miesiac (co ture)
    void zaktualizuj_kredyt() {
        if (liczba_pozostalych_rat > 0) {
            liczba_pozostalych_rat--;
            pozostaly_dlug -= miesieczna_splata_kredytu;
        }
        else {
            miesieczna_rata = 0;
        }
    }

    void wyswietl_info_o_kredycie(){
        std::cout << "Wartosc kredytu: " << wartosc_kredytu << std::endl;
        std::cout << "Czas splaty: " << czas_splaty << std::endl;
        std::cout << "Wysokosc miesiecznej raty: " << miesieczna_rata << std::endl;
        std::cout << "Pozostaly dlug: " << pozostaly_dlug << std::endl;
        std::cout << "Pozostalo rat: " << liczba_pozostalych_rat << std::endl;
    }
};

class Pracownik {
protected:
    std::string imie;

public:
    Pracownik(const std::string& imie) : imie(imie) {}
    virtual ~Pracownik() = default;

    virtual void wyswietlInformacje() const = 0;
};

class Inzynier : public Pracownik {
    std::string wydzial;

public:
    Inzynier(const std::string& imie, const std::string& wydzial)
        : Pracownik(imie), wydzial(wydzial) {}

    void wyswietlInformacje() const override {
        std::cout << "Inzynier: " << imie << ", Wydzial: " << wydzial << "\n";
    }
};

class Magazynier : public Pracownik {
    bool czyObslugujeWozek;

public:
    Magazynier(const std::string& imie, bool czyObslugujeWozek)
        : Pracownik(imie), czyObslugujeWozek(czyObslugujeWozek) {}

    void wyswietlInformacje() const override {
        std::cout << "Magazynier: " << imie
                  << ", Czy obsluguje wozek widlowy: " << (czyObslugujeWozek ? "Tak" : "Nie") << "\n";
    }
};

class Marketer : public Pracownik {
    int liczbaObserwujacych;

public:
    Marketer(const std::string& imie, int liczbaObserwujacych)
        : Pracownik(imie), liczbaObserwujacych(liczbaObserwujacych) {}

    void wyswietlInformacje() const override {
        std::cout << "Marketer: " << imie
                  << ", Liczba obserwujacych: " << liczbaObserwujacych << "\n";
    }
};

class Robotnik : public Pracownik {
    double rozmiarButa;

public:
    Robotnik(const std::string& imie, double rozmiarButa)
        : Pracownik(imie), rozmiarButa(rozmiarButa) {}

    void wyswietlInformacje() const override {
        std::cout << "Robotnik: " << imie
                  << ", Rozmiar buta: " << rozmiarButa << "\n";
    }
};

class Firma {
private:
    double stan_konta = 100000; // poczatkowy stan konta 100 tys zl
    int miesiac = 0; // czas trwania istnienia firmy w miesiacach

    int liczbaInzynierow = 0;
    int liczbaMagazynierow = 0;
    int liczbaMarketerow = 0;
    int liczbaRobotnikow = 0;
    int liczba_pracownikow = 0;

    int wynagrodzenie_inz = 15000;
    int wynagrodzenie_mag = 5000;
    int wynagrodzenie_mark = 10000;
    int wynagrodzenie_rob = 6000;

    // zmienne do obliczania miesiecznego przychodu ze sprzedazy:
    double teoretyczna_maks_produkcja = 0; // np. 1000 produktow
    double pojemnosc_magazynu = 0; // np. 500 produktow
    double faktyczna_produkcja = 0; // np. 500 produktow
    double popyt = 0; // np. 400 produktow
    double liczba_sprzedanych_produktow = 0; // np. 400 produktow
    double cena_produktu = 0; // np. 200 zl
    double mies_przychod_ze_sprzedazy = 0; // 400 * 200 zl = 80000 zl

    double mies_pensje_pracownikow = 0;

    Pracownik* pracownicy[100]; // tablica wskaznikow do obiektow klasy pracownik
    Kredyt* kredyty[3]; // maksymalnie 3 kredyty w trakcie gry
    int aktualna_liczba_kredytow = 0;
    double mies_wydatek_na_splate_kredytow = 0;

public:
    Firma() {
        for (int i = 0; i < 100; i++) {
            pracownicy[i] = nullptr;
        }
        for (int i = 0; i < 3; i++) {
            kredyty[i] = nullptr;
        }
        zatrudnij_pracownika(new Inzynier("Adam", "mechaniczny"));
        zatrudnij_pracownika(new Magazynier("Kacper", true));
        zatrudnij_pracownika(new Marketer("Julia", 5000));
        zatrudnij_pracownika(new Robotnik("Wojtek", 46.0));
    }

    void zatrudnij_pracownika(Pracownik* nowy_pracownik) {
        pracownicy[liczba_pracownikow] = nowy_pracownik;
        liczba_pracownikow++;

        if (dynamic_cast<Inzynier*>(nowy_pracownik)) liczbaInzynierow++;
        else if (dynamic_cast<Magazynier*>(nowy_pracownik)) liczbaMagazynierow++;
        else if (dynamic_cast<Marketer*>(nowy_pracownik)) liczbaMarketerow++;
        else if (dynamic_cast<Robotnik*>(nowy_pracownik)) liczbaRobotnikow++;
    }

    void wez_kredyt(Kredyt* nowy_kredyt) {
        if (aktualna_liczba_kredytow < 3) {
            kredyty[aktualna_liczba_kredytow] = nowy_kredyt;
            aktualna_liczba_kredytow++;
            stan_konta += nowy_kredyt->get_wartosc_kredytu();
        }
        else {
            std::cout << "Wzieto juz maksymalna liczba kredytow." << std::endl;
        }
    }

    void oblicz_mies_wydatek_na_splate_kredytow() {
        double pomocniczo = 0;
        for (int i=0; i<aktualna_liczba_kredytow; i++) {
            pomocniczo += kredyty[i]->get_miesieczna_rata();
            mies_wydatek_na_splate_kredytow = pomocniczo;
        }
    }

    void wyswietl_liste_pracownikow() {
        for (int i=0; i<liczba_pracownikow; i++) {
            pracownicy[i]->wyswietlInformacje();
        }
    }

    void wyswietl_liczbe_pracownikow_wszystkich_typow() {
        std::cout << std::endl;
        std::cout << "liczbaInzynierow: " << liczbaInzynierow << std::endl;
        std::cout << "liczbaMagazynierow: " << liczbaMagazynierow << std::endl;
        std::cout << "liczbaMarketerow: " << liczbaMarketerow << std::endl;
        std::cout << "liczbaRobotnikow: " << liczbaRobotnikow << std::endl;
    }

    // funkcja aktualizuje "zmienne do obliczania miesiecznego przychodu"
    // wywolywana przy zakonczeniu miesiaca
    void obliczenia_przychodu() {
        teoretyczna_maks_produkcja = liczbaRobotnikow * CR;
        pojemnosc_magazynu = liczbaMagazynierow * CMag;
        faktyczna_produkcja = std::min(teoretyczna_maks_produkcja, pojemnosc_magazynu);
        popyt = liczbaMarketerow * CMkt;
        liczba_sprzedanych_produktow = std::min(faktyczna_produkcja, popyt);
        cena_produktu = liczbaInzynierow * CI;
        mies_przychod_ze_sprzedazy = liczba_sprzedanych_produktow * cena_produktu;
    }

    void oblicz_pensje_pracownikow() {
        double wydatki_inz = liczbaInzynierow * wynagrodzenie_inz;
        double wydatki_mag = liczbaMagazynierow * wynagrodzenie_mag;
        double wydatki_mark = liczbaMarketerow * wynagrodzenie_mark;
        double wydatki_rob = liczbaRobotnikow * wynagrodzenie_rob;
        mies_pensje_pracownikow = wydatki_inz + wydatki_mag + wydatki_mark + wydatki_rob;
    }

    // funkcja wyswietlajaca info o produkcji
    void wyswietl_dane_produkcji() {
        std::cout << std::endl << "Szczegolowe dane o produkcji: " << std::endl;
        std::cout << "Teoretyczna maksymalna produkcja: " << teoretyczna_maks_produkcja << std::endl;
        std::cout << "Pojemnosc magazynu: " << pojemnosc_magazynu << std::endl;
        std::cout << "Faktyczna produkcja: " << faktyczna_produkcja << std::endl;
        std::cout << "Popyt: " << popyt << std::endl;
        std::cout << "Liczba sprzedanych produktow: " << liczba_sprzedanych_produktow << std::endl;
        std::cout << "Cena produktu: " << cena_produktu << std::endl;
        std::cout << "Miesieczny przychod ze sprzedazy: " << mies_przychod_ze_sprzedazy << std::endl;
        std::cout << "Wydatek na pensje pracownikow: " << mies_pensje_pracownikow << std::endl;
        std::cout << "Wydatek na splate kredytow: " << mies_wydatek_na_splate_kredytow << std::endl;
        std::cout << "Zysk: " << mies_przychod_ze_sprzedazy - mies_pensje_pracownikow << std::endl;
    }

    void zaktualizuj_stan_konta() {
        stan_konta += mies_przychod_ze_sprzedazy;
        stan_konta -= mies_pensje_pracownikow;
        stan_konta -= mies_wydatek_na_splate_kredytow;
    }

    void wyswietl_stan_konta() {
        std::cout << "Stan konta: " << stan_konta << std::endl;
    }

    double get_stan_konta() {
        return stan_konta;
    }

    void zakoncz_miesiac() {
        obliczenia_przychodu();
        oblicz_pensje_pracownikow();
        oblicz_mies_wydatek_na_splate_kredytow();
        zaktualizuj_stan_konta();
        miesiac++;
        std::cout << std::endl << "Podsumowanie " << miesiac << ". miesiaca:" << std::endl;
        wyswietl_stan_konta();
        wyswietl_liczbe_pracownikow_wszystkich_typow();
        wyswietl_dane_produkcji();
    }
};

class Gra {
private:
    Firma firma;
public:
    double get_stan_konta() {
        return firma.get_stan_konta();
    }
    void wyswietl_akcje_gracza() {
        std::cout << std::endl << "Wybierz opcje:\n";
        std::cout << "1. Wyswietl liste pracownikow\n";
        std::cout << "2. Zatrudnij nowego pracownika\n";
        std::cout << "3. Weź kredyt\n";
        std::cout << "4. Zakoncz miesiac\n";
    }
    void wykonaj_akcje_gracza(int liczba) {
        switch (liczba) {
            case 1: {
                firma.wyswietl_liste_pracownikow();
                break;
            }
            case 2: {
                std::cout << "Wybierz typ pracownika do zatrudnienia:\n";
                std::cout << "1. Inzynier\n2. Magazynier\n3. Marketer\n4. Robotnik\n";
                int typ;
                std::cin >> typ;

                std::string imie;
                std::cout << "Podaj imie pracownika: ";
                std::cin >> imie;

                switch (typ) {
                    case 1: {
                        std::string wydzial;
                        std::cout << "Podaj wydzial: ";
                        std::cin >> wydzial;
                        firma.zatrudnij_pracownika(new Inzynier(imie, wydzial));
                        break;
                    }
                    case 2: {
                        bool obslugujeWozek;
                        std::cout << "Czy obsluguje wozek widlowy (1 - tak, 0 - nie): ";
                        std::cin >> obslugujeWozek;
                        firma.zatrudnij_pracownika(new Magazynier(imie, obslugujeWozek));
                        break;
                    }
                    case 3: {
                        int obserwujacy;
                        std::cout << "Podaj liczbe obserwujacych: ";
                        std::cin >> obserwujacy;
                        firma.zatrudnij_pracownika(new Marketer(imie, obserwujacy));
                        break;
                    }
                    case 4: {
                        double rozmiarButa;
                        std::cout << "Podaj rozmiar buta: ";
                        std::cin >> rozmiarButa;
                        firma.zatrudnij_pracownika(new Robotnik(imie, rozmiarButa));
                        break;
                    }
                    default:
                        std::cout << "Nieprawidlowy wybor!\n";
                } 
                break;
            }
            case 3: {
                std::cout << "Podaj wartosc kredytu: ";
                double wartosc;
                std::cin >> wartosc;
                std::cout << "Podaj czas trwania kredytu (w miesiacach): ";
                int czas;
                std::cin >> czas;
                firma.wez_kredyt(new Kredyt(wartosc, czas));
                break;
            }
            case 4: {
                firma.zakoncz_miesiac();
                break;
            }
            default:
                break;
        }
    }
};

int main() {
    Gra gra;
    double warunek_wygranej = 1000000.0; // 1 milion zl
    int i = 0;
    while (gra.get_stan_konta() > 0 && gra.get_stan_konta() < warunek_wygranej) {
        int wybor = 0;
        while (wybor != 4) { // 4 - zakonczenie miesiaca
            gra.wyswietl_akcje_gracza();
            std::cin >> wybor;
            gra.wykonaj_akcje_gracza(wybor);
        }
    }

    if(gra.get_stan_konta() >= warunek_wygranej) {
        std::cout << "Wygrales";
    }
    else {
        std::cout << "Zbankrutowales";
    }

    return 0;
}
