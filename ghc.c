/** @brif  main function
 * @file Lab08.c
 */


#include <stdio.h>
#include <stdlib.h>
#include "ghcontrol.h"


/** @brief main function
 *  @version 07JUN2022
 *  @author Derhem Faisal
 *  @param void
 *  @return (call functions)
 */


int main(void){

	time_t now;
	int logged;
	char fname[]="ghdata.txt";

	
	struct controls ctrl = {0};
	struct setpoints sets ={0};
	struct readings creadings = {0};
	alarmlimits alimits = {0};
	alarms *arecord;
	arecord = (alarms *) calloc(1,sizeof(alarms));
		if (arecord == NULL)
		{
			printf("\nCannot allocate memory\n");
			return EXIT_FAILURE;
		};


	
	sets = GhSetTargets();
	alimits = GhSetAlarmLimits();
	srand((unsigned)time(NULL));
	GhDisplayHeader("Derhem Faisal");	// Prints header to the screen
       	void GhControllerInt(void);
	
	while(1) {
		now =time(NULL);

		//modules functions
		creadings = GhGetReadings();
		logged = GhLogData("ghdata.txt", creadings);
		ctrl = GhSetControls(sets, creadings);
		GhSetAlarms(arecord,alimits, creadings);
		GhDisplayAll(creadings,sets);
		GhDisplayReadings(creadings);
		GhDisplayTargets(sets);
		GhDisplayControls(ctrl);
		GhDisplayAlarms(arecord);

		GhDelay(GHUPDATE);
	}
	fprintf(stdout,"Press ENTER to continue...");
	fgetc(stdin);

	return EXIT_FAILURE;
}


