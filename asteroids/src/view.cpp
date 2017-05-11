/* Asteroids view
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

#include "asteroids.h"
#include "model.h"
#include "utils.h"
#include "bitmap.h"

Display *graphics = Display::theDisplay();
ship ship;
const colour_t background = rgb(0,51,102); /* Midnight Blue */

int flash = 0; 
bool end = false;
bool win = false;
bool load = false;


///////////////////////////////////////////////////////////////
//									  	Screen Buffering                     //
///////////////////////////////////////////////////////////////

void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
	
	uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;	
}


///////////////////////////////////////////////////////////////
//									  	Asteroid Drawing                     //
///////////////////////////////////////////////////////////////

void drawparticles(struct particle *lst)
{
		//Draws Astroids and Bull
    while(lst) {
		
		if((lst->type == 1) && (lst->active))
		{
			graphics->fillCircle(lst->x, lst->y, 1, WHITE);
			graphics->setCursor(lst->x, lst->y);
		}
		
		//Astroid Drawing small and large
		else if(lst->type == 0){
			int size = lst->size;
			graphics->setCursor(lst->x,lst->y);
			if(size == 2){
				graphics->drawLine(lst->x+10,lst->y,lst->x+20,lst->y-10,WHITE);
				graphics->drawLine(lst->x+20,lst->y-10,lst->x,lst->y-30,WHITE);
				graphics->drawLine(lst->x,lst->y-30,lst->x-20,lst->y-30, WHITE);
				graphics->drawLine(lst->x-10,lst->y-20,lst->x-20,lst->y-30, WHITE);
				graphics->drawLine(lst->x-10,lst->y-20,lst->x-20,lst->y-20,WHITE);
				graphics->drawLine(lst->x-20,lst->y-20,lst->x-20,lst->y+10, WHITE);
				graphics->drawLine(lst->x-20,lst->y+10,lst->x-10,lst->y+20, WHITE);
				graphics->drawLine(lst->x-10,lst->y+20,lst->x+10,lst->y+10, WHITE);
				graphics->drawLine(lst->x+10,lst->y+10,lst->x+15,lst->y+15, WHITE);
				graphics->drawLine(lst->x+15,lst->y+15,lst->x+20,lst->y+5, WHITE);
				graphics->drawLine(lst->x+20,lst->y+5,lst->x+10,lst->y, WHITE);
			}
			if(size == 1)
			{
				graphics->drawLine(lst->x+5,lst->y,lst->x+10,lst->y-5,WHITE);
				graphics->drawLine(lst->x+10,lst->y-5,lst->x,lst->y-15,WHITE);
				graphics->drawLine(lst->x,lst->y-15,lst->x-10,lst->y-15, WHITE);
				graphics->drawLine(lst->x-5,lst->y-10,lst->x-10,lst->y-15, WHITE);
				graphics->drawLine(lst->x-5,lst->y-10,lst->x-10,lst->y-10,WHITE);
				graphics->drawLine(lst->x-10,lst->y-10,lst->x-10,lst->y+5, WHITE);
				graphics->drawLine(lst->x-10,lst->y+5,lst->x-5,lst->y+10, WHITE);
				graphics->drawLine(lst->x-5,lst->y+10,lst->x+5,lst->y+5, WHITE);
				graphics->drawLine(lst->x+5,lst->y+5,lst->x+8,lst->y+8, WHITE);
				graphics->drawLine(lst->x+8,lst->y+8,lst->x+10,lst->y+3, WHITE);
				graphics->drawLine(lst->x+10,lst->y+3,lst->x+5,lst->y, WHITE);
			}
		}
		lst = lst->next;/* itterate along the list */
	}
}


double radians(double a)
{
    const double pi = 3.141592;
    return a * pi /180.0f; 
}


///////////////////////////////////////////////////////////////
//						  			  	Ship Drawing                       //
///////////////////////////////////////////////////////////////

void drawShip(void)
{	
	graphics->drawTriangle(x1+player.p.x, y1+player.p.y, x2+player.p.x, y2+player.p.y, x3+player.p.x, y3+player.p.y, WHITE);
}


///////////////////////////////////////////////////////////////
//						  		  	Shield Drawing                       //
///////////////////////////////////////////////////////////////



void drawShield(void)
{
	
	//shield drawing and flashing
	if(shield && flash < 12 )
	{
		graphics->drawCircle(player.p.x, player.p.y, 20, GREEN);
		flash++;
		
	}
	else if (shield && flash < 24)
	{
		
		graphics->drawCircle(player.p.x, player.p.y, 20, RED);	
		flash++;
	}
	else 
	{
		flash = 0;
	}
	
}


///////////////////////////////////////////////////////////////
//							  		  	Other Drawing                      //
///////////////////////////////////////////////////////////////

void drawScore(void)
{
	graphics->fillRect(0, 0, 55, 272, background);
	graphics->setCursor(5,3);
	graphics->printf("Score:");
	graphics->setCursor(5,15);
	graphics->setTextSize(2);
	graphics->printf("%d",score);
	graphics->drawLine(55, 0, 55, 270, WHITE);
	
	
	// draws lives indicator
	if(lives >= 1){
		graphics->drawTriangle(5,50,11,50,8,40,WHITE); 
	}
	if(lives >= 2){
		graphics->drawTriangle(14,50,20,50,17,40,WHITE); 
	}
	if(lives >= 3){
		graphics->drawTriangle(23,50,29,50,26,40,WHITE); 
	}
	if(lives >= 4){
		graphics->drawTriangle(32,50,38,50,35,40,WHITE); 
	}
	if(lives == 5){
		graphics->drawTriangle(40,50,46,50,43,40,WHITE); 
	}
	
	//Draws Shield Health
	graphics->setCursor(5,60);
	graphics->setTextSize(1);
	graphics->printf("Shield");
	graphics->setCursor(5,70);
	graphics->printf("Health:");
	graphics->setCursor(5,80);
	graphics->printf("%d",damage);
	graphics->setCursor(5,90);
}

void startScreen()
{
	//Draws start screen
	if (!load){
	graphics->drawBitmap(0, 0, image_ship, 480, 270, WHITE);
	}
	load = true;
	graphics->setTextSize(2);
	graphics->setTextColor(GREEN);
	graphics->setCursor(120,230);
	graphics->printf("Press Pause to Begin");
}



void drawEnd()
{
	//draws end screen
	graphics->setTextSize(2);
	graphics->fillScreen(background);
	graphics->setTextSize(2);
	graphics->setCursor(200,90);
	graphics->printf("Game Over");
	graphics->setCursor(195,120);
	graphics->printf("SCORE: %d",score);	
	swap_DBuffer();
	wait(100);
	
}

void draw(void)
{
	//draws to screen
	
	if(lives == 0)
	{
		end =  true;	
	}
	
	if(paused)
	{
		startScreen();
	}
	else if(!end)
	{
		graphics->fillScreen(background);
		drawShip();
		drawShield();
		drawparticles(active);
		drawScore();
		swap_DBuffer();
	}
	
	else 
	{
		drawEnd();				
		inPlay = false;
	}
	
}
