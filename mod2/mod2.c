#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test de passage de parametres et de fct externe");
MODULE_AUTHOR("P. FOUBET");
MODULE_VERSION("1.0");

void MonPrintk(char*);

static char * qui = "tout le monde";
static int combien = 1;

module_param(combien, int, S_IRUGO);
module_param(qui, charp, S_IRUGO);

static int salut_init(void)
{
int i;
        MonPrintk("Debut de l'initialisation");
	for (i=0; i<combien; i++)
		printk(KERN_DEBUG "(%d) Salut a %s !\n", i, qui);
	return 0;
}

static void salut_exit(void)
{
	printk(KERN_DEBUG "Au revoir %s !\n", qui);
        MonPrintk("Sortie du module");
}

module_init(salut_init);
module_exit(salut_exit);







