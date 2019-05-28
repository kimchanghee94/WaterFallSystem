#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

#define GPIO4 4

#define DEV_NAME "buzzer_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");

int buzzer_open(struct inode *pinode, struct file *pfile){
	printk("[BUZZER] Open buzzer_dev\n");
	if(gpio_request(GPIO4,"GPIO4")!=0){
		printk("[BUZZER] Already being used");
		return -1;
	}
	gpio_direction_output(GPIO4,0);
	return 0;
}

int buzzer_release(struct inode *pinode, struct file *pfile){
	printk("[BUZZER] Close buzzer_dev\n");
	gpio_free(GPIO4);
	return 0;
}

void speak_buzzer(int mod){
	int i;

	if(mod==1){
		for(i=0;i<10000;i++){
			gpio_set_value(GPIO4,1);
			udelay(150);
			gpio_set_value(GPIO4,0);
			udelay(150);
		}
	}
}

ssize_t buzzer_write(struct file *filp,const char * buf, size_t count, loff_t *f_pos){
	char get_msg;

	if(copy_from_user(&get_msg,buf,count)<0){
		printk("[BUZZER] Write error\n");
		return -1;
	}

	printk("%c\n",get_msg);

	if(get_msg=='1'){
		speak_buzzer(1);
		printk("[BUZZER] Buzzer Sound\n");
	}
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = buzzer_open,
	.write = buzzer_write,
	.release = buzzer_release,
};

int __init buzzer_init(void){
	printk("[BUZZER] Initialize buzzer_dev\n");
	if(register_chrdev(DEV_NUM,DEV_NAME,&fops)<0){
		printk("[BUZZER] request fail\n");
		return -1;
	}
	return 0;
}

void __exit buzzer_exit(void){
	printk("[BUZZER] Exit buzzer_dev\n");
	unregister_chrdev(DEV_NUM,DEV_NAME);
}

module_init(buzzer_init);
module_exit(buzzer_exit);
