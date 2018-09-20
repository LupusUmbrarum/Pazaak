#include<conio.h> // getch
#include<iostream>
#include<string>
#include<cstdlib> // rand, srand
#include<time.h>
#include "pazaak_card.h"

#define clearScreen system("cls")
#define charOffset 48

Pazaak_Card * getRandomCard()
{
	int num = rand() % 10;
	switch (num)
	{
	case 0:
	case 1:
	case 2://plus
	{
		return new Pazaak_Card(rand() % 6 + 1);
	}
	case 3:
	case 4:
	case 5://minus
	{
		return new Pazaak_Card(-(rand() % 6 + 1));
	}
	case 6://plus or minus
	{
		return new Pazaak_Card_PoM(rand() % 2 + 1);
	}
	case 7://flip
	{
		return new Pazaak_Card_Flip(rand() % 2 + 2);
	}
	case 8://double
	{
		return new Pazaak_Card_Double();
	}
	case 9://tiebreaker
	{
		return new Pazaak_Card_TieBreaker();
	}
	}

	return new Pazaak_Card(rand() % 10 + 1);
}

//take a character from the input and give us a usable number
int getCardValue(int minSelect, int maxSelect, int failValue)
{
	int selection = -999;

	while (selection != 'b' && (selection < minSelect || selection > maxSelect))
	{
		selection = _getch();
	}

	if (selection == 'b' || selection == failValue)
	{
		return -1;
	}
	else
	{
		return selection - 48;
	}
}

Pazaak_Card * makeCard(std::string msg, std::string options, int minSelect, int maxSelect, int failValue, CardType type)
{
	clearScreen;
	std::cout << msg << " (press 'b' to go back)\n" << options;

	int value = getCardValue(minSelect, maxSelect, failValue);

	if (value < 0)
	{
		return NULL;
	}
	else
	{
		switch (type)
		{
		case REGULAR:
		case PLUS:
		{
			return new Pazaak_Card(value);
		}
		case MINUS:
		{
			return new Pazaak_Card(-value);
		}
		case PLUSORMINUS:
		{
			return new Pazaak_Card_PoM(value);
		}
		case FLIP:
		{
			return new Pazaak_Card_Flip(value);
		}
		case DOUBLE:
		{
			return new Pazaak_Card_Double();
		}
		case TIEBREAKER:
		{
			return new Pazaak_Card_TieBreaker();
		}
		default:
		{
			return new Pazaak_Card(3);
		}
		}
	}
}

void makeSideDeck(Pazaak_Card *deck[])
{
	int numLeft = 0;

	while (numLeft < 10)
	{
		clearScreen;
		std::cout << "Select " << 10 - numLeft << " cards to add to your side deck\n(1) Add\n(2) Subtract\n(3) Add/Subtract\n(4) Flip\n(5) Double\n(6) Tiebreaker\n";

		int choice = 0;
		while (choice < '1' || choice > '6')
		{
			choice = _getch();
		}

		int selection = 0;

		switch (choice)
		{
		case '1':
		{
			deck[numLeft] = makeCard("Do you want the card to add 1, 2, 3, 4, 5, or 6?", "", '1', '6', '7', PLUS);
			break;
		}
		case '2':
		{
			deck[numLeft] = makeCard("Do you want the card to subtract 1, 2, 3, 4, 5, or 6?", "", '1', '6', '7', MINUS);
			break;
		}
		case '3':
		{
			deck[numLeft] = makeCard("Are you sure you want a 'plus or minus 1 or 2' card?", "(1) Yes\n(2) No\n", '1', '2', '2', PLUSORMINUS);
			break;
		}
		case '4':
		{
			deck[numLeft] = makeCard("Do you want to flip 2s and 4s or 3s and 6s that you've placed?", "(1) 2s and 4s\n(2) 3s and 6s\n", '1', '2', '3', FLIP);
			break;
		}
		case '5':
		{
			deck[numLeft] = makeCard("Are you sure you want a 'double' card?", "(1) Yes\n(2) No\n", '1', '2', '2', DOUBLE);
			break;
		}
		case '6':
		{
			deck[numLeft] = makeCard("Are you sure you want a 'tiebreaker' card?", "(1) Yes\n(2) No\n", '1', '2', '2', TIEBREAKER);
			break;
		}
		}

		if (deck[numLeft] == NULL)
		{
			numLeft--;
		}

		numLeft++;
	}
}

int hasCardType(Pazaak_Card * cards[], int length, CardType type)
{
	for (int x = 0; x < length; x++)
	{
		if (cards[x]->type == type && !cards[x]->beenPlayed)
		{
			return x;
		}
	}

	return -1;
}

