#ifndef __MYLED_H__
#define __MYLED_H__

#define PHY_LED1_MODER 0x54004000
#define PHY_LED1_ODR   0x54004014
#define PHY_LED1_RCC    0x54000210
#define PHY_LED2_MODER 0x54004000
#define PHY_LED2_ODR   0x54004014
#define PHY_LED2_RCC    0x54000210
#define PHY_LED3_MODER 0x54004000
#define PHY_LED3_ODR   0x54004014
#define PHY_LED3_RCC    0x54000210


struct led_addr
{
	unsigned int *vir_moder;
	unsigned int *vir_odr;
	unsigned int *vir_rcc;
};
#endif
