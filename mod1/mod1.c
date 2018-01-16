#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int  salut_init(void)
{
	printk(KERN_ALERT "Salut à tous !\n");
	return 0;
}

static void salut_exit(void)
{	
	printk(KERN_ALERT "Aurevoir à tous !\n");

}

module_init(salut_init);
module_exit(salut_exit);

