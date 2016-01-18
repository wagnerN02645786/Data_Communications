#include <MKL25Z4.H>

void delay(void);
void Delay(void);//Don't need this function "Not Used"
void crc(int DATA);
int cr[4];
void send(int PRE, int Address, int DATA);

int trial(int A);
int main (void) 
{
	int num=0;
	
	//Diplay what data point you are at
	num=trial(num);

	//Call method send with the parameters Preamble, Address, Data
	send(0x89, 0xBF, 0xD5); //Tansmit to different processor : Bit Cast Addressing Processor B
		num=trial(num);
	send(0x89, 0xAF, 0xD5); //Good inputs : MultiCast Addressing Processor's A
		num=trial(num);
	send(0x89, 0xB1, 0xD5); //Tansmit to different processor : Bit Cast Addressing Processor B
		num=trial(num);
	send(0x89, 0xA7, 0xD5); //Tansmit to different processor : Bit Cast Addressing Processor  A
		num=trial(num);
	send(0x89, 0xA1, 0x89);	//Error with Preamble : Bit Cast Addressing Processor  A
		num=trial(num);
	send(0x89, 0xF0, 0xD5);	//Tansmit to different processor : Bit Cast Addressing Processor F
		num=trial(num);
	send(0x89, 0xFF, 0xFF);	//Good inputs : Broad Cast Addressing
		num=trial(num);
}
//Trial gets inputed a number, Adds 1 and displays the number in PortD
//This is to show what number of data you are transmitting
int trial(int A)
{
	SIM->SCGC5=0x3D80;
	PORTC->PCR[0]=0x100;//Display on LED lights
	PORTC->PCR[1]=0x100;
	PORTC->PCR[2]=0x100;
	PORTC->PCR[3]=0x100;
	PTC->PDDR=0xFFFFFFFF;
	
	A++;
	PTC->PDOR=A;
	return A;
}

//Input the Preamble Address and Data
void send(int PRE, int Address, int DATA)
{ 
	//Preamble inputs
		int a=0;
		int b=0;
		int c=0;
		int d=0;
		int e=0;
		int f=0;
		int g=0;
		int h=0;
	//Data Inputs
		int i=0;
		int j=0;
		int k=0;
		int l=0;
		int m=0;
		int n=0;
		int o=0;
		int p=0;
	//Address inputs
		int A=0;
		int B=0;
		int C=0;
		int D=0;
		int E=0;
		int F=0;
		int G=0;
		int H=0;
	//Set Preamble
		if ((PRE&0x80)==0x80) a=0x1; 
		if ((PRE&0x40)==0x40) b=0x1;
		if ((PRE&0x20)==0x20) c=0x1;
		if ((PRE&0x10)==0x10) d=0x1;
		if ((PRE&0x08)==0x8) e=0x1;
		if ((PRE&0x04)==0x4) f=0x1;
		if ((PRE&0x02)==0x2) g=0x1;
		if ((PRE&0x01)==0x1) h=0x1;
	//Set Addressing
		if ((Address&0x80)==0x80) A=0x1; 
		if ((Address&0x40)==0x40)B=0x1;
		if ((Address&0x20)==0x20)C=0x1;
		if ((Address&0x10)==0x10) D=0x1;
		if ((Address&0x08)==0x8) E=0x1;
		if ((Address&0x04)==0x4) F=0x1;
		if ((Address&0x02)==0x2) G=0x1;
		if ((Address&0x01)==0x1) H=0x1;
		
	//Calculate the CRC
		crc(DATA);
		
	//Set Data
		if ((DATA&0x80)==0x80) i=0x1;
		if ((DATA&0x40)==0x40) j=0x1;
		if ((DATA&0x20)==0x20) k=0x1;
		if ((DATA&0x10)==0x10) l=0x1;
		if ((DATA&0x08)==0x8) m=0x1;
		if ((DATA&0x04)==0x4) n=0x1;
		if ((DATA&0x02)==0x2) o=0x1;
		if ((DATA&0x01)==0x1) p=0x1;
	
	SIM->SCGC5=0x3D80;
	
	PORTE->PCR[0]=0x100;	//DATA Port
	PORTE->PCR[1]=0x100;	//CLK Port
	PTE->PDDR=0xFFFFFFFF;
	PTE->PDOR=0x00;
	
	//Preamble Clock
		PTE->PDOR=0x2|(0x1&a); delay();
		PTE->PDOR=0x0|(0x1&a); delay();
		PTE->PDOR=0x2|(0x1&b); delay();
		PTE->PDOR=0x0|(0x1&b); delay();
		PTE->PDOR=0x2|(0x1&c);delay();
		PTE->PDOR=0x0|(0x1&c); delay();
		PTE->PDOR=0x2|(0x1&d);delay();
		PTE->PDOR=0x0|(0x1&d); delay();
		PTE->PDOR=0x2|(0x1&e);delay();
		PTE->PDOR=0x0|(0x1&e); delay();
		PTE->PDOR=0x2|(0x1&f);delay();
		PTE->PDOR=0x0|(0x1&f); delay();
		PTE->PDOR=0x2|(0x1&g);delay();
		PTE->PDOR=0x0|(0x1&g); delay();
		PTE->PDOR=0x2|(0x1&h);delay();
		PTE->PDOR=0x0|(0x1&h); delay();
		
	//Address Clock
		PTE->PDOR=0x2|(0x1&A); delay();
		PTE->PDOR=0x0|(0x1&A); delay();
		PTE->PDOR=0x2|(0x1&B); delay();
		PTE->PDOR=0x0|(0x1&B); delay();
		PTE->PDOR=0x2|(0x1&C);delay();
		PTE->PDOR=0x0|(0x1&C); delay();
		PTE->PDOR=0x2|(0x1&D);delay();
		PTE->PDOR=0x0|(0x1&D); delay();
		PTE->PDOR=0x2|(0x1&E);delay();
		PTE->PDOR=0x0|(0x1&E); delay();
		PTE->PDOR=0x2|(0x1&F);delay();
		PTE->PDOR=0x0|(0x1&F); delay();
		PTE->PDOR=0x2|(0x1&G);delay();
		PTE->PDOR=0x0|(0x1&G); delay();
		PTE->PDOR=0x2|(0x1&H);delay();
		PTE->PDOR=0x0|(0x1&H); delay();
		
	//Data Clock
		PTE->PDOR=0x2|(0x1&i);delay();
		PTE->PDOR=0x0|(0x1&i); delay();
		PTE->PDOR=0x2|(0x1&j);delay();
		PTE->PDOR=0x0|(0x1&j); delay();
		PTE->PDOR=0x2|(0x1&k);delay();
		PTE->PDOR=0x0|(0x1&k); delay();
		PTE->PDOR=0x2|(0x1&l);delay();
		PTE->PDOR=0x0|(0x1&l); delay();
		PTE->PDOR=0x2|(0x1&m);delay();
		PTE->PDOR=0x0|(0x1&m); delay();
		PTE->PDOR=0x2|(0x1&n);delay();
		PTE->PDOR=0x0|(0x1&n); delay();
		PTE->PDOR=0x2|(0x1&o);delay();
		PTE->PDOR=0x0|(0x1&o); delay();
		PTE->PDOR=0x2|(0x1&p);delay();
		PTE->PDOR=0x0|(0x1&p); delay();
		
	//CRC clock
		PTE->PDOR=0x2|(0x1&cr[3]);delay();
		PTE->PDOR=0x0|(0x1&cr[3]); delay();
		PTE->PDOR=0x2|(0x1&cr[2]);delay();
		PTE->PDOR=0x0|(0x1&cr[2]); delay();
		PTE->PDOR=0x2|(0x1&cr[1]);delay();
		PTE->PDOR=0x0|(0x1&cr[1]); delay();
		PTE->PDOR=0x2|(0x1&cr[0]);delay();
		PTE->PDOR=0x0|(0x1&cr[0]); delay();
		
		PTE->PDOR=0x0;
		Delay();
}

