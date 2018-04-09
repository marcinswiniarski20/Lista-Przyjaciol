// POP 2017-01-23 projekt 2 Œwiniarski Marcin AIR 3 165631 Dev-C++ 5.11 TDM-GCC 4.9.2 64-bit
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <windows.h>
#include <algorithm>

using namespace std;
const int max_wyrazow=10; // maksymalna ilosc wyrazow minus komentarz

struct osoba
{
	string imie;
	string nazwisko;
	string urodziny;
	string imieniny;
	string komentarz;
	string zdjecie;
	bool niedlugo_uro;
	bool niedlugo_imie;
};

int ile_linii(const char*);
void stworz_css(const char*);
void wydobywanie_wyrazow(string, string[]);
void urodziny_i_imieniny(string[], int&, int&);
void czyszczenie_komentarza(string, osoba[], int);
void konwersja_daty(string&);
bool sprawdzenie_daty(string, int, int);
void sortowanie(osoba[], int);
void urodziny_i_imieniny(string[], int&, int&);
void czy_uro_czy_imie(osoba[], int);
int stworz_html(osoba[], int, const char*, const char*);
int sprawdzanie_bledow(string);

int main(int argc, char** argv){
	
	int ile=ile_linii(argv[1]);
	osoba os[ile];
	ifstream odczyt(argv[1]);	
	if(!odczyt){
	    cout << "Nie udalo sie otworzyc pliku do odczytu!";
	    return 0;
	}
	else if(odczyt.good())
		cout<<"Plik do odczytu zostal otwarty poprawnie!"<<endl;
	stworz_css(argv[3]);

	for(int k=0; k<ile; k++){    //pêtla przechodzaca pomiêdzy liniami pliku tekstowego
		string linia;
		string wyrazy[max_wyrazow]; 
		int indeks_uro = 0;
		int indeks_imi = 0;
		getline(odczyt, linia);
		sprawdzanie_bledow(linia);
		wydobywanie_wyrazow(linia, wyrazy);
		urodziny_i_imieniny(wyrazy, indeks_uro, indeks_imi);		
		for(int i=0; i<indeks_uro-1; i++)	//przypisywanie wydobytych wyrazow do okreslonych struktur
			os[k].imie = os[k].imie + " " +  wyrazy[i];	
		os[k].nazwisko = wyrazy[indeks_uro-1];
		os[k].urodziny = wyrazy[indeks_uro+1];
		os[k].imieniny = wyrazy[indeks_imi+1];
		czyszczenie_komentarza(linia, os, k);
		konwersja_daty(os[k].urodziny);	
		konwersja_daty(os[k].imieniny);
		czy_uro_czy_imie(os, k);		
	}	
	sortowanie(os, ile);
	stworz_html(os, ile, argv[2], argv[3]);	
	system ("pause");
	return 0;
}
// funkcja liczaca ilosc linii w pliku tekstowym
int ile_linii(const char* nazwa_pliku){
	string linia;
    int licznik = 0;
	ifstream odczyt(nazwa_pliku);	
    while (getline(odczyt, linia)) 
		licznik ++;
    odczyt.close();
    odczyt.clear();
    return licznik;
}
// funkcja tworzaca plik css, ktory pozwala na estetyczny wyglad strony
void stworz_css(const char* nazwa_pliku){
	ofstream plik(nazwa_pliku);
	plik<<"body"<<endl;
	plik<<"{"<<endl;
	plik<<"	background-color: #303030;"<<endl;
	plik<<"	color: gold;"<<endl;
	plik<<"	font-size: 16px;	"<<endl;
	plik<<"}"<<endl;
	plik<<"#container"<<endl;
	plik<<"{"<<endl;
	plik<<"	width: 900px;"<<endl;
	plik<<"	margin-left: auto;"<<endl;
	plik<<"	margin-right: auto;"<<endl;
	plik<<"}"<<endl;
	plik<<"#logo"<<endl;
	plik<<"{"<<endl;
	plik<<"	background-color: black;"<<endl;
	plik<<"	text-align: center;"<<endl;
	plik<<"	margin: 7.5px;"<<endl;
	plik<<"	padding:0.2px;"<<endl;
	plik<<"}"<<endl;
	plik<<"#menu"<<endl;
	plik<<"{"<<endl;
	plik<<"	float: left;"<<endl;
	plik<<"	background-color: dimgray;"<<endl;
	plik<<"	width: 105px;"<<endl;
	plik<<"	text-align: center;"<<endl;
	plik<<"	min-height: 465px;"<<endl;
	plik<<"	padding:7.5px;"<<endl;
	plik<<"	margin: 7.5px;"<<endl;
	plik<<"}"<<endl;
	plik<<"#content"<<endl;
	plik<<"{"<<endl;
	plik<<"	float: left;"<<endl;
	plik<<"	padding: 15px;"<<endl;
	plik<<"	width: 480px;"<<endl;
	plik<<"	text-align:justify;"<<endl;
	plik<<"}"<<endl;
	plik<<"#footer"<<endl;
	plik<<"{"<<endl;
	plik<<"	clear: both;"<<endl;
	plik<<"	background-color: black;"<<endl;
	plik<<"	text-align: center;"<<endl;
	plik<<"	margin:10px;"<<endl;
	plik<<"	font-size: 12.75px;"<<endl;
	plik<<"	font-style: italic;"<<endl;
	plik<<"}"<<endl;
	plik<<"table "<<endl;
	plik<<"{"<<endl;
	plik<<"	border-collapse:collapse;"<<endl;
	plik<<"	text-align:center;"<<endl;
	plik<<" border-color:black;"<<endl;
	plik<<" border: 3px solid black;"<<endl;
	plik<<"}"<<endl;
	plik<<"th"<<endl;
	plik<<"{"<<endl;
	plik<<"	border: 3px solid black;"<<endl;
	plik<<"	padding: 2px 15px 2px 15px;"<<endl;
	plik<<"}"<<endl;
	plik<<"td"<<endl;
	plik<<"{"<<endl;
	plik<<"	border: 2px solid black;"<<endl;
	plik<<"	padding: 2px 15px 2px 15px;"<<endl;
	plik<<"}"<<endl;
	plik<<"a.tilelink"<<endl;
	plik<<"{"<<endl;
	plik<<"	text-decoration:none;"<<endl;
	plik<<"	color: gold;"<<endl;
	plik<<"}"<<endl;
	plik<<"th.urodziny"<<endl;
	plik<<"{"<<endl;
	plik<<"	background-color: red;"<<endl;
	plik<<"}"<<endl;
	plik<<"th.imieniny"<<endl;
	plik<<"{"<<endl;
	plik<<"	background-color: blue;"<<endl;
	plik<<"}"<<endl;
	plik.close();
	plik.clear();
}

