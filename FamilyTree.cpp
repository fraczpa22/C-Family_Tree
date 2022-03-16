#include <iostream>
#include <vector>
#include <fstream>
#include <conio.h> //getch()
#include <sstream> 
#include <locale.h> // polskie znaki
#include <windows.h>  //gotoxy
#include <iomanip> //setw
#include "Person.h" //plik zmiennych globalnych

using namespace std;

//---------------------------------------------------------------
// Kolory konsoli
//---------------------------------------------------------------

const int BLACK = 0;
const int BLUE = 1;
const int GREEN = 2;
const int CYAN = 3;
const int RED = 4;
const int MAGENTA = 5;
const int BROWN = 6;
const int LIGHTGRAY = 7;
const int DARKGRAY = 8;
const int LIGHTBLUE = 9;
const int LIGHTGREEN = 10;
const int LIGHTCYAN = 11;
const int LIGHTRED = 12;
const int LIGHTMAGENTA = 13;
const int YELLOW = 14;
const int WHITE = 15;

//---------------------------------------------------------------
// Zmiana koloru konsoli
//---------------------------------------------------------------
void textcolor(int color, int color2)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color + (color2 << 4));
}

//---------------------------------------------------------------
// Nowy typ - tablica dynamiczna
// Przechowywanie liczby osób w drzewie
//---------------------------------------------------------------

struct Ttab
{
    vector<int> tab;
};

//---------------------------------------------------------------
// Tablice dynamiczne osób w drzewie
//---------------------------------------------------------------

vector<Person> Osoby;       //tablica główna danych wszystkich osób
vector<Ttab> Rodzice;       //tablica identyfikatorów
vector<Ttab> Partnerzy;     //tablica identyfikatorów 
vector<Ttab> Dzieci;        //tablica identyfikatorów 
ifstream plik;              //zmienna plikowa do wczytania danych
int idOsoby;            //identyfikator dla nowej osoby


//---------------------------------------------------------------
// Współrzędne konsoli
//---------------------------------------------------------------
void gotoxy(int y, int x)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


//---------------------------------------------------------------
// Wyszukuje najwiekszy id osoby
//---------------------------------------------------------------
int MaxID()
{
    int m = 0;
    for (int i = 0; i < Osoby.size(); i++)
    {
        if (Osoby[i].GetId() > m)
        {
            m = Osoby[i].GetId();
        }
    }
    return m;
}

//---------------------------------------------------------------
// Wczytanie danych z plików do tablic
//---------------------------------------------------------------
void WczytajOsoby() //wczytanie głównych danych z pliku osoby.txt
{
    int id;
    string imie, nazw;
    string dataUr;
    string dataZM;

    plik.open("osoby.txt");
    if (plik) //jeżli plik istnieje
    {
        while ( ! plik.eof() ) //dopóki nie ma końca pliku
        {
            plik >> id; 
            plik >> nazw;
            plik >> imie;
            plik >> dataUr;
            plik >> dataZM;

            Person Person(id, nazw, imie, dataUr, dataZM); //dodanie danych do klasy
            Osoby.push_back(Person);     //dodaj dane tablicy Osoby
        }
        idOsoby = MaxID()+1;
    } else
        cout << "brak pliku" << endl;
    
    plik.close();
}

//---------------------------------------------------------------
// Wczytanie danych z plików do tablic
//---------------------------------------------------------------

void WczytajIdentyfikatory(string file, vector<Ttab>& Tab) // & wskaźnik do adresu tablicy
{    
    plik.open(file);
    if (plik) //jeśli plik istnieje
    {
        string linia;
        string wyr;
        int liczba;
        while ( ! plik.eof() )
        {
            getline(plik, linia); //wczytanie linijki tekstu z pliku
            wyr = linia[0]; //przypisz znak 1 do wyr 
            if (linia != "")
            {
                 Ttab tabela;
                 for (int i = 1; i < linia.size(); i++)
                 {
                    if (linia[i] != ' ')                
                        wyr = wyr + linia[i];
                    else
                    {
                        istringstream is(wyr); //zamiana tekstu na liczbe
                        is >> liczba;
                        tabela.tab.push_back(liczba);
                        wyr = "";
                    }
                 }
                 Tab.push_back(tabela); //dodanie tabeli liczb z linijki do głównej tablicy
            }
        }        
    }else    
        cout << "brak pliku" << endl;   
    plik.close();
}

