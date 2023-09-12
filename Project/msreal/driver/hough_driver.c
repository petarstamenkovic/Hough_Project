#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/semaphore.h>

MODULE_AUTHOR ("y23_g00");
MODULE_DESCRIPTION("Test Driver for Hough IP.");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("custom: hough IP");

#define DEVICE_NAME "hough"
#define DRIVER_NAME "hough_driver"

//buffer size
#define BUFF_SIZE 100

//addresses for registers
#define START_REG_OFFSET 0
#define RESET_REG_OFFSET 4
#define WIDTH_REG_OFFSET 8
#define HEIGHT_REG_OFFSET 12
#define RHO_REG_OFFSET 16
#define THRESHOLD_REG_OFFSET 20
#define READY_REG_OFFSET 24

#define ADDR_FACTOR 4

//*******************FUNCTION PROTOTYPES************************************
static int hough_probe(struct platform_device *pdev);
static int hough_open(struct inode *i, struct file *f);
static int hough_close(struct inode *i, struct file *f);
static ssize_t hough_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t hough_write(struct file *f, const char __user *buf, size_t length, loff_t *off);
static int __init hough_init(void);
static void __exit hough_exit(void);
static int hough_remove(struct platform_device *pdev);

//*********************GLOBAL VARIABLES*************************************
struct hough_info 
{
	unsigned long mem_start;
	unsigned long mem_end;
	void __iomem *base_addr;
};

static struct cdev *my_cdev;
static dev_t my_dev_id;
static struct class *my_class;

DECLARE_WAIT_QUEUE_HEAD(readyQ);
struct semaphore sem;

static struct hough_info *acc0_bram = NULL;
static struct hough_info *acc1_bram = NULL;
static struct hough_info *img_bram = NULL;
static struct hough_info *hough_core = NULL;

static struct file_operations my_fops =
{
    .owner = THIS_MODULE,
    .open = hough_open,
    .release = hough_close,
    .read = hough_read,
    .write = hough_write
};

static struct of_device_id hough_of_match[] = 
{
	{ .compatible = "acc0_bram_ctrl", }, // acc0_bram
	{ .compatible = "acc1_bram_ctrl", }, // acc1_bram
	{ .compatible = "img_bram_ctrl", }, // img_bram
	{ .compatible = "hough_core", }, // hough_core
	{ /* end of list */ },
};

static struct platform_driver my_driver = {
    .driver = 
	{
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table	= hough_of_match,
	},
	.probe = hough_probe,
	.remove	= hough_remove,
};

MODULE_DEVICE_TABLE(of, hough_of_match);

//***************************************************************************
// PROBE AND REMOVE
int probe_counter = 0;

