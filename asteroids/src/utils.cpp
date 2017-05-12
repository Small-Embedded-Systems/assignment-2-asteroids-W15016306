
/* utility functions */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "asteroids.h"

//variable Decleration
bool shieldCollision = false;
int damage = 0;
bool shield = false;

//Random number generator
int randrange(int from, int to)
{
    int range = to-from;
    return from + rand()%range;
}


//Joshua Higgins - W15016306
//Connor Moore - W15012760


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
		damage = 100;
		randInterval = randrange(0,10);
	}
	else 
	{
		shield =  true;
		
	}
}