void delay(void)		//0.7 Second delay for each bit
{
	int n=700;
	int i;
	int j;
	n=n/5;
	for(i = 0 ; i < n; i++)
	for (j = 0; j < 7000; j++) {}
}

	void Delay(void)	//2 Second delay for end of process
{
	int n=2000;
	int i;
	int j;
	n=n/5;
	for(i = 0 ; i < n; i++)
	for (j = 0; j < 7000; j++) {}
}

void crc(int DATA)		//Calculating CRC =X^4+X^1+1
{
	int C0=0;
	int C1=0;
	int C2=0;
	int C3=0;
	int Y=0;
	int I;
	int Z[8];
	
	int i=0;
	int j=0;
	int k=0;
	int l=0;
	int m=0;
	int n=0;
	int o=0;
	int p=0;
	
	//Set data for CRC
	if ((DATA&0x80)==0x80) i=0x1;
	if ((DATA&0x40)==0x40) j=0x1;
	if ((DATA&0x20)==0x20) k=0x1;
	if ((DATA&0x10)==0x10) l=0x1;
	if ((DATA&0x08)==0x8) m=0x1;
	if ((DATA&0x04)==0x4) n=0x1;
	if ((DATA&0x02)==0x2) o=0x1;
	if ((DATA&0x01)==0x1) p=0x1;
	
	//Saving the inputs, data to an array
	Z[0]=i;
	Z[1]=j;
	Z[2]=k;
	Z[3]=l;
	Z[4]=m;
	Z[5]=n;
	Z[6]=o;
	Z[7]=p;
	
	//Caculate CRC through XOR
	for(I=0; I<8;I++)
	{
		
		Y=C3 ^ Z[I];
		C3=C2;
		C2=C1;
		C1=Y ^ C0;
		C0=Y;			
	}
	//Save CRC to Global variables
	cr[0]=C0;
	cr[1]=C1;
	cr[2]=C2;
	cr[3]=C3;
}