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

/*
    @params
    fristHand, secondHand, which are the hands of the 2 players that lost
    @return
    integer value of total score
*/
int getScore(std::vector<Card> firstHand, std::vector<Card> secondHand)
{
    int totalScore = 0;
    // firstHand
    for (int i = 0; i < firstHand.size(); i++)
    {
        if (firstHand.at(i).value != -1)
        {
            totalScore += firstHand.at(i).value;
        }
        else
        {
            if (firstHand.at(i).special == "PLUSTWO")
            {
                totalScore += 30;
            }
            else if (firstHand.at(i).special == "REVERSE")
            {
                totalScore += 25;
            }
            else
            {
                totalScore += 20;
            }
        }
    }
    // secondHand
    for (int i = 0; i < secondHand.size(); i++)
    {
        if (secondHand.at(i).value != -1)
        {
            totalScore += secondHand.at(i).value;
        }
        else
        {
            if (secondHand.at(i).special == "PLUSTWO")
            {
                totalScore += 30;
            }
            else if (secondHand.at(i).special == "REVERSE")
            {
                totalScore += 25;
            }
            else
            {
                totalScore += 20;
            }
        }
    }
    return totalScore;
}

void drawCard(std::vector<Card> &player, std::vector<Card> &discardPile, std::vector<Card> &drawPile){
    Card drawnCard = drawPile.at(0);
    Card topCard = discardPile.at(0);
    //Check if drawn card matches any values of the top card of discard pile
    if(drawnCard.color == topCard.color || drawnCard.value == topCard.value || drawnCard.special == topCard.special)
        discardPile.insert(discardPile.begin(),drawnCard);
    else
        player.insert(player.begin(),drawnCard);
    //SORT CARDS AGAIN
}

void playCard(std::string card, std::vector<Card> &player, std::vector<Card> &discardPile){
    //For string Card ([RED2]) retrieve color type and number, and create new card.
    std::string color = findColor(card);
    std::string type = findType(card);
    int value = findNumber(card);
    Card mycard(color,value,type);
    //Delete card from user's cards
    player.erase(std::remove(player.begin(), player.end(), card), player.end());
    //Add the new card(same card) to the discard pile
    discardPile.insert(discardPile.begin(),mycard);
}

std::string findType(std::string card){
    if (card.find("REVERSE")!=std::string::npos)
        return "REVERSE";
    else if (card.find("BLOCK")!=std::string::npos)
        return "BLOCK";
    else if (card.find("PLUSTWO")!=std::string::npos)
        return "PLUSTWO";
    else return "REGULAR";
}

std::string findColor(std::string card){
    if (card.find("YELLOW")!=std::string::npos)
        return "YELLOW";
    else if (card.find("RED")!=std::string::npos)
        return "RED";
    else if (card.find("BLUE")!=std::string::npos)
        return "BLUE";
    else if (card.find("GREEN")!=std::string::npos)
        return "GREEN";
    else return "NOTFOUND";
}

int findNumber(std::string card){
    if (card.find("1")!=std::string::npos)
        return 1;
    else if (card.find("2")!=std::string::npos)
        return 2;
    else if (card.find("3")!=std::string::npos)
        return 3;
    else if (card.find("4")!=std::string::npos)
        return 4;
    if (card.find("5")!=std::string::npos)
        return 5;
    else if (card.find("6")!=std::string::npos)
        return 6;
    else if (card.find("7")!=std::string::npos)
        return 7;
    else if (card.find("8")!=std::string::npos)
        return 8;
    else if (card.find("9")!=std::string::npos)
        return 9;
    else if (card.find("0")!=std::string::npos)
        return 0;
    else return -1;
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