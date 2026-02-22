/*
 * STM32L476RG + Arduino Shield pin/register map.
 *
 * Requested logical mapping:
 *   S1 on A1 -> D1 LED
 *   S2 on A2 -> D2 LED
 *   S3 on A3 -> D3 LED
 *
 * Multi Function Shield assumptions (common Funduino layout):
 *   Buttons:
 *     S1 -> A1, S2 -> A2, S3 -> A3 (active-low)
 *   LEDs (shield labels):
 *     D1 -> Arduino D13, D2 -> Arduino D12, D3 -> Arduino D11 (active-low)
 *
 * NUCLEO-L476RG Arduino header mapping:
 *   A1 -> PA1, A2 -> PA4, A3 -> PB0
 *   D13 -> PA5, D12 -> PA6, D11 -> PA7
 */

/* RCC registers */
.equ RCC_BASE,                         0x40021000
.equ RCC_AHB2ENR,                      (RCC_BASE + 0x4C)
.equ RCC_AHB2ENR_GPIOAEN,              (1 << 0)
.equ RCC_AHB2ENR_GPIOBEN,              (1 << 1)
.equ RCC_AHB2ENR_GPIOABEN_MASK,        (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN)

/* GPIO base addresses */
.equ GPIOA_BASE,                       0x48000000
.equ GPIOB_BASE,                       0x48000400

/* GPIO register offsets */
.equ GPIO_MODER_OFFSET,                0x00
.equ GPIO_PUPDR_OFFSET,                0x0C
.equ GPIO_IDR_OFFSET,                  0x10
.equ GPIO_BSRR_OFFSET,                 0x18

/* GPIO register addresses */
.equ GPIOA_MODER,                      (GPIOA_BASE + GPIO_MODER_OFFSET)
.equ GPIOA_PUPDR,                      (GPIOA_BASE + GPIO_PUPDR_OFFSET)
.equ GPIOA_IDR,                        (GPIOA_BASE + GPIO_IDR_OFFSET)
.equ GPIOA_BSRR,                       (GPIOA_BASE + GPIO_BSRR_OFFSET)

.equ GPIOB_MODER,                      (GPIOB_BASE + GPIO_MODER_OFFSET)
.equ GPIOB_PUPDR,                      (GPIOB_BASE + GPIO_PUPDR_OFFSET)
.equ GPIOB_IDR,                        (GPIOB_BASE + GPIO_IDR_OFFSET)
.equ GPIOB_BSRR,                       (GPIOB_BASE + GPIO_BSRR_OFFSET)

/* Shield button pins */
.equ BTN_S1_A1_PIN,                    1   /* PA1 */
.equ BTN_S2_A2_PIN,                    4   /* PA4 */
.equ BTN_S3_A3_PIN,                    0   /* PB0 */

/* Shield LED pins */
.equ LED_D1_PIN,                       5   /* D13 -> PA5 */
.equ LED_D2_PIN,                       6   /* D12 -> PA6 */
.equ LED_D3_PIN,                       7   /* D11 -> PA7 */

/* Input masks for IDR */
.equ BTN_S1_A1_MASK,                   (1 << BTN_S1_A1_PIN)
.equ BTN_S2_A2_MASK,                   (1 << BTN_S2_A2_PIN)
.equ BTN_S3_A3_MASK,                   (1 << BTN_S3_A3_PIN)

/* BSRR masks for LED set/reset */
.equ LED_D1_SET_MASK,                  (1 << LED_D1_PIN)
.equ LED_D1_RESET_MASK,                (1 << (LED_D1_PIN + 16))
.equ LED_D2_SET_MASK,                  (1 << LED_D2_PIN)
.equ LED_D2_RESET_MASK,                (1 << (LED_D2_PIN + 16))
.equ LED_D3_SET_MASK,                  (1 << LED_D3_PIN)
.equ LED_D3_RESET_MASK,                (1 << (LED_D3_PIN + 16))

/* Multi Function Shield LEDs are active-low (LOW=ON, HIGH=OFF). */
.equ LED_D1_ON_MASK,                   LED_D1_RESET_MASK
.equ LED_D1_OFF_MASK,                  LED_D1_SET_MASK
.equ LED_D2_ON_MASK,                   LED_D2_RESET_MASK
.equ LED_D2_OFF_MASK,                  LED_D2_SET_MASK
.equ LED_D3_ON_MASK,                   LED_D3_RESET_MASK
.equ LED_D3_OFF_MASK,                  LED_D3_SET_MASK

.equ GPIOA_LED_OFF_ALL_MASK,           (LED_D1_OFF_MASK | LED_D2_OFF_MASK | LED_D3_OFF_MASK)

/* MODER masks: 00=input, 01=output */
.equ GPIOA_MODER_LED_CLEAR_MASK,       ((3 << (LED_D1_PIN * 2)) | (3 << (LED_D2_PIN * 2)) | (3 << (LED_D3_PIN * 2)))
.equ GPIOA_MODER_LED_OUTPUT_MASK,      ((1 << (LED_D1_PIN * 2)) | (1 << (LED_D2_PIN * 2)) | (1 << (LED_D3_PIN * 2)))

.equ GPIOA_MODER_BTN_CLEAR_MASK,       ((3 << (BTN_S1_A1_PIN * 2)) | (3 << (BTN_S2_A2_PIN * 2)))
.equ GPIOB_MODER_BTN_CLEAR_MASK,       (3 << (BTN_S3_A3_PIN * 2))

/* PUPDR masks: 00=no pull, 01=pull-up */
.equ GPIOA_PUPDR_BTN_CLEAR_MASK,       ((3 << (BTN_S1_A1_PIN * 2)) | (3 << (BTN_S2_A2_PIN * 2)))
.equ GPIOA_PUPDR_BTN_PULLUP_MASK,      ((1 << (BTN_S1_A1_PIN * 2)) | (1 << (BTN_S2_A2_PIN * 2)))
.equ GPIOB_PUPDR_BTN_CLEAR_MASK,       (3 << (BTN_S3_A3_PIN * 2))
.equ GPIOB_PUPDR_BTN_PULLUP_MASK,      (1 << (BTN_S3_A3_PIN * 2))
