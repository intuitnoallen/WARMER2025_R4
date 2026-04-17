/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  @file       : srvc_button.c
**  @author     : Nguyen Ngoc Tung (ngoctung.dhbk@gmail.com)
**  @date       : 2022 Sep 17
**  @brief      : Implementation of Srvc_Button module
**  @namespace  : BTN
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** @addtogroup  Srvc_Button
**
** @brief       Srvc_Button module encapsulates all button components. It provides services so that other modules
**              in the firmware can easily use the buttons.
** @{
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           INCLUDES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include "srvc_button.h"            /* Public header of this module */
#include "hwa_gpio.h"               /* Use hardware abstraction of GPIO pins */

#include "freertos/FreeRTOS.h"      /* Use FreeRTOS */
#include "freertos/task.h"          /* Use FreeRTOS task */

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           DEFINES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Logging tag of this module */
#define TAG             "Srvc_Button"

/** @brief  Structure encapsulating a button object */
struct BTN_obj
{
    bool                b_initialized;          //!< Specifies whether the object has been initialized or not
    BTN_inst_id_t       enm_inst_id;            //!< Instance ID of this object
    GPIO_inst_t         x_gpio_inst;            //!< Instance handle of the GPIO pin associated with the button
    BTN_state_t         enm_state;              //!< Current button state
    bool                b_active;               //!< Debounced active state of the button
    TickType_t          x_debounce_timer;       //!< Timer used for button debouncing
    TickType_t          x_level_timer;          //!< Timer counting number of OS ticks the button is at current level

    BTN_callback_t      pfnc_cb;                //!< Callback functions invoked when button state changes
    void *              pv_cb_arg;              //!< Argument passed when the callback function was registered

    GPIO_inst_id_t      enm_gpio_inst_id;       //!< Instance ID of the GPIO pin associated with the button
    uint16_t            u16_debounce_ms;        //!< Button debounce time (in milliseconds)
    uint16_t            u16_hold_start_ms;      //!< Button hold start time (in milliseconds)
    uint16_t            u16_hold_short_ms;      //!< Short button hold time (in milliseconds)
    uint16_t            u16_hold_long_ms;       //!< Long button hold time (in milliseconds)
};

/** @brief  Macro expanding BTN_INST_TABLE as initialization value for BTN_obj struct */
#define INST_TABLE_EXPAND_AS_STRUCT_INIT(INST_ID, GPIO_INST_ID, DEBOUNCE_MS,    \
                                HOLD_START_MS, HOLD_SHORT_MS, HOLD_LONG_MS)     \
{                                                                               \
    .b_initialized      = false,                                                \
    .enm_inst_id        = INST_ID,                                              \
    .x_gpio_inst        = NULL,                                                 \
    .enm_state          = BTN_STATE_RELEASED,                                   \
    .b_active           = false,                                                \
    .x_debounce_timer   = 0,                                                    \
    .x_level_timer      = 0,                                                    \
                                                                                \
    .pfnc_cb            = NULL,                                                 \
    .pv_cb_arg          = NULL,                                                 \
                                                                                \
    .enm_gpio_inst_id   = GPIO_INST_ID,                                         \
    .u16_debounce_ms    = DEBOUNCE_MS,                                          \
    .u16_hold_start_ms  = HOLD_START_MS,                                        \
    .u16_hold_short_ms  = HOLD_SHORT_MS,                                        \
    .u16_hold_long_ms   = HOLD_LONG_MS,                                         \
},

/** @brief  ID of the CPU that Button service task runs on */
#define BTN_TASK_CPU_ID                 0

/** @brief  Stack size (in bytes) of Button service task */
#define BTN_TASK_STACK_SIZE             (3*1024)

/** @brief  Priority of Button service task */
#define BTN_TASK_PRIORITY               (tskIDLE_PRIORITY + 2)

/** @brief  FreeRTOS event fired at the point a button is pressed */
#define BTN_BUTTON_PRESSED              BIT0

/** @brief  Cycle in milliseconds of the loop processing buttons while any of them are active */
#define BTN_PROCESS_CYCLE               50

/** @brief  Unused value of button hold time */
#define BTN_NO_HOLD                     0

/** @brief  Debounce interval in milliseconds when a button is released */
#define BTN_DEBOUNCE_RELEASE_MS         100

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           VARIABLES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** @brief  Structure that will hold the TCB of the task being created */
static StaticTask_t g_x_task_buffer;

/** @brief  Buffer that the task being created will use as its stack */
static StackType_t g_x_task_stack [BTN_TASK_STACK_SIZE];