//---------------------------------------------------------------
// Zapis danych do plików
//---------------------------------------------------------------

void ZapisRelacji(string file, vector<Ttab>& Tab)
{
    ofstream plik; //zmienna plikowa do zapisu
    plik.open(file);
    if (plik)
    {
        if (Tab.size() > 0)
        {
            for (int i = 0; i < Tab.size(); i++)
            {
                for (int j = 0; j < Tab[i].tab.size(); j++)
                {
                    plik << Tab[i].tab[j] << " ";
                }

                if (i < Tab.size() - 1)
                {
                    plik << endl;   //dodanie entera po każdej linijce oprócz ostatniej osoby
                }
            }
        }
    }else
        cout << "błąd pliku";
    plik.close();
}

void ZapisOsoby()
{
    ofstream plik; 
    plik.open("osoby.txt");
    if (plik)
    {
        int i;
        if (Osoby.size() > 0)
        {
            for (i = 0; i < Osoby.size() - 1; i++)
            {
                plik << Osoby[i].GetId() << " " << Osoby[i].GetNazwa() << " " << Osoby[i].GetDataUR() << " " << Osoby[i].GetDataZM() << endl;
            }
            plik << Osoby[i].GetId() << " " << Osoby[i].GetNazwa() << " " << Osoby[i].GetDataUR() << " " << Osoby[i].GetDataZM();
        }
    }else
        cout << "błąd pliku";

    plik.close();

    ZapisRelacji("rodzice.txt", Rodzice);
    ZapisRelacji("partnerzy.txt", Partnerzy);
    ZapisRelacji("Dzieci.txt", Dzieci);
}

//---------------------------------------------------------------
// Wyszukiwanie w tablicy osoby
//---------------------------------------------------------------

int FindPerson(int id)   //zwraca nr wiersza w tablicy osoby o wskazanym id
{
    for (int i = 0; i < Osoby.size(); i++)
    {
        if (Osoby[i].GetId()  == id) return i; //zwraca nr wiersza w tablicy
    }         
    return -1;      //jeżeli nie znaleziono id to -1 
}

//---------------------------------------------------------------
// Wyszukiwanie w tablicy relacji
//---------------------------------------------------------------

int FindId(int id, vector<Ttab>& Tab)
{ 
    for (int i = 0; i < Tab.size(); i++)
    {
        if (Tab[i].tab[0] == id) return i; //zwraca nr wiersza w tablicy
    }
    return -1; //jeżeli nie znaleziono id to -1 
}

//---------------------------------------------------------------
// Wyświetlanie danych
//---------------------------------------------------------------

int ShowPerson(int id, vector<Ttab>& Tab, int wiersz, int kol) //wyswietlanie osób w relacji
{
    int nr, nr2;
    //szuka wpisu id osoby w danej tablicy
    nr = FindId(id, Tab); 
    if (nr >= 0)
    {
        //jeśli jest wpis
        for (int j = 1; j < Tab[nr].tab.size(); j++)
        {
            //szuka pozycji osoby przypisanej 
            nr2 = FindPerson(Tab[nr].tab[j]);
            if (nr2 >= 0)
            {
                gotoxy(wiersz + j - 1, kol);
                cout << Osoby[nr2].GetId() << ":" << Osoby[nr2].GetNazwa() << endl;
            }
        }
        return Tab[nr].tab.size(); //zwraca ilosc znalezionych osób
    }
    return 0;
}

