#include <wiringPi.h>
#include <stdio.h>
int LED = 0;			/* GPIO26 as per wiringPi, GPIO12 as per BCM, pin no.32 */
int main(){
	wiringPiSetup();	/* initialize wiringPi setup */
	pinMode(LED,OUTPUT);	/* set GPIO as output */
	int x;

	while (1){
		digitalWrite(LED,LOW);
		scanf("%d",&x);
		printf("x = %d",x);
	    if (x ==1 )
	{
		digitalWrite(LED,HIGH);		/* write high on GPIO */
		delay(1000);
		digitalWrite(LED, LOW);		/* write low on GPIO */
		delay(1000);
		}
	}
}
//run progrem
//gcc -o led led.cpp -l wiringPi
//sudo ./led

