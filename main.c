#include <avr/io.h>
#include <util/delay.h>

#define DDR_SEG DDRB

#define DIG0_PORT PORTD
#define PORT_SEG PORTB
#define DIG0_DDR DDRD
#define DDR_LED DDRC
#define DDR_RES DDRC
#define PORT_RES PORTC

#define DIG0_NO 0
#define DIG1_NO 1
#define DIG2_NO 2
#define DIG3_NO 3

void ssd(void);
void init(void);
void decode(void);
void blink_led(void);
void ohm_range(void);
void kilo_range(void);
void tenkilo_range(void);

unsigned char A[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char digit[4];
unsigned int res_val,res_val2;
float ang_val;
unsigned int resister =1000;
int main(void){
	unsigned int dig_val;
	init();
	while(1){	
		ADCSRA = ADCSRA|(1<<ADSC); //start convertion
		while((ADCSRA & (1<<ADSC))==0);//wait for finish
		
		dig_val = ADCL|ADCH<<8;
		ang_val = (5*dig_val)/1.023;
		if(resister==100){
			ohm_range();
		}else if(resister==1000){
			kilo_range();
		}else if(resister==10000){
			tenkilo_range();
		}
		for(unsigned int i=0;i<20;i++){
			ssd();
			blink_led();
		}
	}
    return 0;
}
void blink_led(void){
	if((res_val>=1)&(res_val<1000)){
		PORTC = (1<<5);
		resister=100;
		
	}else if((res_val>=1000)&(res_val<10000)){
		PORTC = (1<<4);
		resister=1000;	

	}else if((res_val>=10000)&(res_val<100000)){
		PORTC = (1<<3);
		resister=10000;
	}
}
void decode(void){
	digit[0]=res_val2/1000;		
	digit[1]=(res_val2%1000)/100;
	digit[2]=(res_val2%100)/10;
	digit[3]=res_val2%10;
}
void ssd(void){

	decode();

	PORT_SEG = A[digit[3]];
	DIG0_PORT = DIG0_PORT | (1<<DIG3_NO);
	_delay_ms(0.1);
	DIG0_PORT = DIG0_PORT & ~(1<<DIG3_NO);
		
	PORT_SEG = A[digit[2]];
	DIG0_PORT = DIG0_PORT | (1<<DIG2_NO);
	_delay_ms(0.1);
	DIG0_PORT = DIG0_PORT & ~(1<<DIG2_NO);
	
	PORT_SEG = A[digit[1]];
	DIG0_PORT = DIG0_PORT | (1<<DIG1_NO);
	_delay_ms(0.1);
	DIG0_PORT = DIG0_PORT & ~(1<<DIG1_NO);
	
	PORT_SEG = A[digit[0]];
	DIG0_PORT = DIG0_PORT | (1<<DIG0_NO);
	_delay_ms(0.1);
	DIG0_PORT = DIG0_PORT & ~(1<<DIG0_NO);

	PORT_SEG = 0x80;
	DIG0_PORT = DIG0_PORT | (1<<DIG0_NO);
	_delay_ms(0.1);
	DIG0_PORT = DIG0_PORT & ~(1<<DIG0_NO);	
	
}
void init(void){
	DDR_SEG = 0xff;//make as output
	DIG0_DDR = DIG0_DDR | 1<<0 | 1<<1 |1<<2 |1<<3;
	DDR_LED = DDR_LED |1<<3| 1<<4| 1<<5;
	
	ADCSRA =1<<ADEN|1<<ADPS1|1<<ADPS0;
	ADMUX = 1<<REFS0|1<<MUX1;
}
void ohm_range(void){
	DDR_RES = DDR_RES |1<<0;
	PORT_RES = PORT_RES & ~(1<<0);
	DDR_RES = DDR_RES & ~(1<<1);
	DDR_RES = DDR_RES & ~(1<<2);
	res_val = (5000-ang_val)*100/(ang_val);
	res_val2=res_val;
	
}
void kilo_range(void){
	DDR_RES= DDR_RES |1<<1;
	PORT_RES = PORT_RES & ~(1<<1);
	DDR_RES = DDR_RES & ~(1<<0);
	DDR_RES = DDR_RES & ~(1<<2);
	res_val = (5000-ang_val)*1000/(ang_val);
	res_val2=res_val;
}
void tenkilo_range(void){
	DDR_RES = DDR_RES |1<<2;
	PORT_RES = PORT_RES & ~(1<<2);
	DDR_RES = DDR_RES & ~(1<<1);
	DDR_RES = DDR_RES & ~(1<<0);
	res_val = (5000-ang_val)*10000/(ang_val);
	res_val2=res_val/10;
}
