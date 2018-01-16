#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

void MonPrintk(char * m)
{
	printk(KERN_DEBUG "MPK : %s !\n", m);
}

EXPORT_SYMBOL_GPL(MonPrintk);



