#pragma once
#include <string>
#include <iostream>
#include <conio.h>

enum CardType
{
	REGULAR,
	PLUS,
	MINUS,
	PLUSORMINUS,
	FLIP,
	DOBLE,
	TIEBREAKER
};

class Pazaak_Card
{
public:
	int value = 0;
	bool beenPlayed = false, hasSpecialAction = false;
	CardType type;
	std::string name;

	Pazaak_Card();
	Pazaak_Card(int);
	~Pazaak_Card();
	virtual int play(int&, Pazaak_Card*[], int);
};

class Pazaak_Card_PoM : public Pazaak_Card
{
public:

	Pazaak_Card_PoM();
	Pazaak_Card_PoM(int);
	virtual int play(int&, Pazaak_Card*[], int);
};

class Pazaak_Card_Flip : public Pazaak_Card
{
public:

	Pazaak_Card_Flip();
	Pazaak_Card_Flip(int);
	virtual int play(int&, Pazaak_Card*[], int);
};

class Pazaak_Card_Double : public Pazaak_Card
{
public:

	Pazaak_Card_Double();
	virtual int play(int&, Pazaak_Card*[], int);
};

class Pazaak_Card_TieBreaker : public Pazaak_Card
{
public:
	bool wasPlayedThisRound = false;

	Pazaak_Card_TieBreaker();
	virtual int play(int&, Pazaak_Card*[], int);
};