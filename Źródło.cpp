/*
Gra bedzie w stylu Space invaders, gracz bedzie sterowal obiektem lewo/prawo i strzelal w nadciagaj¹cych przeciwnikow.
Bedzie kilka poziomow trudnosci, zwiekszajacych sie przejsciu kazdego z nich. Za kazdego trafionego przeciwnika gracz otrzyma punkty.
Obiekt sterowany przez gracza bêdzie otrzymywal ujemne punkty za kazdego pominietego przeciwnika i tracil jedno z kilku serc.
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

class Przeciwnik
{
private:
	sf::Sprite kosmita;

public:
	Przeciwnik(float polozenie_x, float polozenie_y, const sf::Texture& tekstura);
	void idz();
	void draw(sf::RenderWindow& window);
	sf::Sprite getSprite();
};

Przeciwnik::Przeciwnik(float polozenie_x, float polozenie_y, const sf::Texture& tekstura)
{
	kosmita.setTexture(tekstura);
	kosmita.setPosition(sf::Vector2f(polozenie_x, polozenie_y));

}

sf::Sprite Przeciwnik::getSprite() // funkcja zwracajaca wskaznik na sprite
{
	return kosmita;
}

void Przeciwnik::idz()
{
	kosmita.move(sf::Vector2f(0.0f, 0.01f));
}

void Przeciwnik::draw(sf::RenderWindow& window)
{
	window.draw(kosmita);
	kosmita.move(sf::Vector2f(0.0f, 0.01f));

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
	float odleglosc_miedzy_srodkami_okregow = std::sqrt(std::pow(x_alien - x_pocisku, 2) + std::pow(y_alien - y_pocisku, 2));

	if (odleglosc_miedzy_srodkami_okregow <= promien_aliena + promien_pocisku)
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
	sf::Text menu[6];
	int wybrana_opcja = 0;
	bool esc_flaga = false;
	bool f1_flaga = false;
	int opcje_enter_flaga = 0;
	bool czy_strzelac = false;

public:
	Menu(float width, float height);
	void draw(sf::RenderWindow& window); // powiadamia obiekt Gracz o koniecznosci jego rysowania
	void Gora(); // gdy gracz wcisnie strzalke w gore w menu 
	void Dol(); // gdy gracz wcisnie strzalke w dol w menu
	void Esc();
	void Pomoc(); // gry gracz wcisnie F1
	void wybor_opcji();
	int strzelanie();
};

int Menu::strzelanie()
{
	return czy_strzelac;
}

Menu::Menu(float width, float height)
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
	menu[4].setString("Aby poruszac statkiem, uzywaj klawiszy A oraz D, do strzelania \nwykorzystuj spacji.Twoim zadaniem jest usuniecie zagrozenia, \njakie stanowia zblizajacy sie kosmici.\nOszczedzaj zycia i zbieraj punkty, za kazdego \nunieszkodliwionego kosmite dostajesz 1 punkt.\nTwoj najlepszy wynik zostanie zapisany w tablicy wynikow.\n\nPowodzenia! :)");
	menu[4].setPosition(sf::Vector2f(25, 200));

	menu[5].setFont(font);
	menu[5].setCharacterSize(40);
	menu[5].setFillColor(sf::Color::Red);
	menu[5].setString("Czy na pewno chcesz wyjsc?");
	menu[5].setPosition(sf::Vector2f(250, 25));
}

void Menu::draw(sf::RenderWindow& window)
{
	if (esc_flaga && !f1_flaga)
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

		if (opcje_enter_flaga)
		{
			czy_strzelac = false;
			if (wybrana_opcja == 0)
			{
				if (opcje_enter_flaga == 1) // graj
				{
					czy_strzelac = true;
				}
			}
			else if (wybrana_opcja == 1) // tablica wynikow
			{
				if (opcje_enter_flaga == 1)
				{
					//
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

	if (!esc_flaga && f1_flaga)
	{
		sf::RectangleShape ramka2(sf::Vector2f(1000, 600)); // wymiary
		ramka2.setPosition(0, 0);
		ramka2.setFillColor(sf::Color(30, 144, 255));
		window.draw(ramka2);

		sf::RectangleShape wnetrze2(sf::Vector2f(900, 500));
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
}

void Menu::Pomoc()
{
	f1_flaga = !f1_flaga;
}

void Menu::wybor_opcji()
{
	opcje_enter_flaga++; //zliczanie liczby klikniec enter
}

bool obsluz_kolizje(std::vector <Przeciwnik>& kosmici, std::vector <Pocisk>& pociski,int *kosmita_do_usuniecia, int *pocisk_do_usuniecia) // funkcja zwraca wartosc, czy kolizja zostala wykryta
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


	Menu menu(window.getSize().x, window.getSize().y);
	Gracz gracz(window.getSize().x, window.getSize().y, tekstura_statku);
	std::vector < Pocisk > pociski;
	std::vector < Przeciwnik > kosmici; // klasa/'tablica' vector 'przygotowuje' klase przeciwnik do przechowywania obiektow

	const int liczba_przeciwnikow = 5;

	for (int i = 0; i < liczba_przeciwnikow; i++)
	{
		float wsp_x = (window.getSize().x / 5) * i + 40;
		float wsp_y = 50;

		kosmici.push_back(Przeciwnik(wsp_x, wsp_y, tekstura_kosmity1));
	}

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					menu.Esc();
					break;
				case sf::Keyboard::Enter:
					menu.wybor_opcji();
					break;
				case sf::Keyboard::Up:
					menu.Gora();
					break;
				case sf::Keyboard::Down:
					menu.Dol();
					break;
				case sf::Keyboard::F1:
					menu.Pomoc();
					break;
				case sf::Keyboard::Space: //strzal
					pociski.push_back(Pocisk(gracz.get_gracz().getPosition().x + 25, 500));
					break;
				}
				break;
			case sf::Event::Closed:
				window.close();
				break;
			}
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

		if (menu.strzelanie())
		{
			gracz.draw(window);
			for (Pocisk& pocisk : pociski)
			{
				pocisk.draw(window); // petla dla wszystkich stworzonych pociskow, pokazuje wszystkie obiekty
			}
			for (Przeciwnik& przeciwnik : kosmici)
			{
				przeciwnik.draw(window);
			}

			// wykrywanie i obsluga kolizji
			int kosmita_do_usuniecia;
			int pocisk_do_usuniecia;
			while (obsluz_kolizje(kosmici, pociski, &kosmita_do_usuniecia, &pocisk_do_usuniecia)) //funkcja ze wskaznikami
			{
				//kolizja wykryta
				kosmici.erase(kosmici.begin() + kosmita_do_usuniecia); // erase to funkcja usuwajaca elementy z wektora, begin to funkcja zwracajaca iterator (wskaznik) na poczatek wektora, dodajemy np. kosmite do usuniecia, aby przesunac iterator(wskaznik) na odpowiednie miejsce
				pociski.erase(pociski.begin() + pocisk_do_usuniecia);

				std::cout << "Liczba kosmitow" << " :" << kosmici.size() << std::endl;
				std::cout << "Liczba pociskow" << " :" << pociski.size() << std::endl;
			}
			
		}



		menu.draw(window);
		window.display();
	}

	return 0;
}