//funkcja wydobywajaca wyrazy z okreslonej linii pliku tekstowego
void wydobywanie_wyrazow(string linia, string wyrazy[])
{
	bool znaleziono = false; // na starcie - nie znaleziono spacji, tabulatora ani :
	int ktory = 0;
	int indeks1 = 0;
	int indeks2 = 0;
	int found1;
	int found2;
	found1 = linia.find("#"); 
	linia = linia.substr(found1+1); // ucinamy cala linie o #
	found1 = linia.find("<"); // znajdujemy miejsce poczatku komentarza
	for(int i=0; i<=found1+1; i++)
		{
			if(!znaleziono)
			{
				if(linia.at(i) == 32 || linia.at(i) == 9 || linia.at(i) == 58 || linia.at(i) == 60)  // jesli znaleziono spacjê, tabulator albo :
				{
					indeks2 = i;
					znaleziono = true;
				}
			}
			else
			{
				wyrazy[ktory] = linia.substr(indeks1, indeks2-indeks1); // przypisujemy do tablicy wyrazow wyraz od indeksu1 do znaku ktorego sie pozbywamy
				if(linia.at(i) != 32 && linia.at(i) != 9 && linia.at(i) != 58 && linia.at(i) != 60)
				{
					indeks1 = i;
					znaleziono = false;
					ktory++;
				}
			}
		}
}
//funkcja pozbywajaca sie niepotrzebnych znakow w komentarzu, tak aby wyglad strony byl poprawnie sformatowany
void czyszczenie_komentarza(string linia, osoba os[], int k)
{
	bool znaleziono = false; // poczatkowo przyjmujemy ze sa jakies zbedne znaki
	bool znaleziono2 = true; 
	int ktory = 0;
	int indeks1 = 0;
	int indeks2 = 0;
	int found1 = linia.find("<");
	int found2 = linia.find(">");
	string wstepny_komentarz = linia.substr(found1+1, found2-found1+1); // zapisujemy w wstepnym komentarzu cala zawartosc od < do >
	found1 = linia.find("["); // wydobywanie cech zdjecia
	found2 = linia.find("]");
	os[k].zdjecie = linia.substr(found1+1, found2-found1-1);
	for(int i=0; i<wstepny_komentarz.length(); i++)
	{
		if(!znaleziono)
		{			// pozbywamy sie zbednych spacji, tabulatora, : , przecinka, kropki
			if(wstepny_komentarz.at(i) == 32 || wstepny_komentarz.at(i) == 9 || wstepny_komentarz.at(i) == 44 || wstepny_komentarz.at(i) == 46 || wstepny_komentarz.at(i) == 62)
			{
				znaleziono = true;
				indeks2 = i;
			}
		}
		else
		{
			string str = wstepny_komentarz.substr(indeks1, indeks2-indeks1); // po pozbyciu sie znaku przypisujemy do zmiennej str wydobytego stringa
			if(znaleziono2) //funkcje warunkowe wykluczajace przypadek gdyby wystepowalo wiecej niz jedna kropka/spacja/tabulator obok siebie
			{
				znaleziono2 = false;
				os[k].komentarz = os[k].komentarz + " " + str;
			}
			
			if(wstepny_komentarz.at(i) != 32 && wstepny_komentarz.at(i) != 9 && wstepny_komentarz.at(i) != 44 && wstepny_komentarz.at(i) != 46 && wstepny_komentarz.at(i) != 62)
			{
				indeks1 = i;
				znaleziono = false;
				znaleziono2 = true;
				ktory++;
			}			
		}
	}
}
//funkcja zmieniajaca date na wlasciwy format
void konwersja_daty(string &data)
{
	int found = data.find(".");
	string czlon[3]; // data urodzin sklada sie z 3 czlonow

	if(found == string::npos) // jesli nie znaleziono kropki to format daty jest niepoprawny
	{
		found = data.find("/"); //sprawdzamy czy znajduje sie slesh

		if(found != string::npos) // jesli tak to zmieniamy datê tego typu na datê formatu poprawnego z kropkami
		{
			czlon[0] = data.substr(0, found);
			data = data.substr(found+1);

			found = data.find("/");
			if(found != string::npos)
			{
				czlon[1] = data.substr(0, found);
				czlon[2] = data.substr(found+1);
				data = czlon[1] + "." + czlon[0] + "." + czlon[2];
			}
			else // w przypadku imienin mamy tylko 2 czlony daty
			{
				czlon[1] = data;
				data = czlon[1] + "." + czlon[0];
			}
		}
		else // przypadek analogiczny dla daty formatu z ³¹cznikiem -
		{
			found = data.find("-");
			if(found != string::npos)
			{
				czlon[0] = data.substr(0, found);
				data = data.substr(found+1);

				found = data.find("-");
				if(found != string::npos)
				{
					czlon[1] = data.substr(0, found);
					czlon[2] = data.substr(found+1);
					data = czlon[2] + "." + czlon[1] + "." + czlon[0];
				}
				else
				{
					czlon[1] = data;
					data = czlon[1] + "." + czlon[0];
				}
			}
		}
	}
}
// funkcja sprawdzajaca czy w najblizszym czasie dana osoba ma urodziny badz imieniny
bool sprawdzenie_daty(string str, int dzien, int miesiac)
{
	bool tak_czy_nie = false;
	string zmienna = str;
	int found = zmienna.find(".");
	string czlon[3];
	
	if(found != string::npos)
	{
		czlon[0] = zmienna.substr(0, found);
		zmienna = zmienna.substr(found+1);
		found = zmienna.find(".");

		if(found != string::npos)
		{
			czlon[1] = zmienna.substr(0, found);
			czlon[2] = zmienna.substr(found+1);
		}
		else
			czlon[1] = zmienna.substr(0, found);
	}

	if(miesiac == atoi(czlon[1].c_str()))
	{
		if(dzien <= atoi(czlon[0].c_str()) && atoi(czlon[0].c_str()) <= dzien+7)
			tak_czy_nie = true;
	}
	return tak_czy_nie;
}

