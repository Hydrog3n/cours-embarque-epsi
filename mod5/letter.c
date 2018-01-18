/* Driver lettre version beta */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/uaccess.h>

#define NBL 26 /* nb de lettres */

MODULE_LICENSE("Dual BSD/GPL");

static int lettre_major = 0; /* MAJOR Number */

/* tableau des lettres */
static char TL[NBL];

ssize_t lettre_read (struct file *fp, char __user *buf, size_t nbc, loff_t *pos)
{
int minor,n;
   minor = iminor(fp->f_dentry->d_inode);
   /* verification du minor */
   if (minor < 0) return -ENODEV;
   if (minor >= NBL) return -ENODEV;
   /* on tranfere le bon caractere dans le buffer qui est en user-space */
   for (n=0;n<nbc;n++) if (__copy_to_user((void __user *)(buf+n),(void*)(TL+minor),1) != 0) break;
   return n; /* on a ecrit n octets ! */
}

ssize_t lettre_write (struct file *fp, const char __user *buf, size_t nbc, loff_t *pos)
{
int minor;
   minor = iminor(fp->f_dentry->d_inode);
   return nbc; /* retourne le nb de car ecrits */
}

struct file_operations lettre_fops = {
	.owner = THIS_MODULE,
	.read = lettre_read,
	.write = lettre_write,
};

int lettre_init(void)
{
int res, i;
	res = register_chrdev(lettre_major,  "lettre", &lettre_fops);
	if (res < 0) return res;
	if (lettre_major == 0) lettre_major = res;
        /* initialisation du tableau des lettres */
        for (i=0; i<NBL; i++) TL[i] = 'A' + i;
	return 0;
}

void lettre_exit(void)
{
	unregister_chrdev(lettre_major, "lettre");
}

module_init(lettre_init);
module_exit(lettre_exit);