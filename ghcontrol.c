/** @brif  functions declarations, fucntions body, arguments
 * @file ghcontrol.c
 */

#include "ghcontrol.h"
#include <cstring>
#include <string.h>

const char alarmnames[NALARMS][ALARMNMSZ] =
        {"No Alarms","Low Temperature","High Temperature","Low Humidity",
 "High Humidity","Low Pressure","High Pressure"};

#if SENSEHAT 
SenseHat Sh;
#endif


/** @brief Prints Gh Controller Title
 *  @version 07JUN2022
 *  @author Derhem Faisal
 *  @param aname string with Opreator's name
 *  @return void
 */
void GhDisplayHeader(const char * sname)
{
	fprintf(stdout, "%s's CENG153 Greenhouse Controller\n\n\n", sname);
}


/** @brief Prints Gh Serial Number
 *  @version 07JUN2022
 *  @author Derhem Faisal
 *  @param void
 *  @return void
 */

u_int64_t GhGetSerial (void)
{
	static u_int64_t serial = 0;
	FILE * fp;
	char buf[SYSINFOBUFSZ];
	char searchstring[] = SEARCHSTR;
	fp = fopen ("/proc/cpuinfo", "r");
	if (fp != NULL)
	{
		while (fgets(buf, sizeof(buf), fp) != NULL)
		{
		    if (!strncasecmp(searchstring, buf, strlen(searchstring)))
			{
				sscanf(buf+strlen(searchstring), "%Lx", &serial);
			}
		}
		fclose(fp);
	}
	if(serial ==0)
	{
		system("uname -a");
		system("ls --fu /usr/lib/codeblocks | grep -Po '\\.\\K[^ ]+'>stamp.txt");
		fp = fopen ("stamp.txt", "r");
		if (fp != NULL)
		{
			while (fgets(buf, sizeof(buf), fp) != NULL)
			{
				sscanf(buf, "%Lx", &serial);
			}
			fclose(fp);
		}
	}
	return serial;
}

/** @brief Prints Gh Controller Title
 *  @version 07JUN2022
 *  @author Derhem Faisal
 *  @param range int with Opreter's range
 *  @return random value in that range
 */

int GhGetRandom(int range)
{
	return rand() % range;

}

/** @brief delay time for a pecified number of milliseconds
 *  @version 07JUN2022
 *  @author Derhem Faisal
 *  @param milliseconds int number of milliseconds
 *  @return void
 */

void GhDelay(int milliseconds)
{
	long wait;
	clock_t now, start;

	wait = milliseconds*(CLOCKS_PER_SEC/1000);
	start = clock();
	now = start;
	while( (now-start) < wait )
	{
		now = clock();
	}
}

/** @brief calls other Gh Controller functions
 *  @version 07JUN2022
 *  @author Derhem Faisal
 *  @param void
 *  @return void
 */

void GhControllerInit(void){
	srand((unsigned)time(NULL));
	GhDisplayHeader("Derhem Faisal");
}
/** @brief Prints Gh Controller data
 *  @version 10JUN2022
 *  @author Derhem Faisal
 *  @param ctrl
 *  @return void
 */

void GhDisplayControls(controls ctrl){
	if (ctrl.heater == 0  )	{
		fprintf(stdout, "Heater is OFF \t");
	}else {
		fprintf (stdout, "Heater is ON\t");
	} if (ctrl.humidifier == 0){
		fprintf(stdout, "Humidifer is OFF\n");
	}else {
		fprintf(stdout, "Humidifer is ON\n");
	}

}
/** @brief Prints current date-time and passes Controller data from other struct
 *  @version 10JUN2022
 *  @author Derhem Faisal
 *  @param rdata
 *  @return void
 */

void GhDisplayReadings(readings rdata){

	printf("\n%sReadings\tT: %4.1fC\tH: %4.1f%%\tP: %6.1fmB\n",
ctime(&rdata.rtime),rdata.temperature,rdata.humidity,
rdata.pressure);


}

/** @brief calculate Gh Humidity
 *  @version 10JUN2022
 *  @author Derhem Faisal
 *  @param void
 *  @return random value of Humidity or 55
 */

float GhGetHumidity(void){
#if SIMHUMIDITY
	return GhGetRandomRange(LSHUMID, USHUMID);
#else
    return Sh.GetHumidity();
#endif
}
/** @brief calculate Gh Pressure
 *  @version 07JUN2022
 *  @author Derhem Faisal
 *  @param void
 *  @return random value of Pressure
 */

float GhGetPressure(void){
#if SIMPRESSURE
	return GhGetRandomRange(LSPRESS, USPRESS);
#else 
	return Sh.GetPressure();
#endif
}
/** @brief calculate Gh Temperature
 *  @version 10JUN2022
 *  @author Derhem Faisal
 *  @param void
 *  @return random value of Temperature or 25
 */

