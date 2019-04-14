#ifndef __VIDEO__DRIVER
#define __VIDEO__DRIVER

#include <stdint.h>

typedef struct VESAInfoBlock {
    uint16_t ModeAttributes;
    uint8_t WinAAttributes;
    uint8_t WinBAttributes;
    uint16_t WinGranularity;
    uint16_t WinSize;
    uint16_t WinSegmentA;
    uint16_t WinSegmentB;
    uint32_t WinRealFctPtr;
    uint16_t pitch; // Bytes per ScanLine.

    uint16_t XResolution;
    uint16_t YResolution;
    uint8_t XcharSize;
    uint8_t YcharSize;
    uint8_t NumberOfPlanes;
    uint8_t BitsPerPixel;
    uint8_t NumberOfBanks;
    uint8_t MemoryModel;
    uint8_t BankSize;
    uint8_t NumberOfImagePages;
    uint8_t ReservedPage;

    uint8_t RedMaskSize;
    uint8_t RedMaskPosition;
    uint8_t GreenMaskSize;
    uint8_t GreenMaskPosition;
    uint8_t BlueMaskSize;
    uint8_t BlueMaskPosition;
    uint8_t ReservedMaskSize;
    uint8_t ReservedMaskPosition;
    uint8_t DirectColorAttributes;

    uint32_t PhysBasePtr;  // Your LFB (Linear Framebuffer) address.
    uint32_t OffScreenMemOffset;
    uint16_t OffScreenMemSize;
}VESAInfoBlock __attribute__((packed));

//The __attribute__ modifiers are needed to make gcc pack the structure into
//the standard VESA layout, rather than adding pad bytes between some of the
//fields like it would normally do.
//http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html

typedef struct color_t{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color_t;


typedef struct position_t{
  int x;
  int y;
} position_t;

void drawAPixelWithColour(int x, int y, color_t color);

void drawACharacter(uint8_t character, color_t color, position_t * position);

void drawAFigure(uint64_t x, uint64_t y);

void deleteAt(uint64_t x, uint64_t y);


#endif
