# toolchain settings
CROSS:=arm-none-eabi-
CC:=$(CROSS)gcc
OBJDUMP:=$(CROSS)objdump
OBJCOPY:=$(CROSS)objcopy

# gcc & ld compiler flags
OPT:=-Og
FLAGS:=-mcpu=cortex-m3 -mthumb
CFLAGS:=$(FLAGS) $(APP_CFLAGS) $(OPT) -D$(ARCH) -IInclude -I$(COMMON)/Include -I$(COMMON)/Platform/CMSIS5/Common -I$(COMMON)/Platform/CMSIS5/STM32F1xx/inc -I$(COMMON)/Platform/STM32F1xx/inc -fno-common -g3 -Wall -Werror -Wextra -ffunction-sections -fdata-sections
LDFLAGS:=$(FLAGS) -T$(COMMON)/Linker/standard.ld -Xlinker --gc-sections -Xlinker -Map=$(PROJECT).map -nostartfiles -nostdlib -lnosys

SOURCES_HELPER:= \
	$(COMMON)/Helper/helper_init.c \
	$(COMMON)/Helper/helper_irq.c \
    $(COMMON)/Platform/CMSIS5/STM32F1xx/src/system_stm32f1xx.c

SOURCES_HAL:= \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_adc.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_adc_ex.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_can.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_cec.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_cortex.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_crc.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_dac.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_dac_ex.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_dma.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_eth.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_flash.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_flash_ex.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_gpio.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_gpio_ex.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_hcd.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_i2c.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_i2s.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_irda.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_iwdg.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_mmc.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_nand.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_nor.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_pccard.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_pcd.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_pcd_ex.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_pwr.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_rcc.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_rcc_ex.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_rtc.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_rtc_ex.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_sd.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_smartcard.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_spi.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_spi_ex.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_sram.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_tim.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_tim_ex.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_uart.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_usart.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_hal_wwdg.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_adc.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_crc.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_dac.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_dma.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_exti.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_fsmc.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_gpio.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_i2c.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_pwr.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_rcc.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_rtc.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_sdmmc.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_spi.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_tim.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_usart.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_usb.c \
    $(COMMON)/Platform/STM32F1xx/src/stm32f1xx_ll_utils.c
