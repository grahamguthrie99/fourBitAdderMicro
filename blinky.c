//*****************************************************************************
//
// blinky.c - ECE 323 Lab # _____________ by  ________KEY__________________
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "C:/ti/TivaWare_C_Series-2.1.3.156/inc/tm4c1294ncpdt.h"
/*#include "C:/ti/TivaWare_C_Series-2.1.3.156/inc/hw_nvic.h"
#include "C:/ti/TivaWare_C_Series-2.1.3.156/inc/hw_types.h"
#include "C:/ti/TivaWare_C_Series-2.1.3.156/inc/hw_memmap.h"
#include "C:/ti/TivaWare_C_Series-2.1.3.156/driverlib/cpu.h"
#include "C:/ti/TivaWare_C_Series-2.1.3.156/driverlib/debug.h"
#include "C:/ti/TivaWare_C_Series-2.1.3.156/driverlib/interrupt.h"
#include "C:/ti/TivaWare_C_Series-2.1.3.156/inc/hw_sysctl.h"
#include "C:/ti/TivaWare_C_Series-2.1.3.156/driverlib/systick.h"*/


#define		PN1_MSK 	(1UL << 1)
#define		PN0_MSK 	(1UL << 0)
#define		PF4_MSK 	(1UL << 4)
#define		PF0_MSK 	(1UL << 0)
#define		SW1_MSK 	(1UL << 0)


uint32_t intrptCounter = 0;
uint32_t reqdNumIntrpts;



void SysTick_Setup(void){
		NVIC_ST_CTRL_R =0;
	  NVIC_ST_RELOAD_R =0xFFFFFF;
		NVIC_ST_CURRENT_R= 0;
	  NVIC_ST_CTRL_R = 0x00000005;
}
inline static void tenSecDelay(){
		uint32_t numCounts = 160000;
		NVIC_ST_RELOAD_R = numCounts -1;
		NVIC_ST_CURRENT_R = 0;
		while((NVIC_ST_CTRL_R & 0x00010000) ==0){
			
		}
}

void configDev(){
 //Clear the clock register 
    SYSCTL_RCGCGPIO_R = 0;

    //Enable the GPIO clock for Port N
		SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | SYSCTL_RCGCGPIO_R12;
    // Enable the GPIO pin for Port F
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | SYSCTL_RCGCGPIO_R5;
	 // Enable the GPIO pin for Port J
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | SYSCTL_RCGCGPIO_R8;
	
		//Do a dummy read to insert a few cycles after enabling the peripheral.
		volatile uint32_t portVal = SYSCTL_RCGCGPIO_R;
	
	//Configure PN1 as an output pin
		GPIO_PORTN_DIR_R = GPIO_PORTN_DIR_R | PN1_MSK;
	//Configure PN0 as an output pin
		GPIO_PORTN_DIR_R = GPIO_PORTN_DIR_R | PN0_MSK;
	 //Configure PF4 as an output pin
		GPIO_PORTF_AHB_DIR_R = GPIO_PORTF_AHB_DIR_R | PF4_MSK;
	//Configure PF0 as an output pin
		GPIO_PORTF_AHB_DIR_R = GPIO_PORTF_AHB_DIR_R | PF0_MSK;
		
		//Configure PJ0 as an output pin
		GPIO_PORTJ_AHB_DIR_R = GPIO_PORTJ_AHB_DIR_R & ~(SW1_MSK);
	
	//Enable digital function in PN1
		GPIO_PORTN_DEN_R = GPIO_PORTN_DEN_R | PN1_MSK;
	//Enable digital function in PN0
		GPIO_PORTN_DEN_R = GPIO_PORTN_DEN_R | PN0_MSK;
	//Enable digital funtion in PF4
		GPIO_PORTF_AHB_DEN_R = GPIO_PORTF_AHB_DEN_R | PF4_MSK;
		//Enable digital funtion in PF0
		GPIO_PORTF_AHB_DEN_R = GPIO_PORTF_AHB_DEN_R | PF0_MSK;
	
	//Enable digital funtion in PJ0
		GPIO_PORTJ_AHB_DEN_R = GPIO_PORTJ_AHB_DEN_R | SW1_MSK;
	
		
		//Enable pull up resistor in PJ0
		GPIO_PORTJ_AHB_PUR_R = GPIO_PORTJ_AHB_PUR_R | SW1_MSK;

}
void PN1_led_on()
{
  // Turn on the LED.
  //
   GPIO_PORTN_DATA_R = GPIO_PORTN_DATA_R | PN1_MSK;
}	
void PN1_led_off()
{
  // Turn on the LED.
  //
   GPIO_PORTN_DATA_R = GPIO_PORTN_DATA_R & ~(PN1_MSK);
}	
void PN0_led_on()
{
  // Turn on the LED.
  //
   GPIO_PORTN_DATA_R = GPIO_PORTN_DATA_R | PN0_MSK;
}	
void PN0_led_off()
{
  // Turn on the LED.
  //
   GPIO_PORTN_DATA_R = GPIO_PORTN_DATA_R & ~(PN0_MSK);
}	
void PF4_led_on()
{
  // Turn on the LED.
  //
   GPIO_PORTF_AHB_DATA_R = GPIO_PORTF_AHB_DATA_R | PF4_MSK;
}	
void PF4_led_off()
{
  // Turn off the LED.
  //
   GPIO_PORTF_AHB_DATA_R = GPIO_PORTF_AHB_DATA_R & ~(PF4_MSK);
}	
void PF0_led_on()
{
  // Turn on the LED.
  //
   GPIO_PORTF_AHB_DATA_R = GPIO_PORTF_AHB_DATA_R | PF0_MSK;
}	
void PF0_led_off()
{
  // Turn off the LED.
  //
   GPIO_PORTF_AHB_DATA_R = GPIO_PORTF_AHB_DATA_R & ~(PF0_MSK);
}	
// ---  MAIN function  ------------
void debounce(){

	

}

int main(void)
{  
		configDev();
		SysTick_Setup();
	  bool sw1Open = true;
		int counter = 0;
		

    while(1)
    {
				if(counter > 15)
				{
						counter  =0;
				}

				if(GPIO_PORTJ_AHB_DATA_R & SW1_MSK)
				{
							sw1Open = true;
				}
				else	
				{
							sw1Open =false;
				}
						
				if(!sw1Open)
				{					
					while (!sw1Open)
					{
						
							if(GPIO_PORTJ_AHB_DATA_R & SW1_MSK)
							{
								sw1Open = true;
							}
							else	
							{
								sw1Open =false;
							}
						tenSecDelay();
						
					}
					counter++;
			 }
				
				if(counter < 8)
				{
						PF0_led_on();
								if(counter<4)
								{
									PF4_led_on();
										if(counter <2)
										{
										PN0_led_on();
										}
										else	
										{
											PN0_led_off();
										}	
								}
								else{
									
									PF4_led_off();
									if(counter<6)
									{
									PN0_led_on();
									}
									else
									{
									PN0_led_off();
									}
								 }
				}
				else	
				{
						PF0_led_off();
						if(counter<12)
								{
									PF4_led_on();
										if(counter <10)
										{
										PN0_led_on();
										}
										else	
										{
											PN0_led_off();
										}	
								}
								else{
									
									PF4_led_off();
									if(counter<14)
									{
									PN0_led_on();
									}
									else
									{
									PN0_led_off();
									}
								}
				}
				if(counter%2 == 0)
				{
					PN1_led_on();
				}
				else
				{
					PN1_led_off();
				}
				
		
}
		}
