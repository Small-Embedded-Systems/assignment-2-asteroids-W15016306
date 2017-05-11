/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"
#include "view.h"
#include "node.h"
#include "utils.h"

/* Joystick 5-way switch
*/
//joystick input
DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3};
enum position { left,down,right,up,centre };


bool pressed = false;

void controls(void)
{
	
	///////////////////////////////////////////////////////////////
	//						  			       LEFT                            //
	///////////////////////////////////////////////////////////////
	
	if(joystick[left].read() == 0)
	{
		shipLeft();
	}	
	
	///////////////////////////////////////////////////////////////
	//						  			      RIGHT                            //
	///////////////////////////////////////////////////////////////
	
	if(joystick[right].read() == 0)
	{
		shipRight();
	}	
	
	///////////////////////////////////////////////////////////////
	//						  			         UP                            //
	///////////////////////////////////////////////////////////////
	
	if(joystick[up].read() == 0)
	{
		shipUp();
	}	
	
	///////////////////////////////////////////////////////////////
	//						  			        DOWN                           //
	///////////////////////////////////////////////////////////////
	
	if(joystick[down].read() == 0)
	{
		shipDown();
	}	
	
	///////////////////////////////////////////////////////////////
	//						  			     	CENTRE                           //
	///////////////////////////////////////////////////////////////
	
	//incorporates press check
	if((joystick[centre].read() == 0))
	{
		pressed = true;
	}
	if(!(joystick[centre].read() == 0)&& (pressed))
	{
		pressed = false;
		setFire();
	}
}