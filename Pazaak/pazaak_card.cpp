#include "pazaak_card.h"

Pazaak_Card::Pazaak_Card()
{
	Pazaak_Card(0);
}

Pazaak_Card::Pazaak_Card(int value)
{
	this->value = value;
	beenPlayed = false;
	hasSpecialAction = false;
	type = REGULAR;
	if (value < 0)
	{
		name = "Minus " + std::to_string(abs(value));
	}
	else
	{
		name = "Plus " + std::to_string(value);
	}
}

Pazaak_Card::~Pazaak_Card()
{
	beenPlayed = 0;
	hasSpecialAction = 0;
	value = 0;
}

int Pazaak_Card::play(int &sum, Pazaak_Card *cards[], int length)
{
	sum += value;
	return 0;
}

Pazaak_Card_PoM::Pazaak_Card_PoM() : Pazaak_Card()
{
	Pazaak_Card_PoM(2);
}

Pazaak_Card_PoM::Pazaak_Card_PoM(int value) : Pazaak_Card()
{
	if ((value > 0 && value < 3) || (value > -3 && value < 0))
	{
		this->value = abs(value);
	}
	else
	{
		this->value = 2;
	}

	hasSpecialAction = true;
	type = PLUSORMINUS;
	name = "Plus or Minus " + std::to_string(this->value);
}

int Pazaak_Card_PoM::play(int &sum, Pazaak_Card *cards[], int length)
{
	if (!beenPlayed)
	{
		std::cout << "Add or subtract 1 or 2" << value << "?\n(1) Add 1\n(2) Add 2\n(3) Subtract 1\n(4) Subtract 2";
		int val = 0;

		while (val < '1' || val > '4')
		{
			val = _getch();
		}

		switch (val)
		{
		case '1':
		{
			sum += 1;
			break;
		}
		case '2':
		{
			sum += 2;
			break;
		}
		case '3':
		{
			sum -= 1;
			break;
		}
		case '4':
		{
			sum -= 2;
			break;
		}
		}

		this->beenPlayed = true;
	}

	return 0;
}

Pazaak_Card_Flip::Pazaak_Card_Flip() : Pazaak_Card()
{
	Pazaak_Card_Flip(2);
}

Pazaak_Card_Flip::Pazaak_Card_Flip(int value) : Pazaak_Card(2)
{
	if (abs(value) % 2 != 0 || abs(value) % 3 != 0)
	{
		this->value = 3;
	}
	else
	{
		this->value = value;
	}

	hasSpecialAction = true;
	type = FLIP;
	
	if (abs(this->value) % 3 == 0)
	{
		name = "Flip 3s and 6s";
	}
	else
	{
		name = "Flip 2s and 4s";
	}
}

int Pazaak_Card_Flip::play(int &sum, Pazaak_Card *cards[], int length)
{
	if (!beenPlayed)
	{
		int x = 0;

		while (x < length)
		{
			if (cards[x]->beenPlayed && cards[x] != this && cards[x]->type != FLIP && abs(cards[x]->value) == value || abs(cards[x]->value) == value * 2)
			{
				sum += (-1 * cards[x]->value);
			}

			x++;
		}

		this->beenPlayed = true;
	}

	std::cout << "sum: " << sum << "\n";

	return 0;
}

Pazaak_Card_Double::Pazaak_Card_Double() : Pazaak_Card(0)
{
	hasSpecialAction = true;
	type = DOUBLE;
	value = 0;
	name = "Double";
}

int Pazaak_Card_Double::play(int &sum, Pazaak_Card *cards[], int length)
{
	sum += cards[length - 1]->value;
	return 0;
}

Pazaak_Card_TieBreaker::Pazaak_Card_TieBreaker() : Pazaak_Card(0)
{
	type = TIEBREAKER;
	name = "Tie-Breaker";
}