int  Show(int nr, int wiersz)//wyswietlanie Osoby i relacji
{
    int ileW = 0, w = 0;
    int kol = 0;
    
    gotoxy(wiersz, kol);    
    textcolor(RED, 0);
    cout << Osoby[nr].GetId() << " " << Osoby[nr].GetNazwa()<<endl;
    kol = 31;
    textcolor(15, 0);
    w =  ShowPerson(Osoby[nr].GetId(), Rodzice, wiersz, kol);
    if (w > ileW) ileW = w;//określenie ilości wierszy odstępu
    kol = kol + 30;
    w =  ShowPerson(Osoby[nr].GetId(), Partnerzy, wiersz, kol);
    if (w > ileW) ileW = w;//określenie ilości wierszy odstępu
    kol = kol + 30;
    w = ShowPerson(Osoby[nr].GetId(), Dzieci, wiersz, kol);
    if (w > ileW) ileW = w;    //określenie ilości wierszy odstępu    
    cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
    if (ileW == 0)//określenie ilości wierszy odstępu
        ileW = 2;
    return wiersz + ileW;//zwracanie ilości wierszy odstępu
}
//---------------------------------------------------------------
// Wyświetlanie drzewa
//---------------------------------------------------------------
void ShowTree()
{
    textcolor(0, 15);
    cout << "Nazwisko i imie:" << setw(23) << "Rodzice:" << setw(32) << "Partnerzy:" << setw(27) << "Dzieci:" << setw(22) << " ";//setw - wbudowana funkcja do odstępu w tekście
    textcolor(15, 0);
    int wiersz = 2;
    for (int nr = 0; nr < Osoby.size(); nr++) // wypisywanie kolejnych osób
    {
        wiersz = Show(nr, wiersz);
    }
    textcolor(YELLOW, 0);
    cout << "Nacisnij dowolny klawisz..."; //zakończenie wyświetlania
    _getch();
    textcolor(15, 0);
   
}


//---------------------------------------------------------------
// Edytowanie danych
//---------------------------------------------------------------

void EditPerson()
{
    int id, nr;
    int wyb;
    string imie, nazw;
    string data;
    string idTxt;
    do 
    {                                            //dopóki nie uzyskamy prawidłowych danych
        system("cls");                              //czyszczenie ekranu konsoli
        textcolor(RED, 0);
        for (int i = 0; i < Osoby.size(); i++)           //wyswietlenie listy wszystkich osób
        {
            gotoxy(i, 60);
            cout << Osoby[i].GetId() << ": " << Osoby[i].GetNazwa() << endl;
        }

        textcolor(YELLOW, 0);
   
        gotoxy(0, 0);
        cout << "Podaj id osoby (0 - powrót):";     //wybór osoby
        cin >> idTxt;
        if (idTxt == "0")
            return;
        istringstream is(idTxt); //zamiana tekstu na liczbe
        is >> id;    
        nr = FindPerson(id);  

    } while (nr==-1);

    textcolor(RED, 0);  
    cout << "1.Nazwisko i imię: "; 
    textcolor(15, 0); 
    cout << Osoby[nr].GetNazwa() << endl;
    textcolor(RED, 0);
    cout << "2.Data urodzenia:"; 
    textcolor(15, 0); 
    cout << Osoby[nr].GetDataUR() << endl;
    textcolor(RED, 0);
    cout << "3.Data śmierci:"; 
    textcolor(15, 0); 
    cout << Osoby[nr].GetDataZM() << endl;
    textcolor(YELLOW, 0);

    cout <<endl << "Edytować osobę?(t/n):";
    textcolor(15, 0);

    bool menu = true;

    do {
        switch (_getch())
        {
            case 't':                           //edycja danych osoby
                do {
                    textcolor(YELLOW, 0);
                    cout << endl << "Jakie dane edytować? ( 1-3 / Esc ):" << endl;
                    textcolor(15, 0);
                    switch (_getch())
                    {
                        case '1':

                            cout << "Podaj Nazwisko: ";
                            cin >> nazw;
                            cout << "Podaj Imię: ";
                            cin >> imie;
                            Osoby[nr].Edit(id, nazw, imie, Osoby[nr].GetDataUR(), Osoby[nr].GetDataZM());
                            break;
                        case '2':
                            cout << "Podaj datę urodzenia: ";
                            cin >> data;
                            Osoby[nr].Edit(id, Osoby[nr].GetNazw(), Osoby[nr].GetImie(), data, Osoby[nr].GetDataZM());
                            break;
                        case '3':
                            cout << "Podaj datę śmierci: ";
                            cin >> data;
                            Osoby[nr].Edit(id, Osoby[nr].GetNazw(), Osoby[nr].GetImie(), Osoby[nr].GetDataUR(), data);
                            break;
                        case 27: menu = false;
                            break;
                    }
                } while (menu == true);
                break;
            case 'n': 
            case 27: menu = false; //zakonczenie powrót do menu jesli esc lub "n"
                break;
        }
    } while (menu == true);
    
}


