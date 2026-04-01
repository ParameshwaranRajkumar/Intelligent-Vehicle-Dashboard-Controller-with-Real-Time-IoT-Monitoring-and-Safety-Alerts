// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#define _XTAL_FREQ 4000000
#include<xc.h>
    char high=0,low=0,speed_high=0,speed_low=0;
    int count=0,value=0,fuel_per=0,sel=0,data_new=0,speed=0,door=0,seat=0,bat_per=0;
void delay(unsigned int a)
{
    while(a--);
}
 void enable()
{
    RE1=1;
    __delay_ms(5);
    RE1=0;
    __delay_ms(5);
}
 void tx(unsigned char tx_data)
 {
     TXREG=tx_data;
     while(TXIF==0);
     TXIF=0;
     //__delay_ms(500);
 }
 void seperate(int raw)
 {
     tx(((raw/100)%10)+48);
     tx(((raw/10)%10)+48);
     tx((raw%10)+48);
 }
void lcd(char ins, char data)
{
    RE0=ins;
    PORTD=data;
    enable();
}
void string(char *ptr)
{
    while(*ptr!='\0')
    {
        lcd(1,*ptr++);
    }
}
    
void slave()
{
    unsigned char data=0;
    while(SSPIF==0);
    data=SSPBUF;
    SSPIF=0;
    switch(sel)
    {
        case 0:
            sel=1;
             high=data;
            break;
        case 1:   
            sel=2;
            low=data;
            break;
        case 2:   
            sel=3;
            fuel_per=data;
            break;
        case 3:
            sel=4;
            speed_high=data;
            break;
        case 4:   
            sel=5;
            speed_low=data;
            value=(high<<8)|low;
            speed=(speed_high<<8)|speed_low;
            break;
        case 5:
            sel=6;
            door=data;
            break;
        case 6:
            sel=7;
            seat=data;
            break;
        case 7:
            sel=0;
            bat_per=data;
            data_new=1;
            break;
    }
}

void uart()
{
       seperate(value);
       tx(',');
       seperate(fuel_per);
       tx(',');
       seperate(bat_per);
       tx(',');
       seperate(speed);
       tx('\n');
}
void main()
{
    PORTA=PORTC=PORTD=PORTE=0X00;
    TRISA=0X20;
    TRISC=0X98;
    TRISE=TRISD=0X00;
    ANSEL=ANSELH=0X00;
    SSPCON=0X24;
    SSPSTAT=0X80;
    TXSTA=0X26;
    RCSTA=0X90;
    SPBRG=25;
    lcd(0,0x38);
    lcd(0,0x0C);
    lcd(0,0x80);
    while(1)
    {
        slave();
      
        if(data_new)
        {
            data_new=0;
            
            
            
         lcd(0,0x80);
        lcd(1,(value/100)+48);
        lcd(1,((value/10)%10)+48);
        lcd(1,(value%10)+48);
        lcd(1,223);
        lcd(1,'C');
       // fuel
        
         lcd(0,0xC0);
        lcd(1,(fuel_per/100)+48);
        lcd(1,((fuel_per/10)%10)+48);
        lcd(1,(fuel_per%10)+48);
        lcd(1,'%');
        //battery
        lcd(0,0x8B);
        lcd(1,(bat_per/100)+48);
        lcd(1,((bat_per/10)%10)+48);
        lcd(1,(bat_per%10)+48);
        lcd(1,'%');
        
        
        lcd(0,0xC6);
        lcd(1,(speed/100)+48);
        lcd(1,((speed/10)%10)+48);
        lcd(1,(speed%10)+48);
        string("km");
        lcd(1,'/');
        lcd(1,'h');
        
        if(seat==0)
            {
                RA1=1;
                lcd(0,0x88);
                lcd(1,'/');
                lcd(1,'D');
            }
            else
            {
                RA1=0;
                lcd(0,0x88);
                lcd(1,'/');
                lcd(1,'N');
            }
        
        if(door==0)
        {
        
            RA0=1;
            lcd(0,0x86);
            string("op");
            
        }
        else
        {
            RA0=0;
            lcd(0,0x86);
            string("cl"); 
        }
        if(value>=100)
        {
            RA2=1;
        }
        else
        {
            RA2=0;
        }
        if(fuel_per<=20)
        {
            RA3=1;
        }
        else
        {
            RA3=0;
        }
       uart();
       
    }
       
}
}