static int hough_probe(struct platform_device *pdev)
{
	struct resource *r_mem;
	int rc = 0;

	printk(KERN_INFO "Probing\n");

	r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r_mem) 
	{
		printk(KERN_ALERT "hough_probe: invalid address\n");
		return -ENODEV;
	}

	switch (probe_counter)
	{
		case 0: // acc0_bram
			acc0_bram = (struct hough_info *) kmalloc(sizeof(struct hough_info), GFP_KERNEL);
			if (!acc0_bram)
			{
				printk(KERN_ALERT "hough_probe: Cound not allocate acc0_bram device\n");
				return -ENOMEM;
			}
			
			acc0_bram->mem_start = r_mem->start;
			acc0_bram->mem_end   = r_mem->end;
			if(!request_mem_region(acc0_bram->mem_start, acc0_bram->mem_end - acc0_bram->mem_start+1, DRIVER_NAME))
			{
				printk(KERN_ALERT "hough_probe: Couldn't lock memory region at %p\n",(void *)acc0_bram->mem_start);
				rc = -EBUSY;
				goto error1;
			}
			
			acc0_bram->base_addr = ioremap(acc0_bram->mem_start, acc0_bram->mem_end - acc0_bram->mem_start + 1);
			if (!acc0_bram->base_addr)
			{
				printk(KERN_ALERT "hough_probe: Could not allocate acc0_bram iomem\n");
				rc = -EIO;
				goto error2;
			}
			
			probe_counter++;
			printk(KERN_INFO "hough_probe: acc0_bram driver registered.\n");
			return 0;
			
			error2:
			release_mem_region(acc0_bram->mem_start, acc0_bram->mem_end - acc0_bram->mem_start + 1);
			
			error1:
			return rc;
		
			break;
			
		case 1: // acc1_bram
			acc1_bram = (struct hough_info *) kmalloc(sizeof(struct hough_info), GFP_KERNEL);
			if (!acc1_bram)
			{
				printk(KERN_ALERT "hough_probe: Cound not allocate acc1_bram device\n");
				return -ENOMEM;
			}
			
			acc1_bram->mem_start = r_mem->start;
			acc1_bram->mem_end   = r_mem->end;
			if(!request_mem_region(acc1_bram->mem_start, acc1_bram->mem_end - acc1_bram->mem_start+1, DRIVER_NAME))
			{
				printk(KERN_ALERT "hough_probe: Couldn't lock memory region at %p\n",(void *)acc1_bram->mem_start);
				rc = -EBUSY;
				goto error3;
			}
			
			acc1_bram->base_addr = ioremap(acc1_bram->mem_start, acc1_bram->mem_end - acc1_bram->mem_start + 1);
			if (!acc1_bram->base_addr)
			{
				printk(KERN_ALERT "hough_probe: Could not allocate acc1_bram iomem\n");
				rc = -EIO;
				goto error4;
			}
			
			probe_counter++;
			printk(KERN_INFO "hough_probe: acc1_bram driver registered.\n");
			return 0;
			
			error4:
			release_mem_region(acc1_bram->mem_start, acc1_bram->mem_end - acc1_bram->mem_start + 1);
			
			error3:
			return rc;
			
			break;
			
		case 2: // img_bram
			img_bram = (struct hough_info *) kmalloc(sizeof(struct hough_info), GFP_KERNEL);
			if (!img_bram)
			{
				printk(KERN_ALERT "hough_probe: Cound not allocate img_bram device\n");
				return -ENOMEM;
			}
			
			img_bram->mem_start = r_mem->start;
			img_bram->mem_end   = r_mem->end;
			if(!request_mem_region(img_bram->mem_start, img_bram->mem_end - img_bram->mem_start+1, DRIVER_NAME))
			{
				printk(KERN_ALERT "hough_probe: Couldn't lock memory region at %p\n",(void *)img_bram->mem_start);
				rc = -EBUSY;
				goto error5;
			}
			
			img_bram->base_addr = ioremap(img_bram->mem_start, img_bram->mem_end - img_bram->mem_start + 1);
			if (!img_bram->base_addr)
			{
				printk(KERN_ALERT "hough_probe: Could not allocate img_bram iomem\n");
				rc = -EIO;
				goto error6;
			}
			
			probe_counter++;
			printk(KERN_INFO "hough_probe: img_bram driver registered.\n");
			return 0;
			
			error6:
			release_mem_region(img_bram->mem_start, img_bram->mem_end - img_bram->mem_start + 1);
			
			error5:
			
			return rc;
			break;
			
		case 3: // hough_core
			hough_core = (struct hough_info *) kmalloc(sizeof(struct hough_info), GFP_KERNEL);
			if (!hough_core)
			{
				printk(KERN_ALERT "hough_probe: Cound not allocate hough_core\n");
				return -ENOMEM;
			}
			
			hough_core->mem_start = r_mem->start;
			hough_core->mem_end   = r_mem->end;
			if(!request_mem_region(hough_core->mem_start, hough_core->mem_end - hough_core->mem_start+1, DRIVER_NAME))
			{
				printk(KERN_ALERT "hough_probe: Couldn't lock memory region at %p\n",(void *)hough_core->mem_start);
				rc = -EBUSY;
				goto error7;
			}
			
			hough_core->base_addr = ioremap(hough_core->mem_start, hough_core->mem_end - hough_core->mem_start + 1);
			if (!hough_core->base_addr)
			{
				printk(KERN_ALERT "hough_probe: Could not allocate hough iomem\n");
				rc = -EIO;
				goto error8;
			}
			
			printk(KERN_INFO "hough_probe: hough_core driver registered.\n");
			return 0;
			
			error8:
			release_mem_region(hough_core->mem_start, hough_core->mem_end - hough_core->mem_start + 1);
			
			error7:
			return rc;
			
			break;
			
		default:
			printk(KERN_INFO "hough_probe: Counter has illegal value. \n");
			return -1;
	}
	return 0;
}	
		
