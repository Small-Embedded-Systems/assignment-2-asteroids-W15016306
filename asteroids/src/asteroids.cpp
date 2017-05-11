
//Joshua Higgins - W15016306
//Connor Moore - W15012760


/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"
#include <node.h>
#include "utils.h"


/* Game state */

//basic game variables
int   	score = 0;
int   	lives = 6;
struct 	ship player;
bool 		inPlay = true;
double 	randInterval = 5;
float 	Dt = 0.01f;


//Timers
Ticker model, view, controller, toggle, particles, utils;

//controls if game is paused
bool paused = true;

/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10,PullUp);

const float pi = 3.1415926f;
float radians(float degrees) { return degrees/180*pi; }

///////////////////////////////////////////////////////////////
//									  	  Main Method                        //
///////////////////////////////////////////////////////////////




int main()
{		
	paused = true;
	//Start Screen
	view.attach(draw, 0.030);
	
	while( userbutton.read() ){ 
		paused=true;				
		wait_ms(100);
		
	}	
	
	//Main Game
	init_DBuffer();
  paused = false;
	player.heading = -90;
	player.p.x=240;
	player.p.y=136;	
	initialise_heap();	
	
	//Timers
	utils.attach(scoreControler, 0.7);
	controller.attach( controls, 0.013);
	model.attach(particle_system, Dt);
	toggle.attach(toggleShield,randInterval);
	particles.attach( physics, Dt);
}





