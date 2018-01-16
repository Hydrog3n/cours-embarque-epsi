/* Exemple de l'utilisation d'un module pour obtenir les tailles
 * des elements de base */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/utsname.h>

MODULE_LICENSE("Dual BSD/GPL");

static void test_exit(void)
{
/* RIEN ! pas utilise !! */
}

static int test_init(void)
{
struct new_utsname *S;
    S = utsname();

    /* On affiche les renseignements et on retourne une erreur */
    printk("arch   Size:  char  short  int  long  ptr long-long "
          " u8 u16 u32 u64\n");
    printk("%-12s  %3i   %3i   %3i   %3i   %3i   %3i     "
          "%3i %3i %3i %3i\n",
          S->machine,
          (int)sizeof(char),
          (int)sizeof(short),
          (int)sizeof(int),
          (int)sizeof(long),
          (int)sizeof(void *),
          (int)sizeof(long long),
          (int)sizeof(__u8),
          (int)sizeof(__u16),
          (int)sizeof(__u32),
          (int)sizeof(__u64));
    return -ENODEV;
}

module_init(test_init);
module_exit(test_exit);