static int hough_remove(struct platform_device *pdev)
{
	switch (probe_counter)
	{
		case 0: // acc0_bram
			printk(KERN_ALERT "hough_remove: acc0_bram platform driver removed\n");
			iowrite32(0, acc0_bram->base_addr);
			iounmap(acc0_bram->base_addr);
			release_mem_region(acc0_bram->mem_start, acc0_bram->mem_end - acc0_bram->mem_start + 1);
			kfree(acc0_bram);
			break;
			
		case 1: // acc1_bram
			printk(KERN_ALERT "hough_remove: acc1_bram platform driver removed\n");
			iowrite32(0, acc1_bram->base_addr);
			iounmap(acc1_bram->base_addr);
			release_mem_region(acc1_bram->mem_start, acc1_bram->mem_end - acc1_bram->mem_start + 1);
			kfree(acc1_bram);
			probe_counter--;
			break;
			
		case 2: // img_bram
			printk(KERN_ALERT "hough_remove: img_bram platform driver removed\n");
			iowrite32(0, img_bram->base_addr);
			iounmap(img_bram->base_addr);
			release_mem_region(img_bram->mem_start, img_bram->mem_end - img_bram->mem_start + 1);
			kfree(img_bram);
			probe_counter--;
			break;
			
		case 3: // hough_core
			printk(KERN_ALERT "hough_remove: hough_core platform driver removed\n");
			iowrite32(0, hough_core->base_addr);
			iounmap(hough_core->base_addr);
			release_mem_region(hough_core->mem_start, hough_core->mem_end - hough_core->mem_start + 1);
			kfree(hough_core);
			probe_counter--;
			break;
			
		default:
			printk(KERN_INFO "hough_remove: Counter has illegal value. \n");
			return -1;
	}
	return 0;
}		

//***************************************************
// IMPLEMENTATION OF FILE OPERATION FUNCTIONS*************************************************

static int hough_open(struct inode *i, struct file *f)
{
    //printk("hough opened\n");
    return 0;
}
static int hough_close(struct inode *i, struct file *f)
{
    //printk("hough closed\n");
    return 0;
}

int acc0_i = 0;
int acc1_i = 0;
int img_i = 0;
int endRead = 0;
int theta = 135;
int width, height, rho;
int ready = 1;

ssize_t hough_read(struct file *pfile, char __user *buf, size_t length, loff_t *off)
{
	char buff[BUFF_SIZE];
	long int len = 0;
	int acc0_val, acc1_val, img_val;
	int hough_val[7];
	int minor = MINOR(pfile->f_inode->i_rdev);
  
	if(down_interruptible(&sem))
        	return -ERESTARTSYS;

	if (endRead == 1)
	{		
		endRead = 0;
		up(&sem);
		printk(KERN_INFO "Izlaz iz citanja %d!\n", minor);
		return 0;
	}
	
	switch (minor)
	{
		case 0: // acc0_bram
			while (!ready)
			{
				up(&sem);
				if (wait_event_interruptible(readyQ, (ready == 1)))
						return -ERESTARTSYS;
				if (down_interruptible(&sem))
						return -ERESTARTSYS;
			}

			if(acc0_i < theta * rho)
			{	
				acc0_val = ioread32(acc0_bram->base_addr + ADDR_FACTOR * acc0_i);
				len = scnprintf(buff, BUFF_SIZE, "%d ", acc0_val);
				acc0_i++;
				//printk(KERN_INFO "hough_read: acc0[%d] = %d\n", acc0_i, acc0_val);
				
				if (copy_to_user(buf, buff, len))
				{
					printk(KERN_ERR "hough_read: Copy to user does not work.\n");
					return -EFAULT;
				}
			}
			else
			{
				endRead = 1;
				acc0_i = 0;
			}
			break;
		
		case 1: // acc1_bram
			while (!ready)
			{
				up(&sem);
				if (wait_event_interruptible(readyQ, (ready == 1)))
						return -ERESTARTSYS;
				if (down_interruptible(&sem))
						return -ERESTARTSYS;
			}

			if(acc1_i < theta * rho)
			{	
				acc1_val = ioread32(acc1_bram->base_addr + ADDR_FACTOR * acc1_i);
				len = scnprintf(buff, BUFF_SIZE, "%d ", acc1_val);
				acc1_i++;
				// printk(KERN_INFO "hough_read: acc1[%d] = %d\n", acc1_i, acc1_val);
				
				if (copy_to_user(buf, buff, len))
				{
					printk(KERN_ERR "hough_read: Copy to user does not work.\n");
					return -EFAULT;
				}
			}
			else
			{
				endRead = 1;
				acc1_i = 0;
			}
			break;
			
		case 2: // img_bram
			while (!ready)
			{
				up(&sem);
				if (wait_event_interruptible(readyQ, (ready == 1)))
						return -ERESTARTSYS;
				if (down_interruptible(&sem))
						return -ERESTARTSYS;
			}

			if(img_i < width * height)
			{	
				img_val = ioread32(img_bram->base_addr + ADDR_FACTOR * img_i);
				len = scnprintf(buff, BUFF_SIZE, "%d ", img_val);
				img_i++;
				//printk(KERN_INFO "hough_read: img[%d] = %d\n", img_i, img_val);
				
				if (copy_to_user(buf, buff, len))
				{
					printk(KERN_ERR "hough_read: Copy to user does not work.\n");
					return -EFAULT;
				}
			}
			else
			{
				endRead = 1;
				img_i = 0;
			}
			break;	
			
		case 3: // hough_core
			hough_val[0] = ioread32(hough_core->base_addr + START_REG_OFFSET);
			hough_val[1] = ioread32(hough_core->base_addr + RESET_REG_OFFSET);
			hough_val[2] = ioread32(hough_core->base_addr + WIDTH_REG_OFFSET);
			hough_val[3] = ioread32(hough_core->base_addr + HEIGHT_REG_OFFSET);
			hough_val[4] = ioread32(hough_core->base_addr + WIDTH_REG_OFFSET);
			hough_val[5] = ioread32(hough_core->base_addr + THRESHOLD_REG_OFFSET);
			hough_val[6] = ioread32(hough_core->base_addr + READY_REG_OFFSET);
			
			ready = hough_val[6];
			wake_up_interruptible(&readyQ);

			len = scnprintf(buff, BUFF_SIZE, "%d %d %d %d %d %d %d ", hough_val[0], hough_val[1], hough_val[2], hough_val[3], hough_val[4], hough_val[5], hough_val[6]);
			//printk(KERN_INFO "hough_read: ready_reg = %d\n", hough_val);

			if (copy_to_user(buf, buff, len))
			{	
				printk(KERN_ERR "hough_read: Copy to user does not work.\n");
				return -EFAULT;
			}
			
			endRead = 1;
			break;

		default:
			printk(KERN_ERR "hough_read: Invalid minor. \n");
			endRead = 1;
	}

	up(&sem);	
	return len;
}

