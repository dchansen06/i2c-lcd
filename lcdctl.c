/* Exit codes:
 * 0, No problems detected during run
 * -1, Permission denied
 * -2, The program was not set up correctly
 * -3, Turning the lcd on or off failed
 * -4, Service status could not be determined
 * -5, Could not determine what options were wanted
 * -6, A high-level error occured, but we don't know where...
 *
 * This program is designed to allow CLI access to the lcd control program
 * after ensuring appropriate permission levels are set.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "services.h"

/* Displays help text */
void printhelp(char *name, char *arg)
{
	/* Change these to change the text */
	static const char * h = "--help, -h\t\t\tShow this help text and exits\n";
	static const char * v = "--version, -v\t\tShows the version information and exits\n";
	static const char * status = "status \t\t\tView the status of the lcd systemd program\n";
	static const char * on = "on \t\t\t\tTurns the lcd display on, or if it is on, power cycles it\t\tRequires authentification\n";
	static const char * off = "off \t\t\tTurns the lcd display off, or if it is off, power cycles it\t\tRequires authentification\n";
	static const char * reset = "reset \t\t\tPower cycles the lcd display to regain control\t\t\t\tRequires authentification\n";
	static const char * refresh = "refresh \t\t\tPower cycles the lcd display to refresh the program\t\t\tRequires authentification\n";
	/* Change these to change the text */

	printf("To view this help text enter %s --help\n", name);
	printf("Usage: %s [OPTION] [INFO]\n\n", name);
	printf("Allow simple access to the Python3.7 LCD control program\n\n");

	if (strcmp(arg, "all") == 0)
	{
		printf("Options:\n");
		printf("%s %s", name, h);
		printf("%s %s", name, v);
		printf("%s %s", name, status);
		printf("%s %s", name, on);
		printf("%s %s", name, off);
		printf("%s %s", name, reset);
		printf("%s %s", name, refresh);
	}
	else if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0 || strcmp(arg, "help") == 0)
	{
		printf("No man pages are avaliable at this time\n");
		printf("%s %s\n", name, h);
		printf("Usage:\n%s [OPTION] --help\t\tShows help text for specific option\n%s help (Optional dashes)\t\tShows all help text\n", name, name);
	}
	else if (strcmp(arg, "--version") == 0 || strcmp(arg, "-v") == 0)
	{
		printf("Current version: %s\t\tFor more information see %s --version\n", version, name);
		printf("%s %s\n", name, v);
		printf("Usage:\n%s --version\n", name);
	}
	else if (strcmp(arg, "status") == 0)
	{
		printf("%s %s", name, status);
	}
	else if (strcmp(arg, "on") == 0)
	{
		printf("%s %s", name, on);
	}
	else if (strcmp(arg, "off") == 0)
	{
		printf("%s %s", name, off);
	}
	else if (strcmp(arg, "reset") == 0)
	{
		printf("%s %s", name, reset);
	}
	else if (strcmp(arg, "refresh") == 0)
	{
		printf("%s %s", name, refresh);
	}
	else
	{
		printf("Invalid command\n");
                printf("Options:\n");
                printf("%s %s", name, h);
                printf("%s %s", name, v);
                printf("%s %s", name, status);
                printf("%s %s", name, on);
                printf("%s %s", name, off);
                printf("%s %s", name, reset);
                printf("%s %s", name, refresh);
	}
}

/* Handle command line argument processing and flow control */
int argcontrol(char *name, char *arg, char *help)
{
	if (strcmp(help, "--help") == 0 || strcmp(help, "-h") == 0)	//Help pull-out
	{
		printhelp(name, arg);
		return 1;
	}
	else if (strcmp(help, "none") == 1)
	{
		printf("Error: Unrecognized option\n");
		exit(-5);
	}

	if (lcdservice())	//Prints service status as default for no arguments
	{
		printf("LCD Active\n");
	}
	else
	{
		printf("LCD Inactive\n");
	}

	if (strcmp(arg, "help") == 0 || strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0)	//All other options including all help
	{
		printhelp(name, "all");
	}
	else if (strcmp(arg, "--version") == 0 || strcmp(arg, "-v") == 0)
	{
		printf("%s\nVersion: %s\nThis is not for official use. You have been warned\n", name, version);
	}
	else if (strcmp(arg, "on") == 0)
	{
		lcdon();
	}
	else if (strcmp(arg, "off") == 0)
	{
		if (lcdservice())	//Decides if it needs to powercycle or just turn the lcd off
		{
			lcdoff();
		}
		else
		{
			lcdreset();
		}
	}
	else if (strcmp(arg, "reset") == 0)
	{
		lcdreset();
	}
	else if (strcmp(arg, "refresh") == 0)
	{
		lcdon();
	}
	else if (strcmp(arg, "toggle") == 0)
	{
		lcdtoggle();
	}
	else if (strcmp(arg, "status") == 1)	//If not anything above, including 'status', then exit
	{
		printf("Error: Unrecognized option\n");	//Error handling
		exit(-5);
	}
	return 1;
}

/* Sets up signal handler, sends arguments to argcontrol, initiates the permision check */
int main(int argc, char **argv)
{
	if(argc == 2 && !strcmp(argv[1], "testing"))
	{
		printf("passed\n");
		return 0;
	}

	signal(SIGINT, SIG_IGN);	//Prevent ctrl+c from mangling start and stops

	if (argc == 1)	//No arguments, just show status
	{
		if (argcontrol(argv[0], "status", "none"))
		{
			return 0;
		}
		else	//Error handling
		{
			printf("Somewhere an error occured\n");
			exit(-6);
		}
	}
	else if (argc == 2)	//One argument
	{
		if (argcontrol(argv[0], argv[1], "none"))
		{
			return 0;
		}
		else	//Error handling
		{
			printf("Somewhere an error occured\n");
			exit(-6);
		}
	}
	else if (argc == 3)	//Two arguments, generally a --help
	{
		if (argcontrol(argv[0], argv[1], argv[2]))
		{
			return 0;
		}
		else	//Error handling
		{
			printf("Somewhere an error occured\n");
		}
	}
	else	//Error hangling
	{
		printf("Error: Too many options given\n");
		exit(-5);
	}
}
