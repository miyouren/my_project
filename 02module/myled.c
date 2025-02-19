#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/device.h>

#include "myled.h"


int major;
char kbuf[2] = {0};
gpio_t *virt_led1;
gpio_t *virt_led2;
gpio_t *virt_led3;
unsigned int *virt_rcc;

#define LED1_ON (virt_led1->ODR |= (1 << 5))
#define LED1_OFF (virt_led1->ODR &= ~(1 << 5))
#define LED2_ON (virt_led2->ODR |= (1 << 6))
#define LED2_OFF (virt_led2->ODR &= ~(1 << 6))
#define LED3_ON (virt_led3->ODR |= (1 << 7))
#define LED3_OFF (virt_led3->ODR &= ~(1 << 7))




struct class *cls;
struct device *dev;

int myled_open(struct inode *inode, struct file *file){
    printk("%s:%s:%d", __FILE__, __func__, __LINE__);
    return 0;
}
ssize_t myled_read(struct file *file, char __user *ubuf, size_t size, loff_t *offs){
    int ret = 0;
    if(size > sizeof(kbuf))
        size = sizeof(kbuf);
    ret = copy_to_user(ubuf, kbuf, size);
    if(ret){
        printk("copy to user error!");
        return -EIO;
    }
    return size;
}

ssize_t myled_write(struct file *file, const char __user *ubuf, size_t size, loff_t *off){
    int ret = 0;
    if(size > sizeof(kbuf))
        size = sizeof(kbuf);
    ret = copy_from_user(kbuf, ubuf, size);
    if(ret){
    printk("copy from user error!!!");
    return -EIO;
    }
    return size;
}

int myled_close(struct inode *inode, struct file *file)
{
 printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
 return 0;
}

long myled_ioctl(struct file *file,
     unsigned int cmd, unsigned long arg){
  switch(cmd){
  case LED1ON:
   LED1_ON;
   break;
  case LED1OFF:
   LED1_OFF;
   break;
  case LED2ON:
    LED2_ON;
    break;
  case LED2OFF:
    LED2_OFF;
    break;
  case LED3ON:
    LED3_ON;
    break;
  case LED3OFF:
    LED3_OFF;
    break;
 }
  return 0;
}

    
const struct file_operations fops = {
    .open = myled_open,
    .write = myled_write,
    .read = myled_read,
    .release = myled_close,
    .unlocked_ioctl = myled_ioctl,
};

int all_led_init(void){
    virt_led1 = ioremap(PHY_LED_ADDR, sizeof(gpio_t));
    if(NULL == virt_led1){
        printk("ioremap led1 addr error\n");
        return -ENOMEM;
    }
    virt_led2 = ioremap(PHY_LED_ADDR, sizeof(gpio_t));
    if(NULL == virt_led2){
        printk("ioremap led2 addr error\n");
        return -ENOMEM;
    }
    virt_led3 = ioremap(PHY_LED_ADDR, sizeof(gpio_t));
    if(NULL == virt_led3){
        printk("ioremap led3 addr error\n");
        return -ENOMEM;
    }
    virt_rcc = ioremap(PHY_LED_RCC, sizeof(int));
    if(NULL == virt_rcc){
        printk("ioremap rcc addr error\n");
        return -ENOMEM;
    }

    virt_led1->MODER &= ~(3 << 10);
    virt_led1->MODER |= (1 << 10);
    virt_led1->ODR |= (1 << 5);

    virt_led2->MODER &= ~(3 << 12);
    virt_led2->MODER |= (1 << 12);
    virt_led2->ODR |= (1 << 6);

    virt_led3->MODER &= ~(3 << 14);
    virt_led3->MODER |= (1 << 14);
    virt_led3->ODR |= (1 << 7);

    *virt_rcc |= (1 << 0);
    return 0;   
}

static int __init myled_init(void){

    all_led_init();

    //1.注册字符设备驱动
    major = register_chrdev(0,"myled",&fops);
    if(major < 0){
    printk("register char device driver error\n");
    return major;
    }

    //创建设备节点
    cls = class_create(THIS_MODULE, "01myled");
    if (IS_ERR(cls)) {
        printk("Failed to create class\n");
        return PTR_ERR(cls);
    }
    dev = device_create(cls, NULL, MKDEV(major, 0), NULL, "myled");
    if(IS_ERR(dev)){
        printk("device create error\n");
        return PTR_ERR(dev);
    }
    return 0;
}

static void __exit myled_exit(void){
 device_destroy(cls,MKDEV(major,0));
 class_destroy(cls);

 iounmap(virt_rcc);
 iounmap(virt_led1);
 iounmap(virt_led2);
 iounmap(virt_led3);
 unregister_chrdev(major,"myled");

}
module_init(myled_init);
module_exit(myled_exit);
MODULE_LICENSE("GPL");