ssize_t hough_write(struct file *pfile, const char __user *buf, size_t length, loff_t *off)
{
	char buff[BUFF_SIZE];
	int minor = MINOR(pfile->f_inode->i_rdev);
	int pos = 0;
	int val = 0;

	if(down_interruptible(&sem))
		return -ERESTARTSYS;
	
	if (copy_from_user(buff, buf, length))
		return -EFAULT;
	buff[length]='\0';

	sscanf(buff, "%d, %d\n", &val, &pos);
	
	switch (minor)
	{
		case 0: // acc0_bram
			while (!ready)
			{
				up(&sem);
				if (wait_event_interruptible(readyQ, (ready == 1)))
					return -ERESTARTSYS;
				if (down_interruptible(&sem))
					return -ERESTARTSYS;
			}
			
			iowrite32(val, acc0_bram->base_addr + ADDR_FACTOR * pos);
			// printk(KERN_INFO "hough_write: acc0[%d] = %d\n", pos, val);
			break;
			
		case 1: // acc1_bram
			while (!ready)
			{
				up(&sem);
				if (wait_event_interruptible(readyQ, (ready == 1)))
					return -ERESTARTSYS;
				if (down_interruptible(&sem))
					return -ERESTARTSYS;
			}
			
			iowrite32(val, acc1_bram->base_addr + ADDR_FACTOR * pos);
			// printk(KERN_INFO "hough_write: acc1[%d] = %d\n", pos, val);
			break;
			
		case 2: // img_bram
			while (!ready)
			{
				up(&sem);
				if (wait_event_interruptible(readyQ, (ready == 1)))
					return -ERESTARTSYS;
				if (down_interruptible(&sem))
					return -ERESTARTSYS;
			}
			
			iowrite32(val, img_bram->base_addr + ADDR_FACTOR * pos);
			// printk(KERN_INFO "hough_write: img[%d] = %d\n", pos, val);
			break;
			
		case 3: // hough_core
			if (pos == RESET_REG_OFFSET)
			{
				iowrite32(0, hough_core->base_addr + START_REG_OFFSET);
				iowrite32(1, hough_core->base_addr + RESET_REG_OFFSET);
				while(!ioread32(hough_core->base_addr + READY_REG_OFFSET));
				iowrite32(0, hough_core->base_addr + RESET_REG_OFFSET);
				//printk(KERN_INFO "hough_write: RESET DONE!\n");
			}
			else if(pos == START_REG_OFFSET)
			{
				while (!ready)
				{
					up(&sem);
					if (wait_event_interruptible(readyQ, (ready == 1)))
						return -ERESTARTSYS;
					if (down_interruptible(&sem))
						return -ERESTARTSYS;
				}
			
				iowrite32(1, hough_core->base_addr + START_REG_OFFSET);
				while(ioread32(hough_core->base_addr + READY_REG_OFFSET));
				iowrite32(0, hough_core->base_addr + START_REG_OFFSET);
				//printk(KERN_INFO "hough_write: STARTED!\n");	
			}
			else
			{
				while (!ready)
				{
					up(&sem);
					if (wait_event_interruptible(readyQ, (ready == 1)))
						return -ERESTARTSYS;
					if (down_interruptible(&sem))
						return -ERESTARTSYS;
				}
				
				iowrite32(val, hough_core->base_addr + pos);
				if (pos == WIDTH_REG_OFFSET)
					width = val;
				else if (pos == HEIGHT_REG_OFFSET)
					height = val;
				else if (pos == RHO_REG_OFFSET)
					rho = val;
				//printk(KERN_INFO "hough_write: hough_core[%d] = %d\n", pos, val);
			}
			break;
			
		default:
			printk(KERN_INFO "hough_write: Invalid minor. \n");
	}

	up(&sem);

	return length;
}

