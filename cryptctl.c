#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>

#include "crypt_ioctl.h"

MODULE_LICENSE("GPL");


int minorcount = 0; //count of minors created
int i = 0;
//variables to create the main device
static int times = 0;
static dev_t first;
static struct class *cl;
static struct cdev c_dev;
//variables to create a subdevice
static dev_t sub;
static dev_t endev[16];
static struct cdev sub_c;
char subname[64];

int dev_open(struct inode *, struct file *filep);
int dev_rls(struct inode *, struct file *filep);
static long crypt_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

//function operations for the main device
static struct file_operations fops = {
  .open = dev_open,
  .release = dev_rls,
  .unlocked_ioctl = crypt_ioctl,
};

//These are the operations from ioctl
//User program calls ioctl with a command which gets switch cased
static long crypt_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
  switch(cmd)
    {
      //if the user program calls for CRYPT_CREATE this calls
    case CRYPT_CREATE:
      //limit of 16 minor devices
      if (minorcount < 16){
	printk(KERN_INFO "called read\n");
	sprintf(subname, "Encrypt%d",minorcount);

	//These next two lines created a dev_t for the current minor
	int major = MAJOR(sub);
	endev[minorcount] = MKDEV(major, minorcount);

	//These next lines run exactly like the main device creation
	//except they're for a minor device
	//They use the same module class but have a different dev_t
	//They will also eventually have their own fops
	if (device_create(cl, NULL, endev[minorcount], NULL, subname) == NULL)
	  {
	    class_destroy(cl);
	    unregister_chrdev_region(endev[minorcount], 1);
	    return -1;
	  }
	cdev_init(&sub_c, &fops);
	if(cdev_add(&sub_c, endev[minorcount], 1) == -1)
	  {
	    device_destroy(cl, endev[minorcount]);
	    class_destroy(cl);
	    unregister_chrdev_region(endev[minorcount], 1);
	    return -1;
	  }
	minorcount++;  //minorcount incremented for next create
	break;
      }
      else {
	printk(KERN_INFO "Can't create anymore minors\n"); //prints if at limit
	break;
      }
      
    case CRYPT_WRITE:
      printk(KERN_INFO "called write\n"); //this isn't implemented yet
      break;

    default:
      return -EINVAL;
    }
}

int init_module(void)
{
  printk(KERN_INFO "init_module() called\n");
  //next two if statements try to create the character device regions
  //for the main device and the sub devices
  if (alloc_chrdev_region(&sub, 0, 1, "Encrypt") < 0)
    {
      return -1;
    }
  
  if (alloc_chrdev_region(&first, 0, 1, "cryptctl") < 0)
    {
      return -1;
    }
  //This if statement creates the class for the whole module
  //if it can't create it then it unregisters the character regions
  //from the last part
  if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
      unregister_chrdev_region(sub, 1);
      unregister_chrdev_region(first, 1);
      return -1;
    }
  //This creates the actual device for the main device
  //if it can't it destroys the class and unregisters the regions
  if (device_create(cl, NULL, first, NULL, "cryptctl") == NULL)
    {
      class_destroy(cl);
      unregister_chrdev_region(sub, 1);
      unregister_chrdev_region(first, 1);
      return -1;
    }
  //The next lines of code initiates the device with the proper function
  //Then it tries to load it into the kernel
  //If it's not successful, the devices and classes are destroyed and
  //The character regions are unregistered
  cdev_init(&c_dev, &fops);
  if(cdev_add(&c_dev, first, 1) == -1)
    {
      device_destroy(cl, first);
      class_destroy(cl);
      unregister_chrdev_region(sub, 1);
      unregister_chrdev_region(first, 1);
      return -1;
    }

  return 0;
}
//Cleanup simply destroys or unregisters everything that was created
void cleanup_module(void)
{
  printk(KERN_INFO "cleanup_module() called\n");
  cdev_del(&c_dev);
  cdev_del(&sub_c);

  for (i = 0; i <= minorcount; i++){
    device_destroy(cl, endev[i]);
  }
  device_destroy(cl, first);

  class_destroy(cl);
  for (i = 0; i <= minorcount; i++){
    unregister_chrdev_region(endev[i],1);
  }
  unregister_chrdev_region(sub,1);
  unregister_chrdev_region(first,1);
  
  printk(KERN_INFO "chr dev unregistered\n");
}
//when the device is opened by a user program this runs
int dev_open(struct inode *inode, struct file *filep)
{
  times++;
  printk(KERN_INFO"Device opened %d times\n", times);
  return 0;
}
//when the device is closed by a user program this runs
int dev_rls(struct inode *inode, struct file *filep)
{
  printk(KERN_INFO "Device closed\n");
  return 0;
}
