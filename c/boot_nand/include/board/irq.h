#ifndef __IRQ_H_
#define __IRQ_H_

#define INT_ADC 63
#define INT_PENDNUP 62 
#define INT_WDT 26
#define INT_EINT1 1

#define VIC0IRQSTATUS (*(u32 *)0x71200000)
#define VIC0FIQSTATUS (*(u32 *)0x71200004)
#define VIC0RAWINTR (*(u32 *)0x71200008)
#define VIC0INTSELECT (*(u32 *)0x7120000C)
#define VIC0INTENABLE (*(u32 *)0x71200010)
#define VIC0INTENCLEAR (*(u32 *)0x71200014)
#define VIC0SOFTINT (*(u32 *)0x71200018)
#define VIC0SOFTINTCLEAR (*(u32 *)0x7120001C)
#define VIC0PROTECTION (*(u32 *)0x71200020)
#define VIC0SWPRIORITYMASK (*(u32 *)0x71200024)
#define VIC0PRIORITYDAISY (*(u32 *)0x71200028)
#define VIC0VECTADDR ((u32 *)0x71200100)	//没有*
#define VIC0ADDRESS (*(u32 *)0x71200f00)


#define VIC1IRQSTATUS (*(u32 *)0x71300000)
#define VIC1FIQSTATUS (*(u32 *)0x71300004)
#define VIC1RAWINTR (*(u32 *)0x71300008)
#define VIC1INTSELECT (*(u32 *)0x7130000C)
#define VIC1INTENABLE (*(u32 *)0x71300010)
#define VIC1INTENCLEAR (*(u32 *)0x71300014)
#define VIC1SOFTINT (*(u32 *)0x71300018)
#define VIC1SOFTINTCLEAR (*(u32 *)0x7130001C)
#define VIC1PROTECTION (*(u32 *)0x71300020)
#define VIC1SWPRIORITYMASK (*(u32 *)0x71300024)
#define VIC1PRIORITYDAISY (*(u32 *)0x71300028)
#define VIC1VECTADDR ((u32 *)0x71300100)	//没有*
#define VIC1ADDRESS (*(u32 *)0x71300f00)

int request_irq(int irq_no, void (*handler)(void));
 
#endif

