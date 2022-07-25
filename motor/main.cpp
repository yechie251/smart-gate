#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void led();
void servo();



int x ;
int LED = 0;
int motor =26;


int main()
{
    wiringPiSetup();
     pinMode (motor, PWM_OUTPUT) ;
     pinMode(LED,OUTPUT);

if (wiringPiSetup () == -1)
	exit (1) ;

for(;;)
{
    pwmWrite(motor,LOW);
    scanf("%d",&x);
    printf("x = %d\n",x);
    if (x ==1 )
	{
         servo();
         led();
    }
}
    return 0;

}

void servo()
{
	int bright ;
	for (bright = 0 ; bright < 1024 ; ++bright)
	{
		pwmWrite (motor, bright) ;
		delay (1) ;
	}
     delay (1000) ;
	for (bright = 1023 ; bright >= 0 ; --bright)
	{
		pwmWrite (motor, bright) ;
		delay (1) ;
	}
	
}
			
void led(){	
	    
	{
		digitalWrite(LED,HIGH);		
		delay(2000);
		digitalWrite(LED, LOW);		
		}
}
//run progrem
//gcc -o led led.cpp -l wiringPi
//sudo ./led

