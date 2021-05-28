/*
!!IMPORTANT INFORMATION!!
Conversion for adc values
- 4095 (12bit value) means in Cell1 = 6.6V, Cell2 = 9.9V, Cell3 = 15.4V when voltage divide is like above
- Maximum reading value calculations For Cell1 3.3/(1/2) , Cell2 3.3/(1/3) , Cell3 3.3/(1/5)

- For this setup voltage dividing is following(ADC = 12bit = 4095):
Maximum voltages to measure for each input:
Cell1 3.3/(0.5)=6.6V , Cell2 3.3/(0.3636)=9.07V , Cell3 3.3/(0.2142)=15.40V
Calculations for each cell:
cell1 = adc1*6.60/4095.00;
cell2 = (adc2*(9.07/4095.00))-(cell1);
cell3 = (adc3*(15.40/4095.00))-(cell2+cell1);

IGNORE BELOW
TO DO LIST:
Make code for breadboard testing -> delete inverting outputs
First test with pots

Take video of codeblocks code
Take video of STM32 Breadboard code
Take video of Capacitors code
Take video of Real cell code 

*/
#include "main.h"

ADC_HandleTypeDef hadc;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);

static void GPIO_Blink(uint16_t adc1);
// static void ADC_Read_All_Channels(uint16_t adc1, uint16_t adc2, uint16_t adc3, uint16_t adc4, uint16_t adcval[]);
// ADC channel read
uint16_t ADC_Read_All_Channels(ADC_ChannelConfTypeDef sConfig, uint32_t CHANNEL);
// Charge & Discharge pins OFF
void Charge_Discharge_PINS_OFF(void);
// All Cells pins OFF
void All_Cells_PINS_OFF(void);
// Lockout pin state -> 0 to perform normally
void Lockout_PIN_WRITE(uint16_t state_lockout);
// Specific Cell discharge(balancing) -> 3 different modes
void Cell_Balancing_Discharge(uint16_t CELL_PIN_NUMBER);
// Charging on for 1000ms and then turn charging circuit OFF
void ChargingLoop(void);
// Discharging constantly on
void Discharging_ON(void);
// Cell conversion functions
float cell1_conversion(float CELL);
float cell2_conversion(float CELL2, float previous_cells);
float cell3_conversion(float CELL3, float previous_cells);
// Calculate smallest cell
float calculate_smallest_cell(void);

uint16_t adc1;
uint16_t adc2;
uint16_t adc3;
uint16_t adc4;

float adcfloat1;
float adcfloat2;
float adcfloat3;
float adcfloat4;

uint8_t DEBUG1 = 0;

uint16_t CELL;
uint8_t BatteryFull = 0;
uint8_t BatteryDead = 0;
uint8_t Charge_Discharge_State;
uint16_t state_lockout = 0;
uint16_t CELL_PIN_NUMBER;
float wanted_cell_level;
float cell_level_now;

/*
#define RED 12
#define GREEN 10
#define RESET 15

#define CHARGE 1
#define DISCHARGE 0
*/

// Pins
uint16_t CHARGE_PIN = GPIO_PIN_5;
uint16_t DISCHARGE_PIN = GPIO_PIN_4;
uint16_t CELL1_PIN = GPIO_PIN_6;
uint16_t CELL2_PIN = GPIO_PIN_7;
uint16_t CELL3_PIN = GPIO_PIN_0;
uint16_t LOCKOUT_PIN = GPIO_PIN_1;

uint16_t x = 1;
uint16_t ON = 1;
uint16_t OFF = 0;
uint16_t chargestate;
float cell1;
float cell2;
float cell3;
float current;
float total_voltage;
float smallest_cell;
float voltage_till_cell2;
float voltage_till_cell3;

char cell1str [] = "Cell1";
char cell2str [] = "Cell2";
char cell3str [] = "Cell3";

// Parameters to define when using different type of setup
float Maximum_Total_Voltage = 12.6; // Volts - Total maximum voltage allowed of battery pack -> Battery Full
float Minimum_Total_voltage = 9.60; // Volts - Total minimum voltage allowed of battery pack -> Battery Dead
float Maximum_Cell_Voltage = 4.20; // Volts - Maximum specific cell voltage allowed
float Minimum_Cell_Voltage = 3.25; // Volts - Minimum specific cell voltage allowed
float MaxDifference_Voltage = 0.300; // Volts - Maximum difference between cells allowed before balancing starts
float Upper_safe_level = 4.15;  // Volts - Voltage at what level cell is discharged when Max cell level is exceeded
float Current_limit_discharge = 20.00; // Amps - Maximum current allowed when Discharging
float Current_limit_charge = 5.00; // Amps - Maximum current allowed when Charging