/** @brief  Indicates if this module has been initialized */
static bool g_b_initialized = false;

/** @brief  Array of all button objects */
static struct BTN_obj g_astru_btn_objs[BTN_NUM_INST] =
{
    BTN_INST_TABLE (INST_TABLE_EXPAND_AS_STRUCT_INIT)
};

/** @brief  Handle of Srvc_Button task */
static TaskHandle_t g_x_button_task;

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           PROTOTYPES SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static int32_t s32_BTN_Init_Module (void);
static int32_t s32_BTN_Init_Inst (BTN_inst_t x_inst);
static void v_BTN_Main_Task (void * pv_param);
static void v_BTN_Process_Buttons (void);
static void v_BTN_Debounce (BTN_inst_t x_inst);
static void v_BTN_Gpio_Isr_Handler (GPIO_evt_data_t * pstru_evt_data);

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           FUNCTIONS SECTION
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets instance handle of a button. This instance will be used for other functions in this module.
**
** @details
**      When this function is called on a button instance for the first time, internal data, state and the touch
**      pad associated with the button shall be initialized. This is done only once, all subsequent calls to this
**      function shall simply return instance handle of the button.
**
** @param [in]
**      enm_inst_id: Index of the button instance to get. Button indexes are expanded from Button_Inst_ID option
**                   of BTN_INST_TABLE (srvc_button_ext.h)
**
** @return
**      @arg    NULL: Failed to get instance of the button
**      @arg    Otherwise: Instance handle of the button
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
BTN_inst_t x_BTN_Get_Inst (BTN_inst_id_t enm_inst_id)
{
    BTN_inst_t  x_inst = NULL;
    int32_t     s32_result = STATUS_OK;

    /* Validation */
    if (enm_inst_id >= BTN_NUM_INST)
    {
        return NULL;
    }

    /* If this module has not been initialized, do that now */
    if (!g_b_initialized)
    {
        s32_result = s32_BTN_Init_Module ();
        g_b_initialized = (s32_result == STATUS_OK);
    }

    /* If the retrieved instance has not been initialized yet, do that now */
    if (s32_result == STATUS_OK)
    {
        x_inst = &g_astru_btn_objs[enm_inst_id];
        if (!x_inst->b_initialized)
        {
            s32_result = s32_BTN_Init_Inst (x_inst);
            x_inst->b_initialized = (s32_result == STATUS_OK);
        }
    }

    /* Return instance of the required button */
    return (s32_result == STATUS_OK) ? x_inst : NULL;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Registers a callback function invoked when an event occurs
**
** @note
**      The callback functions shall be called in the context of Srvc_Button task. Therefore, it is essential
**      that callback functions never attempt to block (ie. a callback function must not call vTaskDelay(),
**      vTaskDelayUntil(), or specify a non zero block time when accessing a queue or a semaphore)
**
** @param [in]
**      x_inst: Button instance returned by x_BTN_Get_Inst() function
**
** @param [in]
**      pfnc_cb: Callback function to register
**
** @param [in]
**      pv_arg: Optional argument which will be forwarded to the data of callback function when it's invoked
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void v_BTN_Register_Callback (BTN_inst_t x_inst, BTN_callback_t pfnc_cb, void * pv_arg)
{
    /* Validation */
    ASSERT_PARAM ((x_inst != NULL) && (x_inst->b_initialized));

    /* Store the callback function */
    x_inst->pfnc_cb = pfnc_cb;
    x_inst->pv_cb_arg = pv_arg;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets current state of a button
**
** @param [in]
**      x_inst: Button instance returned by x_BTN_Get_Inst() function
**
** @return
**      Current state of the button
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
BTN_state_t enm_BTN_Get_State (BTN_inst_t x_inst)
{
    /* Validation */
    if ((x_inst == NULL) || (!x_inst->b_initialized))
    {
        return BTN_STATE_RELEASED;
    }

    /* Return current button state */
    return (x_inst->enm_state);
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Gets interval in milliseconds that a button has been pressed or released
**
** @param [in]
**      x_inst: Button instance returned by x_BTN_Get_Inst() function
**
** @return
**      Interval of current button level
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
uint32_t u32_BTN_Get_Level_Interval (BTN_inst_t x_inst)
{
    /* Validation */
    if ((x_inst == NULL) || (!x_inst->b_initialized))
    {
        return 0;
    }

    /* Return interval in milliseconds that the button has been pressed/released */
    return (TIMER_ELAPSED (x_inst->x_level_timer) * portTICK_PERIOD_MS);
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Initializes Srvc_Button module
**
** @return
**      @arg    STATUS_OK
**      @arg    STATUS_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int32_t s32_BTN_Init_Module (void)
{
    /* Create task running Button service */
    g_x_button_task =
        xTaskCreateStaticPinnedToCore ( v_BTN_Main_Task,       /* Function that implements the task */
                                        "Srvc_Button",    /* Text name for the task */
                                        BTN_TASK_STACK_SIZE,   /* Stack size in bytes, not words */
                                        NULL,                   /* Parameter passed into the task */
                                        BTN_TASK_PRIORITY,     /* Priority at which the task is created */
                                        g_x_task_stack,         /* Array to use as the task's stack */
                                        &g_x_task_buffer,       /* Variable to hold the task's data structure */
                                        BTN_TASK_CPU_ID);      /* ID of the CPU that the task runs on */
    ASSERT_PARAM (g_x_button_task != NULL);
    return STATUS_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Initializes a button instance
**
** @param [in]
**      x_inst: Button instance returned by x_BTN_Get_Inst() function
**
** @return
**      @arg    STATUS_OK
**      @arg    STATUS_ERR
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int32_t s32_BTN_Init_Inst (BTN_inst_t x_inst)
{
    /* Get instance handle of the associated GPIO pin */
    if (s8_GPIO_Get_Inst (x_inst->enm_gpio_inst_id, &x_inst->x_gpio_inst) != GPIO_OK)
    {
        LOGE ("Failed to get instance of button %d", x_inst->enm_inst_id);
        return STATUS_ERR;
    }

    /* Enable external interrupt of the associated GPIO */
    if (s8_GPIO_Enable_Interrupt (x_inst->x_gpio_inst, GPIO_INT_BOTH_EDGE, v_BTN_Gpio_Isr_Handler, x_inst) != GPIO_OK)
    {
        LOGE ("Failed to enable interrupt of the GPIO associated with button %d", x_inst->enm_inst_id);
        return STATUS_ERR;
    }

    return STATUS_OK;
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Task running Srvc_Button module
**
** @param [in]
**      pv_param: Parameter passed into the task
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_BTN_Main_Task (void * pv_param)
{
    const uint32_t  u32_bits_to_clear_on_entry = 0x00000000;
    const uint32_t  u32_bits_to_clear_on_exit = 0xFFFFFFFF;
    uint32_t        u32_notify_value;

    /* Endless loop of the task */
    g_b_initialized = true;
    while (g_b_initialized)
    {
        if (!g_b_initialized) {
            break; 
        }
        
        /* Wait until any button is pressed */
        xTaskNotifyWait (u32_bits_to_clear_on_entry, u32_bits_to_clear_on_exit, &u32_notify_value, portMAX_DELAY);
        if (u32_notify_value & BTN_BUTTON_PRESSED)
        {
            /* Process all buttons until all are idle */
            v_BTN_Process_Buttons ();
        }
        // PRINT_STACK_USAGE (4000);
    }

    /* Kill this task */
    vTaskDelete (NULL);
}

void BTN_Stop_Task_For_OTA(void)
{
    if (g_x_button_task != NULL)
    {
        
        g_b_initialized = false;

        // dummy notification to wake it up from portMAX_DELAY
        // eNoAction wakes the task without changing the notification value
        xTaskNotify(g_x_button_task, 0, eNoAction);
        g_x_button_task = NULL; 
    }
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Processes all buttons until all of them are fully released and idle
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_BTN_Process_Buttons (void)
{
    bool b_all_buttons_idle = false;

    /* Process until all buttons are fully released and idle */
    while (!b_all_buttons_idle)
    {
        b_all_buttons_idle = true;

        /* Process all buttons, one by one */
        for (uint8_t u8_btn_idx = 0; u8_btn_idx < BTN_NUM_INST; u8_btn_idx++)
        {
            BTN_state_t enm_new_state = BTN_STATE_RELEASED;
            BTN_inst_t  x_inst = &g_astru_btn_objs[u8_btn_idx];

            /* Only process buttons in use */
            if (!x_inst->b_initialized)
            {
                continue;
            }

            /* Debounce the button and store the debounced active state into x_inst->b_active */
            v_BTN_Debounce (x_inst);

            /* Determine button state */
            if (x_inst->b_active)
            {
                TickType_t x_level_time = TIMER_ELAPSED (x_inst->x_level_timer);
                if ((x_level_time >= pdMS_TO_TICKS (x_inst->u16_hold_long_ms)) &&
                    (x_inst->u16_hold_long_ms != BTN_NO_HOLD))
                {
                    enm_new_state = BTN_STATE_HOLD_LONG;
                }
                else if ((x_level_time >= pdMS_TO_TICKS (x_inst->u16_hold_short_ms)) &&
                         (x_inst->u16_hold_short_ms != BTN_NO_HOLD))
                {
                    enm_new_state = BTN_STATE_HOLD_SHORT;
                }
                else if ((x_level_time >= pdMS_TO_TICKS (x_inst->u16_hold_start_ms)) &&
                         (x_inst->u16_hold_start_ms != BTN_NO_HOLD))
                {
                    enm_new_state = BTN_STATE_HOLD_START;
                }
                else
                {
                    enm_new_state = BTN_STATE_PRESSED;
                }
            }

            /* Invoke callback functions if button state changes */
            if (enm_new_state != x_inst->enm_state)
            {
                BTN_state_t enm_old_state = x_inst->enm_state;
                x_inst->enm_state = enm_new_state;
                if (x_inst->pfnc_cb != NULL)
                {
                    BTN_evt_data_t stru_evt_data =
                    {
                        .x_inst             = x_inst,
                        .pv_arg             = x_inst->pv_cb_arg,
                        .enm_evt            = BTN_EVT_STATE_CHANGED,
                        .stru_state         =
                        {
                            .enm_old_state  = enm_old_state,
                            .enm_new_state  = enm_new_state,
                        },
                    };
                    x_inst->pfnc_cb (&stru_evt_data);
                }
            }

            /*
            ** Determine if this button is fully released and idle.
            ** That happens if its state is BTN_STATE_RELEASED for a time longer enough than its debounce time.
            */
            bool b_btn_idle = (x_inst->enm_state == BTN_STATE_RELEASED) &&
                              (TIMER_ELAPSED (x_inst->x_level_timer) > pdMS_TO_TICKS (x_inst->u16_debounce_ms * 2));
            if (!b_btn_idle)
            {
                b_all_buttons_idle = false;
            }
        }

        if (!b_all_buttons_idle)
        {
            vTaskDelay (pdMS_TO_TICKS (BTN_PROCESS_CYCLE));
        }
    }
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Debounces level of a button
**
** @param [in]
**      x_inst: Button instance returned by btnGetInst() function
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_BTN_Debounce (BTN_inst_t x_inst)
{
    /* Instantaneous active state of the button */
    bool b_active;
    s8_GPIO_Read_Active (x_inst->x_gpio_inst, &b_active);

    /* New button level is only valid if the new level persists for a period of time */
    if (b_active != x_inst->b_active)
    {
        if ((b_active && (TIMER_ELAPSED (x_inst->x_debounce_timer) >= pdMS_TO_TICKS (x_inst->u16_debounce_ms))) ||
            ((!b_active) && (TIMER_ELAPSED (x_inst->x_debounce_timer) >= pdMS_TO_TICKS (BTN_DEBOUNCE_RELEASE_MS))))
        {
            x_inst->b_active = b_active;
            TIMER_RESET (x_inst->x_level_timer);
            TIMER_RESET (x_inst->x_debounce_timer);
        }
    }
    else
    {
        TIMER_RESET (x_inst->x_debounce_timer);
    }
}

/**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** @brief
**      Handler of GPIO external interrupt
**
** @param [in]
**      pstru_evt_data: Argument passed to the handler
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void v_BTN_Gpio_Isr_Handler (GPIO_evt_data_t * pstru_evt_data)
{

    if (g_x_button_task == NULL || !g_b_initialized) {
       // Ignore the hardware interrupt.
        return; 
    }

    BTN_inst_t x_inst = pstru_evt_data->pv_arg;

    /* If the button is pressed */
    if ((x_inst->enm_state == BTN_STATE_RELEASED) && (pstru_evt_data->stru_state.b_active))
    {
        /* Restart all button timers to prepare for button processing */
        TIMER_RESET (x_inst->x_debounce_timer);
        TIMER_RESET (x_inst->x_level_timer);

        /* Wake Button task up (if it's idling) to process the button */
        BaseType_t x_higher_priority_task_woken;
        xTaskNotifyFromISR (g_x_button_task, BTN_BUTTON_PRESSED, eSetBits, &x_higher_priority_task_woken);

        /* If Button task has higher priority than the currently running task */
        if (x_higher_priority_task_woken)
        {
            /* Request a context switch before the interrupt handler exits */
            portYIELD_FROM_ISR ();
        }
    }
}

/**
** @}
*/

/*
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**                           END OF FILE
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
