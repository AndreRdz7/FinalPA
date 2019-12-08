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

void displayHand(std::vector<Card> &hand)
{
    int size = hand.size();
    for (int i = 0; i < size; i++)
    {
        std::cout << "[";
        std::string currentCard;
        if (hand.at(i).special == "REGULAR")
        {
            currentCard = hand.at(i).color + std::to_string(hand.at(i).value);
        }
        else
        {
            currentCard = hand.at(i).color + hand.at(i).special;
        }
        std::cout << currentCard;
        std::cout << "]";
    }
    std::cout << std::endl;
}

void opponentsHand(std::vector<Card> &hand, int numberOfPlayer)
{
    int size = hand.size();
    std::string handDisplay;
    handDisplay += "P" + std::to_string(numberOfPlayer) + "->" + std::to_string(size);
    std::cout << handDisplay << std::endl;
}

void shuffle(std::vector<Card> &drawPile)
{
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(drawPile), std::end(drawPile), rng);
}

void sortHand(std::vector<Card> hand)
{
}

int getScore(std::vector<Card> firstHand, std::vector<Card> secondHand)
{
    return 0;
}

int main(int argc, char const *argv[])
{
    // revisar archivo si hay un juego pendiente, si no, se crea uno:
    std::vector<Card> drawPile;
    startGame(drawPile);
    shuffle(drawPile);
    displayHand(drawPile);
    opponentsHand(drawPile, 1);

    return 0;
}