/* Driver lettre version beta */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/wait.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#define NBL 26 /* nb de lettres */

MODULE_LICENSE("Dual BSD/GPL");

static int lettre_major = 0; /* MAJOR Number */

/* tableau des lettres */
static char TL[NBL];

ssize_t lettre_read (struct file *fp, char __user *buf, size_t nbc, loff_t *pos)
{
int minor,n;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,16,0)
   minor = iminor(fp->f_dentry->d_inode);
#else
   minor = iminor(file_inode(fp));
#endif
   /* verification du minor */
   if (minor < 0) return -ENODEV;
   if (minor >= NBL) return -ENODEV;
   /* on tranfere le bon caractere dans le buffer qui est en user-space */
   for (n=0;n<nbc;n++) if (__copy_to_user((void __user *)(buf+n),(void*)(TL+minor),1) != 0) break;
   return n; /* on a ecrit n octets ! */
}

ssize_t lettre_write (struct file *fp, const char __user *buf, size_t nbc, loff_t *pos)
{
int minor,i=0;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,16,0)
   minor = iminor(fp->f_dentry->d_inode);
#else
   minor = iminor(file_inode(fp));
#endif
   /* verification du minor */
   if (minor < 0) return -ENODEV;
   if (minor >= NBL) return -ENODEV;
   /* on cherche le 1er caractere qui correspond a une majuscule */
   while (i < nbc) {
         if (isupper(buf[i])) break;
         i++;
   }
   if (i<nbc) __copy_from_user((void*)(TL+minor),(void __user *)(buf+i),1);
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







