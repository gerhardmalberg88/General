#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define RED 12
#define GREEN 10
#define RESET 15

#define CHARGE 1
#define DISCHARGE 0

int x = 1;
float cell1;
float cell2;
float cell3;
float current;
float total_voltage;
float smallest_cell;

// Parameters to define when using different type of setup
float Maximum_Total_Voltage = 7.40; // Volts - Total maximum voltage allowed of battery pack -> Battery Full
float Minimum_Total_voltage = 1.00; // Volts - Total minimum voltage allowed of battery pack -> Battery Dead
float Maximum_Cell_Voltage = 2.60; // Volts - Maximum specific cell voltage allowed
float Minimum_Cell_Voltage = 1.00; // Volts - Minimum specific cell voltage allowed
float MaxDifference_Voltage = 0.300; // Volts - Maximum difference between cells allowed before balancing starts
float Upper_safe_level = 2.45;  // Volts - Voltage at what level cell is discharged when Max cell level is exceeded
float Current_limit_discharge = 20.00; // Amps - Maximum current allowed when Discharging
float Current_limit_charge = 5.00; // Amps - Maximum current allowed when Charging

char cell1str [] = "Cell1";
char cell2str [] = "Cell2";
char cell3str [] = "Cell3";


int chargestate;

void red () {
  //printf("\033[1;31m");
}

void yellow () {
  //printf("\033[1;33m");
}

void reset () {
  //printf("\033[0m");
}

void green () {
  //printf("\033[0;32m");
}

void Drain_to_Smallest_Voltage_Level() {
  // Function that Drains cell to safe level if Cell with lowest level is lower than Minimum cell voltage
  if(smallest_cell <= Minimum_Cell_Voltage-0.0001){
    printf("Drain");
  }
}


int main()
{
    // Only for Windows
    HANDLE  hConsole;
    int k;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, RESET);
    // End for windows

