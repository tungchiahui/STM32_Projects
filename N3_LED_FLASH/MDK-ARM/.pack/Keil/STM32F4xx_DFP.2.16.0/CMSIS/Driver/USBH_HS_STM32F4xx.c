/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2021 Arm Limited (or its affiliates). All 
 * rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        23. April 2021
 * $Revision:    V2.25
 *
 * Driver:       Driver_USBH1
 * Configured:   via RTE_Device.h configuration file
 * Project:      USB High/Full/Low-Speed Host Driver for ST STM32F4xx
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                  Value
 *   ---------------------                  -----
 *   Connect to hardware via Driver_USBH# = 1
 *   USB Host controller interface        = Custom
 * --------------------------------------------------------------------------
 * Defines used for driver configuration (at compile time):
 *
 *   USB_OTG_HS_VBUS_Power_Pin_Active:  specifies VBUS pin polarity
 *                      (0 = active low, 1 = active high)
 *     - default value : 0 = active low
 *   USB_OTG_HS_Overcurrent_Pin_Active: specifies Overcurrent pin polarity
 *                      (0 = active low, 1 = active high)
 *     - default value : 0 = active low
 *   USBH1_MAX_PIPE_NUM: defines maximum number of Pipes that driver will
 *                       support, this value impacts driver memory
 *                       requirements
 *     - default value : 16
 *     - maximum value : 16
 *   USBH1_BULK_IN_NAK_DELAY: Delay between NAKs on Bulk IN pipe (in SOFs)
 *     - default value : 0 = not used (no delay between NAKs)
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 2.25
 *    Fixed port resume occasionally getting stuck in resume signaling
 *    Added compile time configuration for reducing Bulk IN NAK rate
 *  Version 2.24
 *    Updated documentation
 *  Version 2.23
 *    OTG_HS ULPI clock disabled in low power if internal PHY is used 
 *    to enable proper operation of OTG_HS port in FS mode during CPU sleep
 *  Version 2.22
 *    Removed VBUS sensing handling as OTG is not used
 *  Version 2.21
 *    Removed CMSIS-RTOS dependency
 *    Added timeout to endless loops
 *    Disabled DMA capability
 *    Used register and bit definitions provided by ST
 *  Version 2.20
 *    On-chip PHY powered down if external ULPI PHY is used
 *  Version 2.19
 *    DMA configuration added to RTE_Device.h
 *    Corrected wrong result of PipeTransferGetResult for In Pipe transfers
 *    in DMA mode
 *    Added check to PipeTransfer function for 4-byte data alignment if DMA is used
 *  Version 2.18
 *    Removed interrupt priority handling
 *  Version 2.17
 *    Added DMA usage to reduce CPU usage for USB.
 *    Renamed externally overridable setting for maximum number of pipes used
 *    from USBH_MAX_PIPE_NUM to USBH1_MAX_PIPE_NUM.
 *  Version 2.16
 *    Corrected speed setting for On-chip Full-speed PHY.
 *  Version 2.15
 *    Corrected multiple packet sending and PING functionality.
 *    Corrected low-speed device on high-speed port functionality.
 *  Version 2.14
 *    Corrected PowerControl function for:
 *      - Unconditional Power Off
 *      - Conditional Power full (driver must be initialized)
 *  Version 2.13
 *    STM32CubeMX generated code can also be used to configure the driver.
 *  Version 2.12
 *    Removed global variable otg_hs_state
 *  Version 2.11
 *    PowerControl for Power OFF and Uninitialize functions made unconditional
 *  Version 2.10
 *    Added pipe bus error signaling of active pipes on port disconnect
 *  Version 2.9
 *    Corrected number of bytes transferred handling on transfers with error
 *  Version 2.8
 *    Corrected signaling of STALL handshake
 *  Version 2.7
 *    Added overcurrent state functionality (without event)
 *  Version 2.6
 *    Controller init/deinit moved to PowerControl function
 *  Version 2.5
 *    Fixed interrupt pipe transfer interval and transfer done
 *    Fixed PING only in High-speed
 *  Version 2.4
 *    Fixed support for Transfers larger then FIFO size
 *  Version 2.3
 *    Limited transfer size to maximum FIFO size
 *    Forced unaligned access for FIFO interactions
 *  Version 2.2
 *    Fixed interrupt IN endpoint handling
 *    Improved bulk endpoint retransfer
 *    Changed default FIFO settings
 *  Version 2.1
 *    Update for USB Host CMSIS Driver API v2.01
 *  Version 2.0
 *    Initial release for USB Host CMSIS Driver API v2.0
 */

/*! \page stm32f4_usbh_hs CMSIS-Driver USBH_HS Setup

The CMSIS-Driver USBH_HS requires:
  - Setup of USB clock to 48MHz (if internal Full-speed Phy is used)
  - Configuration of USB_OTG_HS
  - Optional Configuration for VBUS Power Pin:
    - Configure arbitrary pin in GPIO_Output mode and add User Label: USB_OTG_HS_VBUS_Power
  - Optional Configuration for Overcurrent Pin:
    - Configure arbitrary pin in GPIO_Input mode and add User Label: USB_OTG_HS_Overcurrent
 
\note The User Label name is used to connect the CMSIS-Driver to the GPIO pin.
 
Valid settings for various evaluation boards are listed in the table below:
 
Peripheral Resource     | MCBSTM32F400                  | STM32F4-Discovery | 32F401C-Discovery | 32F429I-Discovery
:-----------------------|:------------------------------|:------------------|:------------------|:------------------
USB_OTG_HS Mode         | External Phy:<b>Host_Only</b> | n/a               | n/a               | n/a 
VBUS Power output pin   | PC2                           | n/a               | n/a               | n/a
Overcurrent input pin   | n/a                           | n/a               | n/a               | n/a
 
For different boards, refer to the hardware schematics to reflect correct setup values.
 
The STM32CubeMX configuration for MCBSTM32F400 with steps for Pinout, Clock, and System Configuration are 
listed below. Enter the values that are marked \b bold.
 
Pinout tab
----------
  1. Configure USBH mode
     - Peripherals \b USB_OTG_HS: External Phy: Mode=<b>Host_Only</b>
  2. Configure USB_OTG_HS_VBUS_Power pin:
     - Click in chip diagram on pin \b PC2 and select \b GPIO_Output
 
Clock Configuration tab
-----------------------
  1. AHB frequency should be higher than 30 MHz
 
Configuration tab
-----------------
  1. Under Connectivity open \b USB_OTG_HS Configuration:
     - DMA Settings: not used
     - <b>GPIO Settings</b>: review settings, no changes required
          Pin Name | Signal on Pin        | GPIO mode | GPIO Pull-up/Pull..| Maximum out | User Label
          :--------|:---------------------|:----------|:-------------------|:------------|:----------
          PA3      | USB_OTG_HS_ULPI_D0   | Alternate | No pull-up and no..| High        |.
          PA5      | USB_OTG_HS_ULPI_CK   | Alternate | No pull-up and no..| High        |.
          PB0      | USB_OTG_HS_ULPI_D1   | Alternate | No pull-up and no..| High        |.
          PB1      | USB_OTG_HS_ULPI_D2   | Alternate | No pull-up and no..| High        |.
          PB5      | USB_OTG_HS_ULPI_D7   | Alternate | No pull-up and no..| High        |.
          PB10     | USB_OTG_HS_ULPI_D3   | Alternate | No pull-up and no..| High        |.
          PB11     | USB_OTG_HS_ULPI_D4   | Alternate | No pull-up and no..| High        |.
          PB12     | USB_OTG_HS_ULPI_D5   | Alternate | No pull-up and no..| High        |.
          PB13     | USB_OTG_HS_ULPI_D6   | Alternate | No pull-up and no..| High        |.
          PC0      | USB_OTG_HS_ULPI_STP  | Alternate | No pull-up and no..| High        |.
          PI11     | USB_OTG_HS_ULPI_DIR  | Alternate | No pull-up and no..| High        |.
          PH4      | USB_OTG_HS_ULPI_NXT  | Alternate | No pull-up and no..| High        |.
     - <b>NVIC Settings</b>: enable interrupts
          Interrupt Table                      | Enable | Preemption Priority | Sub Priority
          :------------------------------------|:-------|:--------------------|:--------------
          USB On The Go HS global interrupt    |\b ON   | 0                   | 0
     - Parameter Settings: not used
     - User Constants: not used
     - Click \b OK to close the USB_OTG_HS Configuration dialog
  2. Under System Core open \b NVIC Configuration and select <b>Code generation</b> tab:
     - Disable <b>Generate IRQ handler</b> for <b>USB On The Go HS global interrupt</b>
  3. Under System open \b GPIO Pin Configuration
     - Enter user label for USB_OTG_HS_Overcurrent pin
          Pin Name | Signal on Pin | GPIO mode        | GPIO Pull-up/Pull..| Maximum out | User Label
          :--------|:--------------|:-----------------|:-------------------|:------------|:----------
          PC2      | n/a           | Output Push Pull | No pull-up and no..| n/a         |\b USB_OTG_HS_VBUS_Power
 
     - Click \b OK to close the Pin Configuration dialog
*/

/*! \cond */

#include <stdint.h>
#include <string.h>

#include "Driver_USBH.h"

#include "OTG_HS_STM32F4xx.h"

#ifndef USBH1_MAX_PIPE_NUM
#define USBH_MAX_PIPE_NUM              (16U)
#else
#define USBH_MAX_PIPE_NUM              (USBH1_MAX_PIPE_NUM)
#endif
#if    (USBH_MAX_PIPE_NUM > 16U)
#error  Too many Pipes, maximum Pipes that this driver supports is 16 !!!
#endif

#ifndef USB_OTG_HS_DMA
#define USBH_USE_DMA                   (0U)
#elif  (USB_OTG_HS_DMA == 1)
#error USBH_HS Driver: DMA not supported!
#else
#define USBH_USE_DMA                   (0U)
#endif

#ifndef USBH1_MANAGE_CACHE
#define USBH_DATA_CACHED               (0U)
#elif  (USBH1_MANAGE_CACHE == 1)
#error USBH_HS Driver: Cache handling not supported!
#else
#define USBH_DATA_CACHED               (0U)
#endif

