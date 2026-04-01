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
int total_1=0,vol=0,temp=0,fuel_per=0,count=0,pulse=0,speed=0,door=0,seat=0,bat_per=0,total_4=0,def_temp=25,ac_temp=0,ac_vol=0;
int seg[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
long int total_2=0,total_3=0;
void delay(unsigned a)
{
    while(a--);
}
void enable()
{
    RE1=1;
    __delay_ms(10);
    RE1=0;
    __delay_ms(10);
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
void ac()
{
    ADCON0=0X89;
    GO=1;
    while(GO);
    total_4=(ADRESH<<8)|(ADRESL);
    
}
void eng_temp()
{
    ADCON0=0X85;
    GO=1;
    while(GO);
    total_1=(ADRESH<<8)|(ADRESL);
    
}
void fuel()
{
    ADCON0=0X81;
    GO=1;
    while(GO);
    total_2=(ADRESH<<8)|(ADRESL);
    
}
void battery()
{
    ADCON0=0X91;
    GO=1;
    while(GO);
    total_3=(ADRESH<<8)|(ADRESL);
    
}
void master(char data_1)
{
    SSPBUF=data_1;
    while(SSPIF==0);
    SSPIF=0;
    delay(5000);
}
void trs(char *ptr)
{
    while(*ptr!='\0')
    {
        master(*ptr++);
    }
}
void data_pack()
{
   // master(0xAA);
    master(temp>>8);
    master(temp&0xFF);
    master(fuel_per);
    master(speed>>8);
    master(speed&0xFF);
    master(door);
    master(seat);
    master(bat_per);

}
void __interrupt() isr()
{
    if(INTF==1)
    {
        pulse++;
        INTF=0;
    }
    
    if(T0IF==1)
    {
    TMR0=131;
    count++;
    T0IF=0;
    }
}
void main()
{
    PORTA=0x00;PORTB=0x00;
    PORTD=0x00;
    PORTC=0x00;
    PORTE=0X00;
    TRISD=0x00;
    TRISE=0X00;
    TRISA=0X37;
    TRISB=0XFF;
    TRISC=0XD3;
    ANSEL=0X17;
    ANSELH=0X00;
    SSPCON=0X20;
    SSPSTAT=0X80;
    ADCON1=0X90;
    OPTION_REG=0X85;
    INTCON=0XB0;
    TMR0=131;
INTE = 1;
INTEDG = 1;
lcd(0,0x38);
lcd(0,0x0C);
lcd(0,0x01);


    while(1)
    {
        ac();
        ac_vol=(int)((float)total_4*4.88758);
        ac_temp=ac_vol/10;
        if(RC0==0 && RC1==0)
        {
            RC2=0;
           lcd(0,0x80);
           string("select AC mode"); 
           lcd(0,0xC0);
           string("                ");
        }
        //AUTO MODE
        if(RC0==1)
        {
            def_temp=25;
            lcd(0,0x80);
            string("Auto mode     ");
            lcd(0,0xC0);
            string("Temp");
            lcd(1,'=');
            
            lcd(1,(def_temp/10)+48);
            lcd(1,(def_temp%10)+48);
        
        if(ac_temp>def_temp)
        {
            RC2=1;
            lcd(0,0xCA);
            string("AC ON ");
        }
        else if(ac_temp<23)
        {
            RC2=0;
            lcd(0,0xCA);
            string("AC OFF");
        }
       }
      //MANUAL MODE
        if(RC1==1)
        {
            if(RC6==1)
            {
                while(RC6==1);
                def_temp++;
            }
            if(RC7==1)
            {
                while(RC7==1);
                def_temp--;
            }
            
            lcd(0,0x80);
            string("Manual mode   ");
            lcd(0,0xC0);
            string("Temp");
            lcd(1,'=');
            
            lcd(1,(def_temp/10)+48);
            lcd(1,(def_temp%10)+48);
            
            if(ac_temp>def_temp)
        {
            RC2=1;
            lcd(0,0xCA);
            string("AC ON ");
        }
            else if(ac_temp<=def_temp-2)
        {
            RC2=0;
            lcd(0,0xCA);
            string("AC OFF");
        }
            
        }
        
        if(RA4==1 && RB1==1 && RB2==1 && RB3==1)
        {
            door=1;
        }
        else
        {
            door=0;
        }
        
        //seat belt
        if(RB4==1 && RB5==1)
        {
            seat=1;
        }
        else
        {
            seat=0;
        }
        eng_temp();
        fuel();
        battery();
        vol=(int)((float)total_1*4.88758);
        temp=vol/10;

        //fuel
        fuel_per=((long)total_2*100)/1023;
        
        //battery
        bat_per=((long)total_3*100)/1023;
        
        //speed
        if(count>=125)
        {
            count=0;
            speed=((long)pulse*2826)/1000;
           // speed++;
            pulse=0;  
            data_pack();
        }

    }
    
}