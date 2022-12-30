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


#define max_liczba_opcji 6

class Menu
{
private:
	int wybrana_opcja = 0;
	sf::Font font;
	sf::Text menu[max_liczba_opcji];

public:
	bool esc_flaga = false;
	bool f1_flaga = false;
	int opcje_enter_flaga = 0;
	Menu(float width, float height);
	~Menu();
	void draw(sf::RenderWindow& window);
	void Gora(); // gdy gracz wcisnie strzalke w gore w menu 
	void Dol(); // gdy gracz wcisnie strzalke w dol w menu
	void Esc();
	void Pomoc(); // gry gracz wcisnie F1
	void wybor_opcji();
};

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

Menu::~Menu()
{

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
	}

	if (opcje_enter_flaga)
	{
		if (wybrana_opcja == 0)
		{
			if (opcje_enter_flaga == 1)
			{
				//
			}
		}
		else if (wybrana_opcja == 1)
		{
			if (opcje_enter_flaga == 1)
			{
				//
			}
		}
		else if (wybrana_opcja == 2)
		{
			window.draw(menu[5]);
			if (opcje_enter_flaga == 2)
			{
				window.close();
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

class gracz
{
	char imie[20];
	int punkty;
	int wsp_x;
	int wsp_y;
	int predkosc; 
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 600), "Space Invaders v2");

	Menu menu(window.getSize().x, window.getSize().y);
	//sf::Texture tlo;
	//tlo.loadFromFile("tlo.jfif");
	//sf::Sprite tloo(tlo);

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
				case sf::Keyboard::A:
					//
					break;
				case sf::Keyboard::D:
					//
					break;
				case sf::Keyboard::Space:
					//
					break;
				}
				
				break;
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		window.clear();
		menu.draw(window);
		//	window.draw(tloo);
		window.display();
	}

	return 0;
}

