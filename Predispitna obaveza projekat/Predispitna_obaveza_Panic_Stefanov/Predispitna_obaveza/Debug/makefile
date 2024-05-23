################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include Drivers/Drajveri_Ekran_Tac/Sors/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Predispitna_obaveza.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Predispitna_obaveza.list \

OBJCOPY_HEX += \
Predispitna_obaveza.hex \


# All Target
all: Predispitna_obaveza.elf secondary-outputs

# Tool invocations
Predispitna_obaveza.elf: $(OBJS) $(USER_OBJS) C:\Users\User\Documents\OPTO\klavir\Predispitna_obaveza\STM32F746ZGTX_FLASH.ld
	arm-none-eabi-gcc -o "Predispitna_obaveza.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\User\Documents\OPTO\klavir\Predispitna_obaveza\STM32F746ZGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Predispitna_obaveza.map" -Wl,--gc-sections -static -L../STemWinLib/Lib --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Predispitna_obaveza.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Predispitna_obaveza.list"
	@echo 'Finished building: $@'
	@echo ' '

Predispitna_obaveza.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Predispitna_obaveza.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