void roboPlayPazaak(bool &stay, bool oppStay, int &sum, int oppSum, Pazaak_Card * hand[], Pazaak_Card * cardsPlayed[], int &numCardsPlayed)
{
	bool hasPlayedCard = false;
	bool turnEnded = false;
	while (!turnEnded && !stay)
	{
		if (oppStay)
		{
			if (sum > oppSum)
			{
				if (sum <= 20)
				{
					stay = true;
					turnEnded = true;
				}
				else if (!hasPlayedCard)//implying sum > 20
				{
					for (int x = 0; x < 4 && !hasPlayedCard; x++)
					{
						if (!hand[x]->beenPlayed)
						{
							if (hand[x]->type == MINUS)
							{
								if (hand[x]->value + sum <= 20 && hand[x]->value + sum > oppSum)
								{
									hand[x]->play(sum, cardsPlayed, numCardsPlayed);
									cardsPlayed[numCardsPlayed++] = hand[x];
									hand[x]->beenPlayed = true;

									hasPlayedCard = true;
									stay = true;
								}
							}
							else if (hand[x]->type == PLUSORMINUS)
							{
								int total = sum - hand[x]->value;

								if (total <= 20 && total > oppSum)
								{
									sum -= 2;
									cardsPlayed[numCardsPlayed++] = hand[x];
									hand[x]->beenPlayed = true;

									hasPlayedCard = true;
									stay = true;
								}

								total = sum - hand[x]->value / 2;

								if (total <= 20 && total > oppSum)
								{
									sum -= 1;
									cardsPlayed[numCardsPlayed++] = hand[x];
									hand[x]->beenPlayed = true;

									hasPlayedCard = true;
									stay = true;
								}
							}
						}// end if(!hand[x]->beenPlayed)
					}// end for(int x = 0; x < 4 && !hasPlayedCard; x++)
				}// end else if(!hasPlayedCard)
				else// sum > 20 && hasPlayedCard
				{
					turnEnded = true;
				}
			}// end if (sum > oppSum)
			else if (sum == oppSum)
			{
				int hct = hasCardType(hand, 4, TIEBREAKER);

				if (hct > 0)
				{
					hand[hct]->play(sum, cardsPlayed, numCardsPlayed);
					cardsPlayed[numCardsPlayed++] = hand[hct];
					hand[hct]->beenPlayed = true;
				}

				stay = true;

			}// end else if(sum == oppSum)
			else// sum < oppSum
			{
				// go through the npc's hand and see if there are any cards that will 
				// increase the npc's score above the players while staying at or under 
				// 20
				for (int x = 0; x < 4 && !hasPlayedCard; x++)
				{
					if (!hand[x]->beenPlayed)
					{
						switch (hand[x]->type)
						{
						case REGULAR:
						case PLUS:
						{
							if (sum + hand[x]->value > oppSum && sum + hand[x]->value <= 20)
							{
								hand[x]->play(sum, cardsPlayed, numCardsPlayed);
								cardsPlayed[numCardsPlayed++] = hand[x];
								hand[x]->beenPlayed = true;

								hasPlayedCard = true;
								break;
							}
						}
						case PLUSORMINUS:
						{
							int total = sum + hand[x]->value;

							if (total <= 20 && total > oppSum)
							{
								sum += 2;
								cardsPlayed[numCardsPlayed++] = hand[x];
								hand[x]->beenPlayed = true;

								hasPlayedCard = true;
								break;
							}

							total = sum + hand[x]->value / 2;

							if (total <= 20 && total > oppSum)
							{
								sum += 1;
								cardsPlayed[numCardsPlayed++] = hand[x];
								hand[x]->beenPlayed = true;

								hasPlayedCard = true;
							}

							break;
						}
						case DOUBLE:
						{
							if (sum + cardsPlayed[numCardsPlayed]->value > oppSum && sum + cardsPlayed[numCardsPlayed]->value <= 20)
							{
								hand[x]->play(sum, cardsPlayed, numCardsPlayed);
								cardsPlayed[numCardsPlayed++] = hand[x];
								hand[x]->beenPlayed = true;

								hasPlayedCard = true;
							}

							break;
						}
						case FLIP:
						{
							int flipSum = sum;
							hand[x]->play(flipSum, cardsPlayed, numCardsPlayed);

							if (flipSum > oppSum && flipSum <= 20)
							{
								cardsPlayed[numCardsPlayed++] = hand[x];
								
								hasPlayedCard = true;
							}
							else
							{
								hand[x]->beenPlayed = false;
							}

							break;
						}
						}// end switch(hand[x]->type)
					}// end if(!hand[x]->beenPlayed)
				}// end for(int x = 0; x < 4 && !hasPlayedCard; x++)

				if (sum > oppSum)
				{
					stay = true;
				}

				turnEnded = true;

			}// end else (i.e. sum < oppSum)

			turnEnded = true;

		}// end if(oppStay)
		else// keep playing even though the player stopped
		{
			if (sum == 20)
			{
				stay = true;
			}
			else if (sum < 20 && sum > 16)// if sum is between 17 and 19 inclusive
			{
				int keepgoing = rand() % 2;
				stay = keepgoing;
			}

			turnEnded = true;
		}
	}
}

