#include "inc/Board.hpp"
#include "inc/Gpio.hpp"

/*================================ define ===================================*/
#define LED_RED_PORT         ( GPIO_PORTF_BASE )
#define LED_RED_PIN          ( GPIO_PIN_1 )

#define LED_BLUE_PORT        ( GPIO_PORTF_BASE )
#define LED_BLUE_PIN         ( GPIO_PIN_2 )

#define LED_GREEN_PORT         ( GPIO_PORTF_BASE )
#define LED_GREEN_PIN          ( GPIO_PIN_3 )

#define SW_1_PORT            ( GPIO_PORTF_BASE )
#define SW_1_PIN             ( GPIO_PIN_4 )

#define SW_2_PORT            ( GPIO_PORTF_BASE )
#define SW_2_PIN             ( GPIO_PIN_0 )

#define SW_3_PORT            ( GPIO_PORTA_BASE )
#define SW_3_PIN             ( GPIO_PIN_7 )

#define TIMER0A_PERIPHERAL      ( SYSCTL_PERIPH_TIMER0 )
#define TIMER0A_BASE            ( TIMER0_BASE )
#define TIMER0A_SOURCE          ( TIMER_BOTH )
#define TIMER0A_CONFIG          ( TIMER_CFG_ONE_SHOT_UP )
#define TIMER0A_INTERRUPT       ( INT_TIMER0A )
#define TIMER0A_INTERRUPT_MODE  ( TIMER_TIMA_TIMEOUT )

#define TIMER1A_PERIPHERAL      ( SYSCTL_PERIPH_TIMER1 )
#define TIMER1A_BASE            ( TIMER1_BASE )
#define TIMER1A_SOURCE          ( TIMER_BOTH )
#define TIMER1A_CONFIG          ( TIMER_CFG_ONE_SHOT_UP )
#define TIMER1A_INTERRUPT       ( INT_TIMER1A )
#define TIMER1A_INTERRUPT_MODE  ( TIMER_TIMA_TIMEOUT )


/*================================ typedef ==================================*/

/*================================ private ==================================*/

/*=============================== variables =================================*/

/* LEDs */
static GpioConfig led_blue_cfg  {LED_BLUE_PORT,  LED_BLUE_PIN,  0, 0, 0};
static GpioConfig led_red_cfg  {LED_RED_PORT,  LED_RED_PIN,  0, 0, 0};
static GpioConfig led_green_cfg  {LED_GREEN_PORT,  LED_GREEN_PIN,  0, 0, 0};

// Constantes para conigurar o modo de interrupção

//! - \b GPIO_FALLING_EDGE sets detection to edge and trigger to falling
//! - \b GPIO_RISING_EDGE sets detection to edge and trigger to rising
//! - \b GPIO_BOTH_EDGES sets detection to both edges
//! - \b GPIO_LOW_LEVEL sets detection to low level
//! - \b GPIO_HIGH_LEVEL sets detection to high level

/* LEDs */
GpioOut led_blue;
GpioOut led_red;
GpioOut led_green;

/* Buttons */
static GpioConfig switch1_cfg  {SW_1_PORT,  SW_1_PIN, 0, GPIO_LOW_LEVEL, 1};
static GpioConfig switch2_cfg  {SW_2_PORT,  SW_2_PIN,  0, GPIO_LOW_LEVEL, 1};
//static GpioConfig switch3_cfg  {SW_3_PORT,  SW_3_PIN,  0, GPIO_LOW_LEVEL, 1};

/* Buttons */
GpioIn sw1;
GpioIn sw2;
//GpioIn sw3;

/* Timers */
static TimerConfig timer0a_cfg {TIMER0A_PERIPHERAL, TIMER0A_BASE, TIMER0A_SOURCE, TIMER0A_CONFIG, TIMER0A_INTERRUPT, TIMER0A_INTERRUPT_MODE};

Timer timer0a;

static TimerConfig timer1a_cfg {TIMER1A_PERIPHERAL, TIMER1A_BASE, TIMER1A_SOURCE, TIMER1A_CONFIG, TIMER1A_INTERRUPT, TIMER1A_INTERRUPT_MODE};

Timer timer1a;

/* Board */
BoardImplementation board;

/*=============================== prototypes ================================*/

/*================================= public ==================================*/

/* Each BoardImplementation needs to provide its own constructor */
BoardImplementation::BoardImplementation(){}

/* Each BoardImplementation needs to provide its own init function */
void BoardImplementation::init(void)
{
	/* Initialize basic board parameters */
	Board::init();

	//init leds and switches
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; // activate UART0
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;     // 1) activate clock for Port F

	UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
	UART0_IBRD_R = 43;                    // IBRD = int(80,000,000 / (16 * 115200)) = int(43.402778)
	UART0_FBRD_R = 26;                    // FBRD = round(0.402778 * 64) = 26
	UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
	UART0_CTL_R |= UART_CTL_UARTEN;       // enable UART

	GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1,PA0
	GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1,PA0
										 // configure PA1,PA0 as UART0
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
	GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA1,PA0

	int delay = SYSCTL_RCGC2_R;           // allow time for clock to start

	//desbloqueia pino PF0
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;   // 2) unlock PORTF_CR_R
	GPIO_PORTF_CR_R |= GPIO_LOCK_LOCKED;           // allow changes to PF0

	GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
//	GPIO_PORTA_PUR_R = 0x80;		// enable pull-up on PA7

	sw1.enable(switch1_cfg);
	sw2.enable(switch2_cfg);
//	sw3.enable(switch3_cfg);

	/* Get a reference to the interruptHandler object */
	InterruptHandler& interruptHandler = InterruptHandler::getInstance();

	/* Inicializa o interruptHandler */
	interruptHandler.init();

	led_blue.enable(led_blue_cfg);
	led_red.enable(led_red_cfg);
	led_green.enable(led_green_cfg);

	led_blue.off();
	led_red.off();
	led_green.off();
	timer0a.init(timer0a_cfg);
	timer0a.setPrescaler(1);
	timer1a.init(timer1a_cfg);
	timer1a.setPrescaler(1);
}

/*================================ private ==================================*/