printf("Start of the Program\n");
		// START
		// Write Charge / Discharge pins OFF (MCU 0 = Driver 1 = PMOS 0, MCU 1 = Driver = PMOS 0)
		green ();
		SetConsoleTextAttribute(hConsole, RESET);
		printf("\nWrite Discharge and Charge pin off");
        // Write Cell pins OFF
        printf("\nWrite Cells pin off");
		// Write LockOut pin OFF
		printf("\nWrite LockOut pin off");
		reset();
		printf("\n*************************************************************************");

		// LOOP
		while(1){
		// Write Charge/Discharge Mosfet pin off for reading values
		printf("\nWrite Charge/Discharge pin off for reading Battery Voltage");
		// ADC Read All Channels
		//adc1 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_1);
		//adc2 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_2);
		//adc3 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_3);
		//adc4 = ADC_Read_All_Channels(sConfig, ADC_CHANNEL_4);
		// Read Charge/Discharge signal
		//Charge_Discharge_State = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
		//// ASK for Cell Voltages and Current
		printf("\nVoltage of cell 1: ");
		scanf("%f", &cell1);
		printf("Voltage of cell 2: ");
		scanf("%f", &cell2);
		printf("Voltage of cell 3: ");
		scanf("%f", &cell3);
		printf("Current: ");
		scanf("%f", &current);
		printf("Charge state(1 = on): ");
		scanf("%d", &chargestate);

		// Discharge
		// If Discharge = ON -> Write Discharge pin ON
		// Read Current
		// If Current > Max Current -> Write Discharge pin OFF for 1000ms
		// If Current < Max Current -> Do nothing

		// Charge
		// If Charge = ON -> Continue
		if(chargestate == 1){
		// Read Total Voltage of Battery
		total_voltage = cell1 + cell2 + cell3;
		printf("\nValues\nMax Voltage: %.2fV\nCell1: %.2fV\nCell2: %.2fV\nCell3: %.2fV\nCurrent: %.2fA", total_voltage, cell1, cell2, cell3, current);
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
		// -> Write charge pin ON
		// Read Current
		// If Current > Max Charging Current -> Write Charge pin OFF for 2000ms
		if(current >= Current_limit_charge){
            SetConsoleTextAttribute(hConsole, RED);
            printf("\nCurrent exceeded %.2f A. Write Charge/Discharge Mosfet pins off\n", Current_limit_charge);
            SetConsoleTextAttribute(hConsole, RESET);
		}
		// If Current < Max Charging Current -> Continue (Do nothing here)
		SetConsoleTextAttribute(hConsole, RED);
		// If Total Voltage > Max Voltage -> Write Charge pin OFF
		if(total_voltage >= Maximum_Total_Voltage){
            printf("\nTotal voltage exceeded %.2fV. Write Charge pin off. BATTERY IS FULL", Maximum_Total_Voltage);
		}
		// If Total Voltage < Min Voltage -> Write Charge pin OFF
		// Warn about dead battery
		if(total_voltage <= Minimum_Total_voltage){
            printf("\nTotal voltage lower Minimum Value %.2fV. BATTERY IS DEAD", Minimum_Total_voltage);
		}
		// If Total Voltage < Max Voltage -> Continue
		if(total_voltage < Maximum_Total_Voltage && total_voltage > Minimum_Total_voltage){
            // Continue
		}

		// If Cell Voltage > Max Cell Voltage -> Drain that cell to level of Lowest Cell Voltage
        if(cell1 >= Maximum_Cell_Voltage && smallest_cell >= Minimum_Cell_Voltage && smallest_cell <= Maximum_Cell_Voltage){
            printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell1str, Maximum_Cell_Voltage, smallest_cell);
            }
        if(cell1 >= Maximum_Cell_Voltage && smallest_cell <= Minimum_Cell_Voltage-0.0001){
            printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell1str, Maximum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
            }
        if(cell1 >= Maximum_Cell_Voltage && smallest_cell >= Maximum_Cell_Voltage+0.0001){
            printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell1str, Maximum_Cell_Voltage, Upper_safe_level);
            }

        if(cell2 >= Maximum_Cell_Voltage && smallest_cell >= Minimum_Cell_Voltage && smallest_cell <= Maximum_Cell_Voltage){
            printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell2str, Maximum_Cell_Voltage, smallest_cell);
            }
        if(cell2 >= Maximum_Cell_Voltage && smallest_cell <= Minimum_Cell_Voltage-0.0001){
            printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell2str, Maximum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
            }
        if(cell2 >= Maximum_Cell_Voltage && smallest_cell >= Maximum_Cell_Voltage+0.0001){
            printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell2str, Maximum_Cell_Voltage, Upper_safe_level);
            }

        if(cell3 >= Maximum_Cell_Voltage && smallest_cell >= Minimum_Cell_Voltage && smallest_cell <= Maximum_Cell_Voltage){
            printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell3str, Maximum_Cell_Voltage, smallest_cell);
            }
        if(cell3 >= Maximum_Cell_Voltage && smallest_cell <= Minimum_Cell_Voltage-0.0001){
            printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell3str, Maximum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
            }
        if(cell3 >= Maximum_Cell_Voltage && smallest_cell >= Maximum_Cell_Voltage+0.0001){
            printf("\n%s voltage exceeded %.2fV. Drain to %.2fV",cell3str, Maximum_Cell_Voltage, Upper_safe_level);
            }
        // If Cell Voltage < Min Cell Voltage -> Warn about that Cell state & Charge the cell safe level
        if(cell1 <= Minimum_Cell_Voltage){
        printf("\n%s voltage lower than minimum %.2fV. Charge to %.2fV", cell1str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }

        if(cell2 <= Minimum_Cell_Voltage){
        printf("\n%s voltage lower than minimum %.2fV. Charge to %.2fV", cell2str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }

        if(cell3 <= Minimum_Cell_Voltage){
        printf("\n%s voltage lower than minimum %.2fV. Charge to %.2fV", cell3str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }
        SetConsoleTextAttribute(hConsole, RESET);
		// If any Cell Voltage Difference >= 100mV -> Drain that cell to level of Lowest Cell Voltage
		red();
		SetConsoleTextAttribute(hConsole, RED);
		if(cell1 >= smallest_cell+MaxDifference_Voltage && smallest_cell >= Minimum_Cell_Voltage+0.0001 && smallest_cell <= Maximum_Cell_Voltage-0.0001){
        printf("\nCell1 voltage difference over %.3fV to Smallest cell. Drain to %.2fV", MaxDifference_Voltage, smallest_cell);
		}
		if(cell1 >= smallest_cell+MaxDifference_Voltage && smallest_cell <= Minimum_Cell_Voltage && cell1 >= Minimum_Cell_Voltage+0.05){
        printf("\nCell1 voltage difference over %.3fV to Smallest cell. Drain to safe level %.2fV", MaxDifference_Voltage, Minimum_Cell_Voltage+0.05);
		}

		if(cell2 >= smallest_cell+MaxDifference_Voltage && smallest_cell >= Minimum_Cell_Voltage+0.0001 && smallest_cell <= Maximum_Cell_Voltage-0.0001){
        printf("\nCell2 voltage difference over %.3fV to Smallest cell. Drain to %.2fV", MaxDifference_Voltage, smallest_cell);
		}
		if(cell2 >= smallest_cell+MaxDifference_Voltage && smallest_cell <= Minimum_Cell_Voltage&& cell2 >= Minimum_Cell_Voltage+0.05){
        printf("\nCell2 voltage difference over %.3fV to Smallest cell. Drain to safe level %.2fV", MaxDifference_Voltage, Minimum_Cell_Voltage+0.05);
		}

		if(cell3 >= smallest_cell+MaxDifference_Voltage && smallest_cell >= Minimum_Cell_Voltage+0.0001 && smallest_cell <= Maximum_Cell_Voltage-0.0001){
        printf("\nCell3 voltage difference over %.3fV to Smallest cell. Drain to %.2fV", MaxDifference_Voltage, smallest_cell);
		}
		if(cell3 >= smallest_cell+MaxDifference_Voltage && smallest_cell <= Minimum_Cell_Voltage&& cell3 >= Minimum_Cell_Voltage+0.05){
        printf("\nCell3 voltage difference over %.3fV to Smallest cell. Drain to safe level %.2fV", MaxDifference_Voltage, Minimum_Cell_Voltage+0.05);
		}
        reset();
        SetConsoleTextAttribute(hConsole, RESET);

		}
		// DisCharging mode on
        if(chargestate == 0){
        // Write all charge pins off
        SetConsoleTextAttribute(hConsole, RESET);
        printf("\nWrite Charge pin and Cell discharge pins OFF\n");
        // Read all voltage values and calculate
        total_voltage = cell1 + cell2 + cell3;
		printf("\nValues\nMax Voltage: %.2fV\nCell1: %.2fV\nCell2: %.2fV\nCell3: %.2fV\nCurrent: %.2fA", total_voltage, cell1, cell2, cell3, current);
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
		// If current = over current -> Write
        if(current >= Current_limit_discharge){
            SetConsoleTextAttribute(hConsole, RED);
            printf("\nCurrent exceeded %.2f A. Write Charge/Discharge Mosfet pins off\n", Current_limit_discharge);
            SetConsoleTextAttribute(hConsole, RESET);
		}
        // If Total Voltage > Total Voltage limit -> Write DisCharge pin off
        SetConsoleTextAttribute(hConsole, RED);
        if(total_voltage <= Minimum_Total_voltage){
            printf("\nTotal voltage lower than Minimum Value %.2fV. Write DisCharge pin off.\nBATTERY IS DEAD", Minimum_Total_voltage);
        // If Cell Voltage <= Minimum Cell Voltage -> Write Discharge pin OFF and warn about the cell
        }
        if(cell1 <= Minimum_Cell_Voltage){
        printf("\n%s voltage lower than minimum %.2fV. DisCharge off & charge to at least %.2fV", cell1str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }

        if(cell2 <= Minimum_Cell_Voltage){
        printf("\n%s voltage lower than minimum %.2fV. DisCharge off & charge to at least %.2fV", cell2str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }

        if(cell3 <= Minimum_Cell_Voltage){
        printf("\n%s voltage lower than minimum %.2fV. DisCharge off & charge to at least %.2fV", cell3str, Minimum_Cell_Voltage, Minimum_Cell_Voltage+0.05);
        }
        SetConsoleTextAttribute(hConsole, RESET);
		}

		//GPIO_Blink(adc1);
		SetConsoleTextAttribute(hConsole, RESET);
		printf("\n*************************************************************************");

		}


}


