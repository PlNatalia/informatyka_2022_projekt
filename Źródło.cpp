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

class gracz
{
	char imie[20];
	int punkty;
};

int main()
{
	sf::Window window(sf::VideoMode(1000, 1000), "Gierka");

	while (window.isOpen())
	{

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		window.display();
	}

	return 0;
}