//---------------------------------------------------------------
// Dodawanie relacji
//---------------------------------------------------------------

void dodajRelacje(int id, int id2, vector<Ttab>& Tab)
{
    bool jest = false;
    bool dodany = false;
    int nr;

    if (Tab.size() > 0)
    {
        nr = FindId(id, Tab);
        if (nr != -1)
        {
            for (int j = 0; j < Tab[nr].tab.size(); j++)  //sprawdzamy czy jest już dodana taka relacja
            {
                if (Tab[nr].tab[j] == id2)
                {
                    jest = true;
                    dodany = true;
                    break;
                }
            }
            if (!jest)  //jeśli jest id ale nie ma relacji to dodaj nową relację
            {
                Tab[nr].tab.push_back(id2);
                dodany = true;
            }
        }
    }

    if (Tab.size() < 1 || dodany == false) //jeśli tablica jest pusta lub nie ma wpisu relacji dla podanego id
    {   //to tworzenie nowego wpisu relscji
        Ttab tablica;
        tablica.tab.push_back(id);
        tablica.tab.push_back(id2);
        Tab.push_back(tablica);
    }
}

//---------------------------------------------------------------
// Dodanie nowej osoby
//---------------------------------------------------------------
void AddPerson()
{
    string idTxt;
    int id, nr;
    int wyb;

    do {
        system("cls");
        textcolor(RED, 0);
        for (int i = 0; i < Osoby.size(); i++)
        {
            gotoxy(i, 60);
            cout << Osoby[i].GetId() << ": " << Osoby[i].GetNazwa() << endl;
        }

        textcolor(YELLOW, 0);

        gotoxy(0, 0);
        cout << "wybierz id osoby ( 0 - powrót):";
        cin >> idTxt;
        if (idTxt == "0")
            return;
        istringstream is(idTxt); //zamiana tekstu na liczbe
        is >> id;

        nr = FindPerson(id);
    } while (nr == -1);

    textcolor(15, 0);
    system("cls");
    cout << "Wybrana Person: " << endl;
    textcolor(0, 15);
    cout << "Nazwisko i imie:" << setw(23) << "Rodzice:" << setw(32) << "Partnerzy:" << setw(27) << "Dzieci:" << setw(22) << " ";
    textcolor(15, 0);
    Show(nr, 2);

    //nowa Person                
    string imie, nazw;
    Person t;
    string data1, data2;
    bool dodaj = true;
    bool spr = false;
    char odp, odp2;
    int id2;
    int dod;

    do {
        textcolor(YELLOW, 0);
        cout << "Czy dodac osobe?(t/n):";
        textcolor(15, 0);
        do {
            switch (_getch())
            {
            case  't':
                spr == false;
                cout << endl;
                cout << "1. Rodzic" << endl;
                cout << "2. Partner" << endl;
                cout << "3. Dziecko" << endl;
                cout << "Esc. Powrót do menu" << endl;
                cout << "-------------------------" << endl;
                textcolor(YELLOW, 0);
                cout << "Wybierz opcję ( 1-3 / Esc ):" << endl;
                textcolor(15, 0);
                do {
                    switch (_getch())
                    {
                    case  '1':
                        odp = '1';
                        spr = true;
                        break;
                    case  '2':
                        odp = '2';
                        spr = true;
                        break;
                    case  '3':
                        odp = '3';
                        spr = true;
                        break;
                    case  27:
                        return;
                        break;
                    }
                } while (spr == false);

                spr = false;
                textcolor(YELLOW, 0);
                cout << "Czy dodac istniejącą osobę? ( t/n )";
                textcolor(15, 0);
                do {
                    switch (_getch())
                    {
                    case  't':
                        textcolor(YELLOW, 0);
                        cout << endl << "Podaj id osoby ( 0 -powrót ):";
                        textcolor(15, 0);
                        do {
                            cin >> idTxt;
                            istringstream is(idTxt); //zamiana tekstu na liczbe
                            is >> id2;
                        } while (id2 <= 0);
                        spr = true;
                        if (id2 == 0)
                            return;
                        break;

                    case  'n':
                        cout << endl << "id:" << idOsoby << endl;
                        cout << "Nazwisko:"; cin >> nazw;
                        cout << "Imię:"; cin >> imie;
                        cout << "Data urodzenia:"; cin >> data1;
                        cout << "Data śmierci:"; cin >> data2;

                        t.Edit(idOsoby, nazw, imie, data1, data2);
                        id2 = idOsoby;
                        Osoby.push_back(t);

                        spr = true;
                        break;

                    case  27:
                        return;
                        break;
                    }
                } while (spr == false);


                //dodanie rekordu do bazy   
                switch (odp)
                {
                case '1': dodajRelacje(id, id2, Rodzice);
                    dodajRelacje(id2, id, Dzieci);
                    break;
                case '2': dodajRelacje(id, id2, Partnerzy);
                    dodajRelacje(id2, id, Partnerzy);
                    break;
                case '3': dodajRelacje(id, id2, Dzieci);
                    dodajRelacje(id2, id, Rodzice);
                    break;
                }

                spr = true;
                break;
            case  'n':
                spr = true;
                dodaj = false;
                break;
            case  27:
                return;
                break;
            }
        } while (spr == false);

    } while (dodaj == true);
}



