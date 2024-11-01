/* Exit codes:
 * -2, The temperature was not readable from sysfs
 * -1, Could not tell if the service was on or off
 * 0, Nothing unexpected happened, everything is ok.
 *
 * The most likely cause of a problem is from a file not being readable.
 *
 * This program is designed to print out user-friendly information to the user
 * about the current state of various the lcd and temperature.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "services.h"

/* Prints the current temperature */
void printtemp(void)
{
	printf("Temperature:\n");
	printf("%4.2fC\n", cputemp() / 1000.0);
}

/* Prints if the lcd is running or not */
void printlcd(void)
{
	printf("LCD:\n");

	if (lcdservice())
	{
		printf("Program running\n");
	}
	else
	{
		printf("Program not running\n");
	}
}

/* Prints all information */
void printall(void)
{
	printtemp();
	printf("\n");
	printlcd();
	printf("\n");
	printapi();
	printf("\n");
	printfan();
}

/* Prints help information */
void printunsure(char * name)
{
	printf("Format: %s [OPTION]\n", name);
	printf("\nGeneral:\n");
	printf("all, unspecified, default, [blank]\t\tDefault option: Returns all information\n");
	printf("\nLCD:\n");
	printf("lcd, lcdprogram, lcdservice, l, d\t\tSee if the lcd program is currently running\n");
	printf("\nTemperature:\n");
	printf("temp, temperature, c, t\t\t\t\tGet the current CPU Temperature in celcius\n");
}

/* Main logic tree */
int main(int argc, char * argv[])
{
	if (argc > 2)
	{
		printunsure(argv[0]);
		return 0;
	}
	else if (argc == 1)
	{
		printall();
		return 0;
	}
	else
	{
		else if (strcmp(argv[1], "temp") == 0 || strcmp(argv[1], "temperature") == 0 || strcmp(argv[1], "c") == 0 || strcmp(argv[1], "t") == 0)
		{
			printtemp();
			return 0;
		}
		else if (strcmp(argv[1], "lcd") == 0 || strcmp(argv[1], "lcdprogram") == 0 || strcmp(argv[1], "lcdservice") == 0 || strcmp(argv[1], "display") == 0 || strcmp(argv[1], "l") == 0 || strcmp(argv[1], "d") == 0)
		{
			printlcd();
			return 0;
		}
		else if (strcmp(argv[1], "all") == 0 || strcmp(argv[1], "unspecified") == 0 || strcmp(argv[1], "default") == 0 || strcmp(argv[1], "[blank]") == 0)
		{
			printall();
			return 0;
		}
		else
		{
			printf("Option provided: %s\n", argv[1]);
			printunsure(argv[0]);
			return 0;
		}
	}
}
