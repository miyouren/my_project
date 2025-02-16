#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/device.h>
#include "led.h"

#define CNAME "devled"
int major;
struct led_addr led1;
struct led_addr led2;
struct led_addr led3;
struct class *cls;//目录结构体指针
struct device *dic; //驱动节点结构体指针
char kbuf[3] = {0};
int devled_open(struct inode *inode, struct file *file)
{
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
 return 0;
}
ssize_t devled_read(struct file *file, 
 char __user *ubuf, size_t size, loff_t *offs)
{
	int ret;
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	if(size > sizeof(kbuf)) size=sizeof(kbuf);
	ret = copy_to_user(ubuf, kbuf, size);
	if(ret){ //成功返回0，失败返回未拷贝的字节的个数
		printk("copy data to user error\n");
		return -EIO; //失败返回错误码
	} 
	return size;
}
ssize_t devled_write(struct file *file, 
 const char __user *ubuf, size_t size, loff_t *off)
{
	int ret;
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	if(size > sizeof(kbuf)) size=sizeof(kbuf);
	ret = copy_from_user(kbuf, ubuf, size);
	if(ret){ //成功返回0，失败返回未拷贝的字节的个数
		printk("copy data from user error\n");
		return -EIO; //失败返回错误码
	} 
	if(kbuf[0] == '1')
	{
		*(led1.vir_odr) |= (1<<5);
	}
	else if(kbuf[0] == '0')
	{
		*(led1.vir_odr) &= ~(1<<5);		
	}
	else if(kbuf[0] == '2')
	{
		*(led2.vir_odr) |= (1<<6);		
	}
	else if(kbuf[0] == '3')
	{
		*(led2.vir_odr) &= ~(1<<6);		
	}
	else if(kbuf[0] == '4')
	{
		*(led3.vir_odr) |= (1<<7);		
	}
	else if(kbuf[0] == '5')
	{
		*(led3.vir_odr) &= ~(1<<7);		
	}
 return size;
}
int devled_close(struct inode *inode, struct file *file)
{
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
 return 0;
}

const struct file_operations fops = {
	.open = devled_open, 
	.read = devled_read,
	.write = devled_write,
	.release = devled_close,
};

static int __init devled_init(void)
{
 //1.注册字符设备驱动
	major = register_chrdev(0,CNAME,&fops);
 if(major < 0){
		printk("register char device driver error\n");
  return major;
	}
	printk("register char device driver success... major = %d\n",major);
 
	//映射LED灯的地址，并且将LED灯进行初始化
	led1.vir_moder = ioremap(PHY_LED1_MODER,4);
	if(led1.vir_moder == NULL)
	{
		printk("ioremap moder addr error\n");
		return -ENOMEM;
	}
	led1.vir_odr = ioremap(PHY_LED1_ODR,4);
	if(led1.vir_odr == NULL)
	{
		printk("ioremap odr addr error\n");
		return -ENOMEM;
	}	
	led1.vir_rcc = ioremap(PHY_LED1_RCC,4);
	if(led1.vir_rcc == NULL)
	{
		printk("ioremap moder rcc error\n");
		return -ENOMEM;
	}
	led2.vir_moder = ioremap(PHY_LED2_MODER,4);
	if(led2.vir_moder == NULL)
	{
		printk("ioremap moder addr error\n");
		return -ENOMEM;
	}
	led2.vir_odr = ioremap(PHY_LED2_ODR,4);
	if(led2.vir_odr == NULL)
	{
		printk("ioremap odr addr error\n");
		return -ENOMEM;
	}	
	led2.vir_rcc = ioremap(PHY_LED2_RCC,4);
	if(led2.vir_rcc == NULL)
	{
		printk("ioremap moder rcc error\n");
		return -ENOMEM;
	}
	led3.vir_moder = ioremap(PHY_LED3_MODER,4);
	if(led3.vir_moder == NULL)
	{
		printk("ioremap moder addr error\n");
		return -ENOMEM;
	}
	led3.vir_odr = ioremap(PHY_LED3_ODR,4);
	if(led3.vir_odr == NULL)
	{
		printk("ioremap odr addr error\n");
		return -ENOMEM;
	}	
	led3.vir_rcc = ioremap(PHY_LED3_RCC,4);
	if(led3.vir_rcc == NULL)
	{
		printk("ioremap moder rcc error\n");
		return -ENOMEM;
	}

	//LED1灯初始化
	*(led1.vir_moder) &= ~(3<<12);
	*(led1.vir_moder) |= (1<<12);
	*(led1.vir_rcc) |= (1<<0);
	*(led1.vir_odr) &= ~(1<<5);
	//LED2灯初始化
	*(led2.vir_moder) &= ~(3<<12);
	*(led2.vir_moder) |= (1<<12);
	*(led2.vir_rcc) |= (1<<0);
	*(led2.vir_odr) |= (1<<6);
	//LED3灯初始化
	*(led3.vir_moder) &= ~(3<<14);
	*(led3.vir_moder) |= (1<<14);
	*(led3.vir_rcc) |= (1<<0);
	*(led3.vir_odr) &= ~(1<<7);
	//创建设备节点
	cls = class_create(THIS_MODULE,"ledlight");
	if(IS_ERR(cls))//根据地址区间判断错误
	{
		printk("class create error\n");
		return PTR_ERR(cls);
	}
	dic = device_create(cls,NULL,MKDEV(major,0),NULL,"leddev");
	if(IS_ERR(dic))
	{
		printk("device create error\n");
		return PTR_ERR(dic);
	}
 return 0;
}
static void __exit devled_exit(void)
{
	//销毁创建的节点和目录
	device_destroy(cls,MKDEV(major,0));
	class_destroy(cls);

	iounmap(led1.vir_moder);
	iounmap(led1.vir_odr);
	iounmap(led1.vir_rcc);
	iounmap(led2.vir_moder);
	iounmap(led2.vir_odr);
	iounmap(led2.vir_rcc);
	iounmap(led3.vir_moder);
	iounmap(led3.vir_odr);
	iounmap(led3.vir_rcc);

 	//2.注销字符设备驱动
	unregister_chrdev(major,CNAME);

}
module_init(devled_init);
module_exit(devled_exit);
MODULE_LICENSE("GPL");
