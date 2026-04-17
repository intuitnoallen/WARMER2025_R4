/*
 * esp_pm.c
 *
 *  Created on: Sep 15, 2021
 *      Author: thaob
 */
#include "esp_sleep.h"
#include "driver/uart.h"
// #include "driver/rtc_io.h"
#include "driver/gpio.h"
#include "rom/rtc.h"
#include "rom/ets_sys.h"
#include "soc/rtc_cntl_reg.h"
//#include "soc/rtc_io_reg.h"
#include "soc/uart_reg.h"
#include "soc/timer_group_reg.h"
#include "srvc_button.h"
#include "esp_pm.h"
#include "app_pm.h"

static const char *TAG = "ESP_PM";

static void RTC_IRAM_ATTR wake_stub( void );

#define BUTTON_POWER_ON_MS      2000 //2000
#define BUTTON_POWER_IN_MS      1000 //2000


#define RTC_GPIO_POWER_ON 4 //GPIO_34
#define RTC_GPIO_PLUG_IN 0 //GPIO_36
#define RTC_GPIO_COVER_DET 1 //GPIO_37


#define POWER_ON_BTN() \
    ((REG_GET_FIELD(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT) \
            & BIT(RTC_GPIO_POWER_ON)) == 0)

#define PLUG_IN_GPIO() \
    ((REG_GET_FIELD(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT) \
            & BIT(RTC_GPIO_PLUG_IN)) == 0)

#define COVER_DET_GPIO() \
    ((REG_GET_FIELD(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT) \
            & BIT(RTC_GPIO_COVER_DET)) == 0)

// #define POWER_ON_BTN() 
//     ((REG_GET_FIELD(GPIO_IN_REG, GPIO_IN_DATA) 
//             & BIT(RTC_GPIO_POWER_ON)) == 0)
            
// Pulse counter value, stored in RTC_SLOW_MEM
static size_t RTC_DATA_ATTR s_pulse_count=0;
static size_t RTC_DATA_ATTR s_max_pulse_count=300;


void app_pm_deep_sleep( void )
{

//  disable wakeup source
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);

//  Set the wake stub function
    esp_set_deep_sleep_wake_stub(&wake_stub);

    // rtc_gpio_init(34);  //OK BTN
    // rtc_gpio_set_direction(34, RTC_GPIO_MODE_INPUT_ONLY);

    //   gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT);
    // gpio_set_level(GPIO_NUM_26, 1);   // drive HIGH

    rtc_gpio_init(GPIO_NUM_26);
    rtc_gpio_set_direction(GPIO_NUM_26, RTC_GPIO_MODE_OUTPUT_ONLY);
    // rtc_gpio_pullup_en(26);    // disable pullup if not needed
    // rtc_gpio_pulldown_dis(26);  // disable pulldown if not needed
    rtc_gpio_set_level(GPIO_NUM_26, 1);
    rtc_gpio_hold_en(GPIO_NUM_26);

    //  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
    // rtc_gpio_pullup_dis(34);    // disable pullup if not needed
    // rtc_gpio_pulldown_dis(34);  // disable pulldown if not needed

    rtc_gpio_init(GPIO_NUM_37); //cover
    rtc_gpio_set_direction(GPIO_NUM_37, RTC_GPIO_MODE_INPUT_ONLY);
    rtc_gpio_pullup_dis(GPIO_NUM_37);    // disable pullup if not needed
    rtc_gpio_pulldown_dis(GPIO_NUM_37);  // disable pulldown if not needed



    s_pulse_count=0;
    // const gpio_config_t config = {       //old
    //     .pin_bit_mask = PIN_WAKEUP,
    //     .mode = GPIO_MODE_INPUT,
    // };
    // ESP_ERROR_CHECK(gpio_config(&config));  //old

    // rtc_gpio_init(34);                   // Init as RTC GPIO
	// rtc_gpio_set_direction(34, RTC_GPIO_MODE_INPUT_ONLY);
	// rtc_gpio_set_level(34, 1);            // Keep high
	// rtc_gpio_hold_en(34); 
	// esp_sleep_enable_ext0_wakeup(34, 1); // 0 = low level wakes up

// rtc_gpio_init(34);
// rtc_gpio_set_direction(34, RTC_GPIO_MODE_INPUT_ONLY);
// rtc_gpio_pullup_dis(34);     // disable pull-up
// rtc_gpio_pulldown_dis(34);   // or configure if needed
    // esp_deep_sleep_enable_gpio_wakeup(BIT(PIN_WAKEUP), ESP_GPIO_WAKEUP_GPIO_HIGH);
    // if(device_data.battery.charging == BATTERY_NOT_CHARGING){
        // ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(36,1)); //PWR_PLUG
        
        ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(36,0)); //PWR_PLUG
    // }  
    

    ESP_ERROR_CHECK(esp_sleep_enable_ext1_wakeup((1ULL << GPIO_NUM_34), ESP_EXT1_WAKEUP_ANY_HIGH)); //PWR_ON_BTN
    //  esp_sleep_enable_ext0_wakeup(PIN_WAKEUP, 1);
    /* Wait until button ON is released */
    // BTN_inst_t x_btn_on_inst = x_BTN_Get_Inst (BTN_BUTTON_ON);
    // while (enm_BTN_Get_State (x_btn_on_inst) != BTN_STATE_RELEASED)
    // {
    //     vTaskDelay (pdMS_TO_TICKS (50));
    // }
    
    // Isolate GPIO12 pin from external circuits. This is needed for modules
    // which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
    // to minimize current consumption.
