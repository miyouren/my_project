#ifndef __MYLED_H__
#define __MYLED_H__


#define STM32_GPIO_MODER   0x00   // 模式寄存器
#define STM32_GPIO_OTYPER  0x04   // 输出类型寄存器
#define STM32_GPIO_OSPEEDR 0x08   // 输出速度寄存器
#define STM32_GPIO_PUPDR   0x0C   // 上拉/下拉寄存器
#define STM32_GPIO_IDR     0x10   // 输入数据寄存器
#define STM32_GPIO_ODR     0x14   // 输出数据寄存器
#define STM32_GPIO_BSRR    0x18   // 置位/复位寄存器

#define PHY_LED_RCC    0x54000210
#define PHY_LED_ADDR    0x54004000

typedef struct{
 volatile unsigned int MODER;
 volatile unsigned int OTYPER;
 
volatile unsigned int OSPEEDR;
 volatile unsigned int PUPDR;
 volatile unsigned int IDR;
 volatile unsigned int ODR;
 volatile unsigned int BSRR;  
}gpio_t;

#define LED1ON _IOR('A', 0, int)
#define LED1OFF _IOR('A', 1, int)

#define LED2ON _IOR('B', 0, int)
#define LED2OFF _IOR('B', 1, int)

#define LED3ON _IOR('C', 0, int)
#define LED3OFF _IOR('C', 1, int)



#endif