void playerPlayPazaak(bool &stay, int &sum, Pazaak_Card * hand[], Pazaak_Card * cardsDown[], int &cardsPlayed)
{
	//print cards

	for (int x = 0; x < 4; x++)
	{
		if (!hand[x]->beenPlayed)
		{
			std::cout << hand[x]->name << " | ";
		}
	}
	std::cout << "\n";

	//let the player play
	bool playCard = true;
	int numCardsPlayed = 0;
	int choice = 0;
	bool continueOn = true;
	while (continueOn && !stay)
	{
		for (int x = 0; x < 4; x++)
		{
			numCardsPlayed += hand[x]->beenPlayed;
		}
		std::cout << "\n(1) End turn\n(2) Stay\n";
		if (playCard && numCardsPlayed < 4)
		{
			std::cout << "(3) Play card\n";
		}

		while (choice < '1' || choice > '3')
		{
			choice = _getch();
		}

		if (choice == '1')
		{
			continueOn = false;
		}
		else if (choice == '2')
		{
			stay = true;
			continueOn = false;
		}
		else if (choice == '3' && numCardsPlayed < 4 && playCard)
		{
			int numCardsInHand = 0;
			std::cout << "Which card do you want to play?";
			int offset = 0;
			for (int x = 0; x < 4; x++, offset++)
			{
				if (!hand[x]->beenPlayed)
				{
					std::cout << "\n(" << x + 1 << ") " << hand[x]->name;
					numCardsInHand++;
				}
			}

			std::cout << "\n";

			int cardChoice = 0;
			while (cardChoice < '1' || cardChoice > '4')
			{
				cardChoice = _getch();
			}

			cardChoice -= charOffset;
			cardChoice--;

			if (hand[cardChoice]->beenPlayed)
			{
				choice = 0;
				continue;
			}
			else
			{
				hand[cardChoice]->play(sum, cardsDown, cardsPlayed);
				cardsDown[cardsPlayed++] = hand[cardChoice];
				hand[cardChoice]->beenPlayed = true;
				playCard = false;
				choice = 0;
			}
		}
		else
		{
			choice = 0;
		}
	}//end while(continueOn)
}