//  rtc_gpio_isolate(GPIO_NUM_12);

    APP_DEBUG(TAG, "Enter deep sleep\n");

//  rtc_gpio_pulldown_en(PIR_SS_1_PIN);

    /* To make sure the complete line is printed before entering sleep mode,
     * need to wait until UART TX FIFO is empty:
     */
    uart_wait_tx_idle_polling(CONFIG_ESP_CONSOLE_UART_NUM);

    /* Enter sleep mode */
    esp_deep_sleep_start();
}


#define IS_BUTTON_ON_PRESSED()             ((REG_GET_FIELD(GPIO_IN_REG, GPIO_IN_DATA) & BIT(BTN_ON_PIN)) != 0)
#define IS_BOTTLE_PRESENT()                ((REG_GET_FIELD(GPIO_IN_REG, GPIO_IN_DATA) & BIT(BTN_BOTTLE_SENSOR_PIN)) != 0)

#define WAKE_STUB_BUTTON_CHECK_MS           10000


#define BUTTON_PIN          34     // must be RTC-capable GPIO
#define HOLD_TIME_MS        2000           // hold for 2 seconds
#define CHECK_INTERVAL_US   10000          // check every 10 ms




// static const char RTC_RODATA_ATTR wake_fmt_str[] = "count=%d\n";
// static const char RTC_RODATA_ATTR sleep_fmt_str[] = "sleeping\n";


static void RTC_IRAM_ATTR wake_stub( void )
{

     // Increment the pulse counter
    
    // and print the pulse counter value:
    // ets_printf(wake_fmt_str, s_pulse_count);
//  esp_default_wake_deep_sleep();
// return;
    // Pulse count is <s_max_pulse_count, go back to sleep
    // and wait for more pulses.

    // Wait for pin level to be high.
    // If we go to sleep when the pin is still low, the chip
    // will wake up again immediately. Hardware doesn't have
    // edge trigger support for deep sleep wakeup.
    //   gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);
    //   gpio_set_level(GPIO_NUM_32, 0);   // drive HIGH
    
    do{
        if ((POWER_ON_BTN()==0 || PLUG_IN_GPIO()==1) && COVER_DET_GPIO()==1) {
            // feed the watchdog
            s_pulse_count++;
            REG_WRITE(TIMG_WDTFEED_REG(0), 1);
            ets_delay_us(WAKE_STUB_BUTTON_CHECK_MS);
                if ((s_pulse_count * WAKE_STUB_BUTTON_CHECK_MS) >= (BUTTON_POWER_ON_MS * 1000)) {
                // On revision 0 of ESP32, this function must be called:
                esp_default_wake_deep_sleep();

                // Return from the wake stub function to continue
                // booting the firmware.
                return;
                }
        }else if (PLUG_IN_GPIO()==1 && COVER_DET_GPIO()==1) {
            // feed the watchdog
            s_pulse_count++;
            REG_WRITE(TIMG_WDTFEED_REG(0), 1);
            ets_delay_us(WAKE_STUB_BUTTON_CHECK_MS);
                if ((s_pulse_count * WAKE_STUB_BUTTON_CHECK_MS) >= (BUTTON_POWER_IN_MS * 1000) ) {
                // On revision 0 of ESP32, this function must be called:
                esp_default_wake_deep_sleep();

                // Return from the wake stub function to continue
                // booting the firmware.
                return;
                }
        }else{
            s_pulse_count=0;
            break;
        }
        // debounce, 10ms
        ets_delay_us(10000);
        
    }while (1);
    
    // if(POWER_ON_BTN()){
    //      // debounce, 10ms
    //     ets_delay_us(10000);
    //     s_pulse_count=0;
    // }

    // Print status
    // ets_printf(sleep_fmt_str);
    // Wait for UART to end transmitting.
    // while (REG_GET_FIELD(UART_STATUS_REG(0), UART_ST_UTX_OUT)) {
    //     ;
    // }
    
    // Set the pointer of the wake stub function.
    REG_WRITE(RTC_ENTRY_ADDR_REG, (uint32_t)&wake_stub);
    // Go to sleep.
    CLEAR_PERI_REG_MASK(RTC_CNTL_STATE0_REG, RTC_CNTL_SLEEP_EN);
    SET_PERI_REG_MASK(RTC_CNTL_STATE0_REG, RTC_CNTL_SLEEP_EN);
    // A few CPU cycles may be necessary for the sleep to start...
    while (true) {
        ;
    }
    // never reaches here.
}



