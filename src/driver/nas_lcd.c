/*
 */

#include "nas_lcd.h"

/* table of the devices, that work with this module */
static struct usb_device_id nas_lcd_table[] = {
	{USB_DEVICE(NAS_LCD_VENDOR_ID, NAS_LCD_PRODUCT_ID)},
	{0}							/* end of table is pointed by 0 */
};
MODULE_DEVICE_TABLE (usb, nas_lcd_table);


/* Device initialization routine */
static int __init nas_lcd_init(void) {
}

static void __exit nas_lcd_exit(void) {
}

module_inti(nas_lcd_init);
module_exit(nas_lcd_exit);

/* Module infos*/
MODULE_LICENSE ("GPL v2");
MODULE_DESCRIPTION ("NAS_LCD Module driver. Used to connect NAS_LCD to Host PC.");
MODULE_AUTHOR ("ArhiChief  http://github.com/ArhiChief  <arhichief@gmail.com>");
