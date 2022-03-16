#include <iostream>

using namespace std;
class Person
{
private:
    int id;
    string imie;
    string nazw;
    string dataUR;
    string dataZM;

public:
    Person(int nr = 0, string n = "", string i = "", string d1 = "", string d2 = "") : id{ nr }, nazw{ n }, imie{ i }, dataUR{ d1 }, dataZM{ d2 } {} //konstruktor

    int GetId()
    {
        return id;
    }
    string GetNazwa()
    {
        return nazw + " " + imie;
    }

    string GetNazw()
    {
        return nazw;
    }
    string GetImie()
    {
        return imie;
    }
    string GetDataUR()
    {
        return dataUR;
    }

    string GetDataZM()
    {
        return dataZM;
    }

    void Edit(int nr, string n, string i, string d1, string d2)// metoda nadaj¹ca wartoœci w klasie
    {
        id = nr;
        nazw = n;
        imie = i;
        dataUR = d1;
        dataZM = d2;
    }
};