float GhGetTemperature(void){
#if SIMTEMPERATURE
	return GhGetRandomRange(LSTEMP, USTEMP);
#else
    return Sh.GetTemperature();
#endif 
}
/** @brief passes data from readings
 *  @version 10JUN2022
 *  @author Derhem Faisal
 *  @param void
 *  @return now
 */

readings GhGetReadings(void){
	readings now = {0};

	now.rtime = time(NULL);
	now.temperature = GhGetTemperature();
	now.humidity = GhGetHumidity();
	now.pressure = GhGetPressure();
	return now;
}
/** @brief validate Gh Controls data
 *  @version 10JUN2022
 *  @author Derhem Faisal
 *  @param struct tsetpontsa and readings
 *  @return cset
 */

controls GhSetControls(setpoints target, readings rdata){

	controls cset = {0};

	if (rdata.temperature < target.temperature ){
		 cset.heater = ON;
	} else {
		 cset.heater = OFF;
	}if (rdata.humidity <  target.humidity ){
		 cset.humidifier = ON;
	} else {
		 cset.humidifier = OFF;
	}

	return cset;
}



/** @brief generate random numbers
 *  @version 07JUN2022
 *  @author Derhem Faisal
 *  @param rlower, rupper int numbers
 *  @return random integer value
 */

int GhGetRandomRange(int rlower, int rupper){
	return rand() % (rupper + 1- rlower) + rlower;
}

int GhLogData(const char *fname, readings ghdata){

	FILE *fp;
	char ltime[CTIMESTRSZ];
	ltime[3]= ',';
	ltime[7]= ',';
	ltime[10]= ',';
	ltime[19]= ',';

	fp = fopen(fname, "a");
	if (fp == NULL)
	{
		fprintf(stdout,"\nCan't open file, data not retrieved!\n");
		return 0;
	}
	
	strcpy(ltime, ctime(&ghdata.rtime) );

	fprintf(fp, "\n%.24s,%5.1f,%5.1f,%6.1f", ltime, ghdata.temperature , ghdata.humidity, ghdata.pressure);
	fclose(fp);
	return 1;
}

int GhSaveSetpoints(const char *fname, setpoints rspts){
	FILE *fp;
	fp = fopen(fname, "w");
	if (fp == NULL){
		fprintf(stdout, "\nCan't open file, data not saved");
		return 0;
	}
	fwrite(&rspts, sizeof(rspts), 1,fp);
	fclose(fp);
	return 1;

}

setpoints GhRetrieveSetpoints(const char *fname){
	setpoints spts = {0};
	FILE *fp;
	fp = fopen(fname, "r");
	if (fp == NULL){
		return spts;
	}
	fread(&spts, sizeof(spts), 1, fp);
	fclose(fp);
	return spts;
}

/** @brief pass data from setponts
 *  @version 10JUN2022
 *  @author Derhem Faisal
 *  @param void
 *  @return cpoints
 */
setpoints GhSetTargets(void){
    setpoints cpoints = {0};
    cpoints = GhRetrieveSetpoints("setpoints.dat");
    if (cpoints.temperature == 0){
	 cpoints.temperature = STEMP;
   	 cpoints.humidity = SHUMID;
   	 GhSaveSetpoints("setpoints.dat", cpoints);
    }
    return cpoints;
}
/** @brief Prints Gh Targets values
 *  @version 07JUN2022
 *  @author Derhem Faisal
 *  @param void
 *  @return void
 */

void GhDisplayTargets(setpoints points){
	printf( "Temperature target = %5.0fC  Humidity target = %5.1f%\n", points.temperature, points.humidity);
}


/** @brief Sets a vertical bar on the Sensehat display
 *  @author Paul Moggach
 *  @author Kristian Medri
 *  @author Brian Markle
 *  @version 2020-05-01
 *  @param int bar to light
 *  @param fbpixel_s pixel colour data
 *  @param uint8_t length how many pixels to light in bar
 *  @return exit status
 */