// FIFO sizes in bytes (total available memory for FIFOs is 4 kB)
#define RX_FIFO_SIZE           (2048U)  // RxFIFO depth is half of max 4 kB
#define TX_FIFO_SIZE_NON_PERI  (1024U)  // Non-periodic Tx FIFO size
#define TX_FIFO_SIZE_PERI      (1024U)  // Periodic Tx FIFO size

#ifndef USBH1_BULK_IN_NAK_DELAY
#define USBH1_BULK_IN_NAK_DELAY   (0U)  // Delay (in SOFs) for new transfer after NAK on Bulk IN pipe
#endif

// Macro definitions for accessing peripheral registers
#define OTG                            (USB_OTG_HS)
#define OTG_HOST                       ((USB_OTG_HostTypeDef *)       ((uint32_t)USB_OTG_HS_PERIPH_BASE + USB_OTG_HOST_BASE))
#define OTG_HPRT                      *(__IO uint32_t *)              ((uint32_t)USB_OTG_HS_PERIPH_BASE + USB_OTG_HOST_PORT_BASE)
#define OTG_PCGCCTL                   *(__IO uint32_t *)              ((uint32_t)USB_OTG_HS_PERIPH_BASE + USB_OTG_PCGCCTL_BASE)
#define OTG_HC_PTR(ch)                 ((USB_OTG_HostChannelTypeDef *)((uint32_t)USB_OTG_HS_PERIPH_BASE + USB_OTG_HOST_CHANNEL_BASE + ((ch) * USB_OTG_HOST_CHANNEL_SIZE)))
#define USB_IRQn                       (OTG_HS_IRQn)