void playPazaak()
{
	bool play = true;

	Pazaak_Card * sideDeck[10];
	Pazaak_Card * npcSideDeck[10];

	//load player side deck
	makeSideDeck(sideDeck);

	//load npc side deck
	for (int x = 0; x < 10; x++)
	{
		npcSideDeck[x] = getRandomCard();
	}

	while (play)
	{
		Pazaak_Card * hand[4];
		Pazaak_Card * npcHand[4];
		Pazaak_Card * mainDeck[40];

		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 10; y++)
			{
				mainDeck[x * 10 + y] = new Pazaak_Card(y + 1);
			}
		}

		//load both players' hands
		for (int x = 0; x < 4; x++)
		{
			int handIndex = rand() % 10;
			while (sideDeck[handIndex]->beenPlayed)
			{
				handIndex = rand() % 10;
			}
			hand[x] = sideDeck[handIndex];
			sideDeck[handIndex]->beenPlayed = true;

			while (npcSideDeck[handIndex]->beenPlayed)
			{
				handIndex = rand() % 10;
			}
			npcHand[x] = sideDeck[handIndex];
			npcSideDeck[handIndex]->beenPlayed = true;
		}

		for (int x = 0; x < 4; x++)
		{
			hand[x]->beenPlayed = false;
			npcHand[x]->beenPlayed = false;
		}

		//actually play the game
		int p1Score = 0;
		int p2Score = 0;

		// play a match
		while (p1Score < 2 && p2Score < 2)
		{
			int p1Sum = 0;
			int p2Sum = 0;
			int p1CardsPlayed = 0;
			int p2CardsPlayed = 0;
			bool p1Stay = false;
			bool p2Stay = false;
			Pazaak_Card * cardsDownP1[9];
			Pazaak_Card * cardsDownP2[9];
			int index = 0;

			// shuffle the deck
			for (int x = 0; x < 80; x++)
			{
				int swapPoint1 = rand() % 40;
				Pazaak_Card *temp = mainDeck[swapPoint1];
				int swapPoint2 = rand() % 40;
				mainDeck[swapPoint1] = mainDeck[swapPoint2];
				mainDeck[swapPoint2] = temp;
			}

			// play a round
			while ((!p1Stay || !p2Stay) && (p1Sum <= 20 && p2Sum <= 20) && (p1CardsPlayed < 9 && p2CardsPlayed < 9))
			{
				clearScreen;

				//print current totals and latest cards
				if (!p1Stay)
				{
					p1Sum += mainDeck[index]->value;
					std::cout << "Player 1s new card: " << mainDeck[index]->value << "\n";
					cardsDownP1[p1CardsPlayed++] = mainDeck[index++];
				}
				if (!p2Stay)
				{
					p2Sum += mainDeck[index]->value;
					std::cout << "Player 2s new card: " << mainDeck[index]->value << "\n";
					cardsDownP2[p2CardsPlayed++] = mainDeck[index++];
				}

				std::cout << "Player 1 Total: " << p1Sum << "\nPlayer 2 Total: " << p2Sum << "\n";

				if (!p1Stay)
				{
					playerPlayPazaak(p1Stay, p1Sum, hand, cardsDownP1, p1CardsPlayed);
				}
				if (!p2Stay)
				{
					roboPlayPazaak(p2Stay, p1Stay, p2Sum, p1Sum, npcHand, cardsDownP2, p2CardsPlayed);
				}

			}//end while(((!p1Stay || !p2Stay) && (p1Sum <= 20 && p2Sum <= 20) && (p1CardsPlayed < 9 && p2CardsPlayed < 9))

			clearScreen;

			std::cout << "final sums: " << p1Sum << " & " << p2Sum << "\n";

			//determine the winner
			//if player's total is greater than 20
			if (p1Sum > 20)
			{
				//if npc's total is less than or equal to 20
				if (p2Sum <= 20)
				{
					std::cout << "Player 2 wins!\n";
					p2Score++;
				}
				//else npc's total is greater than 20
				else
				{
					std::cout << "You both lose!\n";
				}
			}
			//if npc's total is greater than 20
			else if (p2Sum > 20)
			{
				//if player's total is less than or equal to 20
				if (p1Sum <= 20)
				{
					std::cout << "Player 1 wins!\n";
					p1Score++;
				}
				//else player's total is greater than 20
				else
				{
					std::cout << "You both lose!\n";
				}
			}
			else
			{
				if (p1Sum != p2Sum)
				{
					if (p1Sum > p2Sum)
					{
						std::cout << "Player 1 wins!\n";
						p1Score++;
					}
					else
					{
						std::cout << "Player 2 wins!\n";
						p2Score++;
					}
				}
				else
				{
					bool hadTB = false;

					//does either player have the tiebreaker card and did they play it?
					for (int x = 0; x < 4; x++)
					{
						if (sideDeck[x]->type == TIEBREAKER && sideDeck[x]->beenPlayed)
						{
							std::cout << "Player 1 used a Tie-Breaker card and won!\n";
							p1Score++;
							hadTB = true;
							break;
						}

						if (npcSideDeck[x]->type == TIEBREAKER && npcSideDeck[x]->beenPlayed)
						{
							std::cout << "Player 2 used a Tie-Breaker card and won!\n";
							p2Score++;
							hadTB = true;
							break;
						}
					}

					if (!hadTB)
					{
						std::cout << "You both lose!\n";
					}
				}
			}

			std::cout << "Current match score:\nPlayer 1: " << p1Score << "\nPlayer 2: " << p2Score << "\n";
			system("pause");
		}// end while (p1Score < 2 && p2Score < 2)

		clearScreen;

		std::cout << "Player ";

		if (p1Score > p2Score)
		{
			std::cout << 1;
		}
		else
		{
			std::cout << 2;
		}

		std::cout << " wins!\n";

		system("pause");
		clearScreen;

		for (int x = 0; x < 10; x++)
		{
			sideDeck[x]->beenPlayed = false;
			npcSideDeck[x]->beenPlayed = false;
		}

		int restartChoice = 0;

		//ask if the user wants to keep playing
		std::cout << "Do you want to keep playing?\n(1) Yes\n(2) No\n";

		while (restartChoice != '1' && restartChoice != '2')
		{
			restartChoice = _getch();
		}

		if (restartChoice == '2')
		{
			play = false;
			continue;
		}

		restartChoice = 0;
		clearScreen;

		//ask if user wants to use a new deck
		std::cout << "Do you want to keep using the same side deck?\n(1) Yes\n(2) No\n";

		while (restartChoice != '1' && restartChoice != '2')
		{
			restartChoice = _getch();
		}

		if (restartChoice == '2')
		{
			makeSideDeck(sideDeck);
		}
	}//end while(play)

	//clear memory
	for (int x = 0; x < 10; x++)
	{
		if (sideDeck[x] != NULL)
		{
			delete sideDeck[x];
		}
		if (npcSideDeck[x] != NULL)
		{
			delete npcSideDeck[x];
		}
	}
}

int main()
{
	std::srand((unsigned int)time(NULL));

	playPazaak();

	return 0;
}