int main(void)
{

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_ADC_Init();  
	
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_3;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_4;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
	
//****************************************************************************************************************
		// START
		// Write Charge / Discharge pins OFF (MCU 0 = Driver 1 = PMOS 0, MCU 1 = Driver = PMOS 0)
	  Charge_Discharge_PINS_OFF();
        // Write Cell pins OFF
				All_Cells_PINS_OFF();
		// Write LockOut pin OFF to perform normally
		Lockout_PIN_WRITE(ON);
  while (1)
  {
		// Read Charge/Discharge switch signal
		chargestate = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
		// Charge mode
		// If Charge = ON -> Continue
		if(chargestate == 1){
			if(BatteryFull == 0){
			// Write Discharge Mosfet pin OFF
			HAL_GPIO_WritePin(GPIOB, DISCHARGE_PIN, GPIO_PIN_RESET);
		//printf("\nWrite Charge/Discharge pin off for reading Battery Voltage");
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
		// Read Total Voltage of Battery
		total_voltage = cell1 + cell2 + cell3;
		//printf("\nValues\nMax Voltage: %.2fV\nCell1: %.2fV\nCell2: %.2fV\nCell3: %.2fV\nCurrent: %.2fA", total_voltage, cell1, cell2, cell3, current);
		// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
	}
		//SetConsoleTextAttribute(hConsole, RED);
		// If Total Voltage > Max Voltage -> Write Charge pin OFF
		if(total_voltage >= Maximum_Total_Voltage){
			HAL_GPIO_WritePin(GPIOB, CHARGE_PIN, GPIO_PIN_RESET);
			BatteryFull = 1;
            //printf("\nTotal voltage exceeded %.2fV. Write Charge pin off. BATTERY IS FULL", Maximum_Total_Voltage);
		}
		// If Total Voltage < Min Voltage -> Write Charge pin OFF
		// Warn about dead battery
		if(total_voltage <= Minimum_Total_voltage){
			//HAL_GPIO_WritePin(GPIOB, CHARGE_PIN, GPIO_PIN_RESET);
            //printf("\nTotal voltage lower Minimum Value %.2fV. BATTERY IS DEAD", Minimum_Total_voltage);
		}
		// If Total Voltage < Max Voltage -> Continue
		if(total_voltage < Maximum_Total_Voltage && total_voltage > Minimum_Total_voltage){
            // Continue
		}
		// Charging on for 1000ms and then turn OFF for reading the values
		ChargingLoop();
		// If Cell Voltage > Max Cell Voltage -> Drain that cell to level of Lowest Cell Voltage
        if(cell1 >= Maximum_Cell_Voltage && smallest_cell >= Minimum_Cell_Voltage && smallest_cell <= Maximum_Cell_Voltage){
					DEBUG1 = 1;
					// Loop until voltage match
					while(cell1 >= smallest_cell){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					// Discharge cell -> Delay 1000ms
					Cell_Balancing_Discharge(CELL1_PIN);
            //printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell1str, Maximum_Cell_Voltage, smallest_cell);
							}
            }
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
        if(cell1 >= Maximum_Cell_Voltage && smallest_cell <= Minimum_Cell_Voltage-0.0001){
					while(cell1 >= Minimum_Cell_Voltage+0.05){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL1_PIN);
						DEBUG1 = 2;
							}
            //printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell1str, Maximum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
            }
														// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
        if(cell1 >= Maximum_Cell_Voltage && smallest_cell >= Maximum_Cell_Voltage+0.0001){
					while(cell1 >= Upper_safe_level){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL1_PIN);
							}
            //printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell1str, Maximum_Cell_Voltage, Upper_safe_level);
            }
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
        if(cell2 >= Maximum_Cell_Voltage && smallest_cell >= Minimum_Cell_Voltage && smallest_cell <= Maximum_Cell_Voltage){
					while(cell2 >= smallest_cell){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL2_PIN);
							}
            //printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell2str, Maximum_Cell_Voltage, smallest_cell);
            }
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
        if(cell2 >= Maximum_Cell_Voltage && smallest_cell <= Minimum_Cell_Voltage-0.0001){
					while(cell2 >= Minimum_Cell_Voltage+0.05){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL2_PIN);
							}
            //printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell2str, Maximum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
            }
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
        if(cell2 >= Maximum_Cell_Voltage && smallest_cell >= Maximum_Cell_Voltage+0.0001){
					while(cell2 >= Upper_safe_level){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL2_PIN);
							}
            //printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell2str, Maximum_Cell_Voltage, Upper_safe_level);
            }
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
        if(cell3 >= Maximum_Cell_Voltage && smallest_cell >= Minimum_Cell_Voltage && smallest_cell <= Maximum_Cell_Voltage){
					while(cell3 >= smallest_cell){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL3_PIN);
							}
            //printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell3str, Maximum_Cell_Voltage, smallest_cell);
            }
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
        if(cell3 >= Maximum_Cell_Voltage && smallest_cell <= Minimum_Cell_Voltage-0.0001){
					while(cell3 >= Minimum_Cell_Voltage+0.05){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL3_PIN);
							}
            //printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell3str, Maximum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
            }
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
        if(cell3 >= Maximum_Cell_Voltage && smallest_cell >= Maximum_Cell_Voltage+0.0001){
					while(cell3 >= Upper_safe_level){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL3_PIN);
							}
            //printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell3str, Maximum_Cell_Voltage, Upper_safe_level);
            }
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
        // If Cell Voltage < Min Cell Voltage -> Warn about that Cell state & Charge the cell safe level
        if(cell1 <= Minimum_Cell_Voltage){
        //printf("\n%s voltage lower than minimum %.2fV. Charge to %.2fV", cell1str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }

        if(cell2 <= Minimum_Cell_Voltage){
        //printf("\n%s voltage lower than minimum %.2fV. Charge to %.2fV", cell2str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }

        if(cell3 <= Minimum_Cell_Voltage){
        //printf("\n%s voltage lower than minimum %.2fV. Charge to %.2fV", cell3str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }
        //SetConsoleTextAttribute(hConsole, RESET);
		// If any Cell Voltage Difference >= X mV -> Drain that cell to level of Lowest Cell Voltage
		//red();
		//SetConsoleTextAttribute(hConsole, RED);
		if(cell1 >= smallest_cell+MaxDifference_Voltage && smallest_cell >= Minimum_Cell_Voltage+0.0001 && smallest_cell <= Maximum_Cell_Voltage-0.0001){
			while(cell1 >= smallest_cell){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL1_PIN);
							}
        //printf("\nCell1 voltage difference over %.3fV to Smallest cell. Drain to %.2fV", MaxDifference_Voltage, smallest_cell);
		}
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
		if(cell1 >= smallest_cell+MaxDifference_Voltage && smallest_cell <= Minimum_Cell_Voltage && cell1 >= Minimum_Cell_Voltage+0.05){
			while(cell1 >= Minimum_Cell_Voltage+0.05){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL1_PIN);
							}
        //printf("\nCell1 voltage difference over %.3fV to Smallest cell. Drain to safe level %.2fV", MaxDifference_Voltage, Minimum_Cell_Voltage+0.05);
		}
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
		if(cell2 >= smallest_cell+MaxDifference_Voltage && smallest_cell >= Minimum_Cell_Voltage+0.0001 && smallest_cell <= Maximum_Cell_Voltage-0.0001){
			while(cell2 >= smallest_cell){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL2_PIN);
							}
        //printf("\nCell2 voltage difference over %.3fV to Smallest cell. Drain to %.2fV", MaxDifference_Voltage, smallest_cell);
		}
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
		if(cell2 >= smallest_cell+MaxDifference_Voltage && smallest_cell <= Minimum_Cell_Voltage && cell2 >= Minimum_Cell_Voltage+0.05){
			while(cell2 >= Minimum_Cell_Voltage+0.05){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL2_PIN);
							}
        //printf("\nCell2 voltage difference over %.3fV to Smallest cell. Drain to safe level %.2fV", MaxDifference_Voltage, Minimum_Cell_Voltage+0.05);
		}
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
		if(cell3 >= smallest_cell+MaxDifference_Voltage && smallest_cell >= Minimum_Cell_Voltage+0.0001 && smallest_cell <= Maximum_Cell_Voltage-0.0001){
			while(cell3 >= smallest_cell){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL3_PIN);
							}
        //printf("\nCell3 voltage difference over %.3fV to Smallest cell. Drain to %.2fV", MaxDifference_Voltage, smallest_cell);
		}
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
		if(cell3 >= smallest_cell+MaxDifference_Voltage && smallest_cell <= Minimum_Cell_Voltage && cell3 >= Minimum_Cell_Voltage+0.05){
			while(cell3 >= Minimum_Cell_Voltage+0.05){
		// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
					Cell_Balancing_Discharge(CELL3_PIN);
							}
        //printf("\nCell3 voltage difference over %.3fV to Smallest cell. Drain to safe level %.2fV", MaxDifference_Voltage, Minimum_Cell_Voltage+0.05);
		}
										// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
        //reset();
        //SetConsoleTextAttribute(hConsole, RESET);

		// -> Write charge pin ON
		// Read Current
		// If Current > Max Charging Current -> Write Charge pin OFF for 2000ms
		if(current >= Current_limit_charge){
            //SetConsoleTextAttribute(hConsole, RED);
			Charge_Discharge_PINS_OFF();
			HAL_Delay(1000);
            //printf("\nCurrent exceeded %.2f A. Write Charge/Discharge Mosfet pins off\n", Current_limit);
		}
		// If Current < Max Charging Current -> Continue (Do nothing here)
		// Write BatteryDead state back to 0 to perform normally
		BatteryDead = 0;
	}
		}
