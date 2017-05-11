/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"
#include "view.h"


//variable Decleration
static const int heapsize = 25;
static node_t heap[heapsize];
static node_t *freenodes;
int n = 0;
float shot0[4] = {3000,3000,0,0},
shot1[4] = {3000,3000,0,0}, 
shot2[4] = {3000,3000,0,0}, 
shot3[4] = {3000,3000,0,0}, 
shot4[4] = {3000,3000,0,0};
bool collision = false;
int hits = 15;
int newPosX, newPosY;
int asteroidCount, totalAsteroids, destroyedAsteroids;
int fired, firedCount;
bool asteroidSplitMode = false;
double x1, y1, x2, y2, x3, y3;
double 	acceleration = 0.0;
float 	bulletAcceleration = 4;
float 	newHeading,	currentHeading;

//Heap Creation for linked list
void initialise_heap(void)
{
    for( n=0 ; n<(heapsize-0) ; n++) {
        heap[n].next = &heap[n+1];
	}
    heap[n].next = NULL;
    freenodes = &heap[0];
}

node_t *allocnode(void)
{
    node_t *node = NULL;
    if( freenodes ) {
        node = freenodes;
        freenodes = freenodes->next;
	}
    return node;
}

void freenode(node_t *n)
{
    n->next = freenodes;
    freenodes = n;
}

//Games physics 
void physics(void)
{
	
	//shio movement
	if(acceleration > 0.0)
	{		
		player.p.x += acceleration*cos(radians(newHeading));
		player.p.y += acceleration*sin(radians(newHeading));
		if(acceleration > 0.5)
		{
			acceleration = acceleration-0.006;	
		}
		else
		{
			acceleration = acceleration-0.002;
		}			
	}	
	
	x1=-7*cos(radians(player.heading)) -  7*sin(radians(player.heading));
	y1=-6*sin(radians(player.heading)) +  6*cos(radians(player.heading));
	
	x2= -6* cos(radians(player.heading)) - -6*sin(radians(player.heading));
	y2= -6* sin(radians(player.heading)) + -6*cos(radians(player.heading));	
	
	x3= 12*cos(radians(player.heading)) -  sin(radians(player.heading));
	y3= 12*sin(radians(player.heading)) +  cos(radians(player.heading));
	
	
	//Ship Wrapping 
	if(player.heading >= 360)
	{
		player.heading = 0;
	}
	
	if(player.heading <= -360)
	{
		player.heading = 0;
	}
	
	if(player.p.y < -10)
	{
		player.p.y = 272;
	}
	
	if(player.p.y > 282)
	{
		player.p.y = 0;
	}
	
	if(player.p.x > 490)
	{
		player.p.x = 40;
	}
	
	if(player.p.x < 40)
	{
		player.p.x = 480;
	}
	
	//destroy check
	if(destroyedAsteroids == 11)
	{
		asteroidCount = 1;
		totalAsteroids = 1;
		destroyedAsteroids = 0;
	}	
}


///////////////////////////////////////////////////////////////
//									  	Asteroid Control                     //
///////////////////////////////////////////////////////////////

//creates elements for list
void strike(struct particle *l)
{
	
	if(asteroidCount < 5){
		if(!asteroidSplitMode){
			l->x = randrange(0,480);
			l->y = randrange(0,270);
			l->size = 2;
			l->heading = randrange(-259,360);
			l->ttl = 10;
			l->active = true;
			l->type = 0;			
		}
		
		else
		{
			l->x = randrange(newPosX,newPosX+15);
			l->y = randrange(newPosY,newPosY+15);
			l->size = 1;
			l->heading = randrange(-259,360);
			l->ttl = 10;
			l->active = true;
			l->type = 0;			
			asteroidSplitMode = false;
		}
		asteroidCount++;
		totalAsteroids++;
	}
	
	
	if((fired == 1)&&(firedCount <=5))
	{
		fired = 0;
		l->x = player.p.x+x3;
		l->y = player.p.y+y3;
		l->active = true;
		l->distanceCount = 0;
		l->count = randrange(0,90000);
		l->heading = player.heading;
		l->type = 1;
		
		
		if(shot0[3]==0)
		{
			shot0[2]=l->count;
			shot0[3]=1;
		}
		else if(shot1[3]==0)
		{
			shot1[2]=l->count;
			shot1[3]=1;
		}
		else if(shot2[3]==0)
		{
			shot2[2]=l->count;
			shot2[3]=1;
		}
		else if(shot3[3]==0)
		{
			shot3[2]=l->count;
			shot3[3]=1;
		}
		if(shot4[3]==0)
		{
			shot4[2]=l->count;
			shot4[3]=1;
		}
		firedCount++;
	}
	else{
		fired = 0;
	}
	
}



