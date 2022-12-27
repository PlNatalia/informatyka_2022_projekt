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

#define max_liczba_opcji 3

class Menu
{
private:
	int wybrana_opcja;
	sf::Font font;
	sf::Text text[max_liczba_opcji];
	sf::Text menu[max_liczba_opcji];

public:
	Menu(float width, float height);
	~Menu();
	void draw(sf::RenderWindow &window);
	void Prawo(); //gdy gracz wcisnie strzalke w prawo w menu 
	void Lewo(); //gdy gracz wcisnie strzalke w lewo w menu
};

Menu::Menu(float width, float height)
{
	if (!font.loadFromFile("arial.ttf"))
		return;

	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setString("Graj");
	menu[0].setPosition(sf::Vector2f((width / 3) * 0 + 100, 25));

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setString("Tablica wynikow");
	menu[1].setPosition(sf::Vector2f((width / 3) * 1 + 100, 25));

	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setString("Wyjscie");
	menu[2].setPosition(sf::Vector2f((width / 3) * 2 + 100, 25));

	wybrana_opcja = 0;
}

Menu::~Menu()
{

}

void Menu::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < max_liczba_opcji; i++)
	{
		window.draw(menu[i]);
	}
}

void Menu::Prawo()
{
	if (wybrana_opcja - 1 >= 0)
	{
		menu[wybrana_opcja].setFillColor(sf::Color::White);
		wybrana_opcja--;
		menu[wybrana_opcja].setFillColor(sf::Color::Red);
	}
}

void Menu::Lewo()
{
	if (wybrana_opcja + 1 < max_liczba_opcji)
	{
		menu[wybrana_opcja].setFillColor(sf::Color::White);
		wybrana_opcja++;
		menu[wybrana_opcja].setFillColor(sf::Color::Red);
	}
}

class gracz
{
	char imie[20];
	int punkty;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 650), "Space Invaders v2");
	
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
				case sf::Keyboard::Left:
					menu.Prawo();
					break;
				
				case sf::Keyboard::Right:
					menu.Lewo();
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

