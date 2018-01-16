/* Exemple de l'utilisation d'un module pour obtenir les alignements
 * des elements de base */
/* Avec gestion des differentes versions de noyau 2.6 ! */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/utsname.h>
#include <linux/version.h>

MODULE_LICENSE("Dual BSD/GPL");

struct c {char c; char x;} c;
struct s {char c; short x;} s;
struct i {char c; int x;} i;
struct l {char c; long x;} l;
struct p {char c; void * x;} p;
struct ll {char c; long long x;} ll;
struct U8 {char c; __u8 x;} U8;
struct U16 {char c; __u16 x;} U16;
struct U32 {char c; __u32 x;} U32;
struct U64 {char c; __u64 x;} U64;

static void test_exit(void)
{
/* RIEN ! pas utilise !! */
}

static int test_init(void)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26)
#else
struct new_utsname *S;
    S = utsname();
#endif
    /* On affiche les renseignements et on retourne une erreur */
    printk("arch   Size:  char  short  int  long  ptr long-long "
          " u8 u16 u32 u64\n");
    printk("%-12s  %3i   %3i   %3i   %3i   %3i   %3i     "
          "%3i %3i %3i %3i\n",
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26)
	  system_utsname.machine,
#else
          S->machine,
#endif
          (int)((void *)&(c.x) - (void *)(&c)),
          (int)((void *)&(s.x) - (void *)(&s)),
          (int)((void *)&(i.x) - (void *)(&i)),
          (int)((void *)&(l.x) - (void *)(&l)),
          (int)((void *)&(p.x) - (void *)(&p)),
          (int)((void *)&(ll.x) - (void *)(&ll)),
          (int)((void *)&(U8.x) - (void *)(&U8)),
          (int)((void *)&(U16.x) - (void *)(&U16)),
          (int)((void *)&(U32.x) - (void *)(&U32)),
          (int)((void *)&(U64.x) - (void *)(&U64)) );
    return -ENODEV;
}

module_init(test_init);
module_exit(test_exit);


