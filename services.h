/* This program is a header file, it is not meant to be run
 *
 * This program provides basic access to all of the services and daemons running
 * that are controlled through the Makefil system, including fand, lcd and apid
 *
 * The most likely cause of failure is a missed name file not being updated
 *
 * Exit Codes:
 * -3: A service could not be started or stopped
 * -2: The current state of a service could not be determined
 * -1: Other failures
 *
 * This is a header file and as such should never exit the program without an error
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Check if lcd service is running, returns success/fail */
int lcdservice(void)
{
        FILE * fp;
        fp = popen("/usr/bin/systemctl is-active lcd.service --quiet", "r");

        if (fp == NULL) //Error checking
        {
		fprintf(stderr, "Falled to check lcd service!\n");
                exit(-2);
        }

        if (0 == WEXITSTATUS(pclose(fp)))       //Exit code given through '--quiet'
        {
                return 1;       //On
        }
        else
        {
                return 0;       //Off
        }
}

/* Turn the lcd on */
void lcdon(void)
{
	FILE * fp;
	fp = popen("/usr/bin/sudo /usr/bin/systemctl restart lcd.service", "r");	//Difference between 'restart' and 'start' crucial for option 'refresh'

	if (fp == NULL)	//Error checking
	{
		fprintf(stderr, "Falled to start lcd service!\n");
		exit(-3);
	}

	if (0 == WEXITSTATUS(pclose(fp)))	//Error checking
	{
		printf("Started service\n");
	}
	else
	{
		fprintf(stderr, "Falled to start lcd service!\n");
		exit(-3);
	}
}

/* Turn the lcd off */
void lcdoff(void)
{
	FILE * fp;
	fp = popen("/usr/bin/sudo /usr/bin/systemctl stop lcd.service", "r");

	if (fp == NULL)	//Error checking
	{
		fprintf(stderr, "Falled to stop lcd service!\n");
		exit(-3);
	}

	if (0 == WEXITSTATUS(pclose(fp)))	//Error checking
	{
		printf("Stopped service\n");
	}
	else
	{
		fprintf(stderr, "Falled to stop lcd service!\n");
		exit(-3);
	}
}

/* Script power cycles the lcd to off */
void lcdreset(void)
{
	lcdon();
	sleep(2);
	lcdoff();	//Time between lcdon() and lcdoff() is +1 second due to the daemon-side signal catcher
}

/* Script toggles the operation of the fan */
void lcdtoggle(void)
{
	if (lcdservice())	// Checks if fan program is running
	{
		lcdoff();
	}
	else
	{
		lcdon();
	}
}
