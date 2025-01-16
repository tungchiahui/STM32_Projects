# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/ST-Cube-MX/RTOS_MPU6050/MDK-ARM/tmp/RTOS_MPU6050+RTOSMPU6050"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/ST-Cube-MX/RTOS_MPU6050/MDK-ARM/tmp/1"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/ST-Cube-MX/RTOS_MPU6050/MDK-ARM/tmp/RTOS_MPU6050+RTOSMPU6050"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/ST-Cube-MX/RTOS_MPU6050/MDK-ARM/tmp/RTOS_MPU6050+RTOSMPU6050/tmp"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/ST-Cube-MX/RTOS_MPU6050/MDK-ARM/tmp/RTOS_MPU6050+RTOSMPU6050/src/RTOS_MPU6050+RTOSMPU6050-stamp"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/ST-Cube-MX/RTOS_MPU6050/MDK-ARM/tmp/RTOS_MPU6050+RTOSMPU6050/src"
  "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/ST-Cube-MX/RTOS_MPU6050/MDK-ARM/tmp/RTOS_MPU6050+RTOSMPU6050/src/RTOS_MPU6050+RTOSMPU6050-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/ST-Cube-MX/RTOS_MPU6050/MDK-ARM/tmp/RTOS_MPU6050+RTOSMPU6050/src/RTOS_MPU6050+RTOSMPU6050-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/tung/UserFolder/XiaomiCloud/Work/Source/ST-Cube-MX/RTOS_MPU6050/MDK-ARM/tmp/RTOS_MPU6050+RTOSMPU6050/src/RTOS_MPU6050+RTOSMPU6050-stamp${cfgdir}") # cfgdir has leading slash
endif()
