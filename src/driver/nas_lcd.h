/*
 *  NAS_LCD USB Driver.
 *
 *  Copyright (C) 2012 ArhiChief (arhichief@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of GNU GPL as published by Free
 *  Software Foundation, version 2.
 *
 *  This driver is based on version of drivers/usb/usb-skeleton.c
 */

#include <linux/kernel.h>
#include <linux/errno.h>				/* Error codes */
#include <linux/init.h>
#include <linux/slab> 					/* Slab memory allocation */
#include <linux/module.h>
#include <linux/usb.h>					/* USB defines */
#include <asm/uaccess.h>				/* put_user() & get_user functions */
#include <linux/types.h>

/* 
 * USB device Vendor ID & Product ID
 * Vendor ID was token from http://www.linux-usb.org/usb.ids and belongs to Microchip Inc.
 */
#define NAS_LCD_VENDOR_ID		0x04d8
#define NAS_LCD_PRODUCT_ID		0xfff0

/* Some information */
#define DEVICE_NAME nas_lcd

/* NAS_LCD device structure */
struct nas_lcd_t {
	struct usb_device *		udev		/* the usb device for this device */
	struct usb_interface *		interface	/* the interface for this device */
	__u16				bt_buffer	/* contains pressed buttons */
	char *				msg_buffer	/* contains messages that device shows to user */
};

