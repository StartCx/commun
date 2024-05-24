CAN1_Configuration();		//TX PA11， RX PA12 	波特率 250k	优先级 2，0

Usart1_Init(50000);		//TX PA9 ， RX PA10	波特率 50k 	优先级 1，0

SIM_Uart_Init(&SIM_UART);	//TX PA2 ， RX PA3 	波特率 50k	

Led_Device.Init(&Led_Device);//PC13

I2Cx_Init(I2C1_M_HARDWARE);	//SCL PB6 ， SDA PB7 	50k	bus = 0

I2Cx_Init(I2C2_M_BITBANG);	//SCL PB10， SDA PB11	50k 	bus = 1

I2Cx_Init(I2C3_M_SOFTWARE);	//SCL PB8 ， SDA PB9	100k+ 	bus = 2

I2Cx_Init(I2C4_S_BITBANG);	//SCL PB4 ， SDA PB5	50k		
	
SPIx_Init(SPI1_M_HARDWARE);	//SCK PA5,  MISO PA6,  MOSI PA7,  CS0 PA15; 		bus = 0

SPIx_Init(SPI2_M_BITBANG); 	//SCK PB13, MISO PB14, MOSI PB15, CS0 PB12, CS1 PA8; 	bus = 1

SPIx_Init(SPI3_M_SOFTWARE);	//SCK PC14, MISO PC15, MOSI PA4,  CS0 PB3;		bus = 2

ADC_Configuration();		//PA0，PA1

TIM4_Configuration();		//优先级 0，0

SIM_Servo_GPIO_Init(&Servo_Output0);//SIM PB0 period 20ms	id = 0

TIM3_Configuration();		    //PWM PB1 period 20ms	id = 1

shell使用模拟串口

自定义协议使用串口1



需要添加：
1. gpio_output 需要添加 shadow_reg;
2. simulate_pwm 需要添加shadow_reg;
   

