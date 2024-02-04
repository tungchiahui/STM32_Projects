# CMSIS Build CMakeLists generated on 2024-02-04T18:20:49

cmake_minimum_required(VERSION 3.22)

# Target options

set(TARGET MDK6_CPP_Windows)
set(CPU Cortex-M3)
set(PRJ_DIR "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/MDK-ARM")
set(OUT_DIR "C:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/MDK-ARM/out/MDK6_CPP_Windows/MDK6CPPWindows")
set(INT_DIR "C:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/MDK-ARM/tmp/MDK6_CPP_Windows/MDK6CPPWindows")
set(FPU NO_FPU)
set(SECURE Non-secure)
set(BYTE_ORDER Little-endian)
set(OPTIMIZE speed)
set(DEBUG on)
set(AS_FLAGS_GLOBAL "-masm=auto")
set(CC_FLAGS_GLOBAL "-std=c99 -fno-rtti -funsigned-char -fshort-enums -fshort-wchar -ffunction-sections -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality")
set(CXX_FLAGS_GLOBAL "-xc++ -std=c++11 -fno-rtti -funsigned-char -fshort-enums -fshort-wchar -ffunction-sections -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality")
set(LD_FLAGS_GLOBAL "--map --load_addr_map_info --xref --callgraph --symbols --info sizes --info totals --info unused --info veneers --strict --summary_stderr --info summarysizes")
set(LD_SCRIPT "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/MDK-ARM/MDK6_CPP_Windows_MDK6CPPWindows.sct")

set(DEFINES
  STM32F10X_MD
  _RTE_
  USE_HAL_DRIVER
  STM32F103xB
)

set(INC_PATHS
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Core/Inc"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Inc"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/include"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM3"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/CMSIS/Device/ST/STM32F1xx/Include"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/CMSIS/Include"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/MDK-ARM/RTE/_MDK6CPPWindows"
  "C:/Users/tungchiahui/AppData/Local/arm/packs/ARM/CMSIS/6.0.0/CMSIS/Core/Include"
  "C:/Users/tungchiahui/AppData/Local/arm/packs/Keil/STM32F1xx_DFP/2.4.1/Device/Include"
)

set(AS_ARM_SRC_FILES
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/MDK-ARM/startup_stm32f103xb.s"
)

set(CC_SRC_FILES
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Core/Src/freertos.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Core/Src/gpio.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Core/Src/main.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Core/Src/stm32f1xx_hal_msp.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Core/Src/stm32f1xx_hal_timebase_tim.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Core/Src/stm32f1xx_it.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Core/Src/system_stm32f1xx.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/croutine.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/list.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM3/port.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/queue.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/tasks.c"
  "c:/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N46_MDK6_CPP_Windows/Middlewares/Third_Party/FreeRTOS/Source/timers.c"
)

# Toolchain config map

set(REGISTERED_TOOLCHAIN_ROOT "c:/Users/tungchiahui/.vcpkg/artifacts/fd3d78d7/compilers.arm.armclang/6.21.0/bin")
set(REGISTERED_TOOLCHAIN_VERSION "6.21.0")
set(TOOLCHAIN_VERSION_MIN "6.21.0")
include ("C:/Users/tungchiahui/.vcpkg/artifacts/fd3d78d7/tools.open.cmsis.pack.cmsis.toolbox/2.2.1/etc/AC6.6.18.0.cmake")
include ("C:/Users/tungchiahui/.vcpkg/artifacts/fd3d78d7/tools.open.cmsis.pack.cmsis.toolbox/2.2.1/etc/CMSIS-Build-Utils.cmake")

# Setup project

project(${TARGET} LANGUAGES AS_ARM C)

cbuild_get_running_toolchain(TOOLCHAIN_ROOT TOOLCHAIN_VERSION C)

# Global Flags

set(CMAKE_AS_ARM_FLAGS "${AS_ARM_CPU} ${AS_ARM_BYTE_ORDER} ${AS_ARM_DEFINES} ${AS_ARM_OPTIONS_FLAGS} ${AS_ARM_FLAGS} ${AS_FLAGS_GLOBAL}")
cbuild_get_system_includes(CC_SYS_INC_PATHS_LIST CC_SYS_INC_PATHS)
set(CMAKE_C_FLAGS "${CC_CPU} ${CC_BYTE_ORDER} ${CC_DEFINES} ${CC_SECURE} ${CC_OPTIONS_FLAGS} ${CC_FLAGS} ${CC_FLAGS_GLOBAL} ${CC_SYS_INC_PATHS}")
set(CMAKE_C_LINK_FLAGS "${LD_CPU} ${_LS}\"${LD_SCRIPT}\" ${LD_SECURE} ${LD_FLAGS_GLOBAL} ${LD_OPTIONS_FLAGS} ${LD_FLAGS}")

# Local Flags

foreach(SRC ${AS_ARM_SRC_FILES})
  set_source_files_properties(${SRC} PROPERTIES LANGUAGE AS_ARM)
endforeach()

# Compilation Database

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(database COMMAND ${CMAKE_COMMAND} -E copy_if_different "${INT_DIR}/compile_commands.json" "${OUT_DIR}")

# Setup Target

add_executable(${TARGET} ${AS_ARM_SRC_FILES} ${CC_SRC_FILES})
set_target_properties(${TARGET} PROPERTIES PREFIX "" SUFFIX ".axf" OUTPUT_NAME "MDK6_CPP_Windows")
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR} LINK_DEPENDS ${LD_SCRIPT})
target_include_directories(${TARGET} PUBLIC ${INC_PATHS})
