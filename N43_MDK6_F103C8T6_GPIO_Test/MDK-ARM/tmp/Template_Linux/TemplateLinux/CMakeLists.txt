# CMSIS Build CMakeLists generated on 2024-01-31T14:39:56

cmake_minimum_required(VERSION 3.22)

# Target options

set(TARGET Template_Linux)
set(CPU Cortex-M3)
set(PRJ_DIR "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/MDK-ARM")
set(OUT_DIR "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/MDK-ARM/out/Template_Linux/TemplateLinux")
set(INT_DIR "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/MDK-ARM/tmp/Template_Linux/TemplateLinux")
set(FPU NO_FPU)
set(SECURE Non-secure)
set(BYTE_ORDER Little-endian)
set(OPTIMIZE speed)
set(DEBUG on)
set(AS_FLAGS_GLOBAL "-masm=auto")
set(CC_FLAGS_GLOBAL "-std=c99 -fno-rtti -funsigned-char -fshort-enums -fshort-wchar -ffunction-sections -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality")
set(CXX_FLAGS_GLOBAL "-xc++ -std=c++11 -fno-rtti -funsigned-char -fshort-enums -fshort-wchar -ffunction-sections -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality")
set(LD_FLAGS_GLOBAL "--map --load_addr_map_info --xref --callgraph --symbols --info sizes --info totals --info unused --info veneers --strict --summary_stderr --info summarysizes")
set(LD_SCRIPT "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/MDK-ARM/Template_Linux_TemplateLinux.sct")

set(DEFINES
  STM32F10X_MD
  _RTE_
  USE_HAL_DRIVER
  STM32F103xB
)

set(INC_PATHS
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Core/Inc"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Inc"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/CMSIS/Device/ST/STM32F1xx/Include"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/CMSIS/Include"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/bsp/boards/Inc"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/applications/Inc"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/MDK-ARM/RTE/_TemplateLinux"
  "/home/tungchiahui/.cache/arm/packs/ARM/CMSIS/6.0.0/CMSIS/Core/Include"
  "/home/tungchiahui/.cache/arm/packs/Keil/STM32F1xx_DFP/2.4.1/Device/Include"
)

set(AS_ARM_SRC_FILES
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/MDK-ARM/startup_stm32f103xb.s"
)

set(CC_SRC_FILES
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Core/Src/gpio.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Core/Src/main.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Core/Src/stm32f1xx_hal_msp.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Core/Src/stm32f1xx_it.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Core/Src/system_stm32f1xx.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c"
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/bsp/boards/Src/gpio_test.c"
)

set(CXX_SRC_FILES
  "/media/tungchiahui/6A0A73946EE82411/UserFolder/XiaomiCloud/Work/Source/STM32_Projects/N1_F103C8T6_GPIO_Test/bsp/boards/Src/gpio_demo.cpp"
)

# Toolchain config map

set(REGISTERED_TOOLCHAIN_ROOT "/home/tungchiahui/.vcpkg/artifacts/fd3d78d7/compilers.arm.armclang/6.21.0/bin")
set(REGISTERED_TOOLCHAIN_VERSION "6.21.0")
set(TOOLCHAIN_VERSION_MIN "6.21.0")
include ("/home/tungchiahui/.vcpkg/artifacts/fd3d78d7/tools.open.cmsis.pack.cmsis.toolbox/2.2.1/etc/AC6.6.18.0.cmake")
include ("/home/tungchiahui/.vcpkg/artifacts/fd3d78d7/tools.open.cmsis.pack.cmsis.toolbox/2.2.1/etc/CMSIS-Build-Utils.cmake")

# Setup project

project(${TARGET} LANGUAGES AS_ARM C CXX)

cbuild_get_running_toolchain(TOOLCHAIN_ROOT TOOLCHAIN_VERSION CXX)

# Global Flags

set(CMAKE_AS_ARM_FLAGS "${AS_ARM_CPU} ${AS_ARM_BYTE_ORDER} ${AS_ARM_DEFINES} ${AS_ARM_OPTIONS_FLAGS} ${AS_ARM_FLAGS} ${AS_FLAGS_GLOBAL}")
cbuild_get_system_includes(CC_SYS_INC_PATHS_LIST CC_SYS_INC_PATHS)
set(CMAKE_C_FLAGS "${CC_CPU} ${CC_BYTE_ORDER} ${CC_DEFINES} ${CC_SECURE} ${CC_OPTIONS_FLAGS} ${CC_FLAGS} ${CC_FLAGS_GLOBAL} ${CC_SYS_INC_PATHS}")
cbuild_get_system_includes(CXX_SYS_INC_PATHS_LIST CXX_SYS_INC_PATHS)
set(CMAKE_CXX_FLAGS "${CXX_CPU} ${CXX_BYTE_ORDER} ${CXX_DEFINES} ${CXX_SECURE} ${CXX_OPTIONS_FLAGS} ${CXX_FLAGS} ${CXX_FLAGS_GLOBAL} ${CXX_SYS_INC_PATHS}")
set(CMAKE_CXX_LINK_FLAGS "${LD_CPU} ${_LS}\"${LD_SCRIPT}\" ${LD_SECURE} ${LD_FLAGS_GLOBAL} ${LD_OPTIONS_FLAGS} ${LD_FLAGS}")

# Local Flags

foreach(SRC ${AS_ARM_SRC_FILES})
  set_source_files_properties(${SRC} PROPERTIES LANGUAGE AS_ARM)
endforeach()

# Compilation Database

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(database COMMAND ${CMAKE_COMMAND} -E copy_if_different "${INT_DIR}/compile_commands.json" "${OUT_DIR}")

# Setup Target

add_executable(${TARGET} ${AS_ARM_SRC_FILES} ${CC_SRC_FILES} ${CXX_SRC_FILES})
set_target_properties(${TARGET} PROPERTIES PREFIX "" SUFFIX ".axf" OUTPUT_NAME "Template_Linux")
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR} LINK_DEPENDS ${LD_SCRIPT})
target_include_directories(${TARGET} PUBLIC ${INC_PATHS})
