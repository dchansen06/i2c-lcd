/* Exit codes:
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

/* Prints if the lcd is running or not */
int main()
{
	printf("LCD: ");

	if (lcdservice())
	{
		printf("Program running\n");
	}
	else
	{
		printf("Program not running\n");
	}
}
