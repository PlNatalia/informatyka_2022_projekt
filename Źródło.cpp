/*
Gra bedzie w stylu Space invaders, gracz bedzie sterowal obiektem lewo/prawo i strzelal w nadciagaj¹cych przeciwnikow.
Bedzie kilka poziomow trudnosci rozniacych sie predkoscia przeciwnikow. Za kazdego trafionego przeciwnika gracz otrzyma punkt/y.
Obiekt sterowany przez gracza bêdzie tracil jedno z trzech serc za kazdego pominietego przeciwnika.
Utracenie wszystkich skutkowac bedzie zakonczeniem gry. Uzyskane wyniki beda widoczne po wybraniu opcji z menu gry.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <random>
#include <fstream> // zapis do pliku
#include <algorithm> // do sortowania

class Przeciwnik
{
private:
	sf::Sprite kosmita;
	bool poziom = true;
	float dx = 0.2;

public:
	Przeciwnik(float polozenie_x, float polozenie_y, const sf::Texture& tekstura);
	void draw(sf::RenderWindow& window);
	sf::Sprite getSprite();
	void poziom_trudnosci(bool wartosc);
	void generuj();
};

void Przeciwnik::generuj()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(-5, 5);
}

void Przeciwnik::poziom_trudnosci(bool wartosc)
{
	poziom = wartosc;
}

Przeciwnik::Przeciwnik(float polozenie_x, float polozenie_y, const sf::Texture& tekstura)
{
	kosmita.setTexture(tekstura);
	kosmita.setPosition(sf::Vector2f(polozenie_x, polozenie_y));
}

sf::Sprite Przeciwnik::getSprite() // funkcja zwracajaca wskaznik na sprite
{
	return kosmita;
}

void Przeciwnik::draw(sf::RenderWindow& window)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(-1, 1);
	window.draw(kosmita);
	if (!poziom) // trudny
	{
		kosmita.move(sf::Vector2f(dx, 0.0f));
		if (kosmita.getPosition().x <= 0 || kosmita.getPosition().x + 50 >= 1000)
		{
			dx = -dx;
			kosmita.move(sf::Vector2f(0.0f, 50.0f));
		}
	}
	else // latwy
	{
		kosmita.move(sf::Vector2f(dist(gen), 0.015f));
		if (kosmita.getPosition().x <= 0)
		{
			kosmita.move(sf::Vector2f(50.0f, 0.0f));
		}
		if (kosmita.getPosition().x + 50 >= 1000)
		{
			kosmita.move(sf::Vector2f(-50.0f, 0.0f));
		}
	}
}

class Pocisk
{
private:
	static const int promien_pocisku = 2; // static powoduje ze dla kazdego obiektu ta zmienna ma te sama wartosc, a tylko dla const bylaby stala, ale kazdy obiekt moglby miec te wartosc inna
	sf::CircleShape pocisk;

public:
	Pocisk(float width, float height);
	void draw(sf::RenderWindow& window);
	void przesun();
	bool czy_kolizja(Przeciwnik& alien);
	bool kolizja_koniec_mapy(Przeciwnik& alien);
};

Pocisk::Pocisk(float wsp_x, float wsp_y) : pocisk(promien_pocisku) // jedyne miejsce na inicjalizacje pola klasy z konstruktorem
{
	pocisk.setPosition(sf::Vector2f(wsp_x, wsp_y));
}

void Pocisk::przesun()
{
	pocisk.move(sf::Vector2f(0.0f, -0.2f));
}

bool Pocisk::czy_kolizja(Przeciwnik& alien)
{
	float x_alien = alien.getSprite().getPosition().x;
	float y_alien = alien.getSprite().getPosition().y; //alien.getSprite to wskaznik na sprite w alienie -- dereferencja
	float alien_dlugosc = alien.getSprite().getTexture()->getSize().x; //getTexture to wskaznik
	float alien_szerokosc = alien.getSprite().getTexture()->getSize().y;
	float promien_aliena = std::sqrt(alien_dlugosc * alien_dlugosc + alien_szerokosc * alien_szerokosc) / 2;
	float x_pocisku = pocisk.getPosition().x;
	float y_pocisku = pocisk.getPosition().y;
	float odleglosc_miedzy_srodkami_okregow = std::sqrt(std::pow(x_alien - x_pocisku + 25, 2) + std::pow(y_alien - y_pocisku, 2));

	if (odleglosc_miedzy_srodkami_okregow <= promien_aliena + promien_pocisku)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool kolizja_koniec_mapy(Przeciwnik& alien)
{
	if (alien.getSprite().getPosition().y >= 600)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Pocisk::draw(sf::RenderWindow& window)
{
	window.draw(pocisk);
	przesun();
}

class Gracz
{
private:
	const int wsp_x = 450; // wsp. poczatkowe
	const int wsp_y = 500; // wsp. poczatkowe
	sf::Sprite gracz;

public:
	Gracz(float width, float height, const sf::Texture& tekstura);
	void animujlewo();
	void animujprawo();
	void draw(sf::RenderWindow& window);
	sf::Sprite get_gracz();
};

sf::Sprite Gracz::get_gracz()
{
	return gracz;
}

Gracz::Gracz(float width, float height, const sf::Texture& tekstura)
{
	gracz.setTexture(tekstura);
	gracz.setPosition(sf::Vector2f(wsp_x, wsp_y)); // poczatkowa pozycja
}

void Gracz::animujlewo()
{
	if (gracz.getPosition().x > 50)
	{
		gracz.move(sf::Vector2f(-0.2f, 0.0f));
	}
}

void Gracz::animujprawo()
{
	if (gracz.getPosition().x < 900)
	{
		gracz.move(sf::Vector2f(0.2f, 0.0f));
	}
}

void Gracz::draw(sf::RenderWindow& window)
{
	sf::RectangleShape tlo(sf::Vector2f(1000, 600));
	tlo.setPosition(0, 0);
	tlo.setFillColor(sf::Color(38, 33, 55));
	window.draw(tlo);
	window.draw(gracz);
}

class Menu
{
private:
	sf::Font font;
	sf::Text menu[14];
	int wybrana_opcja = 0;
	bool esc_flaga = false;
	bool f1_flaga = false;
	int opcje_enter_flaga = 0;
	bool czy_strzelac = false;
	int punkty = 0;
	int serca = 3;
	sf::Sprite serce[3];
	bool czy_koniec = false; // czy wszyscy przeciwnicy znikli
	sf::Text imie;
	bool start = true;
	int licznik = 0;
	bool wybor_poziomu = true;
	bool poczatek_gry = false;
	bool nowa_gra = false;
	bool pokaz_wyniki = false;

public:
	Menu(float width, float height, const sf::Texture& tekstura);
	void draw(sf::RenderWindow& window); // powiadamia obiekt Gracz o koniecznosci jego rysowania
	void Gora(); // gdy gracz wcisnie strzalke w gore w menu 
	void Dol(); // gdy gracz wcisnie strzalke w dol w menu
	void Esc();
	void Pomoc(); // gry gracz wcisnie F1
	void wybor_opcji();
	int strzelanie();
	void get_punkty(int punkty);
	void get_zycia(int zycia);
	void koniec(bool wartosc);
	void nick(sf::Text nick);
	void start_programu();
	int F1();
	void poziom(bool wartosc2);
	int Escc();
	sf::Text k_imie();
	bool nowa_gierka();
	void nie_rysuj(bool wartosc3);
	int zwroc_punkty();
	bool czy_pokazac_wyniki();
};

bool Menu::czy_pokazac_wyniki()
{
	return pokaz_wyniki;
}

int Menu::zwroc_punkty()
{
	return punkty;
}

void Menu::nie_rysuj(bool wartosc3)
{
	nowa_gra = wartosc3;
}

bool Menu::nowa_gierka()
{
	return nowa_gra;
}

sf::Text Menu::k_imie()
{
	return imie;
}

void Menu::poziom(bool wartosc2)
{
	wybor_poziomu = wartosc2;
}

int Menu::F1()
{
	return f1_flaga;
}

int Menu::Escc()
{
	return esc_flaga;
}

void Menu::start_programu()
{
	if (licznik == 0)
	{
		start = !start;
		opcje_enter_flaga = 0;
		esc_flaga = true;
		licznik++;
	}
}

void Menu::nick(sf::Text nick)
{
	if (start)
	{
		imie = nick;
		imie.setFont(font);
		imie.setCharacterSize(40);
		imie.setFillColor(sf::Color::White);
		imie.setPosition(400, 380);
	}
}

void Menu::koniec(bool wartosc)
{
	czy_koniec = wartosc;
}

void Menu::get_zycia(int zycia)
{
	serca = zycia;
	std::cout << "\nSerca: " << serca << std::endl;
}

void Menu::get_punkty(int points)
{
	if (wybor_poziomu)
	{
		punkty = points; // punkty dla latwego poziomu
	}
	else
	{
		punkty = 2 * points; // punkty dla trudnego poziomu
	}
}

int Menu::strzelanie()
{
	return czy_strzelac;
}

Menu::Menu(float width, float height, const sf::Texture& tekstura)
{
	if (!font.loadFromFile("arial.ttf"))
		return;

	menu[0].setFont(font);
	menu[0].setCharacterSize(40);
	menu[0].setFillColor(sf::Color::Blue);
	menu[0].setString("Graj");
	menu[0].setPosition(sf::Vector2f((width / 3), (height / 2) - 120));

	menu[1].setFont(font);
	menu[1].setCharacterSize(30);
	menu[1].setFillColor(sf::Color(0, 0, 128));
	menu[1].setString("Tablica wynikow");
	menu[1].setPosition(sf::Vector2f((width / 3), (height / 2) - 20));

	menu[2].setFont(font);
	menu[2].setCharacterSize(30);
	menu[2].setFillColor(sf::Color(0, 0, 128));
	menu[2].setString("Wyjscie");
	menu[2].setPosition(sf::Vector2f((width / 3), (height / 2) + 80));

	menu[3].setFont(font);
	menu[3].setCharacterSize(40);
	menu[3].setFillColor(sf::Color(0, 0, 128));
	menu[3].setString("Pomoc\n\n");
	menu[3].setPosition(sf::Vector2f(25, 100));

	menu[4].setFont(font);
	menu[4].setCharacterSize(30);
	menu[4].setFillColor(sf::Color(0, 0, 128));
	menu[4].setString("Aby poruszac statkiem, uzywaj klawiszy A oraz D, do strzelania \nwykorzystuj spacje.Twoim zadaniem jest usuniecie zagrozenia, \njakie stanowia zblizajacy sie kosmici.\nOszczedzaj zycia i zbieraj punkty, za kazdego \nunieszkodliwionego kosmite dostajesz 1 punkt (na trudnym poziomie 2).\nTwoj wynik zostanie zapisany w tablicy wynikow.\n\nPowodzenia! :)");
	menu[4].setPosition(sf::Vector2f(25, 200));

	menu[5].setFont(font);
	menu[5].setCharacterSize(40);
	menu[5].setFillColor(sf::Color::Red);
	menu[5].setString("Czy na pewno chcesz wyjsc?");
	menu[5].setPosition(sf::Vector2f(250, 25));

	menu[6].setFont(font);
	menu[6].setCharacterSize(20);
	menu[6].setFillColor(sf::Color::White);
	//menu[6].setString("Liczba punktow: " + std::to_string(punkty)); // zamiana zmiennej na tekst, tu sie nie aktualizuje co kratke
	menu[6].setPosition(sf::Vector2f(900, 25));

	menu[7].setFont(font);
	menu[7].setCharacterSize(80);
	menu[7].setFillColor(sf::Color::Red);
	menu[7].setString("Przegrales! :(");
	menu[7].setPosition(sf::Vector2f(250, 220));

	menu[8].setFont(font);
	menu[8].setCharacterSize(80);
	menu[8].setFillColor(sf::Color::Red);
	menu[8].setString("Wygrales! :)");
	menu[8].setPosition(sf::Vector2f(280, 220));

	menu[9].setFont(font);
	menu[9].setCharacterSize(90);
	menu[9].setFillColor(sf::Color::White);
	menu[9].setString("Space Invaders");
	menu[9].setPosition(sf::Vector2f(190, 120));

	menu[10].setFont(font);
	menu[10].setCharacterSize(30);
	menu[10].setFillColor(sf::Color::White);
	menu[10].setString("Wprowadz swoj nick:");
	menu[10].setPosition(sf::Vector2f(330, 280));

	menu[11].setFont(font);
	menu[11].setCharacterSize(20);
	menu[11].setFillColor(sf::Color::White);
	menu[11].setString("Poziom trudnosci:\n1 - latwy\n2 - trudny");
	menu[11].setPosition(sf::Vector2f(820, 220));

	menu[12].setFont(font);
	menu[12].setCharacterSize(30);
	menu[12].setFillColor(sf::Color::White);
	menu[12].setString("latwy");
	menu[12].setPosition(sf::Vector2f(820, 320));

	menu[13].setFont(font);
	menu[13].setCharacterSize(30);
	menu[13].setFillColor(sf::Color::White);
	menu[13].setString("trudny");
	menu[13].setPosition(sf::Vector2f(820, 320));

	for (int j = 0; j < 3; j++)
	{
		serce[j].setTexture(tekstura);
		serce[j].setPosition(25 + 40 * j, 25);
	}
}

void Menu::draw(sf::RenderWindow& window)
{
	if (start)
	{
		window.draw(menu[9]);
		window.draw(menu[10]);
		window.draw(imie);
	}

	if (pokaz_wyniki)
	{

	}

	if (czy_strzelac)
	{
		menu[6].setString("Wynik: " + std::to_string(punkty)); // zamiana zmiennej na tekst, w tym miejscu zeby sie co kratke aktualizowalo
		window.draw(menu[6]);
		if (serca >= 3)
		{
			window.draw(serce[0]);
			window.draw(serce[1]);
			window.draw(serce[2]);
			if (czy_koniec)
			{
				menu[6].setPosition(450, 360);
				window.draw(menu[6]);
				window.draw(menu[8]);
			}
			else
			{
				menu[6].setPosition(900, 25);
			}
		}
		else if (serca == 2)
		{
			window.draw(serce[0]);
			window.draw(serce[1]);
			if (czy_koniec)
			{
				menu[6].setPosition(450, 360);
				window.draw(menu[6]);
				window.draw(menu[8]);
			}
		}
		else if (serca == 1)
		{
			window.draw(serce[0]);
			if (czy_koniec)
			{
				menu[6].setPosition(450, 360);
				window.draw(menu[6]);
				window.draw(menu[8]);
			}
		}
		else if (serca <= 0 && czy_koniec)
		{
			menu[6].setPosition(450, 360);
			window.draw(menu[6]);
			window.draw(menu[7]);
		}
	}
	if (esc_flaga)
	{
		sf::RectangleShape ramka(sf::Vector2f(600, 400)); // wymiary
		ramka.setPosition(200, 100);
		ramka.setFillColor(sf::Color(30, 144, 255));
		window.draw(ramka);

		sf::RectangleShape wnetrze(sf::Vector2f(500, 300));
		wnetrze.setPosition(250, 150);
		wnetrze.setFillColor(sf::Color(240, 248, 255));
		window.draw(wnetrze);

		for (int i = 0; i < 3; i++) // 3 to liczba opcji po kliknieciu Esc
		{
			window.draw(menu[i]);
		}
		window.draw(menu[11]);
		if (wybor_poziomu)
		{
			window.draw(menu[12]);
		}
		else
		{
			window.draw(menu[13]);
		}

		if (opcje_enter_flaga)
		{
			czy_strzelac = false;
			pokaz_wyniki = false;
			if (wybrana_opcja == 0)
			{
				if (opcje_enter_flaga == 1) // graj
				{
					czy_strzelac = true;
					esc_flaga = false;
					nowa_gra = !nowa_gra; // do restartu
					opcje_enter_flaga = 0;
				}
			}
			else if (wybrana_opcja == 1) // tablica wynikow
			{
				if (opcje_enter_flaga == 1)
				{
					pokaz_wyniki = true;
					opcje_enter_flaga = 0;														
				}
			}
			else if (wybrana_opcja == 2) // wyjscie
			{
				window.draw(menu[5]);
				if (opcje_enter_flaga == 2)
				{
					window.close();
				}
			}
		}
	}

	if (f1_flaga)
	{
		sf::RectangleShape ramka2(sf::Vector2f(1000, 600)); // wymiary
		ramka2.setPosition(0, 0);
		ramka2.setFillColor(sf::Color(30, 144, 255));
		window.draw(ramka2);

		sf::RectangleShape wnetrze2(sf::Vector2f(1000, 500));
		wnetrze2.setPosition(0, 50);
		wnetrze2.setFillColor(sf::Color(240, 248, 255));
		window.draw(wnetrze2);

		window.draw(menu[3]);
		window.draw(menu[4]);
	}
}

void Menu::Gora()
{
	opcje_enter_flaga = 0; //zerowanie liczby klikniec enter po kazdym przejsciu do innej opcji
	if (wybrana_opcja - 1 >= 0)
	{
		menu[wybrana_opcja].setFillColor(sf::Color(0, 0, 128));
		menu[wybrana_opcja].setCharacterSize(30);
		wybrana_opcja--;
		menu[wybrana_opcja].setFillColor(sf::Color::Blue);
		menu[wybrana_opcja].setCharacterSize(40);
	}
}

void Menu::Dol()
{
	opcje_enter_flaga = 0; //zerowanie liczby klikniec enter po kazdym przejsciu do innej opcji
	if (wybrana_opcja + 1 < 3) // 3 to liczba opcji po kliknieciu Esc
	{
		menu[wybrana_opcja].setFillColor(sf::Color(0, 0, 128));
		menu[wybrana_opcja].setCharacterSize(30);
		wybrana_opcja++;
		menu[wybrana_opcja].setFillColor(sf::Color::Blue);
		menu[wybrana_opcja].setCharacterSize(40);
	}
}

void Menu::Esc()
{
	esc_flaga = !esc_flaga;
	opcje_enter_flaga = 0;
}

void Menu::Pomoc()
{
	f1_flaga = !f1_flaga;
}

void Menu::wybor_opcji()
{
	opcje_enter_flaga++; //zliczanie liczby klikniec enter
}

bool obsluz_kolizje(std::vector <Przeciwnik>& kosmici, std::vector <Pocisk>& pociski, int* kosmita_do_usuniecia, int* pocisk_do_usuniecia) // funkcja zwraca wartosc, czy kolizja zostala wykryta
{
	for (int pocisk_i = 0; pocisk_i < pociski.size(); pocisk_i++)
	{
		for (int kosmita_i = 0; kosmita_i < kosmici.size(); kosmita_i++)
		{
			if (pociski[pocisk_i].czy_kolizja(kosmici[kosmita_i]))
			{
				*kosmita_do_usuniecia = kosmita_i;
				*pocisk_do_usuniecia = pocisk_i;
				return true;
			}
		}
	}
	return false;
}

bool obsluz_kolizje_z_koncem_mapy(std::vector <Przeciwnik>& kosmici, int* kosmita_do_usuniecia_km)
{
	for (int kosmita_i = 0; kosmita_i < kosmici.size(); kosmita_i++)
	{
		if (kolizja_koniec_mapy(kosmici[kosmita_i]))
		{
			*kosmita_do_usuniecia_km = kosmita_i;
			return true;
		}
	}
	return false;
}

struct tablica_wynikow
{
	std::string nickame;
	int punktacja = 0;
	bool operator < (const tablica_wynikow& x)const
	{                
		return punktacja > x.punktacja;
	}
} wyniki[10];

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 600), "Space Invaders v2");

	sf::Texture tekstura_kosmity1;
	if (!tekstura_kosmity1.loadFromFile("alien1.jpg")) {
		return -1;
	}

	sf::Texture tekstura_statku;
	if (!tekstura_statku.loadFromFile("statek.jpg")) {
		return -1;
	}

	sf::Texture tekstura_serca;
	if (!tekstura_serca.loadFromFile("serce.jpg")) {
		return -1;
	}

	Menu menu(window.getSize().x, window.getSize().y, tekstura_serca);
	Gracz gracz(window.getSize().x, window.getSize().y, tekstura_statku);
	std::vector < Pocisk > pociski;
	std::vector < Przeciwnik > kosmici; // klasa/'tablica' vector 'przygotowuje' klase przeciwnik do przechowywania obiektow
	tablica_wynikow wyniki[20];

	const int liczba_przeciwnikow = 10;
	int punkty = 0;
	int highscore = 0;
	bool raz = true;
	int zycia = 3;
	bool poziom = true;
	bool zapisz = false;
	std::string znak;
	sf::Text nick;
	std::string imie;
	int licznik = 0;

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::TextEntered:
				if (event.text.unicode < 128)
				{
					znak += static_cast<char>(event.text.unicode);
					imie += znak;
					
					nick.setString(znak);
					menu.nick(nick);
				}
				break;
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					menu.Esc();
					break;
				case sf::Keyboard::Enter:
					menu.wybor_opcji();
					menu.start_programu();
					licznik = 0;
					break;
				case sf::Keyboard::Up:
					menu.Gora();
					break;
				case sf::Keyboard::Down:
					menu.Dol();
					break;
				case sf::Keyboard::F1: //pomoc
					menu.Pomoc();
					break;
				case sf::Keyboard::Space: //strzal
					pociski.push_back(Pocisk(gracz.get_gracz().getPosition().x + 25, 500));
					break;
				case sf::Keyboard::Num1: // 1 na klawiaturze (nie na numpadzie)					
					poziom = true;
					break;
				case sf::Keyboard::Num2:					
					poziom = false;
					break;
				case sf::Keyboard::P:
					zapisz = true;			
					break;
				}
				break;
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
	
		menu.poziom(poziom); // do wyswietlania menu
		for (Przeciwnik& przeciwnik : kosmici)
		{
			przeciwnik.poziom_trudnosci(poziom); // do ruchu kosmitow
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			gracz.animujlewo();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			gracz.animujprawo();
		}

		window.clear();

		sf::RectangleShape tlo(sf::Vector2f(1000, 600));
		tlo.setPosition(0, 0);
		tlo.setFillColor(sf::Color(38, 33, 55));
		window.draw(tlo);

		if (menu.strzelanie())
		{
			if (menu.nowa_gierka())
			{
				raz = true;
				menu.koniec(false);
				kosmici.clear();
				punkty = 0;
				menu.get_punkty(punkty);
				zycia = 3;
				menu.get_zycia(zycia);
				for (int i = 0; i < liczba_przeciwnikow; i++)
				{
					float wsp_x = (window.getSize().x / liczba_przeciwnikow) * i + 40;
					float wsp_y = 50;

					kosmici.push_back(Przeciwnik(wsp_x, wsp_y, tekstura_kosmity1));
				}
				menu.nie_rysuj(false);
			}
			gracz.draw(window);
			for (Pocisk& pocisk : pociski)
			{
				pocisk.draw(window); // petla dla wszystkich stworzonych pociskow, pokazuje wszystkie obiekty
			}
			for (Przeciwnik& przeciwnik : kosmici)
			{
				if (!menu.F1() && !menu.Escc()) // rysuj tylko jest pomoc nie jest wlaczona
				{
					przeciwnik.draw(window);
				}
			}

			// wykrywanie i obsluga kolizji
			int kosmita_do_usuniecia;
			int pocisk_do_usuniecia;
			while (obsluz_kolizje(kosmici, pociski, &kosmita_do_usuniecia, &pocisk_do_usuniecia)) //funkcja ze wskaznikami
			{
				//kolizja wykryta
				kosmici.erase(kosmici.begin() + kosmita_do_usuniecia); // erase to funkcja usuwajaca elementy z wektora, begin to funkcja zwracajaca iterator (wskaznik) na poczatek wektora, dodajemy np. kosmite do usuniecia, aby przesunac iterator(wskaznik) na odpowiednie miejsce
				pociski.erase(pociski.begin() + pocisk_do_usuniecia);
				punkty++;
				
				menu.get_punkty(punkty);
				std::cout << "Liczba kosmitow" << " :" << kosmici.size() << std::endl;
				std::cout << "Liczba pociskow" << " :" << pociski.size() << std::endl;
			}

			int kosmita_do_usuniecia_km;
			int pocisk_do_usuniecia_km;

			while (obsluz_kolizje_z_koncem_mapy(kosmici, &kosmita_do_usuniecia_km))
			{
				kosmici.erase(kosmici.begin() + kosmita_do_usuniecia_km);
				zycia--;
				menu.get_zycia(zycia);
				std::cout << "Liczba kosmitow" << " :" << kosmici.size() << std::endl;
				std::cout << "Liczba pociskow" << " :" << pociski.size() << std::endl;
			}

			if (kosmici.size() == 0) // do wyswietlania wygrales / przegrales
			{
				menu.koniec(true);

				if (raz)
				{
					if (menu.zwroc_punkty() > highscore)
					{
						highscore = menu.zwroc_punkty();
					}					

					raz = false;
				}
			}
		} // gdy nie jest wlaczona gra

		if (zapisz)
		{
			std::cout << "Zapisano wynik!" << std::endl;
			std::fstream plik;
			plik.open("dane.txt", std::ios::app); // std::ios::app -> ustawia wewnêtrzny wskaŸnik zapisu pliku na jego koniec, mozna tylko dopisywac
			plik << highscore << std::endl;
			plik << menu.k_imie().getString().toAnsiString() << std::endl;
			plik.close();
				
			// sortowanie i przypisywanie do tablicy struktur
			plik.open("dane.txt", std::ios::in);
			if (!plik.good())
			{
				std::cout << "Plik nie istnieje! " << std::endl;
				return 0;
			}

			std::string linia;
			int nr_linii = 1;
			int nr_iteracji = 0;
			while (getline(plik, linia))
			{
				switch (nr_linii)
				{
				case 1:
					wyniki[nr_iteracji].punktacja = atoi(linia.c_str());
					break;
				case 2:
					wyniki[nr_iteracji].nickame = linia;
					break;
				}
				if (nr_linii == 2) 
				{ 
					nr_linii = 0; 
					nr_iteracji++; 
				}
				nr_linii++;
			}
			plik.close();

			std::sort(wyniki, wyniki + 20); // sortowanie wg punktow
			
			zapisz = false;
		}

		if (menu.czy_pokazac_wyniki() && licznik == 0)
		{
			for (int x = 0; x < 10; x++)
				{
					std::cout << x + 1;
					std::cout << ". Nick: " << wyniki[x].nickame << std::endl;
					std::cout << "   Wynik: " << wyniki[x].punktacja << std::endl;
				}
				licznik++;		
		}

		menu.draw(window);
		window.display();
	}

	return 0;
}