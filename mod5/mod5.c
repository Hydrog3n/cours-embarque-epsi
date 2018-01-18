/* exemple de creation de device caractere MAJOR number */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/wait.h>

MODULE_LICENSE("GPL");

static int attend_major = 0; /* MAJOR number */
static DECLARE_WAIT_QUEUE_HEAD(wq);
static int vav = 0;

ssize_t attend_read (struct file *fp, char __user *buf, size_t size, loff_t *pos)
{
int minor;
	minor = iminor(fp->f_dentry->d_inode);
	printk(KERN_DEBUG "Read sur attend %i !\n", minor);
	printk(KERN_DEBUG "Processus %i (%s) bloqué !\n", current->pid, current->comm);
	wait_event_interruptible(wq, vav != 0);
	vav = 0; 
	printk(KERN_DEBUG " Debloquage %i (%s)\n", current->pid, current->comm);
	return 0; /* idem EOF */
}

ssize_t attend_write (struct file *fp, const char __user *buf, size_t size, loff_t *pos)
{
int minor;
        minor = iminor(fp->f_dentry->d_inode);
        printk(KERN_DEBUG "write sur attend %i !\n", minor);
	printk(KERN_DEBUG "Processus %i (%s) debloque le lecteur !\n", current->pid, current->comm);
	vav = 1;
	wake_up_interruptible(&wq);
	return size;

}

struct file_operations attend_fops = {
	.owner = THIS_MODULE,
	.read = attend_read,
	.write = attend_write, 
};

int attend_init(void)
{
int res;
	res = register_chrdev(attend_major, "attend", &attend_fops);
	if (res < 0) return res;
	if (attend_major == 0) attend_major = res;
	printk("id major %i", attend_major);
	return 0;

}

void attend_exit(void)
{
	printk("Aurevoir");
	unregister_chrdev(attend_major, "attend");
}

module_init(attend_init);
module_exit(attend_exit);