//****************************************************************************************************************
		// Discharging mode
        if(chargestate == 0 && BatteryDead == 0){
        // Write all charge pins OFF
				HAL_GPIO_WritePin(GPIOB, CHARGE_PIN, GPIO_PIN_RESET);
				// Discharging constantly ON
				HAL_GPIO_WritePin(GPIOB, DISCHARGE_PIN, GPIO_PIN_SET);
				// ADC Read All Channels
		adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_0); // Cell1
		adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1); // Cell2
		adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3); // Cell3
		adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4); // Current
				// Make adc values to float types
				adcfloat1 = adc1*1.00;
				adcfloat2 = adc2*1.00;
				adcfloat3 = adc3*1.00;
				adcfloat4 = adc4*1.00;
		// Convert adc values to float type voltage/current values
cell1 = cell1_conversion(adcfloat1);
cell2 = cell2_conversion(adcfloat2, cell1);
cell3 = cell3_conversion(adcfloat3, cell2+cell1);
		current = 0.00122*adcfloat4;
        // Read all voltage values and calculate
        total_voltage = cell1 + cell2 + cell3;
		//printf("\nValues\nMax Voltage: %.2fV\nCell1: %.2fV\nCell2: %.2fV\nCell3: %.2fV\nCurrent: %.2fA", total_voltage, cell1, cell2, cell3, current);
		// Calculate each Cell Voltage and define smallest cell