#if (USBH_USE_DMA == 0U)                // If DMA is not used (Slave Mode)
static const uint32_t *OTG_DFIFO[] = { (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE                            ),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + ( 1U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + ( 2U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + ( 3U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + ( 4U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + ( 5U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + ( 6U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + ( 7U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + ( 8U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + ( 9U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + (10U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + (11U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + (12U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + (13U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + (14U * USB_OTG_FIFO_SIZE)),
                                       (uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_FIFO_BASE + (15U * USB_OTG_FIFO_SIZE))
                                     };
#endif


// Definitions used to remove instances from source code
#define HHCD                            hhcd_USB_OTG_HS
#define HHCD_INSTANCE                   USB_OTG_HS
#define ROLE                            otg_hs_role

#ifdef RTE_DEVICE_FRAMEWORK_CUBE_MX
#ifdef MX_USB_OTG_HS_HOST
extern HCD_HandleTypeDef hhcd_USB_OTG_HS;
#endif
#endif


// USBH Driver *****************************************************************

#define ARM_USBH_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(2,25)

// Driver Version
static const ARM_DRIVER_VERSION usbh_driver_version = { ARM_USBH_API_VERSION, ARM_USBH_DRV_VERSION };

// Driver Capabilities
static const ARM_USBH_CAPABILITIES usbh_driver_capabilities = {
  0x0001U,      // Root HUB available Ports Mask
  0U,           // Automatic SPLIT packet handling
  1U,           // Signal Connect event
  1U,           // Signal Disconnect event
  0U            // Signal Overcurrent event
#if (defined(ARM_USBH_API_VERSION) && (ARM_USBH_API_VERSION >= 0x202U))
, 0U            // Reserved
#endif
};

typedef struct {                        // Pipe structure definition
           uint32_t  packet;
           uint8_t  *data;
           uint32_t  num;
  volatile uint32_t  num_transferred_total;
  volatile uint32_t  num_transferring;
           uint16_t  ep_max_packet_size;
           uint16_t  interval_reload;
  volatile uint16_t  interval;
           uint8_t   pad0;
           uint8_t   ep_dir;
           uint8_t   ep_type;
  volatile uint8_t   active;
  volatile uint8_t   in_progress;
  volatile uint8_t   event;
} PIPE_t;

static ARM_USBH_SignalPortEvent_t SignalPortEvent;
static ARM_USBH_SignalPipeEvent_t SignalPipeEvent;

static bool hw_initialized = false;
static bool hw_powered;
static bool port_reset;

// Pipes runtime information
static PIPE_t pipe[USBH_MAX_PIPE_NUM];


// Auxiliary functions

/**
  \fn          uint32_t USBH_CH_GetIndexFromAddress (USB_OTG_HostChannelTypeDef *ptr_ch)
  \brief       Get Index of Channel from it's Address.
  \param[in]   ptr_ch   Pointer to Channel
  \return      Index of the Channel
*/
__STATIC_INLINE uint32_t USBH_CH_GetIndexFromAddress (USB_OTG_HostChannelTypeDef *ptr_ch) {
  return (uint32_t)(ptr_ch - OTG_HC_PTR(0));
}

/**
  \fn          USB_OTG_HostChannelTypeDef *USBH_CH_GetAddressFromIndex (uint32_t index)
  \brief       Get Channel Address from it's Index.
  \param[in]   index    Index of Channel
  \return      Address of the Channel
*/
__STATIC_INLINE USB_OTG_HostChannelTypeDef *USBH_CH_GetAddressFromIndex (uint32_t index) {
  return (OTG_HC_PTR(0) + index);
}

/**
  \fn          USB_OTG_HostChannelTypeDef *USBH_CH_FindFree (void)
  \brief       Find a free Channel.
  \return      Pointer to first free Channel (NULL = no free Channel is available)
*/
__STATIC_INLINE USB_OTG_HostChannelTypeDef *USBH_CH_FindFree (void) {
  USB_OTG_HostChannelTypeDef *ptr_ch;
  uint32_t                    i;

  ptr_ch = OTG_HC_PTR(0);

  for (i = 0U; i < USBH_MAX_PIPE_NUM; i++) {
    if ((ptr_ch->HCCHAR & 0x3FFFFFFFU) == 0U) { return ptr_ch; }
    ptr_ch++;
  }

  return NULL;
}

/**
  \fn          bool USBH_CH_Disable (USB_OTG_HostChannelTypeDef *ptr_ch)
  \brief       Disable Channel.
  \param[in]   ptr_ch   Pointer to Channel
  \return      true = success, false = fail
*/
__STATIC_INLINE bool USBH_CH_Disable (USB_OTG_HostChannelTypeDef *ptr_ch) {
  uint32_t i;

  if (ptr_ch == 0U) { return false; }

  ptr_ch->HCTSIZ &= ~USB_OTG_HCTSIZ_DOPING;

  if ((ptr_ch->HCINT & USB_OTG_HCINT_CHH) != 0U) { return true; }

  if ((ptr_ch->HCCHAR & USB_OTG_HCCHAR_CHENA) != 0U) {
    ptr_ch->HCINTMSK = 0U;
    HAL_Delay(1U);
    if ((ptr_ch->HCINT & USB_OTG_HCINT_NAK) != 0U) {
      ptr_ch->HCINT  =  0x7FFU;
      return true;
    }
    ptr_ch->HCINT  =  0x7FFU;
    ptr_ch->HCCHAR =  ptr_ch->HCCHAR | USB_OTG_HCCHAR_CHENA | USB_OTG_HCCHAR_CHDIS;
    for (i = 0U; i < 1000U; i++) {
      if ((ptr_ch->HCINT & USB_OTG_HCINT_CHH) != 0U) {
        ptr_ch->HCINT = 0x7FFU;
        return true;
      }
    }
    return false;
  }

  return true;
}

/**
  \fn          bool USBH_HW_StartTransfer (PIPE_t *ptr_pipe, USB_OTG_HostChannelTypeDef *ptr_ch)
  \brief       Start transfer on Pipe.
  \param[in]   ptr_pipe Pointer to Pipe
  \param[in]   ptr_ch   Pointer to Channel
  \return      true = success, false = fail
*/
static bool USBH_HW_StartTransfer (PIPE_t *ptr_pipe, USB_OTG_HostChannelTypeDef *ptr_ch) {
  uint32_t  hcchar;
  uint32_t  hctsiz;
  uint32_t  hcintmsk;
  uint32_t  num_to_transfer;
#if (USBH_USE_DMA == 0U)                        // If DMA is not used (Slave Mode)
  uint32_t  txsts;
  uint32_t  pckt_num;
  uint32_t  data_num;
  uint32_t  max_pckt_size;
  uint32_t  max_data;
  uint32_t  max_num_pckt;
  uint8_t  *ptr_src;
  uint32_t *ptr_dest;
  uint16_t  cnt;
  uint8_t   out;
#else                                           // If DMA is used
#if ((USBH_DATA_CACHED == 1U) && (__DCACHE_PRESENT == 1U))
  uint8_t   out;
#endif
#endif

  if (ptr_pipe == 0U)                        { return false; }
  if (ptr_ch   == 0U)                        { return false; }
  if ((OTG_HPRT & USB_OTG_HPRT_PCSTS) == 0U) { return false; }

  hcchar   = ptr_ch->HCCHAR;                    // Read channel characteristics
  hctsiz   = ptr_ch->HCTSIZ;                    // Read channel size info
  hcintmsk = 0U;
#if (USBH_USE_DMA == 0U)                        // If DMA is not used (Slave Mode)
  cnt      = 0U;
  out      = 0U;
#else                                           // If DMA is used
#if ((USBH_DATA_CACHED == 1U) && (__DCACHE_PRESENT == 1U))
  out      = 0U;
#endif
#endif

  // Prepare transfer
                                                // Prepare HCCHAR register
  hcchar       &= (USB_OTG_HCCHAR_ODDFRM    |   // Keep ODDFRM
                   USB_OTG_HCCHAR_DAD       |   // Keep DAD
                   USB_OTG_HCCHAR_MC        |   // Keep MCNT
                   USB_OTG_HCCHAR_EPTYP     |   // Keep EPTYP
                   USB_OTG_HCCHAR_LSDEV     |   // Keep LSDEV
                   USB_OTG_HCCHAR_EPNUM     |   // Keep EPNUM
                   USB_OTG_HCCHAR_MPSIZ)    ;   // Keep MPSIZ
  hctsiz       &=  USB_OTG_HCTSIZ_DPID;         // Keep DPID
#if (USBH_USE_DMA != 0U)                        // If DMA is used
  switch (ptr_pipe->packet & ARM_USBH_PACKET_TOKEN_Msk) {
    case ARM_USBH_PACKET_IN:
      hcchar   |=  USB_OTG_HCCHAR_EPDIR;
      hcintmsk  = (USB_OTG_HCINTMSK_DTERRM  |
                   USB_OTG_HCINTMSK_BBERRM  |
                   USB_OTG_HCINTMSK_TXERRM  |
                   USB_OTG_HCINTMSK_STALLM  |
                   USB_OTG_HCINTMSK_XFRCM)  ;
      break;
    case ARM_USBH_PACKET_OUT:
      hcchar   &= ~USB_OTG_HCCHAR_EPDIR;
      hcintmsk  = (USB_OTG_HCINTMSK_TXERRM  |
                   USB_OTG_HCINTMSK_STALLM  |
                   USB_OTG_HCINTMSK_XFRCM)  ;
#if ((USBH_DATA_CACHED == 1U) && (__DCACHE_PRESENT == 1U))
      out       =  1U;
#endif
      break;
    case ARM_USBH_PACKET_SETUP:
      hcchar   &= ~USB_OTG_HCCHAR_EPDIR;
      hcintmsk  = (USB_OTG_HCINTMSK_TXERRM  |
                   USB_OTG_HCINTMSK_XFRCM)  ;
      hctsiz   |=  USB_OTG_HCTSIZ_DPID;
#if ((USBH_DATA_CACHED == 1U) && (__DCACHE_PRESENT == 1U))
      out       =  1U;
#endif
      break;
    default:
      return false;
  }
  if (ptr_pipe->ep_type == ARM_USB_ENDPOINT_INTERRUPT) {
    hcintmsk   |=  USB_OTG_HCINTMSK_NAKM    ;   // Enable NAK for interrupt endpoint as DMA does not handle periodic transmissions
  }

  num_to_transfer = ptr_pipe->num - ptr_pipe->num_transferred_total;
  if (num_to_transfer > (1023U*512U)) {
    // Limit number of bytes to transfer to maximum that can fit into XFRSIZ field of HCTSIZ register
    num_to_transfer = 1023U*512U;
  }
  if (ptr_pipe->ep_type == ARM_USB_ENDPOINT_INTERRUPT) {
    if ((OTG_HOST->HFNUM & 1U) != 0U) {
      hcchar &= ~USB_OTG_HCCHAR_ODDFRM;
    } else {
      hcchar |=  USB_OTG_HCCHAR_ODDFRM;
    }
  }
#else                                           // If DMA is not used (Slave Mode)
  if ((ptr_pipe->packet & ARM_USBH_PACKET_PING) != 0U) {
    hcchar     &= ~USB_OTG_HCCHAR_EPDIR;
    hcintmsk    =  USB_OTG_HCINTMSK_TXERRM |
                   USB_OTG_HCINTMSK_ACKM   |
                   USB_OTG_HCINTMSK_NAKM   |
                   USB_OTG_HCINTMSK_STALLM |
                   USB_OTG_HCINTMSK_XFRCM  ;
    hctsiz     |=  USB_OTG_HCTSIZ_DOPING;       // Do PING
    out         =  1U;
    num_to_transfer = 0U;
  } else {
    switch (ptr_pipe->packet & ARM_USBH_PACKET_TOKEN_Msk) {
      case ARM_USBH_PACKET_IN:
        hcchar   |=  USB_OTG_HCCHAR_EPDIR;
        hcintmsk  = (USB_OTG_HCINTMSK_DTERRM  |
                     USB_OTG_HCINTMSK_BBERRM  |
                     USB_OTG_HCINTMSK_TXERRM  |
                     USB_OTG_HCINTMSK_ACKM    |
                     USB_OTG_HCINTMSK_NAKM    |
                     USB_OTG_HCINTMSK_STALLM  |
                     USB_OTG_HCINTMSK_XFRCM)  ;
        break;
      case ARM_USBH_PACKET_OUT:
        hcchar   &= ~USB_OTG_HCCHAR_EPDIR;
        hcintmsk  = (USB_OTG_HCINTMSK_TXERRM  |
                     USB_OTG_HCINTMSK_NYET    |
//                   USB_OTG_HCINTMSK_ACKM    | // After ACK there must be other relevant interrupt so ACK is ignored
                     USB_OTG_HCINTMSK_NAKM    |
                     USB_OTG_HCINTMSK_STALLM  |
                     USB_OTG_HCINTMSK_XFRCM)  ;
        out       =  1U;
        break;
      case ARM_USBH_PACKET_SETUP:
        hcchar   &= ~USB_OTG_HCCHAR_EPDIR;
        hcintmsk  = (USB_OTG_HCINTMSK_TXERRM  |
                     USB_OTG_HCINTMSK_XFRCM)  ;
        hctsiz   |=  USB_OTG_HCTSIZ_DPID;
        out       =  1U;
        break;
      default:
        return false;
    }

    num_to_transfer = ptr_pipe->num - ptr_pipe->num_transferred_total;
    if (out != 0U) {
      txsts = OTG->HNPTXSTS;                    // Read non-periodic FIFO status
    } else {
      txsts = 0U;
    }
    switch (ptr_pipe->ep_type) {
      case ARM_USB_ENDPOINT_CONTROL:
      case ARM_USB_ENDPOINT_BULK:
        break;
      case ARM_USB_ENDPOINT_ISOCHRONOUS:
      case ARM_USB_ENDPOINT_INTERRUPT:
        if ((OTG_HOST->HFNUM & 1U) != 0U) {
          hcchar &= ~USB_OTG_HCCHAR_ODDFRM;
        } else {
          hcchar |=  USB_OTG_HCCHAR_ODDFRM;
        }
        break;
    }
    if (out != 0U) {
      // For packet OUT/SETUP limit number of bytes to send to maximum FIFO size
      // and maximum number of packets
      max_pckt_size =  ptr_pipe->ep_max_packet_size;
      max_data      = (txsts & 0x0000FFFFU) <<  2;
      max_num_pckt  = (txsts & 0x00FF0000U) >> 16;
      if (num_to_transfer > max_data) {
        num_to_transfer = max_data;
      }
      data_num = num_to_transfer;
      for (pckt_num = 1; data_num > max_pckt_size; pckt_num++) {
        data_num -= max_pckt_size;
      }
      if (pckt_num > max_num_pckt) {
        pckt_num = max_num_pckt;
      }

      if (num_to_transfer > (pckt_num * max_pckt_size)) {
        num_to_transfer = pckt_num * max_pckt_size;
      }
      cnt = (uint16_t)((num_to_transfer + 3U) / 4U);
    }
  }
#endif

  hcchar &= ~USB_OTG_HCCHAR_CHDIS;
  hcchar |=  USB_OTG_HCCHAR_CHENA;

                                                // Prepare HCTSIZ register
  switch (ptr_pipe->packet & ARM_USBH_PACKET_DATA_Msk) {
    case ARM_USBH_PACKET_DATA0:
      hctsiz   &= ~USB_OTG_HCTSIZ_DPID;
      hctsiz   |=  USB_OTG_HCTSIZ_DPID_0;
      break;
    case ARM_USBH_PACKET_DATA1:
      hctsiz   &= ~USB_OTG_HCTSIZ_DPID;
      hctsiz   |=  USB_OTG_HCTSIZ_DPID_1;
      break;
    default:
      break;
  }

                                                // Prepare HCTSIZ register
  if (num_to_transfer != 0U) {                  // Normal packet
                                                // Prepare PKTCNT field
    hctsiz |= ((num_to_transfer + ptr_pipe->ep_max_packet_size - 1U) / ptr_pipe->ep_max_packet_size) << 19;
    hctsiz |=   num_to_transfer;                // Prepare XFRSIZ field
  } else {                                      // Zero length packet
    hctsiz |=   1U << 19;                       // Prepare PKTCNT field
    hctsiz |=   0U;                             // Prepare XFRSIZ field
  }

#if (USBH_USE_DMA != 0U)                        // If DMA is used
  ptr_pipe->num_transferring = num_to_transfer;
#if ((USBH_DATA_CACHED == 1U) && (__DCACHE_PRESENT == 1U))
  if ((SCB->CCR & SCB_CCR_DC_Msk) != 0U) {      // If Data Cache is enabled
    if ((out != 0U) && (num_to_transfer > 0U)) {
      SCB_CleanDCache_by_Addr ((uint32_t *)((uint32_t)(ptr_pipe->data + ptr_pipe->num_transferred_total) & (~0x1FU)), (int32_t)num_to_transfer + 31);
    }
  }
#endif
  ptr_ch->HCDMA    = (uint32_t)(ptr_pipe->data + ptr_pipe->num_transferred_total);
  ptr_ch->HCINTMSK = hcintmsk;                  // Enable channel interrupts
  ptr_ch->HCTSIZ   = hctsiz;                    // Write ch transfer size
  ptr_ch->HCCHAR   = hcchar;                    // Write ch characteristics
#else                                           // If DMA is not used (Slave Mode)
  if (cnt != 0U) {
    ptr_src  = ptr_pipe->data + ptr_pipe->num_transferred_total;
    ptr_dest = (uint32_t *)((uint32_t)OTG_DFIFO[USBH_CH_GetIndexFromAddress (ptr_ch)]);
  } else {
    ptr_src  = NULL;
    ptr_dest = NULL;
  }

  if (out != 0U) {
    // For OUT/SETUP transfer num_transferring represents num of bytes to be sent
    ptr_pipe->num_transferring = num_to_transfer;
  } else {
    // For IN transfer num_transferring represents num of bytes received (handled in IRQ)
    ptr_pipe->num_transferring = 0U;
  }
  NVIC_DisableIRQ (USB_IRQn);                   // Disable OTG interrupt
  ptr_ch->HCINTMSK = hcintmsk;                  // Enable channel interrupts
  ptr_ch->HCTSIZ   = hctsiz;                    // Write ch transfer size
  ptr_ch->HCCHAR   = hcchar;                    // Write ch characteristics
  while (cnt != 0U) {                           // Load data
    *ptr_dest = __UNALIGNED_UINT32_READ(ptr_src);
    ptr_src  += 4U;
    cnt--;
  }
  NVIC_EnableIRQ  (USB_IRQn);                   // Enable OTG interrupt
#endif

  return true;
}


// USBH Driver functions

/**
  \fn          ARM_DRIVER_VERSION USBH_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION USBH_GetVersion (void) { return usbh_driver_version; }

/**
  \fn          ARM_USBH_CAPABILITIES USBH_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_USBH_CAPABILITIES
*/
static ARM_USBH_CAPABILITIES USBH_GetCapabilities (void) { return usbh_driver_capabilities; }

/**
  \fn          int32_t USBH_Initialize (ARM_USBH_SignalPortEvent_t cb_port_event,
                                        ARM_USBH_SignalPipeEvent_t cb_pipe_event)
  \brief       Initialize USB Host Interface.
  \param[in]   cb_port_event  Pointer to \ref ARM_USBH_SignalPortEvent
  \param[in]   cb_pipe_event  Pointer to \ref ARM_USBH_SignalPipeEvent
  \return      \ref execution_status
*/
static int32_t USBH_Initialize (ARM_USBH_SignalPortEvent_t cb_port_event,
                                ARM_USBH_SignalPipeEvent_t cb_pipe_event) {

  if (hw_initialized) { return ARM_DRIVER_OK; }

  SignalPortEvent = cb_port_event;
  SignalPipeEvent = cb_pipe_event;

  ROLE = ARM_USB_ROLE_HOST;

#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
  OTG_HS_PinsConfigure (ARM_USB_PIN_DP | ARM_USB_PIN_DM | ARM_USB_PIN_OC | ARM_USB_PIN_VBUS);
#else
  HHCD.Instance = HHCD_INSTANCE;
#endif

  hw_powered = false;
  port_reset = false;

  // Clear all pipes information
  memset((void *)((uint32_t)pipe), 0, sizeof(pipe));

  hw_initialized = true;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_Uninitialize (void)
  \brief       De-initialize USB Host Interface.
  \return      \ref execution_status
*/
static int32_t USBH_Uninitialize (void) {

#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
  OTG_HS_PinsUnconfigure (ARM_USB_PIN_DP | ARM_USB_PIN_DM | ARM_USB_PIN_OC | ARM_USB_PIN_VBUS);
#else
  HHCD.Instance = NULL;
#endif

  ROLE = ARM_USB_ROLE_NONE;

  hw_initialized = false;
  hw_powered     = false;
  port_reset     = false;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_PowerControl (ARM_POWER_STATE state)
  \brief       Control USB Host Interface Power.
  \param[in]   state  Power state
  \return      \ref execution_status
*/
static int32_t USBH_PowerControl (ARM_POWER_STATE state) {
  uint32_t tout_ms;

  if ((state != ARM_POWER_OFF)  && 
      (state != ARM_POWER_FULL) && 
      (state != ARM_POWER_LOW)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  switch (state) {
    case ARM_POWER_OFF:
#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
      NVIC_DisableIRQ      (USB_IRQn);                  // Disable interrupt
      NVIC_ClearPendingIRQ (USB_IRQn);                  // Clear pending interrupt
#else
      if (HHCD.Instance != NULL) {
        HAL_HCD_MspDeInit(&HHCD);
      }
#endif

      hw_powered = false;
      port_reset = false;
      memset((void *)((uint32_t)pipe), 0, sizeof(pipe));

#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
#if  (MX_USB_OTG_HS_EXT_HS_PHY == 1)                    // External ULPI High-speed PHY
      __USB_OTG_HS_ULPI_CLK_DISABLE();
#endif
      __USB_OTG_HS_CLK_DISABLE();
#endif
      break;

    case ARM_POWER_FULL:
      if (!hw_initialized) { return ARM_DRIVER_ERROR; }
      if ( hw_powered)     { return ARM_DRIVER_OK;    }

#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
      __USB_OTG_HS_CLK_ENABLE();
#if   (MX_USB_OTG_HS_EXT_HS_PHY == 1)                   // External ULPI High-speed PHY
      __USB_OTG_HS_ULPI_CLK_ENABLE();
#endif
#if   (MX_USB_OTG_HS_INT_FS_PHY == 1)                   // Internal Full-speed PHY
      __USB_OTG_HS_ULPI_CLK_DISABLE();                  // Disable OTG_HS ULPI clock
      __HAL_RCC_USB_OTG_HS_ULPI_CLK_SLEEP_DISABLE();    // Disable OTG_HS ULPI clock in low power mode
#endif
#else
      if (HHCD.Instance != NULL) {
        HAL_HCD_MspInit(&HHCD);
      }
#endif

      __HAL_RCC_USB_OTG_HS_FORCE_RESET();               // Reset OTG module
      HAL_Delay(1U);
      __HAL_RCC_USB_OTG_HS_RELEASE_RESET();             // Clear reset of OTG module
      HAL_Delay(1U);

#if   (MX_USB_OTG_HS_INT_FS_PHY == 1)                   // Internal Full-speed PHY
      OTG_PCGCCTL   &= ~USB_OTG_PCGCCTL_STOPCLK;        // Start PHY clock
      OTG->GCCFG    |=  USB_OTG_GCCFG_PWRDWN;           // Power Full-speed transceiver
      OTG->GUSBCFG  |= (USB_OTG_GUSBCFG_PHYSEL  |       // Full-speed transceiver
                        USB_OTG_GUSBCFG_PHYLPCS);       // 48 MHz external clock
#elif (MX_USB_OTG_HS_EXT_HS_PHY == 1)                   // External ULPI High-speed PHY
      OTG->GCCFG    &= ~USB_OTG_GCCFG_PWRDWN;           // Power down Full-speed transceiver
      OTG->GUSBCFG  &=~(USB_OTG_GUSBCFG_TSDPS      |    // Data line pulsing using utmi_txvalid (default)
                        USB_OTG_GUSBCFG_ULPIFSLS   |    // ULPI interface
                        USB_OTG_GUSBCFG_PHYSEL     |    // High-speed transceiver
                        USB_OTG_GUSBCFG_ULPIEVBUSI);    // ULPI int VBUS indicator
      OTG->GUSBCFG  |=  USB_OTG_GUSBCFG_ULPIEVBUSD ;    // ULPI ext VBUS drive
#endif

      // Wait until AHB Master state machine is in the idle condition, max 100 ms
      tout_ms = 100U;
      while ((OTG->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL) == 0U) {
        if (tout_ms == 0U) {
          return ARM_DRIVER_ERROR_TIMEOUT;
        }
        tout_ms--;
        HAL_Delay(1U);
      }

      OTG->GRSTCTL  |=  USB_OTG_GRSTCTL_CSRST;          // Core soft reset

      // Wait Core reset to finish, max 100 ms
      tout_ms = 100U;
      while ((OTG->GRSTCTL & USB_OTG_GRSTCTL_CSRST) != 0U) {
        if (tout_ms == 0U) {
          return ARM_DRIVER_ERROR_TIMEOUT;
        }
        tout_ms--;
        HAL_Delay(1U);
      }
      HAL_Delay(1U);

      // Wait until AHB Master state machine is in the idle condition, max 100 ms
      tout_ms = 100U;
      while ((OTG->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL) == 0U) {
        if (tout_ms == 0U) {
          return ARM_DRIVER_ERROR_TIMEOUT;
        }
        tout_ms--;
        HAL_Delay(1U);
      }

      port_reset = false;
      memset((void *)((uint32_t)pipe), 0, sizeof(pipe));

      if (((OTG->GUSBCFG & USB_OTG_GUSBCFG_FHMOD) == 0U) || ((OTG->GUSBCFG & USB_OTG_GUSBCFG_FDMOD) != 0U)) {
        OTG->GUSBCFG &= ~USB_OTG_GUSBCFG_FDMOD;         // Clear force device mode
        OTG->GUSBCFG |=  USB_OTG_GUSBCFG_FHMOD;         // Force host mode
        HAL_Delay(100U);
      }

      // Rx FIFO setting
      OTG->GRXFSIZ   = (RX_FIFO_SIZE/4U);
      // Non-periodic Tx FIFO setting
      OTG->DIEPTXF0_HNPTXFSIZ = ((TX_FIFO_SIZE_NON_PERI/4U) << 16) |  (RX_FIFO_SIZE / 4U);
      // Periodic Tx FIFO setting
      OTG->HPTXFSIZ  = ((TX_FIFO_SIZE_PERI    /4U) << 16) | ((RX_FIFO_SIZE + TX_FIFO_SIZE_NON_PERI) / 4U);

      OTG_HOST->HAINTMSK  = (1U << USBH_MAX_PIPE_NUM) - 1U;     // Enable channel interrupts
      OTG->GINTMSK   = (USB_OTG_GINTMSK_DISCINT |       // Unmask interrupts
                        USB_OTG_GINTMSK_HCIM    |
                        USB_OTG_GINTMSK_PRTIM   |
                        USB_OTG_GINTMSK_RXFLVLM |
                        USB_OTG_GINTMSK_SOFM)   ;

#if (USBH_USE_DMA != 0U)                                // If DMA is used
      OTG->GAHBCFG  |=  USB_OTG_GAHBCFG_DMAEN;          // Enable DMA
#endif
      OTG->GAHBCFG  |=  USB_OTG_GAHBCFG_GINT;           // Enable interrupts

      hw_powered     =  true;                           // Set powered flag
#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
      NVIC_EnableIRQ   (USB_IRQn);                      // Enable interrupt
#endif
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_PortVbusOnOff (uint8_t port, bool vbus)
  \brief       Root HUB Port VBUS on/off.
  \param[in]   port  Root HUB Port Number
  \param[in]   vbus
                - \b false VBUS off
                - \b true  VBUS on
  \return      \ref execution_status
*/
static int32_t USBH_PortVbusOnOff (uint8_t port, bool vbus) {

  if (!hw_powered) { return ARM_DRIVER_ERROR;           }
  if (port != 0U)  { return ARM_DRIVER_ERROR_PARAMETER; }

  if (vbus != 0U) {                                     // If VBUS power on
    OTG_HPRT |=  USB_OTG_HPRT_PPWR;                     // Port power on
    OTG_HS_PinVbusOnOff (true);                         // VBUS pin on
  } else {                                              // If VBUS power off
    OTG_HS_PinVbusOnOff (false);                        // VBUS pin off
    OTG_HPRT &= ~USB_OTG_HPRT_PPWR;                     // Port power off
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_PortReset (uint8_t port)
  \brief       Do Root HUB Port Reset.
  \param[in]   port  Root HUB Port Number
  \return      \ref execution_status
*/
static int32_t USBH_PortReset (uint8_t port) {
  uint32_t hprt;

  if (!hw_powered) { return ARM_DRIVER_ERROR;           }
  if (port != 0U)  { return ARM_DRIVER_ERROR_PARAMETER; }

  port_reset = true;
  hprt  =  OTG_HPRT;
  hprt &= ~USB_OTG_HPRT_PENA;                           // Disable port
  hprt |=  USB_OTG_HPRT_PRST;                           // Port reset
  OTG_HPRT = hprt;
  HAL_Delay(11U);
  hprt &= ~USB_OTG_HPRT_PRST;                           // Clear port reset
  OTG_HPRT = hprt;
  HAL_Delay(50U);
  if (port_reset == true) {
    port_reset = false;
    return ARM_DRIVER_ERROR;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_PortSuspend (uint8_t port)
  \brief       Suspend Root HUB Port (stop generating SOFs).
  \param[in]   port  Root HUB Port Number
  \return      \ref execution_status
*/
static int32_t USBH_PortSuspend (uint8_t port) {

  if (!hw_powered) { return ARM_DRIVER_ERROR;           }
  if (port != 0U)  { return ARM_DRIVER_ERROR_PARAMETER; }

  OTG_HPRT |=  USB_OTG_HPRT_PSUSP;                      // Port suspend

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_PortResume (uint8_t port)
  \brief       Resume Root HUB Port (start generating SOFs).
  \param[in]   port  Root HUB Port Number
  \return      \ref execution_status
*/
static int32_t USBH_PortResume (uint8_t port) {

  if (!hw_powered) { return ARM_DRIVER_ERROR;           }
  if (port != 0U)  { return ARM_DRIVER_ERROR_PARAMETER; }

  OTG_HPRT |=  USB_OTG_HPRT_PRES;                       // Port resume
  HAL_Delay(25U);                                       // Minimum 20 ms
  OTG_HPRT &= ~USB_OTG_HPRT_PRES;                       // Clear port resume

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_USBH_PORT_STATE USBH_PortGetState (uint8_t port)
  \brief       Get current Root HUB Port State.
  \param[in]   port  Root HUB Port Number
  \return      Port State \ref ARM_USBH_PORT_STATE
*/
static ARM_USBH_PORT_STATE USBH_PortGetState (uint8_t port) {
  ARM_USBH_PORT_STATE port_state;
  uint32_t hprt;

  *((volatile uint32_t *)&port_state) = 0U;

  if (!hw_powered) { return port_state; }
  if (port != 0U)  { return port_state; }

  hprt = OTG_HPRT;

  port_state.connected   = (hprt & USB_OTG_HPRT_PCSTS) != 0U;
#ifdef MX_USB_OTG_HS_Overcurrent_Pin
  port_state.overcurrent = OTG_HS_PinGetOC ();
#elif (MX_USB_OTG_HS_EXT_HS_PHY == 1)   // External ULPI High-speed PHY
  port_state.overcurrent = (hprt & USB_OTG_HPRT_POCA) != 0U;
#else
  port_state.overcurrent = 0U;
#endif
  switch ((hprt & USB_OTG_HPRT_PSPD_Msk) >> USB_OTG_HPRT_PSPD_Pos) {
    case 0:                                             // High speed
     port_state.speed = ARM_USB_SPEED_HIGH;
     break;
    case 1:                                             // Full speed
     port_state.speed = ARM_USB_SPEED_FULL;
     break;
    case 2:                                             // Low speed
     port_state.speed = ARM_USB_SPEED_LOW;
     break;
    default:
     break;
  }

  return port_state;
}

/**
  \fn          ARM_USBH_PIPE_HANDLE USBH_PipeCreate (uint8_t  dev_addr,
                                                     uint8_t  dev_speed,
                                                     uint8_t  hub_addr,
                                                     uint8_t  hub_port,
                                                     uint8_t  ep_addr,
                                                     uint8_t  ep_type,
                                                     uint16_t ep_max_packet_size,
                                                     uint8_t  ep_interval)
  \brief       Create Pipe in System.
  \param[in]   dev_addr   Device Address
  \param[in]   dev_speed  Device Speed
  \param[in]   hub_addr   Hub Address
  \param[in]   hub_port   Hub Port
  \param[in]   ep_addr    Endpoint Address
                - ep_addr.0..3: Address
                - ep_addr.7:    Direction
  \param[in]   ep_type    Endpoint Type (ARM_USB_ENDPOINT_xxx)
  \param[in]   ep_max_packet_size Endpoint Maximum Packet Size
  \param[in]   ep_interval        Endpoint Polling Interval
  \return      Pipe Handle \ref ARM_USBH_PIPE_HANDLE
*/
static ARM_USBH_PIPE_HANDLE USBH_PipeCreate (uint8_t dev_addr, uint8_t dev_speed, uint8_t hub_addr, uint8_t hub_port, uint8_t ep_addr, uint8_t ep_type, uint16_t ep_max_packet_size, uint8_t  ep_interval) {
  PIPE_t                     *ptr_pipe;
  USB_OTG_HostChannelTypeDef *ptr_ch;

  (void)hub_addr;
  (void)hub_port;

  if (!hw_powered) { return 0U; }

  ptr_ch = USBH_CH_FindFree ();                 // Find free Channel
  if (ptr_ch == 0U) { return 0U; }              // If no free

  ptr_pipe = (PIPE_t *)((uint32_t)&pipe[USBH_CH_GetIndexFromAddress (ptr_ch)]);

  memset((void *)ptr_pipe, 0, sizeof(PIPE_t));  // Clear Pipes runtime information

  // Fill in all fields of Endpoint Descriptor
  ptr_ch->HCCHAR = (((uint32_t)ep_max_packet_size << USB_OTG_HCCHAR_MPSIZ_Pos) & USB_OTG_HCCHAR_MPSIZ_Msk) |
                   (((uint32_t)ep_addr            << USB_OTG_HCCHAR_EPNUM_Pos) & USB_OTG_HCCHAR_EPNUM_Msk) |
                   (USB_OTG_HCCHAR_EPDIR * (((ep_addr >> 7) & 0x0001U) == 0U))                             |
                   (USB_OTG_HCCHAR_LSDEV * (dev_speed == ARM_USB_SPEED_LOW)  )                             |
                   (((uint32_t)ep_type            << USB_OTG_HCCHAR_EPTYP_Pos) & USB_OTG_HCCHAR_EPTYP_Msk) |
                   (((uint32_t)dev_addr           << USB_OTG_HCCHAR_DAD_Pos  ) & USB_OTG_HCCHAR_DAD_Msk)   ;

  // Store Pipe settings
  ptr_pipe->ep_max_packet_size = ep_max_packet_size;
  ptr_pipe->ep_type            = ep_type;
  ptr_pipe->ep_dir             = (ep_addr >> 7) & 1U;
  switch (ep_type) {
    case ARM_USB_ENDPOINT_CONTROL:
    case ARM_USB_ENDPOINT_BULK:
      break;
    case ARM_USB_ENDPOINT_ISOCHRONOUS:
    case ARM_USB_ENDPOINT_INTERRUPT:
      if (dev_speed == ARM_USB_SPEED_HIGH) {
        if ((ep_interval > 0U) && (ep_interval <= 16U)) {
          ptr_pipe->interval_reload = (uint16_t)(1U << (ep_interval - 1U));
        }
      } else if ((dev_speed == ARM_USB_SPEED_FULL) || (dev_speed == ARM_USB_SPEED_LOW)) {
        if (ep_interval > 0U) {
          ptr_pipe->interval_reload = ep_interval;
        }
      }
      ptr_pipe->interval = 0U;
      ptr_ch->HCCHAR |= ((((ep_max_packet_size >> 11) + 1U) & 3U) << USB_OTG_HCCHAR_MC_Pos) & USB_OTG_HCCHAR_MC_Msk;
      break;
  }

  return ((ARM_USBH_EP_HANDLE)ptr_ch);
}

/**
  \fn          int32_t USBH_PipeModify (ARM_USBH_PIPE_HANDLE pipe_hndl,
                                        uint8_t              dev_addr,
                                        uint8_t              dev_speed,
                                        uint8_t              hub_addr,
                                        uint8_t              hub_port,
                                        uint16_t             ep_max_packet_size)
  \brief       Modify Pipe in System.
  \param[in]   pipe_hndl  Pipe Handle
  \param[in]   dev_addr   Device Address
  \param[in]   dev_speed  Device Speed
  \param[in]   hub_addr   Hub Address
  \param[in]   hub_port   Hub Port
  \param[in]   ep_max_packet_size Endpoint Maximum Packet Size
  \return      \ref execution_status
*/
static int32_t USBH_PipeModify (ARM_USBH_PIPE_HANDLE pipe_hndl, uint8_t dev_addr, uint8_t dev_speed, uint8_t hub_addr, uint8_t hub_port, uint16_t ep_max_packet_size) {
  PIPE_t                     *ptr_pipe;
  USB_OTG_HostChannelTypeDef *ptr_ch;
  uint32_t                    hcchar;

  (void)hub_addr;
  (void)hub_port;

  if (!hw_powered)            { return ARM_DRIVER_ERROR;           }
  if (pipe_hndl == 0U)        { return ARM_DRIVER_ERROR_PARAMETER; }

  ptr_ch   = (USB_OTG_HostChannelTypeDef *)(pipe_hndl);
  ptr_pipe = (PIPE_t                     *)((uint32_t)&pipe[USBH_CH_GetIndexFromAddress (ptr_ch)]);
  if (ptr_pipe->active != 0U) { return ARM_DRIVER_ERROR_BUSY;      }

  // Fill in all fields of Endpoint Descriptor
  hcchar  =   ptr_ch->HCCHAR;
  hcchar &= ~(USB_OTG_HCCHAR_MPSIZ |                    // Clear maximum packet size field
              USB_OTG_HCCHAR_LSDEV |                    // Clear device speed bit
              USB_OTG_HCCHAR_DAD)  ;                    // Clear device address field
  hcchar |=  (((uint32_t)ep_max_packet_size << USB_OTG_HCCHAR_MPSIZ_Pos) & USB_OTG_HCCHAR_MPSIZ_Msk) |
             (USB_OTG_HCCHAR_LSDEV * (dev_speed == ARM_USB_SPEED_LOW)  )                             |
             (((uint32_t)dev_addr           << USB_OTG_HCCHAR_DAD_Pos  ) & USB_OTG_HCCHAR_DAD_Msk)   ;
  ptr_ch->HCCHAR = hcchar;                              // Update modified fields

  ptr_pipe->ep_max_packet_size = ep_max_packet_size;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_PipeDelete (ARM_USBH_PIPE_HANDLE pipe_hndl)
  \brief       Delete Pipe from System.
  \param[in]   pipe_hndl  Pipe Handle
  \return      \ref execution_status
*/
static int32_t USBH_PipeDelete (ARM_USBH_PIPE_HANDLE pipe_hndl) {
  PIPE_t                     *ptr_pipe;
  USB_OTG_HostChannelTypeDef *ptr_ch;

  if (!hw_powered)            { return ARM_DRIVER_ERROR;           }
  if (pipe_hndl == 0U)        { return ARM_DRIVER_ERROR_PARAMETER; }

  ptr_ch   = (USB_OTG_HostChannelTypeDef *)(pipe_hndl);
  ptr_pipe = (PIPE_t                     *)((uint32_t)&pipe[USBH_CH_GetIndexFromAddress (ptr_ch)]);
  if (ptr_pipe->active != 0U) { return ARM_DRIVER_ERROR_BUSY;      }

  ptr_ch->HCCHAR   = 0U;
  ptr_ch->HCINT    = 0U;
  ptr_ch->HCINTMSK = 0U;
  ptr_ch->HCTSIZ   = 0U;

  // Clear all fields of Pipe structure
  memset((void *)ptr_pipe, 0, sizeof(PIPE_t));

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_PipeReset (ARM_USBH_PIPE_HANDLE pipe_hndl)
  \brief       Reset Pipe.
  \param[in]   pipe_hndl  Pipe Handle
  \return      \ref execution_status
*/
static int32_t USBH_PipeReset (ARM_USBH_PIPE_HANDLE pipe_hndl) {
  PIPE_t                     *ptr_pipe;
  USB_OTG_HostChannelTypeDef *ptr_ch;

  if (!hw_powered)            { return ARM_DRIVER_ERROR;           }
  if (pipe_hndl == 0U)        { return ARM_DRIVER_ERROR_PARAMETER; }

  ptr_ch   = (USB_OTG_HostChannelTypeDef *)(pipe_hndl);
  ptr_pipe = (PIPE_t                     *)((uint32_t)&pipe[USBH_CH_GetIndexFromAddress (ptr_ch)]);
  if (ptr_pipe->active != 0U) { return ARM_DRIVER_ERROR_BUSY;      }

  ptr_ch->HCINT    = 0U;
  ptr_ch->HCINTMSK = 0U;
  ptr_ch->HCTSIZ   = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USBH_PipeTransfer (ARM_USBH_PIPE_HANDLE pipe_hndl,
                                          uint32_t             packet,
                                          uint8_t             *data,
                                          uint32_t             num)
  \brief       Transfer packets through USB Pipe.
  \param[in]   pipe_hndl  Pipe Handle
  \param[in]   packet     Packet information
  \param[in]   data       Pointer to buffer with data to send or for data to receive
  \param[in]   num        Number of data bytes to transfer
  \return      \ref execution_status
*/
static int32_t USBH_PipeTransfer (ARM_USBH_PIPE_HANDLE pipe_hndl, uint32_t packet, uint8_t *data, uint32_t num) {
  PIPE_t *ptr_pipe;

  if (!hw_powered)                           { return ARM_DRIVER_ERROR;           }
  if (pipe_hndl == 0U)                       { return ARM_DRIVER_ERROR_PARAMETER; }
  if ((OTG_HPRT & USB_OTG_HPRT_PCSTS) == 0U) { return ARM_DRIVER_ERROR;           }

  ptr_pipe = (PIPE_t *)(&pipe[USBH_CH_GetIndexFromAddress ((USB_OTG_HostChannelTypeDef *)(pipe_hndl))]);
  if (ptr_pipe->active != 0U)                { return ARM_DRIVER_ERROR_BUSY;      }

#if (USBH_USE_DMA != 0U)                        // If DMA is used
                                                // Check 4-byte alignment
  if ((((uint32_t)(data)) & 3U) != 0U)       { return ARM_DRIVER_ERROR;           }
#endif

  // Update current transfer information
  ptr_pipe->packet                = packet;
  ptr_pipe->data                  = data;
  ptr_pipe->num                   = num;
  ptr_pipe->num_transferred_total = 0U;
  ptr_pipe->num_transferring      = 0U;
  ptr_pipe->active                = 0U;
  ptr_pipe->in_progress           = 0U;
  ptr_pipe->event                 = 0U;
  if ((ptr_pipe->ep_type == ARM_USB_ENDPOINT_INTERRUPT) && (ptr_pipe->interval != 0U)) {
    // Already active interrupt endpoint (it will restart in IRQ based on interval)
    ptr_pipe->active              = 1U;
  } else {
    ptr_pipe->in_progress         = 1U;
    ptr_pipe->active              = 1U;
    if (USBH_HW_StartTransfer (ptr_pipe, (USB_OTG_HostChannelTypeDef *)pipe_hndl) == 0U) {
      ptr_pipe->in_progress       = 0U;
      ptr_pipe->active            = 0U;
      return ARM_DRIVER_ERROR;
    }
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          uint32_t USBH_PipeTransferGetResult (ARM_USBH_PIPE_HANDLE pipe_hndl)
  \brief       Get result of USB Pipe transfer.
  \param[in]   pipe_hndl  Pipe Handle
  \return      number of successfully transferred data bytes
*/
static uint32_t USBH_PipeTransferGetResult (ARM_USBH_PIPE_HANDLE pipe_hndl) {

  if (pipe_hndl == 0U) { return 0U; }

  return (pipe[USBH_CH_GetIndexFromAddress((USB_OTG_HostChannelTypeDef *)pipe_hndl)].num_transferred_total);
}

/**
  \fn          int32_t USBH_PipeTransferAbort (ARM_USBH_PIPE_HANDLE pipe_hndl)
  \brief       Abort current USB Pipe transfer.
  \param[in]   pipe_hndl  Pipe Handle
  \return      \ref execution_status
*/
static int32_t USBH_PipeTransferAbort (ARM_USBH_PIPE_HANDLE pipe_hndl) {
  PIPE_t *ptr_pipe;

  if (!hw_powered)     { return ARM_DRIVER_ERROR;           }
  if (pipe_hndl == 0U) { return ARM_DRIVER_ERROR_PARAMETER; }

  ptr_pipe = (PIPE_t *)((uint32_t)&pipe[USBH_CH_GetIndexFromAddress ((USB_OTG_HostChannelTypeDef *)(pipe_hndl))]);

  if (ptr_pipe->active != 0U) {
    ptr_pipe->active = 0U;
    if (USBH_CH_Disable((USB_OTG_HostChannelTypeDef *)(pipe_hndl)) == 0U) { return ARM_DRIVER_ERROR; }
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          uint16_t USBH_GetFrameNumber (void)
  \brief       Get current USB Frame Number.
  \return      Frame Number
*/
static uint16_t USBH_GetFrameNumber (void) {

  if (!hw_powered) { return 0U; }

  return ((OTG_HOST->HFNUM >> 3) & 0x7FFU);
}

/**
  \fn          void USBH_HS_IRQ (uint32_t gintsts)
  \brief       USB Host Interrupt Routine (IRQ).
*/
void USBH_HS_IRQ (uint32_t gintsts) {
  PIPE_t                     *ptr_pipe;
  USB_OTG_HostChannelTypeDef *ptr_ch;
  uint32_t                    hprt, haint, hcint, pktcnt, mpsiz;
  uint32_t                    ch;
#if (USBH_USE_DMA != 0U)                                // If DMA is used
  uint32_t                    xfrsiz;
#else                                                   // If DMA is not used (Slave Mode)
  uint32_t                   *dfifo;
  uint8_t                    *ptr_data;
  uint32_t                    grxsts, bcnt, dat, len, len_rest;
#endif
  uint8_t                     hchalt;

  if ((gintsts & USB_OTG_GINTSTS_HPRTINT) != 0U) {      // If host port interrupt
    hprt = OTG_HPRT;
    OTG_HPRT = hprt & (~USB_OTG_HPRT_PENA);             // Leave PENA bit
    if ((hprt & USB_OTG_HPRT_PCDET) != 0U) {            // Port connect detected
      switch ((hprt >> 17) & 3U) {
        case 0:                                         // High-speed detected
          OTG_HOST->HFIR = 60000U / 8U;                 // Number of clocks for micro-SOF
          break;
        case 1:                                         // Full-speed detected
#if  (MX_USB_OTG_HS_EXT_HS_PHY == 1)                    // External ULPI PHY
          OTG_HOST->HFIR = 60000U / 8U;                 // Number of clocks for SOF
#else                                                   // On-chip Full-speed PHY
          OTG_HOST->HFIR = 48000U;                      // Number of clocks for SOF
          OTG_HOST->HCFG = USB_OTG_HCFG_FSLSPCS_0;
#endif
          break;
        case 2:                                         // Low-speed detected
#if  (MX_USB_OTG_HS_EXT_HS_PHY == 1)                    // External ULPI PHY
          OTG_HOST->HFIR = 60000U / 8U;                 // Number of clocks for SOF
#else                                                   // On-chip Full-speed PHY
          OTG_HOST->HFIR = 6000U;                       // Number of clocks for SOF
          OTG_HOST->HCFG = USB_OTG_HCFG_FSLSPCS_1;
#endif
          break;
        default:
          break;
      }
      if (port_reset == false) {                        // If port not under reset
        SignalPortEvent(0, ARM_USBH_EVENT_CONNECT);
      }
    }
    if ((hprt & USB_OTG_HPRT_PENCHNG) != 0U) {          // If port enable changed
      if ((hprt & USB_OTG_HPRT_PENA) != 0U) {           // If device connected
        if (port_reset == true) {
          port_reset = false;
          SignalPortEvent(0, ARM_USBH_EVENT_RESET);
        }
      }
    }
  }
  if ((gintsts & USB_OTG_GINTSTS_DISCINT) != 0U) {      // If device disconnected
    OTG->GINTSTS = USB_OTG_GINTSTS_DISCINT;             // Clear disconnect interrupt
    if (!port_reset) {                                  // Ignore disconnect under reset
      ptr_ch   =  OTG_HC_PTR(0);
      ptr_pipe = (PIPE_t *)((uint32_t)pipe);
      for (ch = 0U; ch < USBH_MAX_PIPE_NUM; ch++) {
        if (ptr_pipe->active != 0U) {
          ptr_pipe->active = 0U;
          ptr_ch->HCINT    = 0x7FFU;                    // Clear all interrupts
          ptr_ch->HCINTMSK = USB_OTG_HCINT_CHH;         // Enable halt interrupt
          ptr_ch->HCCHAR  |= USB_OTG_HCCHAR_CHENA | USB_OTG_HCCHAR_CHDIS;       // Activate Halt
          SignalPipeEvent((ARM_USBH_EP_HANDLE)ptr_ch, ARM_USBH_EVENT_BUS_ERROR);
        }
        ptr_ch++;
        ptr_pipe++;
      }
      SignalPortEvent(0, ARM_USBH_EVENT_DISCONNECT);
    }
  }
#if (USBH_USE_DMA == 0U)                                // If DMA is not used (Slave Mode)
                                                        // Handle reception interrupt
  if ((gintsts & USB_OTG_GINTSTS_RXFLVL) != 0U) {       // If RXFIFO non-empty interrupt
    OTG->GINTMSK &= ~USB_OTG_GINTSTS_RXFLVL;
    grxsts = OTG->GRXSTSR;
    if (((grxsts >> 17) & 0x0FU) == 0x02U) {            // If PKTSTS = 0x02
      grxsts     = (OTG->GRXSTSP);
      ch         = (grxsts     ) & 0x00FU;
      bcnt       = (grxsts >> 4) & 0x7FFU;
      dfifo      = (uint32_t *)((uint32_t)OTG_DFIFO[ch]);
      ptr_data   =  pipe[ch].data + pipe[ch].num_transferred_total;
      len        =  bcnt / 4U;                          // Received number of 32-bit data
      len_rest   =  bcnt & 3U;                          // Number of bytes left
      while (len != 0U) {
        __UNALIGNED_UINT32_WRITE(ptr_data, *dfifo);
        ptr_data += 4U;
        len--;
      }
      if (len_rest != 0U) {
        dat = *dfifo;
        while (len_rest != 0U) {
          *ptr_data++ = (uint8_t)(dat & 0xFFU);
          dat >>= 8;
          len_rest--;
        }
      }
      pipe[ch].num_transferring      += bcnt;
      pipe[ch].num_transferred_total += bcnt;
    } else {                                            // If PKTSTS != 0x02
      grxsts      = OTG->GRXSTSP;
    }
    OTG->GINTMSK |= USB_OTG_GINTSTS_RXFLVL;
  }
#endif
                                                        // Handle host ctrl interrupt
  if ((gintsts & USB_OTG_GINTSTS_HCINT) != 0U) {        // If host channel interrupt
    haint = OTG_HOST->HAINT;
    for (ch = 0U; ch < USBH_MAX_PIPE_NUM; ch++) {
      if (haint == 0U) { break; }
      if ((haint & (1U << ch)) != 0U) {                 // If channels interrupt active
        haint     &= ~(1U << ch);
        ptr_ch     =   OTG_HC_PTR(ch);
        ptr_pipe   =  (PIPE_t *)((uint32_t)&pipe[ch]);
        hcint      =   ptr_ch->HCINT & ptr_ch->HCINTMSK;
        hchalt     =   0U;

#if (USBH_USE_DMA != 0U)                                // If DMA is used
        ptr_ch->HCINTMSK = 0U;                          // Disable all channel interrupts
        ptr_ch->HCINT    = 0x7FFU;                      // Clear all interrupts
        ptr_pipe->in_progress = 0U;                     // Transfer not in progress

        if ((hcint & USB_OTG_HCINT_CHH) != 0U) {        // If channel halted
          ptr_pipe->active = 0U;                        // Transfer not active any more
        } else if ((hcint & USB_OTG_HCINT_XFRC) != 0U) {// If data transfer finished
          if ((ptr_ch->HCCHAR & (1U << 15)) != 0U) {    // If IN pipe
            // Update transferred count
            xfrsiz =  ptr_ch->HCTSIZ        & 0x0003FFFFU;
            ptr_pipe->num_transferred_total += ptr_pipe->num_transferring - xfrsiz;
            ptr_pipe->num_transferring = 0U;
            if ((ptr_pipe->num == ptr_pipe->num_transferred_total) || (xfrsiz != 0U)) {
              // If previous transfer ended by receiving all expected bytes or was terminated by short or
              // ZLP packet then it is completed, else further data reception will be started at the end of this IRQ routine
              ptr_pipe->active = 0U;                    // Transfer not active any more
              ptr_pipe->event = ARM_USBH_EVENT_TRANSFER_COMPLETE;
#if ((USBH_DATA_CACHED == 1U) && (__DCACHE_PRESENT == 1U))
              if ((SCB->CCR & SCB_CCR_DC_Msk) != 0U) {  // If Data Cache is enabled
                SCB_InvalidateDCache_by_Addr ((uint32_t *)((uint32_t)ptr_pipe->data & (~0x1FU)), (int32_t)ptr_pipe->num_transferred_total + 31);
              }
#endif
            }
          } else {                                      // If OUT pipe
            ptr_pipe->num_transferred_total +=  ptr_pipe->num_transferring;
            ptr_pipe->num_transferring = 0U;
            if (ptr_pipe->num == ptr_pipe->num_transferred_total) {
              // If previous transfer ended by transmitting all requested bytes then it is completed
              ptr_pipe->active = 0U;                    // Transfer not active any more
              ptr_pipe->event = ARM_USBH_EVENT_TRANSFER_COMPLETE;
            }
          }
        } else if ((hcint & (USB_OTG_HCINT_STALL |      // If STALL received
                             USB_OTG_HCINT_NAK   |      // If NAK received (only for interrupt pipe)
                             USB_OTG_HCINT_TXERR |      // If errors
                             USB_OTG_HCINT_BBERR |      // ..
                             USB_OTG_HCINT_FRMOR |      // ..
                             USB_OTG_HCINT_DTERR)) != 0U) {
          if ((ptr_ch->HCCHAR & (1U << 15)) != 0U) {    // If IN pipe
            if ((hcint & USB_OTG_HCINT_NAK) == 0U) {    // If not NAK received
              // Update transferred count
              mpsiz  =  ptr_ch->HCCHAR        & 0x000007FFU;
              pktcnt = (ptr_ch->HCTSIZ >> 19) & 0x000003FFU;
              if (ptr_pipe->num_transferring > mpsiz) {
                ptr_pipe->num_transferred_total += ptr_pipe->num_transferring - mpsiz * pktcnt;
              }
            }
          } else {                                      // If endpoint OUT and
            // Update transferred count
            pktcnt = (ptr_ch->HCTSIZ >> 19) & 0x000003FFU;
            mpsiz  = (ptr_ch->HCCHAR      ) & 0x000007FFU;
            if ((ptr_pipe->num_transferring >= mpsiz) && (pktcnt > 0U)) {
              ptr_pipe->num_transferred_total += ptr_pipe->num_transferring - mpsiz * pktcnt;
            }
          }
          ptr_pipe->num_transferring = 0U;
                                                        // If NAK on interrupt pipe (request will be restarted with period)
          if ((hcint & USB_OTG_HCINT_STALL) != 0U){     // If STALL received
            ptr_pipe->event = ARM_USBH_EVENT_HANDSHAKE_STALL;
            hchalt = 1U;
          } else if ((hcint & (USB_OTG_HCINT_TXERR |    // If errors
                               USB_OTG_HCINT_BBERR |    // ..
                               USB_OTG_HCINT_FRMOR |    // ..
                               USB_OTG_HCINT_DTERR)) != 0U) {
            ptr_pipe->event = ARM_USBH_EVENT_BUS_ERROR;
            hchalt = 1U;
          }
        }
#else                                                   // If DMA is not used (Slave Mode)
        if ((hcint & USB_OTG_HCINT_CHH) != 0U) {        // If channel halted
          ptr_ch->HCINTMSK = 0U;                        // Disable all channel interrupts
          ptr_ch->HCINT    = 0x7FFU;                    // Clear all interrupts
          ptr_pipe->in_progress = 0U;                   // Transfer not in progress
        } else if ((hcint & USB_OTG_HCINT_XFRC) != 0U) {// If data transfer finished
          if ((ptr_ch->HCCHAR & (1U << 15)) == 0U) {    // If endpoint OUT
            ptr_ch->HCINTMSK = 0U;                      // Disable all channel interrupts
            ptr_pipe->in_progress = 0U;                 // Transfer not in progress
          }
          ptr_ch->HCINT   = 0x7FFU;                     // Clear all interrupts
          if ((ptr_ch->HCCHAR & (1U << 15)) != 0U) {    // If endpoint IN
            ptr_pipe->active = 0U;                      // Transfer not active any more
            ptr_pipe->event = ARM_USBH_EVENT_TRANSFER_COMPLETE;
          } else {                                      // If endpoint OUT
            ptr_pipe->num_transferred_total += ptr_pipe->num_transferring;
            ptr_pipe->num_transferring       = 0U;
            if (ptr_pipe->num_transferred_total == ptr_pipe->num) {
              ptr_pipe->active = 0U;                    // Transfer not active any more
              ptr_pipe->event = ARM_USBH_EVENT_TRANSFER_COMPLETE;
            }
          }
          if ((ptr_ch->HCCHAR & (1U << 15)) != 0U) {    // If endpoint IN
            hchalt = 1U;
          }
        } else {
          if ((hcint & USB_OTG_HCINT_ACK) != 0U) {      // If ACK received
            ptr_ch->HCINT = USB_OTG_HCINT_ACK;          // Clear ACK interrupt
            // On ACK, ACK is not an event that can be returned so if transfer
            // is completed another interrupt will happen otherwise for IN
            // endpoint transfer will be restarted for remaining data
            if ((ptr_ch->HCCHAR & (1U << 15)) != 0U) {  // If endpoint IN
              if ((ptr_pipe->num != ptr_pipe->num_transferred_total) &&                   // If all data was not transferred
                  (ptr_pipe->num_transferring != 0U)                 &&                   // If zero-length packet was not received
                 ((ptr_pipe->num_transferred_total%ptr_pipe->ep_max_packet_size) == 0U)){ // If short packet was not received
                ptr_ch->HCCHAR |= USB_OTG_HCCHAR_CHENA;
              }
            } else {                                    // If endpoint OUT
              if ((ptr_pipe->packet & ARM_USBH_PACKET_PING) != 0U) {
                ptr_pipe->packet&=~ARM_USBH_PACKET_PING;// Clear Do Ping
              }
              hchalt = 1U;
            }
          } else if ((hcint & (USB_OTG_HCINT_STALL |            // If STALL received
                               USB_OTG_HCINT_NAK   |            // If NAK received
                               USB_OTG_HCINT_NYET  |            // If NYET received
                               USB_OTG_HCINT_TXERR |            // If errors
                               USB_OTG_HCINT_BBERR |            // ..
                               USB_OTG_HCINT_FRMOR |            // ..
                               USB_OTG_HCINT_DTERR)) != 0U) {   // ..
            if (((ptr_ch->HCCHAR & (1U << 15)) == 0U) &&        // If endpoint OUT and
                ((ptr_pipe->packet&ARM_USBH_PACKET_PING)==0U)){ // not PING
              // Update transferred count
              pktcnt = (ptr_ch->HCTSIZ >> 19) & 0x000003FFU;
              mpsiz  = (ptr_ch->HCCHAR      ) & 0x000007FFU;
              if ((ptr_pipe->num_transferring >= mpsiz) && (pktcnt > 0U)) {
                ptr_pipe->num_transferred_total += ptr_pipe->num_transferring - mpsiz * pktcnt;
              } else if ((hcint & USB_OTG_HCINT_NYET) != 0U){   // If NYET recalculate size below max packet size
                ptr_pipe->num_transferred_total += ptr_pipe->num_transferring;
              }
              ptr_pipe->num_transferring = 0U;
            }
            if ((hcint & USB_OTG_HCINT_NYET) != 0U) {   // If NYET received
              ptr_ch->HCINT = USB_OTG_HCINT_NYET;       // Clear NYET interrupt
              if (ptr_pipe->num != ptr_pipe->num_transferred_total) {
                // If transfer is not done we need to restart sending,
                // else if transfer is finished XFRC will follow and then
                // transfer done will be handled there
                ptr_pipe->packet|=ARM_USBH_PACKET_PING; // Do Ping
                hchalt = 1U;
              }
            } else if ((hcint & USB_OTG_HCINT_NAK)!=0U){// If NAK received
              ptr_ch->HCINT = USB_OTG_HCINT_NAK;        // Clear NAK interrupt
              // On NAK, NAK is not returned to middle layer but transfer is
              // restarted from driver for remaining data, unless it is interrupt
              // endpoint in which case transfer is restarted on SOF event
              // when period has expired
              if ((ptr_ch->HCCHAR & (1U << 15)) != 0U) {// If endpoint IN
                if (ptr_pipe->ep_type == ARM_USB_ENDPOINT_INTERRUPT) {
                  hchalt = 1U;
#if (USBH1_BULK_IN_NAK_DELAY != 0U)
                } else if (ptr_pipe->ep_type == ARM_USB_ENDPOINT_BULK) {
                  ptr_pipe->in_progress = 0U;
                  ptr_pipe->interval    = USBH1_BULK_IN_NAK_DELAY;
#endif
                } else {
                  ptr_ch->HCCHAR |= USB_OTG_HCCHAR_CHENA;
                }
              } else {                                  // If endpoint OUT
                if (((OTG_HPRT & USB_OTG_HPRT_PSPD_Msk) >> USB_OTG_HPRT_PSPD_Pos) == 0U) { // If high-speed active
                  switch (ptr_pipe->ep_type) {
                    case ARM_USB_ENDPOINT_CONTROL:
                    case ARM_USB_ENDPOINT_BULK:
                      ptr_pipe->packet |= ARM_USBH_PACKET_PING; // Do Ping
                      break;
                    case ARM_USB_ENDPOINT_ISOCHRONOUS:
                    case ARM_USB_ENDPOINT_INTERRUPT:
                      break;
                  }
                }
                hchalt = 1U;
              }
            } else if ((hcint&USB_OTG_HCINT_STALL)!=0U){// If STALL received
              ptr_ch->HCINT    = USB_OTG_HCINT_STALL;   // Clear STALL interrupt
              ptr_pipe->active = 0U;                    // Transfer not active any more
              ptr_pipe->event  = ARM_USBH_EVENT_HANDSHAKE_STALL;
              hchalt = 1U;
            } else {
              ptr_ch->HCINT    = USB_OTG_HCINT_TXERR |  // Clear all error interrupts
                                 USB_OTG_HCINT_BBERR |
                                 USB_OTG_HCINT_FRMOR |
                                 USB_OTG_HCINT_DTERR ;
              ptr_pipe->active = 0U;                    // Transfer not active any more
              ptr_pipe->event  = ARM_USBH_EVENT_BUS_ERROR;
              hchalt = 1U;
            }
          }
        }
#endif
        if (hchalt != 0U) {                             // If channel should be halted
          ptr_ch->HCINTMSK = USB_OTG_HCINT_CHH;         // Enable halt interrupt
          ptr_ch->HCCHAR  |= USB_OTG_HCCHAR_CHENA | USB_OTG_HCCHAR_CHDIS;
        }
        if ((ptr_pipe->in_progress == 0U) && (ptr_pipe->active == 0U) && (ptr_pipe->event != 0U)) {
          SignalPipeEvent((ARM_USBH_EP_HANDLE)ptr_ch, ptr_pipe->event);
          ptr_pipe->event  = 0U;
        }
      }
    }
  }

  // Handle periodic transfer timings
  if ((gintsts & USB_OTG_GINTSTS_SOF) != 0U) {          // If start of frame interrupt
    OTG->GINTSTS =  USB_OTG_GINTSTS_SOF;                // Clear SOF interrupt
    ptr_pipe     = (PIPE_t *)((uint32_t)pipe);
    for (ch = 0U; ch < USBH_MAX_PIPE_NUM; ch++) {
      // If interrupt transfer is active handle period (interval)
      if ((ptr_pipe->active != 0U) && (ptr_pipe->interval != 0U)) {
        ptr_pipe->interval--;
      }
      ptr_pipe++;
    }
  }

  // Handle restarts of unfinished transfers (due to NAK or ACK)
  ptr_pipe = (PIPE_t *)((uint32_t)pipe);
  for (ch = 0U; ch < USBH_MAX_PIPE_NUM; ch++) {
    if ((ptr_pipe->active != 0U) && (ptr_pipe->in_progress == 0U)) {
      // Restart periodic transfer if not in progress and interval expired
      if (ptr_pipe->ep_type == ARM_USB_ENDPOINT_INTERRUPT) {
        if (ptr_pipe->interval == 0U) {
          ptr_pipe->interval = ptr_pipe->interval_reload;
        } else {
          continue;
        }
      }

#if (USBH1_BULK_IN_NAK_DELAY != 0U)
      if ((ptr_pipe->ep_type == ARM_USB_ENDPOINT_BULK) && (ptr_pipe->ep_dir == 1U)) {
        // Restart bulk (IN) transfer if not in progress and interval expired
        if (ptr_pipe->interval == 0U) {
          ptr_ch = OTG_HC_PTR(ch);
          ptr_pipe->in_progress = 1U;
          ptr_ch->HCCHAR |= USB_OTG_HCCHAR_CHENA;
        }
      } else {
#endif
        ptr_pipe->in_progress = 1U;
        if (USBH_HW_StartTransfer (ptr_pipe, USBH_CH_GetAddressFromIndex (ch)) == 0U) {
          ptr_pipe->in_progress = 0U;
          ptr_pipe->active      = 0U;
        }
#if (USBH1_BULK_IN_NAK_DELAY != 0U)
      }
#endif
    }
    ptr_pipe++;
  }
}


// Structure exported by driver
ARM_DRIVER_USBH Driver_USBH1 = {
  USBH_GetVersion,
  USBH_GetCapabilities,
  USBH_Initialize,
  USBH_Uninitialize,
  USBH_PowerControl,
  USBH_PortVbusOnOff,
  USBH_PortReset,
  USBH_PortSuspend,
  USBH_PortResume,
  USBH_PortGetState,
  USBH_PipeCreate,
  USBH_PipeModify,
  USBH_PipeDelete,
  USBH_PipeReset,
  USBH_PipeTransfer,
  USBH_PipeTransferGetResult,
  USBH_PipeTransferAbort,
  USBH_GetFrameNumber
};

/*! \endcond */
