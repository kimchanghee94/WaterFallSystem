#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define A1 4

MODULE_LICENSE("GPL");

int raindrop_read(struct file *flip, char *buf, size_t count, loff_t *f_pos) {
	int value = gpio_get_value(A1);
	copy_to_user(buf, &value, sizeof(int));
	return count;
}

static struct file_operations rain_fops = {
	.read = raindrop_read
};

int __init raindrop_init(void){
	
	
	if(register_chrdev(243, "rain_dev", &rain_fops)<0)
		printk(KERN_ALERT "Raindrop sensor init failed\n");
	else
		printk(KERN_ALERT "Raindrop sensor init successful\n");
	
	gpio_direction_input(A1);
	
	
	return 0;
}

void __exit raindrop_exit(void) {
	gpio_free(A1);
	unregister_chrdev(243, "rain_dev");
	printk(KERN_ALERT "Raindrop sensor exit\n"));
}

module_init(raindrop_init);
module_exit(raindrop_exit);