// Calculate each Cell Voltage and smallest voltage
		calculate_smallest_cell();
        // If Total Voltage <= Minimum_Total_voltage -> Write DisCharge pin off
        if(total_voltage <= Minimum_Total_voltage){
						BatteryDead = 1;
						HAL_GPIO_WritePin(GPIOB, DISCHARGE_PIN, GPIO_PIN_RESET);
					
            //printf("\nTotal voltage lower than Minimum Value %.2fV. Write DisCharge pin off.\nBATTERY IS DEAD", Minimum_Total_voltage);
        // If Cell Voltage <= Minimum Cell Voltage -> Write Discharge pin OFF and warn about the cell
        }
        if(cell1 <= Minimum_Cell_Voltage){
					BatteryDead = 1;
					HAL_GPIO_WritePin(GPIOB, DISCHARGE_PIN, GPIO_PIN_RESET);
        //printf("\n%s voltage lower than minimum %.2fV. DisCharge off & charge to at least %.2fV", cell1str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }

        if(cell2 <= Minimum_Cell_Voltage){
					BatteryDead = 1;
					HAL_GPIO_WritePin(GPIOB, DISCHARGE_PIN, GPIO_PIN_RESET);
        //printf("\n%s voltage lower than minimum %.2fV. DisCharge off & charge to at least %.2fV", cell2str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }

        if(cell3 <= Minimum_Cell_Voltage){
					BatteryDead = 1;
					HAL_GPIO_WritePin(GPIOB, DISCHARGE_PIN, GPIO_PIN_RESET);
        //printf("\n%s voltage lower than minimum %.2fV. DisCharge off & charge to at least %.2fV", cell3str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }
        //SetConsoleTextAttribute(hConsole, RESET);
		}
        // If current = over current -> Write discharge OFF for 1000ms
        if(current >= Current_limit_discharge){
					HAL_GPIO_WritePin(GPIOB, DISCHARGE_PIN, GPIO_PIN_RESET);
					HAL_Delay(1000);
            //printf("\nCurrent exceeded %.2f A. Write Charge/Discharge Mosfet pins off\n", Current_limit);
		}
		// Write BatteryFull to OFF because Discharging has occured
		BatteryFull = 0;
		//GPIO_Blink(adc1);
		}

  
//****************************************************************************************************************
uint16_t ADC_Read_All_Channels(ADC_ChannelConfTypeDef sConfig, uint32_t CHANNEL)
{
    sConfig.Channel = CHANNEL;
		HAL_ADC_ConfigChannel(&hadc, &sConfig);
		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, 500);
		uint16_t adcval = HAL_ADC_GetValue(&hadc);
	  return(adcval);
}
void Charge_Discharge_PINS_OFF(void){
	HAL_GPIO_WritePin(GPIOB, CHARGE_PIN|DISCHARGE_PIN, GPIO_PIN_RESET);
	HAL_Delay(1000);
}
void All_Cells_PINS_OFF(void){
	HAL_GPIO_WritePin(GPIOB, CELL1_PIN|CELL2_PIN|CELL3_PIN, GPIO_PIN_RESET);
}
void Lockout_PIN_WRITE(uint16_t state_lockout){
	// Remember! -> ON state == not working & OFF state == working
	if(state_lockout == ON){
	HAL_GPIO_WritePin(GPIOB, LOCKOUT_PIN, GPIO_PIN_RESET);
	}
	if(state_lockout == OFF){
	HAL_GPIO_WritePin(GPIOB, LOCKOUT_PIN, GPIO_PIN_SET);
	}
}
void Cell_Balancing_Discharge(uint16_t CELL_PIN_NUMBER){
	// Discharge cell for 1000ms
	HAL_GPIO_WritePin(GPIOB, CELL_PIN_NUMBER, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOB, CELL_PIN_NUMBER, GPIO_PIN_RESET);
	HAL_Delay(200);
}
// Charging on for 1000ms
void ChargingLoop(void){
	HAL_GPIO_WritePin(GPIOB, CHARGE_PIN, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOB, CHARGE_PIN, GPIO_PIN_RESET);
	HAL_Delay(100);
}
// Discharging constantly on
void Discharging_ON(void){
	HAL_GPIO_WritePin(GPIOB, CHARGE_PIN, GPIO_PIN_SET);
}
// Cell conversion functions
float cell1_conversion(float CELL1){
	float converted_cell_value1;
	//converted_cell_value = (CELL1*6.60/4095.00);
	//converted_cell_value = CELL1;
	converted_cell_value1 = CELL1*5.00/4095.00;
	return converted_cell_value1;
}
float cell2_conversion(float CELL2, float previous_cells){
	float converted_cell_value2;
	//voltage_till_cell2 = CELL2*9.07/4095.00;
	//converted_cell_value = (CELL2*9.07/4095.00)-(previous_cells);
	converted_cell_value2 = CELL2*5.00/4095.00;
	return converted_cell_value2;
}
float cell3_conversion(float CELL3, float previous_cells){
	float converted_cell_value3;
	//voltage_till_cell3 = CELL3*15.40/4095.00;
	//converted_cell_value = (CELL3*15.40/4095.00)-(previous_cells);
	converted_cell_value3 = CELL3*5.00/4095.00;
	return converted_cell_value3;
}
float calculate_smallest_cell(void){
		// Calculate each Cell Voltage
		if(cell1 <= cell2 && cell1 <= cell3){
        smallest_cell = cell1;
		}

		if(cell2 <= cell1 && cell2 <= cell3){
        smallest_cell = cell2;
		}

		if(cell3 <= cell2 && cell3 <= cell1){
        smallest_cell = cell3;
		}
		return smallest_cell;
	}