//---------------------------------------------------------------
// Main
//---------------------------------------------------------------

int main()
{        
    setlocale(LC_CTYPE, "Polish"); //polskie znaki
      //wczytanie danych z plików do tablic
    WczytajOsoby();                           
    WczytajIdentyfikatory("rodzice.txt", Rodzice);
    WczytajIdentyfikatory("partnerzy.txt", Partnerzy);
    WczytajIdentyfikatory("dzieci.txt", Dzieci);

    //tworzenie menu
   
    do {
        int kolumna = 40;
        int w;
        system("cls");
        textcolor(GREEN, 0);
        gotoxy(0, kolumna);
        cout << "AKADEMIA GÓRNICZO-HUTNICZA W KRAKOWIE" << endl;
        gotoxy(1, kolumna + 6);
        cout << " WIEIT Elektronika 1 rok " << endl;        
        gotoxy(3, kolumna +10);
        textcolor(13, 0);
        cout << "Paweł Frączkiewicz" << endl;
        textcolor(GREEN, 0);
        gotoxy(5, kolumna -3);
        cout << "- D R Z E W O    G E N E A L O G I C Z N E -" << endl;
        gotoxy(8,0);
        textcolor(LIGHTBLUE, 0);
        w = 8;
        kolumna += 2;
        gotoxy(w++, kolumna); cout << "--------------MENU--------------" << endl;
        textcolor(RED, 0);
        gotoxy(w++, kolumna); cout << "   1. Dodaj osobę" << endl;
        gotoxy(w++, kolumna); cout << "   2. Zmień/wyświetl osobę" << endl;
        gotoxy(w++, kolumna); cout << "   3. Wyswietl drzewo" << endl;
        gotoxy(w++, kolumna); cout << " Esc.Koniec" << endl;
        textcolor(LIGHTBLUE, 0);
        gotoxy(w++, kolumna); cout << "--------------------------------" << endl;
        textcolor(YELLOW, 0);
        gotoxy(w++, kolumna); cout << "wybierz opcję ( 1-3 /Esc ):";
        textcolor(15, 0);
    
        switch ( _getch() )
        {
            case  '1':
                system("cls"); 
                AddPerson();
                ZapisOsoby();
                break;
            case  '2':
                system("cls");
                EditPerson();
                ZapisOsoby();
                break;
            case  '3':    
                system("cls");
                ShowTree();
                ZapisOsoby();
                break;
            case  27: 
                ZapisOsoby();
                return 0;
                break;
        }
    } while (true);


}