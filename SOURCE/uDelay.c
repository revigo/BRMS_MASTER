#include <xc.h> 
void Delay_us(unsigned int us)
 {
	 while(us--)
     {
        Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
        Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
        Nop();Nop();Nop();Nop();Nop();Nop();Nop();asm("clrwdt");
     }
 }
 void Delay_ms(unsigned int ms)
 {
     unsigned int i;
     while(ms--)
     {
         for(i=0;i<4002;i++)
         {
             asm("clrwdt");
         }
     }
 }



