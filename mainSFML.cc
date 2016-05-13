#include <SFML/Main.hpp>
#include <SFML/Graphics/Image.hpp>
//#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
        sf::RenderWindow window(sf::VideoMode(800, 600), "ass");


        sf::Event ev;
        while(window.pollEvent(ev)) {
                if(ev.type == sf::Event::Closed)
                        window.close();

                window.clear();
        }


        return 0;
}
