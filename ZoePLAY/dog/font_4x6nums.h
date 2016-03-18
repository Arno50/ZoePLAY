/*File '6X8.FV' as include
The font was generated with EA FontEditor. Please contact ELECTRONIC ASSEMBLY
for more details (techik@lcd-module.de)

 the array starts with a 8 char header:
  1st char: 'F' first 2 chars are always FV
  2nd char: 'V' for FONT VERTICAL
  3rd char: First code to define
  4th char: Last  code to define
  5th char: Width of character in dots
  6th char: Height of character in dots
  7th char: Height of character in chars
  8th char: chars needed for each character (1..255)
            or 0 for big fonts calculate WidthInDots * HeightInchars
  After that font data will follow */

#define FONT_4X6_LEN  76

const uint8_t font_4x6[FONT_4X6_LEN] __attribute__((section(".progmem.data"))) = 
{   
	84, 86, 42,58,  4,  6,  1,  4,
      0x00,  0x1B,  0x04,  0x1B,  // 42: *
      0x00,  0x04,  0x0E,  0x04,  // 43: +
      0x00,  0x10,  0x18,  0x00,  // 44: ,
      0x00,  0x04,  0x04,  0x04,  // 45: -
      0x00,  0x10,  0x00,  0x00,  // 46: .
      0x00,  0x18,  0x04,  0x03,  // 47: /
      0x00,  0x1F,  0x11,  0x1F,  // 48: 0
      0x00,  0x02,  0x1F,  0x00,  // 49: 1
      0x00,  0x1D,  0x15,  0x17,  // 50: 2
      0x00,  0x15,  0x15,  0x1F,  // 51: 3
      0x00,  0x07,  0x04,  0x1F,  // 52: 4
      0x00,  0x17,  0x15,  0x1D,  // 53: 5
      0x00,  0x1F,  0x15,  0x1D,  // 54: 6
      0x00,  0x01,  0x01,  0x1F,  // 55: 7
      0x00,  0x1F,  0x15,  0x1F,  // 56: 8
      0x00,  0x17,  0x15,  0x1F,  // 57: 9
      0x00,  0x00,  0x0A,  0x00   // 58: :
};