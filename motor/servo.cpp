#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main (void)
{
	int bright ;
    int x ;
    


	if (wiringPiSetup () == -1)
	exit (1) ;

	pinMode (26, PWM_OUTPUT) ;
for(;;)
{
    pwmWrite(26,LOW);
    scanf("%d",&x);
    printf("x = %d",x);
	if (x ==1 )
	{
	for (bright = 0 ; bright < 1024 ; ++bright)
	{
		pwmWrite (26, bright) ;
		delay (1) ;
	}
     delay (1000) ;
	for (bright = 1023 ; bright >= 0 ; --bright)
	{
		pwmWrite (26, bright) ;
		delay (1) ;
	}
	}
    
}
return 0;
	
}