#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <random>
#include <condition_variable>

std::condition_variable cv;
int option;

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
	bool operator==(const Card& card) {
		return card.color == color && card.value == value && card.special == special;
	}
};

std::string findType(std::string card) {
	if (card.find("REVERSE") != std::string::npos)
		return "REVERSE";
	else if (card.find("BLOCK") != std::string::npos)
		return "BLOCK";
	else if (card.find("PLUSTWO") != std::string::npos)
		return "PLUSTWO";
	else return "REGULAR";
}

std::string findColor(std::string card) {
	if (card.find("YELLOW") != std::string::npos)
		return "YELLOW";
	else if (card.find("RED") != std::string::npos)
		return "RED";
	else if (card.find("BLUE") != std::string::npos)
		return "BLUE";
	else if (card.find("GREEN") != std::string::npos)
		return "GREEN";
	else return "NOTFOUND";
}

int findNumber(std::string card) {
	if (card.find("1") != std::string::npos)
		return 1;
	else if (card.find("2") != std::string::npos)
		return 2;
	else if (card.find("3") != std::string::npos)
		return 3;
	else if (card.find("4") != std::string::npos)
		return 4;
	if (card.find("5") != std::string::npos)
		return 5;
	else if (card.find("6") != std::string::npos)
		return 6;
	else if (card.find("7") != std::string::npos)
		return 7;
	else if (card.find("8") != std::string::npos)
		return 8;
	else if (card.find("9") != std::string::npos)
		return 9;
	else if (card.find("0") != std::string::npos)
		return 0;
	else return -1;
}