//sortowanie listy osob - sortowanie babelkowe
void sortowanie(osoba os[], int ile){
	for(int k=1; k<ile; k++){
		for(int j=0; j<ile-1; j++){
			osoba buf;
			
			if(os[j].nazwisko.at(0) > os[j+1].nazwisko.at(0)){
				buf.imie=os[j].imie;
				buf.nazwisko=os[j].nazwisko;
				buf.urodziny=os[j].urodziny;
				buf.imieniny=os[j].imieniny;
				buf.komentarz=os[j].komentarz;
				buf.zdjecie=os[j].zdjecie;
				buf.niedlugo_imie=os[j].niedlugo_imie;
				buf.niedlugo_uro=os[j].niedlugo_uro;
				os[j].imie=os[j+1].imie;
				os[j].nazwisko=os[j+1].nazwisko;
				os[j].urodziny=os[j+1].urodziny;
				os[j].imieniny=os[j+1].imieniny;
				os[j].komentarz=os[j+1].komentarz;
				os[j].zdjecie=os[j+1].zdjecie;
				os[j].niedlugo_imie=os[j+1].niedlugo_imie;
				os[j].niedlugo_uro=os[j+1].niedlugo_uro;
				os[j+1].imie=buf.imie;
				os[j+1].imieniny=buf.imieniny;
				os[j+1].komentarz=buf.komentarz;
				os[j+1].nazwisko=buf.nazwisko;
				os[j+1].niedlugo_imie=buf.niedlugo_imie;
				os[j+1].niedlugo_uro=buf.niedlugo_uro;
				os[j+1].urodziny=buf.urodziny;
				os[j+1].zdjecie=buf.zdjecie;
			}
			else if(os[j].nazwisko.at(0) == os[j+1].nazwisko.at(0)){
				if(os[j].nazwisko.at(1) > os[j+1].nazwisko.at(1)){
					buf.imie=os[j].imie;
					buf.nazwisko=os[j].nazwisko;
					buf.urodziny=os[j].urodziny;
					buf.imieniny=os[j].imieniny;
					buf.komentarz=os[j].komentarz;
					buf.zdjecie=os[j].zdjecie;
					buf.niedlugo_imie=os[j].niedlugo_imie;
					buf.niedlugo_uro=os[j].niedlugo_uro;
					os[j].imie=os[j+1].imie;
					os[j].nazwisko=os[j+1].nazwisko;
					os[j].urodziny=os[j+1].urodziny;
					os[j].imieniny=os[j+1].imieniny;
					os[j].komentarz=os[j+1].komentarz;
					os[j].zdjecie=os[j+1].zdjecie;
					os[j].niedlugo_imie=os[j+1].niedlugo_imie;
					os[j].niedlugo_uro=os[j+1].niedlugo_uro;
					os[j+1].imie=buf.imie;
					os[j+1].imieniny=buf.imieniny;
					os[j+1].komentarz=buf.komentarz;
					os[j+1].nazwisko=buf.nazwisko;
					os[j+1].niedlugo_imie=buf.niedlugo_imie;
					os[j+1].niedlugo_uro=buf.niedlugo_uro;
					os[j+1].urodziny=buf.urodziny;
					os[j+1].zdjecie=buf.zdjecie;
				}
			}
		}
	}
}
// funkcja wydobywajaca indeksy w linii wyrazow urodziny i imieniny
void urodziny_i_imieniny(string wyrazy[], int &indeks_uro, int &indeks_imi){

	for(int i=0; i<max_wyrazow; i++)
	{
		if(!stricmp("urodziny", wyrazy[i].c_str()))
			indeks_uro = i;
		else if(!stricmp("imieniny", wyrazy[i].c_str()))
			indeks_imi = i;
	}
}
//porownanie wyczytanej daty z dat¹ komputera
void czy_uro_czy_imie(osoba os[], int k){
	SYSTEMTIME st;
	GetLocalTime(&st);
	
	int dzien = st.wDay;
	int miesiac = st.wMonth;
	int rok = st.wYear;
	
	os[k].niedlugo_uro = sprawdzenie_daty(os[k].urodziny, dzien, miesiac);
	os[k].niedlugo_imie = sprawdzenie_daty(os[k].imieniny, dzien, miesiac);
}
// funkcja tworzaca plik html
int stworz_html(osoba os[], int ile, const char* nazwa_pliku, const char* css){
	fstream plik;
	plik.open(nazwa_pliku, ios::out);	
	if(!plik) 
	{
	    cout << "Nie udalo sie utworzyc pliku html!"<<endl;
	    return 0;
	}
	else if(plik.good())
		cout<<"Plik html zostal utworzony poprawnie!"<<endl;
	plik<<"<!DOCTYPE HTML>"<<endl;
	plik<<"<html lang= \"pl\">"<<endl;
	plik<<"<head>"<<endl;
	plik<<"	<meta charset=\"utf-8\"/>"<<endl;
	plik<<"	<title>Lista moich najlepszych przyjació³!</title>"<<endl;
	plik<<"	<meta name=\"description\" content=\"Lista moich najlepszych przyjació³!\"/>"<<endl;
	plik<<"	<meta name=\"keywords\" content=\"Projekt, c++,html, przyjaciele, lista\"/>"<<endl;
	plik<<"	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"/>"<<endl;
	plik<<"	<link rel=\"stylesheet\" href="<<css<<" type=\"text/css\" />"<<endl;
	plik<<"</head>"<<endl;
	plik<<"<body>"<<endl;
	plik<<"	<div id=\"container\">"<<endl;
	plik<<"		<div id=\"logo\">"<<endl;
	plik<<"			<h1>Lista moich najlepszych przyjaciol!</h1>"<<endl;
	plik<<"		</div>"<<endl;
	plik<<"		<div id=\"menu\">"<<endl;
	plik<<" NAWIGACJA: <br/>"<<endl;
	for(int k=0; k<ile; k++){
		plik<<"<a href=\"#"<<k<<"\" class=\"tilelink\">"<<os[k].nazwisko.at(0)<<"</a><br/>"<<endl;
		for(int m=k+1; m<ile; m++){
			if(os[k].nazwisko.at(0)==os[m].nazwisko.at(0))
				k++;
		}
	}
	plik<<"		</div>"<<endl;
	plik<<"		<div id=\"content\">"<<endl;
	plik<<"			<table>"<<endl;
	plik<<"				<tr>"<<endl;
	plik<<"					<th> Zdjecie </th><th> Imie </th><th> Nazwisko </th> <th> Urodziny </th> <th> Imieniny </th> <th> Komentarz </th>"<<endl;
	plik<<"				</tr>"<<endl;
	for(int k=0; k<ile; k++){
		plik<<"<tr id=\""<<k<<"\">" "<th><img width=\"80\" height=\"80\" src="+os[k].zdjecie+" alt=\"logo\"/>""</th><th>"+os[k].imie+"</th><th>"+os[k].nazwisko+"</th>";
		plik<<"<th";
		if(os[k].niedlugo_uro) 
			plik<< " class=\"urodziny\">"+os[k].urodziny+"</th>";
		else
			plik<<">"+os[k].urodziny+"</th>";
		plik<<"<th";
		if(os[k].niedlugo_imie)
			plik<< " class=\"imieniny\">"+os[k].imieniny+"</th> <th>"+os[k].komentarz+"</th> </tr>";
		else
			plik<<">"+os[k].imieniny+"</th> <th>"+os[k].komentarz+"</th> </tr>";
	}
	plik<<"			</table>"<<endl;
	plik<<"		</div>"<<endl;
	plik<<"		<div id=\"footer\">"<<endl;
	plik<<"			Projekt2 POP Marcin Swiniarski 165631"<<endl;
	plik<<"		</div>"<<endl;
	plik<<"	</div>"<<endl;
	plik<<"</body>"<<endl;
	plik<<"</html>"<<endl;
	plik.close();
	plik.clear();
}
// funkcja sprawdzajaca czy w pliku wejsciowym nie ma bledow
int sprawdzanie_bledow(string linia){	
	transform(linia.begin(), linia.end(), linia.begin(), ::tolower);
	int found1;
	found1 = linia.find("urodziny");
	if(found1 == string::npos){
		cout<<"W pliku wejsciowym podano niekompletne dane w sekcji urodziny!"<<endl;			
		exit(0);
	}
	found1 = linia.find("imieniny");
	if(found1 == string::npos){
		cout<<"W pliku wejsciowym podano niekompletne dane w sekcji imieniny!"<<endl;			
		exit(0);
	}
	found1 = linia.find("<");
	if(found1 == string::npos){
		cout<<"W pliku wejsciowym podano niekompletne dane w sekcji komentarz!"<<endl;			
		exit(0);
	}
	found1 = linia.find(">");
	if(found1 == string::npos){
		cout<<"W pliku wejsciowym podano niekompletne dane w sekcji komentarz!"<<endl;			
		exit(0);
	}
	found1 = linia.find("[");
	if(found1 == string::npos){
		cout<<"W pliku wejsciowym podano niekompletne dane w sekcji zdjecie!"<<endl;			
		exit(0);
	}
	found1 = linia.find("]");
	if(found1 == string::npos){
		cout<<"W pliku wejsciowym podano niekompletne danew sekcji zdjecie!"<<endl;			
		exit(0);
	}
	found1 = linia.find("#");
	if(found1 == string::npos){
		cout<<"W pliku wejsciowym podano niekompletne dane! Brakuje znaku #!"<<endl;			
		exit(0);
	}
}
