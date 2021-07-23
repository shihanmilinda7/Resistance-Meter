# Resistance-Meter
This was programed with C programming. This can be use for measure unknow resister value. There 3 ranges which are Ohm range, kOhm range and 10kOhm range.  This was program for ATMega 32A microcontroller. Here I used inbuild ADC converter of microcontroller. 
# pin definition
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
# Basic inits
void init(void){
	DDR_SEG = 0xff;//make as output
	DIG0_DDR = DIG0_DDR | 1<<0 | 1<<1 |1<<2 |1<<3;
	DDR_LED = DDR_LED |1<<3| 1<<4| 1<<5;
	
	ADCSRA =1<<ADEN|1<<ADPS1|1<<ADPS0;
	ADMUX = 1<<REFS0|1<<MUX1;
}
# ADC conversion
ADCSRA = ADCSRA|(1<<ADSC); //start convertion
		while((ADCSRA & (1<<ADSC))==0);//wait for finish
		
		dig_val = ADCL|ADCH<<8;
		ang_val = (5*dig_val)/1.023;

# Range selection
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
## 4 digit init
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