void startGame(std::vector<Card>& drawPile)
{
	std::string colors[4] = { "RED", "YELLOW", "BLUE", "GREEN" };
	std::string specials[3] = { "BLOCK", "PLUSTWO", "REVERSE" };
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

void displayHand(std::vector<Card>& hand)
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

void displayTopCard(std::vector<Card>& discardPile) {
	std::string topCard;
	if (discardPile.at(0).special == "REGULAR")
	{
		topCard = discardPile.at(0).color + std::to_string(discardPile.at(0).value);
	}
	else
	{
		topCard = discardPile.at(0).color + discardPile.at(0).special;
	}
	std::cout << "[" << topCard << "]" << std::endl;
}

void opponentsHand(std::vector<Card>& hand, int numberOfPlayer)
{
	int size = hand.size();
	std::string handDisplay;
	handDisplay += "P" + std::to_string(numberOfPlayer) + "->" + std::to_string(size);
	std::cout << handDisplay << std::endl;
}

void shuffle(std::vector<Card>& drawPile)
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
int getScore(std::vector<Card> firstHand, std::vector<Card> secondHand, int ifh, int ish)
{
	int totalScore = 0;
	// firstHand
	if (ifh < firstHand.size())
	{
		if (firstHand.at(ifh).value != -1)
		{
			totalScore += firstHand.at(ifh).value;
		}
		else
		{
			if (firstHand.at(ifh).special == "PLUSTWO")
			{
				totalScore += 30;
			}
			else if (firstHand.at(ifh).special == "REVERSE")
			{
				totalScore += 25;
			}
			else
			{
				totalScore += 20;
			}
		}
		getScore(firstHand, secondHand, ifh + 1, ish);
	}
	// secondHand
	if (ish < secondHand.size())
	{
		if (secondHand.at(ish).value != -1)
		{
			totalScore += secondHand.at(ish).value;
		}
		else
		{
			if (secondHand.at(ish).special == "PLUSTWO")
			{
				totalScore += 30;
			}
			else if (secondHand.at(ish).special == "REVERSE")
			{
				totalScore += 25;
			}
			else
			{
				totalScore += 20;
			}
		}
		getScore(firstHand, secondHand, ifh, ish + 1);
	}
	return totalScore;
}

void drawCardAndPlay(std::vector<Card>& player, std::vector<Card>& discardPile, std::vector<Card>& drawPile) {
	Card drawnCard = drawPile.at(0);
	Card topCard = discardPile.at(0);

	drawPile.erase(drawPile.begin());

	//Check if drawn card matches any values of the top card of discard pile
	if (drawnCard.color == topCard.color || drawnCard.value == topCard.value || drawnCard.special == topCard.special)
		discardPile.insert(discardPile.begin(), drawnCard);
	else
		player.insert(player.begin(), drawnCard);
	//SORT CARDS AGAIN
}

void giveCardsAtStart(std::vector<Card>& player,  std::vector<Card>& drawPile, int numberOfCardsToGive) {
	for (int i = 0; i < numberOfCardsToGive; i++)
	{
		Card drawnCard = drawPile.at(0);
		player.insert(player.begin(), drawnCard);
		drawPile.erase(drawPile.begin());
	}
}


void printMenu() {
	std::cout << "Choose the number of yhe option you want to make:\n";
	std::cout << "1. DRAW\n";
	std::cout << "2. UNO\n";
	std::cout << "3. PLAYCARD\n";
	std::cout << "4. HELP\n";
	std::cout << "5. EXIT\n";
}

void printInstructivo() {
	std::cout << "Objetivo del juego\n El objetivo de UNO es deshacerse de todas las cartas que se “roban” inicialmente, diciendo la palabra “UNO” cuando queda la última carta en la mano.El primero que llega a 500 puntos gana.Se recibe puntos por todas las cartas que los otros jugadores todavía tienen en sus manos(véase puntos).\nLa preparación del juego\nSe baraja las cartas y cada jugador recibe siete cartas.Las cartas restantes se ponen encubiertas en el centro y forman el mazo.La primera carta se desvela y se pone al lado.Este mazo es el mazo de descartes.Un jugador se sortea y empieza la ronda.\nDecurso del juego\nEl primero jugador pone una carta de su mano al mazo de descartes.Aquí vale : Una carta sólo se puede superponer en una carta del mismo color o del mismo número.Las cartas negras son cartas de acción especiales con reglas particulares(ver cartas de acción).Si un jugador no puede poner la carta oportuna, tiene que tomar una carta de pena del mazo.Puede jugar esta carta ahora mismo, si la vale bien.Si no, es el turno del siguiente jugador.Quién pone la penúltima carta, debe decir “UNO”(que significa “Eins”) y señala que tiene sólo una última carta en la mano.Si un jugador lo olvida y el otro lo nota a tiempo(antes de que el siguiente jugador haya tomado o ha depuesto una carta) tiene que tomar dos cartas de pena.El ganador de la ronda es él que depone la última carta.Los puntos se suman y se comienza una nueva ronda.\nCartas de Acción\nEn el juego hay cartas de acción negras con diferentes funciones, que se explican a continuación.CARTA TOMA DOS\nCuando se pone esta carta, el siguiente jugador debe tomar dos cartas y no puede deponer ninguna carta en esta ronda.Esta carta sólo puede superponer en una carta con el color apropiado u otras cartas “toma dos”.Si se revela al principio del juego, las mismas reglas aplican.\nCARTA DE RETORNO\nCon esta carta se cambia la dirección.Si se ha jugado por la izquierda, ahora se juega por la derecha y por la inversa.La carta sólo se puede superponer en una carta con color correspondiente o en una otra carta de retorno.Cuando esta carta se toma en el principio del juego, el dador comienza y entonces el jugador a su derecha continúa.\nCARTA DE INTERMISIÓN\nDespués de poner esta carta, el siguiente jugador será “saltado”.La carta sólo se puede superponer en una carta con color correspondiente o en una otra carta de intermisión.Cuando esta carta se toma en el principio del juego, el jugador se “salta” a la izquierda del dador y el jugador a la izquierda de este jugador continúa el juego.\nCARTA DE ELECCIÓN DE COLORES\nCon esta carta el jugador decide qué color sigue en el juego.También el color presente puede ser seleccionado.Una carta de elección de colores también se puede poner cuando el jugador puede poner una carta diferente.Si se toma una carta de elección de colores en el principio del juego, el jugador a la izquierda del dador decide qué color va a seguir.\nCARTA DE TOMAR CUATRO COLORES Esta carta es la mejor.El jugador decide qué color sigue en el juego.Además, el siguiente jugador debe tomar cuatro cartas.No se puede deponer cualquier carta en esta ronda.Por desgracia, la carta sólo se puede poner si el jugador que la tiene, no tiene ninguna carta en la mano que corresponde con el color del montón.Si el jugador tiene una carta con el número o una carta de acción, sin embargo la carta de tomar cuatro colores se puede poner.Un jugador que tiene una carta de tomar cuatro colores puede tirarse un farol y poner esa carta prohibida.Si el jugador se atrapa ciertas reglas aplican(ver cartas de pena).Si esta carta se toma en el principio del juego, se devuelve en el mazo y se toma otra carta.\nPenas\nSi un jugador no respeta las reglas, debe tomar una o más cartas de pena.Las reglas son los siguientes :\n \tUNO: Si un jugador olvida llamar “UNO” después de su penúltima carta y el siguiente jugador aún no ha jugado su carta, debe tomar una carta de pena.\n \tPropuestas : Quién hace propuestas a sus jugadores, debe tomar dos cartas de pena.\n \tPuesto incorrecto : Quién pone una carta, a pesar de que no está o una carta incorrecta, debe reanudarla y además, recibe una carta de pena.\n \tLa + 4 sólo se puede poner cuando el jugador no puede utilizar el color actual con excepción de otras cartas de acción.Si el jugador que tiene la + 4 cree que la carta se ha jugado mal, puede desafiar el jugador anterior.Debe entonces justificar al mostrar sus cartas que en realidad no podía operar correctamente el color.Puede confirmarlo, el jugador desafiante ahora debe tomar seis cartas en lugar de cuatro.Sin embargo fue trasladado a tener la + 4 ilegalmente, debe tomar las cuatro cartas a su mismo.";
}

void selectOptionOfMenu(int numberOfPlayer, std::vector<Card>& actualPlayerDeck, std::vector<Card>& drawPile, std::vector<Card>& discardPile) {
	int choice;
	// if es el player imprime el menu
	if(numberOfPlayer == 1)
	printMenu();

	// si es player, se le dara 5 segundos para contestar que quiere hacer
	if (numberOfPlayer) {
		std::cin >> choice;
	}
	else {
		// si es cpu, checara con nun metodo si tiene una carta
		std::string myCard = metodoMagico(actualPlayerDeck, discardPile);
		playCard(myCard, actualPlayerDeck, discardPile);
	}
	

	switch (choice) {
	case 1:
		std::cout << "You choose: DRAW\n";
		drawCardAndPlay(actualPlayerDeck, discardPile, drawPile);
		break;

	case 2:
		std::cout << "You choose: UNOOOOOOOOOOO\n";
		// check if the UNO was correct, and if yes let the player win.... if not add two cards and continue with next player
		// play card function()
		// check if win()
		// finish Turn()
		break;

	case 3:
		std::cout << "You choose: PLAYCARD\n";
		// check if the the user had one card... if yes, punish him for not saying UNO
		if (actualPlayerDeck.size() == 1) {
			// dumb, and the UNOOOOO?
			// give 2 cards to this dumb guy
			giveCardsAtStart(actualPlayerDeck,drawPile, 2);
		}else {
			try {
				std::string carta;
				std::cout << "Que carta deseas jugar? (Escribela exactamente como la vez)\n";
				std::cin >> carta;
				playCard(carta, actualPlayerDeck, discardPile);
			}
			catch (const char* msg) {
				std::cout << msg << std::endl;
				std::cout << "La carta que mandaste fue incorrecta, elige de nuevo: \n";
				selectOptionOfMenu(numberOfPlayer, actualPlayerDeck, drawPile, discardPile);
			}
		}

		// check if win()
		// finish Turn()
		break;

	case 4:
		std::cout << "You choose: HELP\n";
		printInstructivo();
		selectOptionOfMenu(numberOfPlayer, actualPlayerDeck, drawPile, discardPile);
		break;

	case 5:
		std::cout << "You choose: EXIT\n";
		exit(1);
		break;

	default:
		std::cout << "You choose: Error, option not recognized... try again.\n";
		selectOptionOfMenu(numberOfPlayer, actualPlayerDeck, drawPile, discardPile);
		break;
	}

}

bool gameIsRunning(std::vector<Card> player, std::vector<Card> pc1, std::vector<Card> pc2) {
	if (pc1.empty() || player.empty() || pc2.empty()) {
		return false;
	}
	else {
		return true;
	}
}


void playCard(std::string card, std::vector<Card>& player, std::vector<Card>& discardPile) {
	//For string Card ([RED2]) retrieve color type and number, and create new card.
	std::string color = findColor(card);
	std::string type = findType(card);
	int value = findNumber(card);
	Card mycard(color, value, type);
	//Delete card from user's cards
	std::vector<Card>::iterator itr = std::find(player.begin(), player.end(), mycard);

	if (itr != player.cend()) {
		player.erase(player.begin() + std::distance(player.begin(), itr));
	}
	else {
		throw "Element not found";
	}
	//Add the new card(same card) to the discard pile
	discardPile.insert(discardPile.begin(), mycard);
}

int main(int argc, char const* argv[])
{
	// revisar archivo si hay un juego pendiente, si no, se crea uno:
	std::vector<Card> drawPile;
	startGame(drawPile); //put 100 cards in the drawPile
	shuffle(drawPile); // shuffle it

	// discard pile
	std::vector<Card> discardPile;

	std::vector<Card> playerOne; //the human
	std::vector<Card> cpuOne; //the pc1
	std::vector<Card> cpuTwo; //the pc2

	//give 7/7/7 cards to each player from the drawPile


	// start of game... //each player already has 7 cards...
	int turn = 1; // 1 -> Player, 2 -> Pc1, 3 -> Pc2 
	while (gameIsRunning(playerOne, cpuOne, cpuTwo)) {
		// play

		// see mi mano
		std::cout << "Your hand:\n";
		displayHand(playerOne);

		// see cpu hands
		std::cout << "Others hands:\n";
		opponentsHand(cpuOne, 1);
		opponentsHand(cpuTwo, 2);

		// see the top card of the pile
		std::cout << "Pile:\n";
		displayTopCard(drawPile);

		std::vector<Card> actualV = (turn == 1) ? playerOne : (turn == 2) ? cpuOne : cpuTwo;
		selectOptionOfMenu(turn, actualV, drawPile, discardPile);
	}
	return 0;
}