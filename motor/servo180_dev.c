#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define GPIO_180 18
#define DEV_NAME "SERVO180_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");

int servo180_open(struct inode *pinode, struct file *pfile){
	printk("[SERVO180] Open servo180_dev\n");
	if(gpio_request(GPIO_180,"servo180"!=0)){
		printk("[SERVO180] Already being used");
		return -1;
	}
	gpio_direction_output(GPIO_180,0);
	return 0;
}

int servo180_close(struct inode *pinode, struct file *pfile){
	printk("[SERVO180] Close servo180_dev\n");
	gpio_free(GPIO_180);
	return 0;
}

void turn_servo(int mode){
	int i;
	
	if(mode==0){
		for(i=0;i<50;i++){
			gpio_set_value(GPIO_180,1);
			mdelay(30);
			gpio_set_value(GPIO_180,0);
			mdelay(30);
		}
	}
	else if(mode==1){
		for(i=0;i<50;i++){
			gpio_set_value(GPIO_180,1);
			udelay(500);
			gpio_set_value(GPIO_180,0);
			mdelay(19);
			udelay(500);
		}
	}
}

ssize_t servo180_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){               
  char get_msg;

  if(copy_from_user(&get_msg,buffer,length)<0){
    printk("[SERVO180] Write error\n");
    return -1;
  }

  if(get_msg=='0'){                        
    turn_servo(0);
    printk("[SERVO180] Turn 0 degrees\n");
  }
  else if(get_msg=='1'){		
    turn_servo(1);
    printk("[SERVO180] Turn 180 degrees\n");
  }
  return 0;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = servo180_open,
  .write = servo180_write,
  .release = servo180_close,
};

int __init servo180_init(void){
  printk("[SERVO180] Initialize SERVO180_dev\n");
  register_chrdev(DEV_NUM,DEV_NAME,&fop);
  return 0;
}

void __exit servo180_exit(void){
  printk("[SERVO180] Exit SERVO180_dev\n");
  unregister_chrdev(DEV_NUM,DEV_NAME);
}

module_init(servo180_init);
module_exit(servo180_exit);