void GPIO_Blink(uint16_t adc1)
{
	if(adc1 <= 2000){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);
		HAL_Delay(200);
		}
		
		if(adc1 > 2000){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_Delay(200);
		}

}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc.Init.Resolution = ADC_RESOLUTION_10B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_1;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_3;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_4;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 PB4 PB5 
                           PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

/*		TRIED IDEAS HERE



		sConfig.Channel = ADC_CHANNEL_0;
		HAL_ADC_ConfigChannel(&hadc, &sConfig);
		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, 500);
			adc1 = HAL_ADC_GetValue(&hadc);
			adcval[0] = adc1;
		
		sConfig.Channel = ADC_CHANNEL_1;
		HAL_ADC_ConfigChannel(&hadc, &sConfig);
		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, 500);
			adc2 = HAL_ADC_GetValue(&hadc);
			adcval[1] = adc2;
		
		sConfig.Channel = ADC_CHANNEL_3;
		HAL_ADC_ConfigChannel(&hadc, &sConfig);
		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, 500);
			adc3 = HAL_ADC_GetValue(&hadc);
			adcval[2] = adc3;
		
		sConfig.Channel = ADC_CHANNEL_4;
		HAL_ADC_ConfigChannel(&hadc, &sConfig);
		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, 500);
			adc4 = HAL_ADC_GetValue(&hadc);
			adcval[3] = adc4;
			
			
*/