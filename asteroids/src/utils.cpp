
/* utility functions */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "asteroids.h"

//variable Decleration
bool shieldCollision = false;
bool buttonPressed = false;
int damage = 0;
bool shield = false;

//Random number generator
int randrange(int from, int to)
{
    int range = to-from;
    return from + rand()%range;
}

//Incremetns the score over time
void scoreControler(void){
	
	if(inPlay)
	{
		score = score + 2;
	}
	
}

//toggles the shiled on and off
void toggleShield(void)
{
	if(shield)
	{
		shield = false;
		randInterval = randrange(0,10);
	}
	else 
	{
		shield =  true;
		
	}
}