///////////////////////////////////////////////////////////////
//						           List Managment                      //
///////////////////////////////////////////////////////////////

//updates elements in list
void update(struct particle *l)
{
	
	for( ; l ; l = l->next ) {
		if(firedCount < 0)
		{
			firedCount = 0;
		}
		if((!l->next->active)) {
			if(l->type == 0)
			{
				destroyedAsteroids++;
				struct particle *expired = l->next;
				l->next = l->next->next;
				freenode(expired);			
			}
			if(l->type == 1)
			{
				firedCount--;
				fired = 0;
				struct particle *expired = l->next;
				l->next = l->next->next;
				freenode(expired);			
			}						
			
		}
		
		
		if(l->type == 0){		
			
			//Bullets and Asteroids
			int size;
			if(l->size == 2)
			{
				size = 30;
			}
			else if(l->size == 1)
			{
				size = 15;
			}
			
			if ((((shot0[0] >= l->x-size)&&(shot0[0] <= l->x+size)) && ((shot0[1] >= l->y-size)&&(shot0[1] <= l->y+size))&&(shot0[3]==1)))
			{
				collision = true;
				shot0[3]=0;
			}
			if ((((shot1[0] >= l->x-size)&&(shot1[0] <= l->x+size)) && ((shot1[1] >= l->y-size)&&(shot1[1] <= l->y+size))&&(shot1[3]==1)))
			{
				collision = true;
				shot1[3]=0;
			}
			if ((((shot2[0] >= l->x-size)&&(shot2[0] <= l->x+size)) && ((shot2[1] >= l->y-size)&&(shot2[1] <= l->y+size))&&(shot2[3]==1)))
			{
				collision = true;
				shot2[3]=0;
			}
			if ((((shot3[0] >= l->x-size)&&(shot3[0] <= l->x+size)) && ((shot3[1] >= l->y-size)&&(shot3[1] <= l->y+size))&&(shot3[3]==1)))
			{
				collision = true;
				shot3[3]=0;
			}
			if ((((shot4[0] >= l->x-size)&&(shot4[0] <= l->x+size)) && ((shot4[1] >= l->y-size)&&(shot4[1] <= l->y+size))&&(shot4[3]==1)))
			{
				collision = true;
				shot4[3]=0;
			}
			if(collision)
			{
				if(l->size == 2){
					l->size=1;
					asteroidSplitMode = true;
					asteroidCount --;
					
					newPosX = l->x;
					newPosY = l->y;
					
					if(firedCount >0){
						firedCount-=1;
					}
				}
				else if(l->size == 1)
				{
					l->active = false;
					asteroidSplitMode = false;
					if(firedCount > 0){
						firedCount-=1;
					}
				}
				
				collision = false;
				
			}

			//shield collision detection
			if(!shield && ((player.p.x >= l->x-30)&&(player.p.x <= l->x+30)) && ((player.p.y >= l->y-30)&&(player.p.y <= l->y+30)))
			{
				
				lives--;
				shield = true;
				player.p.x = 240;
				player.p.y = 136;
			}
			
			if(shield && ((player.p.x >= l->x-30)&&(player.p.x <= l->x+30)) && ((player.p.y >= l->y-30)&&(player.p.y <= l->y+30)))
			{
				shieldCollision = true;
				damage--;
			}
			else
			{
				shieldCollision = false;
			}
			
			//astroid speed
			if(l->size == 2)
			{
				
				l->x += 1*cos(radians(l->heading));
				l->y += 1*sin(radians(l->heading));
			}
			if(l->size == 1){
				
				l->x += 1.5*cos(radians(l->heading));
				l->y += 1.5*sin(radians(l->heading));
				
			}
			
			//content wrapping
			if(l->y > 300)
			{
				l->y = -30;					
			}
			if(l->y < -30)
			{
				l->y = 300;
			}
			if(l -> x < 0)
			{
				l->x = 500;
			}
			
			if(l -> x > 500)
			{
				l->x = 0;
			}				
			
		}
		
		//Bullets 
		if(l->type ==1){
			if(l->count == shot0[2])
			{
				if(shot0[3]==0)
				{
					l->active = false;
				}
				
			}
			if(l->count == shot1[2])
			{
				if(shot1[3]==0)
				{
					l->active = false;
				}
			}
			if(l->count == shot2[2])
			{
				if(shot2[3]==0)
				{
					l->active = false;
				}
			}
			if(l->count == shot3[2])
			{
				if(shot3[3]==0)
				{
					l->active = false;
				}
			}
			if(l->count == shot4[2])
			{
				if(shot4[3]==0)
				{
					l->active = false;
				}
			}
			
			//Bullet Movement
			
			l->x += 2*cos(radians(l->heading));
			l->y += 2*sin(radians(l->heading));
			l->distanceCount++;
			if(l->distanceCount == 300)
			{
				l->active = false;
				l->x = 3000;
				l->y = 3000;
			}
			
			//Bullet Wrapping
			
			if(l->x < 40)
			{
				l->x = 480;
			}
			else if(l->x > 480)
			{
				l->x = 40;
			}
			if(l->y < 0)
			{
				l->y = 270;
			}
			else if(l->y > 270)
			{
				l->y = 0;
			}
			
			
			//Bullet Position Tracking
			
			if(l->count == shot0[2])
			{
				shot0[0]=l->x;
				shot0[1]=l->y;
			}
			if(l->count == shot1[2])
			{
				shot1[0]=l->x;
				shot1[1]=l->y;
			}
			if(l->count == shot2[2])
			{
				shot2[0]=l->x;
				shot2[1]=l->y;
			}
			if(l->count == shot3[2])
			{
				shot3[0]=l->x;
				shot3[1]=l->y;
			}
			if(l->count == shot4[2])
			{
				shot4[0]=l->x;
				shot4[1]=l->y;
			}
			
			
			//Bullet/Ship Interaction
			if(shield && l->active){
				
				if(((player.p.x >= l->x-5)&&(player.p.x <= l->x+5)) && ((player.p.y >= l->y-5)&&(player.p.y <= l->y+5)))
				{
					damage-=10;
					l->x = 3000;
					l->y = 3000;
				}
				
			}
			else if(((player.p.x >= l->x-5)&&(player.p.x <= l->x+5)) && ((player.p.y >= l->y-5)&&(player.p.y <= l->y+5))&&l->active)
			{
				lives--;
				l->x = 3000;
				l->y = 3000;
				player.p.x = 240;
				player.p.y = 136;
				
			}
		}
		
		//shield reset
		if(damage < 1)
		{
			lives --;
			player.p.x = 240;
			player.p.y = 136;
			damage = 150;
			shield = true;
			
		}
	}
}

struct particle *active = NULL;


void particle_system(void)
{
	if((asteroidCount < 5)&&(totalAsteroids<10))
	{
        struct particle *spark = allocnode();
		
        if(spark) {
            spark->next = active;
            active = spark;
            strike(spark);
		}
	}
	
	if((fired == 1) && (firedCount < 5))
	{
		struct particle *spark = allocnode();
		
        if(spark) {
            spark->next = active;
            active = spark;
            strike(spark);	
		}
	}
	
	
	update(active);
}


///////////////////////////////////////////////////////////////
//									  	  SHIP CONTROLS                      //
///////////////////////////////////////////////////////////////

void shipRight(void)
{
	player.heading -=3;	
}

void shipLeft(void)
{
	player.heading +=3;	
}

void shipUp(void)
{
	newHeading = player.heading;
	player.p.x += acceleration*cos(radians(player.heading));
	player.p.y += acceleration*sin(radians(player.heading));
	
	if(acceleration <= 1.5)
	{					
		acceleration = acceleration+0.04;
	}
}

void shipDown(void)
{
	if(acceleration > 0.0)
	{
		acceleration = acceleration-0.004;
	}
}

void setFire()
{
	fired = 1;
}
