/**
 ******************************************************************************
 * @file    PWM/PWM_PWMOutput/main.c
 * @author  WIZnet
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2018 WIZnet</center></h2>
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup W7500x_StdPeriph_Examples
 * @{
 */

/** @addtogroup PWM_PWMOutput
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void UART_Config(void);
void delay_ms(__IO uint32_t nCount);
static void SG90_Servo_Init(PWM_TypeDef* PWMx);
void SG90_Servo_Angle(PWM_TypeDef* PWMx, uint16_t angle);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
    SystemInit();

    UART_Config();
    SG90_Servo_Init(PWM0);

	printf("W7500x Standard Peripheral Library version : %d.%d.%d\r\n", __W7500X_STDPERIPH_VERSION_MAIN, __W7500X_STDPERIPH_VERSION_SUB1, __W7500X_STDPERIPH_VERSION_SUB2);
	
    printf("SourceClock : %d\r\n", (int) GetSourceClock());
    printf("SystemClock : %d\r\n", (int) GetSystemClock());

    printf("System Loop Start\r\n");

    while (1) {
        SG90_Servo_Angle(PWM0, 1);
        delay_ms(300);
        SG90_Servo_Angle(PWM0, 90);
        delay_ms(300);
        SG90_Servo_Angle(PWM0, 180);
        delay_ms(300);
        SG90_Servo_Angle(PWM0, 90);
        delay_ms(300);
    }
	
	return 0;
}

/**
 * @brief  Configures the UART Peripheral.
 * @note
 * @param  None
 * @retval None
 */
static void UART_Config(void)
{
    UART_InitTypeDef UART_InitStructure;

    UART_StructInit(&UART_InitStructure);
	
#if defined (USE_WIZWIKI_W7500_EVAL)
    UART_Init(UART1, &UART_InitStructure);
    UART_Cmd(UART1, ENABLE);
#else
    S_UART_Init(115200);
    S_UART_Cmd(ENABLE);
#endif
}

void delay_ms(__IO uint32_t nCount)
{
    volatile uint32_t delay = nCount * 2500;  // approximate loops per ms at 24 MHz, Debug config
    for(; delay != 0; delay--)
        __NOP();
}

/**
 * @brief  Configures the PWM Peripheral.
 * @note
 * @param  None
 * @retval None
 */
static void SG90_Servo_Init(PWM_TypeDef* PWMx)
{
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource0, PAD_AF2);
    PWM_InitTypeDef PWM_InitStructure;
    PWM_StructInit(&PWM_InitStructure);

    PWM_InitStructure.PWM_Output = PWM_Output_OutEnable_InDisable;
    // 1/8,000,000Hz = 0.000000125s
    // 0.000000125 * 8 = 0.000001s
    PWM_InitStructure.PWM_Duty = 500 - 1;
    PWM_InitStructure.PWM_Prescale_Counter = 8 - 1;
    // 0.000001 * 20000 = 20ms = 50Hz
    PWM_InitStructure.PWM_Period = 20000 - 1;
    PWM_Init(PWMx, &PWM_InitStructure);
    PWM_Cmd(PWMx, ENABLE);
}

void SG90_Servo_Angle(PWM_TypeDef* PWMx, uint16_t angle) {

    uint32_t pulse_length;
    PWM_InitTypeDef PWM_InitStructure;

    PWM_Cmd(PWMx, DISABLE);

    pulse_length = 500 + (angle * 2000 / 180);

    PWM_InitStructure.PWM_Duty = pulse_length - 1;

    PWM_Init(PWMx, &PWM_InitStructure);
    PWM_Cmd(PWMx, ENABLE);
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/******************** (C) COPYRIGHT WIZnet *****END OF FILE********************/