void app_pm_config( void )
{
#if CONFIG_PM_ENABLE
    // Configure dynamic frequency scaling:
    // maximum and minimum frequencies are set in sdkconfig,
    // automatic light sleep is enabled if tickless idle support is enabled.
//  rtc_cpu_freq_t max_freq;
//  rtc_clk_cpu_freq_from_mhz(CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ, &max_freq);

    esp_pm_config_esp32_t pm_config =
    {
        .max_freq_mhz = CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ,
        .min_freq_mhz = 40,
#if CONFIG_FREERTOS_USE_TICKLESS_IDLE
        .light_sleep_enable = true
#endif
    };
    ESP_ERROR_CHECK( esp_pm_configure(&pm_config) );
#endif // CONFIG_PM_ENABLE
}

void app_pm_light_sleep( void )
{
    //disable wakeup source
//  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);

    gpio_wakeup_enable(PIN_WAKEUP, (WAKEUP_LEVEL == 0)? GPIO_INTR_LOW_LEVEL : GPIO_INTR_HIGH_LEVEL);
    esp_sleep_enable_gpio_wakeup();

    /* Wait until button ON is released */
    BTN_inst_t x_btn_on_inst = x_BTN_Get_Inst (BTN_BUTTON_ON);
    while (enm_BTN_Get_State (x_btn_on_inst) != BTN_STATE_RELEASED)
    {
        vTaskDelay (pdMS_TO_TICKS (50));
    }

    APP_DEBUG(TAG, "Enter light sleep\n");
    APP_DELAY_MS(20);

    /* To make sure the complete line is printed before entering sleep mode,
     * need to wait until UART TX FIFO is empty:
     */
    uart_wait_tx_idle_polling(CONFIG_ESP_CONSOLE_UART_NUM);

    /* Enter sleep mode */
    esp_light_sleep_start();
//  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
}







// esp_default_wake_deep_sleep();
// //  static RTC_RODATA_ATTR const char fmt_str[] = "Wake count %d\n";
// //     esp_rom_printf(fmt_str, wake_count++);
// //  uint32_t elapsed_ms = 0;
// // stub_state = 1234;  // mark stub ran
// // Check if button is pressed (HIGH)
// // if (rtc_gpio_get_level(BUTTON_PIN) == 1) {
// // rtc_gpio_init(34);                   // Init as RTC GPIO
// // rtc_gpio_set_direction(34, RTC_GPIO_MODE_INPUT_ONLY);    
// // while (rtc_gpio_get_level(34) == 1) {
// //     ets_delay_us(CHECK_INTERVAL_US);
// //     elapsed_ms += CHECK_INTERVAL_US / 1000;
// //     // Feed watchdog
// //     REG_WRITE(TIMG_WDTFEED_REG(0), 1);
// //     // Held long enough → continue boot
// //     if (elapsed_ms >= HOLD_TIME_MS) {
 // //         return;
// //     }
// // }
// // If button released too soon → go back to deep sleep
// // REG_WRITE(RTC_ENTRY_ADDR_REG, (uint32_t)&wake_stub);
// // CLEAR_PERI_REG_MASK(RTC_CNTL_STATE0_REG, RTC_CNTL_SLEEP_EN);
// // SET_PERI_REG_MASK(RTC_CNTL_STATE0_REG, RTC_CNTL_SLEEP_EN);
// // while (true) {;}   // wait until sleep engages
// //     uint32_t wake_stub_ms_count = 0;
// //     do {
// //         if (IS_BUTTON_ON_PRESSED()==1 )
// //         {
// //             REG_WRITE(TIMG_WDTFEED_REG(0), 1);
// //             ets_delay_us(WAKE_STUB_BUTTON_CHECK_MS);
// //             wake_stub_ms_count++;
// //             if( (wake_stub_ms_count * WAKE_STUB_BUTTON_CHECK_MS) >= (BUTTON_POWER_ON_MS * 1000) )
// //             {
// //                 esp_default_wake_deep_sleep();
// //                 device_data.power = PWR_ON;
// //                 return;
// //             }
// //         }
// //         else
// //         {
// //             break;
// //         }
// //     }while(1);
// // //  Set the pointer of the wake stub function.
// //     REG_WRITE(RTC_ENTRY_ADDR_REG, (uint32_t)&wake_stub);
// // //  Go to sleep.
// //     CLEAR_PERI_REG_MASK(RTC_CNTL_STATE0_REG, RTC_CNTL_SLEEP_EN);
// //     SET_PERI_REG_MASK(RTC_CNTL_STATE0_REG, RTC_CNTL_SLEEP_EN);
// // //  A few CPU cycles may be necessary for the sleep to start...
// //     while (true) {
// //         ;
// //     }
// // never reaches here.

