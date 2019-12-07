#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <random>

class Card
{
public:
    std::string color;
    int value;
    std::string special;
    Card(std::string cardColor, int cardValue, std::string cardSpecial)
    {
        color = cardColor;
        value = cardValue;
        special = cardSpecial;
    }
};

void startGame(std::vector<Card> &drawPile)
{
    std::string colors[4] = {"RED", "YELLOW", "BLUE", "GREEN"};
    std::string specials[3] = {"BLOCK", "PLUSTWO", "REVERSE"};
    for (int i = 0; i < 4; i++)
    {
        Card card0(colors[i], 0, "REGULAR");
        drawPile.push_back(card0);
        for (int num = 1; num < 10; num++)
        {
            Card card1(colors[i], num, "REGULAR");
            drawPile.push_back(card1);
            drawPile.push_back(card1);
        }
        for (int esp = 0; esp < 3; esp++)
        {
            Card card2(colors[i], -1, specials[esp]);
            drawPile.push_back(card2);
            drawPile.push_back(card2);
        }
    }
    std::cout << "Game with " << drawPile.size() << " cards" << std::endl;
}

void shuffle(std::vector<Card> &drawPile)
{
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(drawPile), std::end(drawPile), rng);
}

int main(int argc, char const *argv[])
{
    // revisar archivo si hay un juego pendiente, si no, se crea uno:
    std::vector<Card> drawPile;
    startGame(drawPile);
    shuffle(drawPile);
    return 0;
}