#include <htc.h>

/* Buttons codes
 * Number of pressed buttons is converted by next code:
 *              [xxxxx][yyy]
 *  where xxxxx - contains position of button line
 *        yyy   - contains position of interrupt line
 *
 * i.e. button #5 is activated, and it's code will be
 *              [1000][001]
 * all codes are hexdecimal, and will be present at PORTB
 */
#define BT1_C   0x09           /* 00001001 */
#define BT2_C   0x11           /* 00010001 */
#define BT3_C   0x21           /* 00100001 */
#define BT4_C   0x41           /* 01000001 */
#define BT5_C   0x81           /* 10000001 */

#define BT6_C   0x0A           /* 00001010 */
#define BT7_C   0x12           /* 00010010 */
#define BT8_C   0x22           /* 00100010 */
#define BT9_C   0x42           /* 01000010 */
#define BT10_C  0x82           /* 10000010 */

#define BT11_C  0x0C           /* 00001100 */
#define BT12_C  0x14           /* 00010100 */
#define BT13_C  0x24           /* 00100100 */
#define BT14_C  0x44           /* 01000100 */
#define BT15_C  0x84           /* 10000100 */

void interrupt irq (void) {

}