int GhSetVerticalBar(int bar,COLOR_SENSEHAT pxc, uint8_t length)
{
    int i;

    if (length>7){ length=7; }
	if (bar >= 0 && bar < 8 && length >= 0 && length < 8)
	{
        for(i=0; i<= length; i++)
        {
            Sh.LightPixel(i,bar,pxc);
        }
        for(i=length+1; i< 8;i++)
        {
            Sh.LightPixel(i,bar,BLACK);
        }
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

void GhDisplayAll(readings rd, setpoints sd){
	int rv, sv, avh, avl;
	Sh.WipeScreen();

	rv =(NUMPTS * (((rd.temperature-LSTEMP)/(USTEMP-LSTEMP))+0.05))-1.0;
	sv =(NUMPTS * (((sd.temperature-LSTEMP)/(USTEMP-LSTEMP))+0.05))-1.0;
	GhSetVerticalBar(TBAR, GREEN,rv);
 	if(rd.temperature < sd.temperature) Sh.LightPixel(sv,TBAR,RED);
   		else Sh.LightPixel(sv,TBAR,MAGENTA);


	rv =(NUMPTS * (((rd.humidity-LSHUMID)/(USHUMID-LSHUMID))+0.05))-1.0;
	sv =(NUMPTS * (((sd.humidity-LSHUMID)/(USHUMID-LSHUMID))+0.05))-1.0;
	GhSetVerticalBar(HBAR, GREEN,rv);
	if(rd.humidity < sd.humidity) Sh.LightPixel(sv,HBAR,RED);
 		else Sh.LightPixel(sv,HBAR,MAGENTA);
	

	rv =(NUMPTS * (((rd.pressure-LSPRESS)/(USPRESS-LSPRESS))+0.05))-1.0;
	if(rv >= avh || rv <= avl)
		GhSetVerticalBar(PBAR,GREEN,rv);
	
}


alarmlimits GhSetAlarmLimits(void){
	alarmlimits calarm = {0};
	
	calarm.lowt = LOWATEMP;
	calarm.hight = HIGHATEMP;
	calarm.lowh = LOWAHUMID;
	calarm.highh = HIGHAHUMID;
	calarm.lowp = LOWAPRESS;
	calarm.highp = HIGHAPRESS;

	return calarm;
}
void GhDisplayAlarms(alarms *head){


	alarms *cur = head;
	
	while(cur != NULL){
		if(cur->code != NOALARM)
		{
		    fprintf(stdout,"%s alarm %s",alarmnames[cur->code],ctime( &(cur->atime) ));
		}
		cur =cur->next;
	}

}
int GhSetOneAlarm (alarm_e code, time_t atime, float value, alarms *head)
{
	alarms *cur;
   	alarms *last;
	
	cur = head;
	//Is it the first item on an empty list?
	if (cur-> code != NOALARM)
	 {
            while (cur != NULL) // loop until the last record of the linked list
	     {
            	if (cur->code == code) // alarm already set?
            	{
                return 0; 
            	}
           	last = cur;
            	cur = cur->next; // current is not the next item in list
        }
        cur = (alarms *) calloc(1,sizeof(alarms));
        if (cur == NULL) { return 0;} // return if fail
        last->next = cur; // update the previous record
    }
    cur->code = code;
    cur->atime = atime;
    cur->value = value;
    cur->next = NULL;
    return 1;
}

alarms *GhClearOneAlarm(alarm_e code, alarms *head)
{

	alarms *cur;
   	alarms *last;

	cur = head;
	last = head;
	
	// case 1: only one item in the list
	if(cur->code ==code && cur->next == NULL)
	{
		cur->code = NOALARM;
		return head;
	}

	// case 2: first item in a multi-item list
        if (cur->code == code && cur->next != NULL) {
        	head = cur->next;
       		free(cur);
        	return head;
    	}

	// case 3: other items in a multi-item list
  	  while (cur != NULL) {
	if (cur->code == code) {
		last->next = cur->next;
            	free(cur);
            	return head;
        }
        last = cur;
        cur = cur->next;
    }

	//no match found
	return head;
}


alarms *GhSetAlarms(alarms *head, alarmlimits alarmpt, readings rdata) {
    if (rdata.temperature >= alarmpt.hight) {
        GhSetOneAlarm(HTEMP, rdata.rtime, rdata.temperature, head);
    } else {
        head = GhClearOneAlarm(HTEMP, head);
    }

    if (rdata.temperature <= alarmpt.lowt) {
        GhSetOneAlarm(LTEMP, rdata.rtime, rdata.temperature, head);
    } else {
        head = GhClearOneAlarm(LTEMP, head);
    }

    if (rdata.humidity >= alarmpt.highh) {
        GhSetOneAlarm(HHUMID, rdata.rtime, rdata.humidity, head);
    } else {
        head = GhClearOneAlarm(HHUMID, head);
    }

    if (rdata.humidity <= alarmpt.lowh) {
        GhSetOneAlarm(LHUMID, rdata.rtime, rdata.humidity, head);
    } else {
        head = GhClearOneAlarm(LHUMID, head);
    }

    if (rdata.pressure >= alarmpt.highp) {
        GhSetOneAlarm(HPRESS, rdata.rtime, rdata.pressure, head);
    } else {
        head = GhClearOneAlarm(HPRESS, head);
    }

    if (rdata.pressure <= alarmpt.lowp) {
        GhSetOneAlarm(LPRESS, rdata.rtime, rdata.pressure, head);
    } else {
        head = GhClearOneAlarm(LPRESS, head);
    }

    return head;
}










