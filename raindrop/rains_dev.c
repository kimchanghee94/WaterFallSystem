#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define A1 17

MODULE_LICENSE("GPL");

static int value = 0;

int raindrop_read(struct file *flip, char *buf, size_t count, loff_t *f_pos) {
	
	gpio_direction_input(A1);

	printk(KERN_INFO "rain detect");
	
	value = gpio_get_value(A1);
	copy_to_user(buf, &value, sizeof(int));
	return count;
}

static struct file_operations rain_fops = {
	.read = raindrop_read
};

int __init raindrop_init(void){
	
	int a= register_chrdev(0, "rains_dev", &rain_fops);
	if(a<0){
		printk(KERN_INFO "Raindrop sensor init failed\n");
		}
	else{
		printk(KERN_INFO "Raindrop sensor init successful\n");
		printk(KERN_INFO "major number is %d\n",a);
	}
	
	
	
	return 0;
}

void __exit raindrop_exit(void) {
	gpio_free(A1);
	unregister_chrdev(0, "rains_dev");
	printk(KERN_INFO "Raindrop sensor exit\n");
}

module_init(raindrop_init);
module_exit(raindrop_exit);
