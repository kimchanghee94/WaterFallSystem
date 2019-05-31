#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#define GPIO4 4
static char *buffer = NULL;

MODULE_LICENSE("GPL");

int temperature_open(struct inode* pinode, struct file* pfile){
	printk("[TEMPERATURE] Open temperature_dev\n");
	
	if(gpio_request(GPIO4 ,"GPIO4")!=0){
		printk(KERN_ALERT "GPIO Request Fail\n");
		return -1;
	}
	gpio_direction_input(GPIO4);

	return 0;
}

int temperature_release(struct inode* pinode, struct file* pfile){
	printk("[TEMPERATURE] Release temperature_dev\n");

	return 0;
}

ssize_t temperature_write(struct file *flip,const char * buf, size_t count, loff_t *f_pos){
	printk(KERN_ALERT "virtual_device write function called\n");
	strcpy(buffer,buf);
	return count;
}

ssize_t temperature_read(struct file * flip, char * buf, size_t count, loff_t * f_pos){
	int value = gpio_get_value(GPIO4);
	printk("--------------%s\n",buffer);
	if(value>0){
		copy_to_user(buf,buffer,1024);
	}
	return count;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = temperature_open,
	.write = temperature_write,
	.read = temperature_read,
	.release = temperature_release,
};

int __init temperature_init(void){
	printk(KERN_INFO "temperature : init\n");
	register_chrdev(242,"temperature_dev",&fops);
	buffer = (char*)kmalloc(1024,GFP_KERNEL);
	if(buffer!=NULL){
		memset(buffer,0,1024);
	}

	return 0;
}

void __exit temperature_exit(void){
	unregister_chrdev(242,"temperature_dev");
	printk(KERN_INFO "temperature : Exit\n");
	kfree(buffer);
}

module_init(temperature_init);
module_exit(temperature_exit);
