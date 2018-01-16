/*
 * Exemple simple de test des fcts timer, clock et autres
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/version.h>

MODULE_AUTHOR("L. Vaillé");
MODULE_LICENSE("DUAL BSD/GPL");

int delay = HZ; /* le delai par defaut */

module_param(delay, int, S_IRUGO);

enum hor_files {
	HOR_BUSY,
	HOR_SCHED,
	HOR_WAIT,
	HOR_SCHEDTO
};
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)


int hor_fct(char *buf, char **start, off_t offset, int len, int *oef, void *p)
{
	unsigned long j0, j1; /* jiffies */
	wait_queue_head_t wait;

	init_waitqueue_head(&wait);
	j0 = jiffies;
	j1 = j0 + delay;
	switch((long)p) {
	  case HOR_BUSY:
		while (time_before(jiffies, j1)) cpu_relax();
		break;
          case HOR_SCHED:
		while (time_before(jiffies, j1)) schedule();
		break;
	  case HOR_WAIT:
		wait_event_interruptible_timeout(wait, 0, delay);
		break;
	  case HOR_SCHEDTO:
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(delay);
		break;
	}
	j1 = jiffies; /* valeur après attente */
	len = sprintf(buf, "%9li %9li %9li\n", j0, j1, j1-j0);
	*start = buf;
	return(len);
}

int hor_clock(char *buf, char **start, off_t offset, int len, int *oef, void *p)
{
	struct timeval tv1;
	struct timespec tv2;
	unsigned long j1;
	u64 j2;
	
	/* lecture des valuers */
	j1 = jiffies; /* nombre de battement en jiffies depuis le lancement du noyau */
	j2 = get_jiffies_64();
	do_gettimeofday(&tv1);
	tv2 = current_kernel_time();
	
	/* affichage */
	len  = 0;
	len += sprintf(buf, "0x%08Lx 0x%016Lx %10i.%06i\n"
		"%40i.%09i\n",
		j1, j2,
		(int) tv1.tv_sec, (int) tv1.tv_usec,
		(int) tv2.tv_sec, (int) tv2.tv_nsec);
	*start = buf;
	
	return(len);
}		

#else

int my_read (struct file *fp, char __user *buf, size_t size, loff_t * pos)
{
	char *data;
	int len;

	data=PDE_DATA(file_inode(fp));

	if (data) 
	{
		unsigned long j0, j1; /* jiffies */
	        wait_queue_head_t wait;

        	init_waitqueue_head(&wait);
	        j0 = jiffies;
        	j1 = j0 + delay;
	        switch((long)data) {
          	case HOR_BUSY:
	                while (time_before(jiffies, j1)) cpu_relax();
                	break;
          	case HOR_SCHED:
	                while (time_before(jiffies, j1)) schedule();
                	break;
          	case HOR_WAIT:
	                wait_event_interruptible_timeout(wait, 0, delay);
	                break;
	        case HOR_SCHEDTO:
	                set_current_state(TASK_INTERRUPTIBLE);
	                schedule_timeout(delay);
	                break;
	        }
	        j1 = jiffies; /* valeur après attente */
	        len = sprintf(buf, "%9li %9li %9li\n", j0, j1, j1-j0);

        

	} else {
	        struct timeval tv1;
       		struct timespec tv2;
	        unsigned long j1;
	        u64 j2;

       		/* lecture des valuers */
	        j1 = jiffies; /* nombre de battement en jiffies depuis le lancement du noyau */
        	j2 = get_jiffies_64();
	        do_gettimeofday(&tv1);
        	tv2 = current_kernel_time();

	        /* affichage */
	        len  = 0;
        	len += sprintf(buf, "0x%08Lx 0x%016Lx %10i.%06i\n"
                	"%40i.%09i\n",
                	j1, j2,
	                (int) tv1.tv_sec, (int) tv1.tv_usec,
	                (int) tv2.tv_sec, (int) tv2.tv_nsec);
	/*        *start = buf; */
	}
        return(len);

}

const struct file_operations proc_file = {
  .owner = THIS_MODULE,
  .read  = my_read,
};

#endif

int __init hor_init(void)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
	create_proc_read_entry("HORclock", 0, NULL, hor_clock, NULL);
 	create_proc_read_entry("HORbusy", 0, NULL, hor_fct, (void*)HOR_BUSY);
 	create_proc_read_entry("HORsched", 0, NULL, hor_fct, (void*)HOR_SCHED);
 	create_proc_read_entry("HORwait", 0, NULL, hor_fct, (void*)HOR_WAIT);
	create_proc_read_entry("HORsched2", 0, NULL, hor_fct, (void*)HOR_SCHEDTO);
#else
	proc_create("HORclock", 0, NULL, &proc_file);
	proc_create_data("HORbusy", 0, NULL,&proc_file, (void*)HOR_BUSY);
	proc_create_data("HORsched", 0, NULL, &proc_file, (void*)HOR_SCHED);
	proc_create_data("HORwait", 0, NULL, &proc_file, (void*)HOR_WAIT);
 	proc_create_data("HORschedto", 0, NULL, &proc_file, (void*)HOR_SCHEDTO);
#endif
	return 0;
}

void __exit hor_exit(void)
{
	remove_proc_entry("HORclock", NULL);
	remove_proc_entry("HORbusy", NULL);
	remove_proc_entry("HORsched", NULL);
	remove_proc_entry("HORwait", NULL);
	remove_proc_entry("HORschedto", NULL);
}

module_init(hor_init);
module_exit(hor_exit);