//***************************************************
// INIT AND EXIT FUNCTIONS OF THE DRIVER

static int __init hough_init(void)
{
	sema_init(&sem,1);

	printk(KERN_INFO "\n");
	printk(KERN_INFO "hough driver starting insmod.\n");

	if (alloc_chrdev_region(&my_dev_id, 0, 4, "hough_region") < 0)
	{
		printk(KERN_ERR "failed to register char device\n");
		return -1;
	}
	printk(KERN_INFO "char device region allocated\n");

	my_class = class_create(THIS_MODULE, "hough_class");
	if (my_class == NULL)
	{
		printk(KERN_ERR "failed to create class\n");
		goto fail_0;
	}
	printk(KERN_INFO "class created\n");

	if (device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 0), NULL, "acc0_bram_ctrl") == NULL) 
	{
		printk(KERN_ERR "failed to create device acc0_bram\n");
		goto fail_1;
	}
	printk(KERN_INFO "device created - acc0_bram\n");

	if (device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 1), NULL, "acc1_bram_ctrl") == NULL) 
	{
		printk(KERN_ERR "failed to create device acc1_bram\n");
		goto fail_2;
	}
	printk(KERN_INFO "device created - acc1_bram\n");

	if (device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 2), NULL, "img_bram_ctrl") == NULL) 
	{
		printk(KERN_ERR "failed to create device img_bram\n");
		goto fail_3;
	}
	printk(KERN_INFO "device created - img_bram\n");


	if (device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 3), NULL, "hough_core") == NULL) 
	{
		printk(KERN_ERR "failed to create device hough_coe\n");
		goto fail_4;
	}
	printk(KERN_INFO "device created - hough_core\n");

	my_cdev = cdev_alloc();
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;

	if (cdev_add(my_cdev, my_dev_id, 4) == -1)
	{
		printk(KERN_ERR "failed to add cdev\n");
		goto fail_5;
	}
	printk(KERN_INFO "cdev added\n");
	printk(KERN_INFO "hough driver initialized.\n");

	return platform_driver_register(&my_driver);

	fail_5:
		device_destroy(my_class, MKDEV(MAJOR(my_dev_id),3));
	fail_4:
		device_destroy(my_class, MKDEV(MAJOR(my_dev_id),2));
	fail_3:
		device_destroy(my_class, MKDEV(MAJOR(my_dev_id),1));
	fail_2:
		device_destroy(my_class, MKDEV(MAJOR(my_dev_id),0));
	fail_1:
		class_destroy(my_class);
	fail_0:
		unregister_chrdev_region(my_dev_id, 1);
	return -1;
}

static void __exit hough_exit(void)
{
	printk(KERN_INFO "hough driver starting rmmod.\n");
	platform_driver_unregister(&my_driver);
	cdev_del(my_cdev);

	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),3));	
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),2));
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),1));
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),0));
	class_destroy(my_class);
	unregister_chrdev_region(my_dev_id, 1);
	printk(KERN_INFO "hough driver exited.\n");
}

module_init(hough_init);
module_exit(hough_exit);
