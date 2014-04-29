/**
  ******************************************************************************
  * @file: 		gMath.c	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   cac phep toan.
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 
/* include */
#include "gMath.h"
/* define */

/* variable */

/* funtion */
float sinFast(float rad){
	float Sin;
	//always wrap input angle to -PI..PI
	if (rad < -3.14159265)
			rad += 6.28318531;
	else
	if (rad >  3.14159265)
			rad -= 6.28318531;
	
	if (rad < 0)
	{
			Sin = 1.27323954 * rad + .405284735 * rad * rad;
			
			if (Sin < 0)
					Sin = .225 * (Sin *-Sin - Sin) + Sin;
			else
					Sin = .225 * (Sin * Sin - Sin) + Sin;
	}
	else
	{
			Sin = 1.27323954 * rad - 0.405284735 * rad * rad;
			
			if (Sin < 0)
					Sin = .225 * (Sin *-Sin - Sin) + Sin;
			else
					Sin = .225 * (Sin * Sin - Sin) + Sin;
	}
	return Sin;
}

float cosFast(float rad){
	float Cos;
	rad += 1.57079632;
	//always wrap input angle to -PI/2..PI/2
	if (rad < -3.14159265)
			rad += 6.28318531;
	else
	if (rad >  3.14159265)
			rad -= 6.28318531;

	if (rad < 0)
	{
			Cos = 1.27323954 * rad + 0.405284735 * rad * rad;
			
			if (Cos < 0)
					Cos = .225 * (Cos *-Cos - Cos) + Cos;
			else
					Cos = .225 * (Cos * Cos - Cos) + Cos;
	}
	else
	{
			Cos = 1.27323954 * rad - 0.405284735 * rad * rad;

			if (Cos < 0)
					Cos = .225 * (Cos *-Cos - Cos) + Cos;
			else
					Cos = .225 * (Cos * Cos - Cos) + Cos;
	}
	
	return Cos;
}

float tanFast(float rad){
	return sinFast(rad)/cosFast(rad);
}

float ultraFastAtan2(float y, float x)
{
  float angle; 
  float coeff_1 = PI/4;
   float coeff_2 = 3*coeff_1;
   float abs_y = fabs(y)+1e-10 ;     // kludge to prevent 0/0 condition
   if (x>=0)
   {
      float r = (x - abs_y) / (x + abs_y);
      angle = coeff_1 - coeff_1 * r;
   }
   else
   {
      float r = (x + abs_y) / (abs_y - x);
      angle = coeff_2 - coeff_1 * r;
   }
   if (y < 0)
   return(-angle* (180.0f / PI));     // negate if in quad III or IV
   else
   return(angle* (180.0f / PI));
}

float absFast(float a){
	if(a < 0) return -a;
	return a;
}

float sqrtFast(float m)
{
   float i=0;
	 int j=0;
   float x1,x2;
   while( (i*i) <= m )
          i+=0.1f;
   x1=i;
   for(j=0;j<10;j++)
   {
       x2=m;
      x2/=x1;
      x2+=x1;
      x2/=2;
      x1=x2;
   }
   return x2;
}

double sqrt9(double Nbr)
{
	double Number = Nbr / 2;	
	const double Tolerance = 1.0e-7;	
	do
	{
		Number = (Number + Nbr / Number) / 2;
	}while( absFast(Number * Number - Nbr) > Tolerance);
	
	return Number;
}

// float sinT(float anpha){
// 	float fract = 0;
// 	int i = 0;
// 	float y0,y1;
// 	
// 	/* lay so sau dau phay*/
// 	if(anpha > 0){
// 		fract = anpha - (float)((int)(anpha));
// 	}
// 	else{
// 		fract = anpha - (float)((int)(anpha)) + 1;
// 	}
// 	/* chuyen vi goc*/
// 	i = (int)(anpha + 179);
// 	/* limit value angle */
// 	if(i < 0) i = 0;
// 	else if(i >= 359)	i = 359;
// 	/* reding value sin of sintable */
// 	y0 = sinTable[i];
// 	y1 = sinTable[i+1];
// 	/* tinh toan sin(anpha) */
// 	y1 -= y0;
// 	y1 *= fract;
// 	return (y0+y1);
// }

// float cosT(float anpha){
// 	float fract = 0;
// 	int i = 0;
// 	float y0,y1;
// 	
// 	/* lay so sau dau phay*/
// 	if(anpha > 0){
// 		fract = anpha - (float)((int)(anpha));
// 	}
// 	else{
// 		fract = anpha - (float)((int)(anpha)) + 1;
// 	}
// 	/* chuyen vi goc*/
// 	i = (int)(anpha + 179);
// 	/* limit value angle */
// 	if(i < 0) i = 0;
// 	else if(i >= 359)	i = 359;
// 	/* reding value sin of sintable */
// 	y0 = cosTable[i];
// 	y1 = cosTable[i+1];
// 	/* tinh toan sin(anpha) */
// 	y1 -= y0;
// 	y1 *= fract;
// 	return (y0+y1);
// }

// float tanT(float anpha){
// 	return (sinT(anpha)/cosT(anpha));
// }
