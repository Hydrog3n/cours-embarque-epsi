/* Exemple de fabrication d'un compteur via
 * l'interface /proc 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

MODULE_AUTHOR("L. Vaillé");
MODULE_LICENSE("Dual BSD/GPL");

/* Les fonctions sequentielles */
static void *cpt_seq_start(struct seq_file *s, loff_t *p)
{
    loff_t *sp = kmalloc(sizeof(loff_t), GFP_KERNEL);
    if (!sp) return NULL;
    *sp = *p;
    return(sp);
}

static void cpt_seq_stop(struct seq_file *s, void *v)
{
    kfree(v);
}

static void *cpt_seq_next(struct seq_file *s, void *v, loff_t *p)
{
    loff_t * sp = (loff_t *)v;
    *p = ++(*sp);
    return sp;
}

static int cpt_seq_show(struct seq_file *s, void *v)
{
    loff_t *sp = (loff_t *)v;
    seq_printf(s, "%Ld\n", *sp);
    return 0;
}


/* declaration des fonctions speciales */
static struct seq_operations cpt_seq_ops = {
    .start = cpt_seq_start,
    .next  = cpt_seq_next,
    .stop  = cpt_seq_stop,
    .show  = cpt_seq_show
};

static int cpt_open(struct inode * inode, struct file *file)
{
   return seq_open(file, &cpt_seq_ops);
}


/* on utilise les fonctions standards seq_ */
static struct file_operations cpt_file_ops = {
    .owner   = THIS_MODULE,
    .open    = cpt_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = seq_release
};

static int cpt_init(void)
{
struct proc_dir_entry *entree;
    entree = create_proc_entry("compteur", 0, NULL);
    if (entree) /* AFFECTATION DE LA TABLE DES OPERATIONS */
       entree->proc_fops = &cpt_file_ops;
    return 0;
}

static void cpt_exit(void)
{
    remove_proc_entry("compteur", NULL);
}

module_init(cpt_init);
module_exit(cpt_exit);


