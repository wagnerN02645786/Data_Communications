#include "MKL25Z4.h"
void delayMs(int n);
//Global Variables
int data=0;
int seq;
//Preamble variables
int pstate=0;
int nstate=0;
//Position
int pos=0;
int pr=0;
int pre=0;
int p=0;
int dc=0;
int dcr=0;

//Used for CRC checking
int C0=0;
int C1=0;
int C2=0;
int C3=0;
int Y=0;
int I;

int main(void) {
	__disable_irq(); /* disable all IRQs */
	
	SIM->SCGC5=0x3D80;
	
	SIM->SCGC5 |= 0x400; /* enable clock to Port B */
	PORTB->PCR[18] = 0x100; /* make PTB18 pin as GPIO */
	PORTB->PCR[19] = 0x100; /* make PTB19 pin as GPIO */
	PTB->PDDR |= 0xC0000; /* make PTB18, 19 as output pin */
	PTB->PDOR |= 0xC0000; /* turn off LEDs */
	SIM->SCGC5 |= 0x200; /* enable clock to Port A */
	/* configure PTA1 for interrupt */
	PORTA->PCR[1] |= 0x00100; /* make it GPIO */
	//PORTA->PCR[1] |= 0x00003; /* enable pull-up */
	PTA->PDDR &= ~0x0002; /* make pin input */
	PORTA->PCR[1] &= ~0xF0000; /* clear interrupt selection */
	PORTA->PCR[1] |= 0xA0000; /* enable falling edge interrupt */
	// configure PTA2 for interrupt
	PORTA->PCR[2] |= 0x00100; /* make it GPIO */
	PORTA->PCR[2] |= 0x00003; /* enable pull-up */
	PTA->PDDR &= ~0x0004; /* make pin input */
	PORTA->PCR[2] &= ~0xF0000; /* clear interrupt selection */
	PORTA->PCR[2] |= 0xA0000; /* enable falling edge interrupt */
	NVIC->ISER[0] |= 0x40000000; /* enable INT30 (bit 30 of ISER[0]) */
	__enable_irq(); /* global enable IRQs */
	
	
	PORTE->PCR[0]=0x100;//DATA
	PTE->PDDR=0xFFFFFF00;
	
	//Turning on the LED's
	PORTC->PCR[0]=0x100;//Display on LED board
	PORTC->PCR[1]=0x100;
	PORTC->PCR[2]=0x100;
	PORTC->PCR[3]=0x100;
	PORTC->PCR[4]=0x100;//Display on LED board
	PORTC->PCR[5]=0x100;
	PORTC->PCR[6]=0x100;
	PORTC->PCR[7]=0x100;
	PTC->PDDR=0xFFFFFFFF;

	PORTB->PCR[0]=0x100;//3different LED
	PORTB->PCR[1]=0x100;
	PORTB->PCR[2]=0x100;
	PTB->PDDR=0xFFFFFFFF;
	
	PORTD->PCR[0]=0x100;//Port D is used for the oscilloscope
	PTD->PDDR=0xFFFFFFFF;
	
	/* toggle the red LED continuously */
	while(1) {
	//	PTB->PTOR |= 0x40000; /* toggle red LED */
		//delayMs(100);
		
		
		;
	}
}

