/*
 * NAS_LCD module Linux USB Driver.
 * 
 * 
 * Copyright (C) ArhiChief 2012 (arhichief@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 * Public License, as published by the Free Software Foundation, version 2.
 *
 * This driver is based on the drivers/usb/usb-skeleton.c but have been rewriten for project.
 *
 * Project repository: https://github.com/Toseter/nas_lcd
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/kref.h>
#include <linux/usb.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h> 


MODULE_AUTHOR ("ArhiChief <arhichief@gmail.com>");
MODULE_LICENSE ("GPL v2");
MODULE_DESCRIPTION ("NAS_LCD Module Linux USB Driver.");

/* Vendor and Product IDs for device */
#define NAS_LCD_VENDOR_ID		0x04d8			// Official Microchip Inc. Vendor ID
#define NAS_LCD_PRODUCT_ID		0xfff0			// Could be changed if nessecery

#define DEV_NAME			"nas_lcd"

#define MSG_SIZE			16			// Size of 1 message (we used 16x2 LCD Screen)
#define MAX_MSGS			5			// Number of message that contains NAS_LCD

/* table of the devices that work with this driver */
static struct usb_device_id nas_lcd_table[] = {
	{USB_DEVICE(NAS_LCD_VENDOR_ID, NAS_LCD_PRODUCT_ID)},
	{0}							// Terminating entry.
};
MODULE_DEVICE_TABLE(usb, nas_lcd_table);

/*Get a minor range for device from the usb maintainer */
#define MINOR_BASE			180

/* NAS_LCD Module device */
struct nas_lcd {
	struct usb_device		*udev;			// the usb device for NAS_LCD
	struct usb_interface		*interface;		// the interface for device
	unsigned char			in[MAX_MSGS][MSG_SIZE];	// buffer for bulk in endpoint
	__u8				bulk_in_endpointAddr;	// the address of bulk in endpoint
	__u8				bulk_out_endpointAddr;	// the address of bulk out endpoint
	struct kref			kref;
};

static struct usb_driver nas_lcd_driver;

#define to_nas_lcd_dev(dev)		container_of(dev, struct nas_lcd, kref)

static void nas_lcd_delete (struct kref *kref) {
	struct nas_lcd *dev = to_nas_lcd_dev(kref);

	usb_put_dev(dev->udev);
	kfree (dev->in);
	kfree (dev);
}

static int nas_lcd_open(struct inode *inode, struct file *file) {
	struct nas_lcd *dev;
	struct usb_interface *interface;
	int subminor;

	subminor = iminor(inode);

	interface = usb_find_interface(&nas_lcd_driver, subminor);
	if (!interface) {
		err("NAS_LCD: %s - error, can't find device for minor %d",
		    __FUNCTION__, subminor);
		return -ENODEV;
	}

	dev = usb_get_intfdata(interface);
	if (!dev)
		return -ENODEV;
	kref_get(&dev->kref);					//increment usage count for the device

	file->private_data = dev;
	return 0;
}

static int nas_lcd_release (struct inode *inode, struct file *file) {
	struct nas_lcd *dev = (struct nas_lcd *)file->private_data;
	if (dev == NULL)
		return -ENODEV;

	kref_put (&dev->kref, nas_lcd_delete);
	return 0;
}

static ssize_t nas_lcd_read (struct file *file, char __user *buffer, size_t count, loff_t *ppos) {
	return 0;
}

//static void nas_lcd_write_bulk_callback (struct urb *urb) {}

static ssize_t nas_lcd_write (struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
	return 0;
}

static struct file_operations nas_lcd_fops = {
	.owner = THIS_MODULE,
	.read = nas_lcd_read,
	.write = nas_lcd_write,
	.open = nas_lcd_open,
	.release = nas_lcd_release,
};

/*
 * usb class driver info in order to get a minor from the usb core, and to have the device registred with
 * devfs and the driver core
 */
static struct usb_class_driver nas_lcd_class = {
	.name = DEV_NAME,
	.fops = &nas_lcd_fops,
	.minor_base = MINOR_BASE,
};

static int nas_lcd_probe (struct usb_interface *interface, const struct usb_device_id *id) {
	return 0;
}

static void nas_lcd_disconnect (struct usb_interface *interface) {
}

static struct usb_driver nas_lcd_driver = {
	.name = DEV_NAME,
	.probe = nas_lcd_probe,
	.disconnect = nas_lcd_disconnect,
	.id_table = nas_lcd_table,
};

/* Initialize and cleanup routine */
module_usb_driver(nas_lcd_driver);
