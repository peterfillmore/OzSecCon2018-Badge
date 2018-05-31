CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

#uGFX library
GFXLIB=/home/peter/src/OzSecCon_Badge_Dev/LCDTest/Libraries/ugfx
#GFXSRC=$(GFXLIB)/src
GFXINC=$(GFXLIB)

###########################################

include ./ugfx/gfx.mk
include ./ugfx/drivers/gdisp/SSD1306/driver.mk
#include ./ugfx/drivers/gdisp/ST7735/driver.mk
#include ./ugfx/src/gfile/gfile.mk
include ./ugfx/src/gos/gos.mk
include ./ugfx/src/gfile/gfile.mk

vpath %.c STM32F0xx_StdPeriph_Driver/src M24SR/src $(GFXSRC)

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6s-m
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -ICMSIS/Include -ICMSIS/Device/ST/STM32F0xx/Include -ISTM32F0xx_StdPeriph_Driver/inc -I../inc
CFLAGS += -IM24SR/inc 
CFLAGS += -includestm32f0xx_conf.h
#CFLAGS += -I $(GFXLIB)/drivers/gdisp/ST7735
CFLAGS += -I $(GFXLIB)/drivers/gdisp/SSD1306
CFLAGS += -I$(GFXINC)

SRCS = stm32f0xx_adc.c stm32f0xx_cec.c stm32f0xx_comp.c stm32f0xx_crc.c \
	stm32f0xx_dac.c stm32f0xx_dbgmcu.c stm32f0xx_dma.c stm32f0xx_exti.c \
	stm32f0xx_flash.c stm32f0xx_gpio.c stm32f0xx_i2c.c stm32f0xx_iwdg.c \
	stm32f0xx_misc.c stm32f0xx_pwr.c stm32f0xx_rcc.c stm32f0xx_rtc.c \
	stm32f0xx_spi.c stm32f0xx_syscfg.c stm32f0xx_tim.c \
	stm32f0xx_usart.c stm32f0xx_wwdg.c \
	drv_M24SR.c drv_I2C_M24SR.c lib_M24SR.c lib_ISO7816_Password.c \
	lib_TagType4.c lib_NDEF.c lib_NDEF_URI.c lib_NDEF_SMS.c lib_NDEF_Email.c \
	lib_NDEF_Vcard.c lib_NDEF_Geo.c lib_NDEF_MyApp.c lib_NDEF_AAR.c \
    $(GFXSRC)
OBJS = $(SRCS:.c=.o)

.PHONY: libstm32f0.a

all: libstm32f0.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libstm32f0.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libstm32f0.a