/* A pushbutton switch is connecting either PTA1 or PTA2 to ground to trigger PORTA
interrupt */
void PORTA_IRQHandler(void) {
	int i=100;
		PTD->PDOR=0x1;//oscilloscope D0 represent time period
	//We are recieving info
	//Red light represents if in the interrupt or not
			PTB->PDOR &= ~0x40000; /* turn on red 0x40000 LED */
			delayMs(i);
			PTB->PDOR |= 0x40000; /* turn off red 0x40000 LED */
			delayMs(i);
			if((PTE->PDIR&&0x1)==0x1)//Input to check if data is high or low
			{
				seq=1;
			}
			else
			{
				seq=0;
			}
			
			//Preamble
			if (pre==0)
			{
				PTB->PDOR=0x1;			//PortB[0] (Red LED) represents Preamble
				switch(pstate)			//Sequence diagram for 0x89
				{
					case 0: {if(seq==0) nstate=0; else nstate=1; break;}
					case 1: {if(seq==0) nstate=2; else nstate=1; break;}
					case 2: {if(seq==0) nstate=3; else nstate=1; break;}
					case 3: {if(seq==0) nstate=4; else nstate=1; break;}
					case 4: {if(seq==0) nstate=0; else nstate=5; break;}
					case 5: {if(seq==0) nstate=6; else nstate=1; break;}
					case 6: {if(seq==0) nstate=7; else nstate=1; break;}
					case 7: {if(seq==0) nstate=4; else nstate=1; break;}
				}
				PTC->PDOR=nstate; 				
			
				if(pstate==7) //At the end of the sequence diagram display 0xFF and reset LEDs
				{
					PTC->PDOR=0xFF; 
					delayMs(100); 
					PTC->PDOR=0x0;
					
					pre=1;					// We are done with preamble		
				}
				pstate=nstate;		//Save next state for present state
			}
			
			//Addressing
			if(pr==1)
			{
				PTB->PDOR=0x2;		//PortB[1] (Blue LED) represents Addressing
				data=data*2+seq; 	//Storing address through a shift
				PTC->PDOR=data; 
				delayMs(100);
				pos++;
				//0xA1---Bit Cast Addressing
				//0xFF---Broad Cast Addressing
				//0xAF---Multi Cast Addressing
				if((pos==8)&&((data==0xA1)||(data==0xFF)||((data&0xFF)==0xAF)))
				{
					dc=1;
					PTC->PDOR=0x0;
					pr=0;
					p=1;
					data=0;
					pos=0;
				}
				if(pos>8)	//Didn't retrieve address start over
				{
					PTD->PDOR=0x0;
					//ZEROED OUT
					data=0;
					pstate=0;
					nstate=0;
					pos=0;
					pr=0;
					pre=0;
					p=0;
					dc=0;
					dcr=0;

					C0=0;
					C1=0;
					C2=0;
					C3=0;
					Y=0;					
				}
			}
					
			//Data and CRC
			if(dcr==1)
			{
				PTB->PDOR=0x4;		//PortB[2] (Green LED) represents Data & CRC
				data=data*2+seq;	//Storing data through a shift
				PTC->PDOR=data;		//Display data values
				
				//CRC check
				Y=C3 ^ seq;
				C3=C2;
				C2=C1;
				C1=Y ^ C0;
				C0=Y;	
				
				//Pos 11 is the end of data and one before end of CRC
				if((pos==11)&&(C3==0)&&(C2==0)&&(C1==0)&&(C0==0))
				{
					PTB->PDOR=0x7;	//Red, Blue, and Green LED's on represent Good Data received
					PTC->PDOR=0x0;
					delayMs(50);
					data=data/16;		//Retreive actual data
					PTC->PDOR=data; //Display 8-bit data
					delayMs(300); 
					
					//ZEROED OUT
					data=0;
					pstate=0;
					nstate=0;
					pos=0;			//Position is set back to zero
					pr=0;
					pre=0;
					p=0;
					dc=0;
					dcr=0;
					
					seq=0;		//Need to reset the seq.

					C0=0;
					C1=0;
					C2=0;
					C3=0;
					Y=0;
					delayMs(100);
					PTC->PDOR=0x0;
				}
				else			//If data hasn't finished
				{
					pos=pos+1;
					//If the position is to large and CRC didn't check start over
					if((pos>=12)&&((C3!=0)||(C2!=0)||(C1!=0)||(C0!=0)))
					{
						PTD->PDOR=0x0;
						PTC->PDOR=0x0;
						//ZEROED OUT
						data=0;
						pstate=0;
						nstate=0;
						pos=0;			//Position is set back to zero
						pr=0;
						pre=0;
						p=0;
						dc=0;
						dcr=0;
						
						seq=0;		//Need to reset the seq.

						C0=0;
						C1=0;
						C2=0;
						C3=0;
						Y=0;
					}
				}	
			}
			
			if((p==0)&&(pre==1))// Doing addressing
				{ 
					pr=1;
				}		
			if(dc==1)		//Doing data 
				{
					dcr=1;
				}
	
	PORTA->ISFR = 0x00000006; /* clear interrupt flag */
}

/* Delay n milliseconds
* The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit().
*/
void delayMs(int n) {
	
	int i;
	int j;
	n=n/5;
	for(i = 0 ; i < n; i++)
	for (j = 0; j < 7000; j++) {}

}