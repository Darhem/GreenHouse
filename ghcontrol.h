/** @brief Gh control constants, structures, function prototypes for GhControl code
 * @file ghcontrol.h
 */

#ifndef GHCONTROL_H
#define GHCONTROL_H
#define SEARCHSTR "serial\t\t:"
#define SYSINFOBUFSZ 512
#define GHUPDATE 2000
// Includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "time.h"
#include "sensehat.h"
// Constants
#define SENSORS 3
#define TEMPERATURE 0
#define HUMIDITY 1
#define PRESSURE 2
#define SIMULATE 1
#define LSTEMP -10
#define USTEMP 50
#define LSHUMID 0
#define USHUMID 100
#define LSPRESS 975
#define USPRESS 1016
#define STEMP 23.0
#define SHUMID 55.0
#define ON 1
#define OFF 0
#define SIMTEMPERATURE 0
#define SIMHUMIDITY 0
#define SIMPRESSURE 0
#define CTIMESTRSZ 25
#define SENSEHAT 1
#define NUMBARS 8
#define NUMPTS 8.0
#define TBAR 7
#define HBAR 5
#define PBAR 3
#define ALARMNMSZ 18
#define NALARMS 7
#define LOWATEMP 10
#define HIGHATEMP 26
#define LOWAHUMID 25
#define HIGHAHUMID 70
#define LOWAPRESS 985
#define HIGHAPRESS 1016
//Structures
struct readings GhGetReadings(void);

//Enumerated Types
typedef enum {NOALARM, LTEMP, HTEMP,LHUMID, HHUMID, LPRESS, HPRESS} alarm_e;

typedef struct readings
{
time_t rtime;
    float temperature;
    float humidity;
    float pressure;
}readings;

typedef struct setpoints
{
    float temperature;
    float humidity;
}setpoints;

typedef struct controls
{
    int heater;
    int humidifier;
}controls;

typedef struct alarmlimits
{
	float lowt;
	float hight;
	float lowh;
	float highh;
	float lowp;
	float highp;
} alarmlimits;

typedef struct alarms
{
	alarm_e code;
	time_t atime;
	double value;
	struct alarms *next;
}alarms;




setpoints GhSetTargets(void);
controls GhSetControls(setpoints target, readings rdata);
setpoints GhRetrieveSetpoints(const char *fname);


// Functions Prototypes
void GhDisplayHeader(const char * sname);
u_int64_t GhGetSerial(void);
int GhGetRandom(int range);
void GhDelay(int milliseconds);


//lab03 functions
void GhControllerInt(void);
void GhDisplayControls(controls ctrl);
void GhDisplayReadings(readings rdata);
void GhDisplayTargets(setpoints spoints);
float GhGetHumidity(void);
float GhGetPressure(void);
float GhGetTemperature(void);



//lab04 functions
int GhGetRandomRange(int lower, int upper);


//lab07 functions
int GhLogData(const char *fname, struct readings ghdata);
int GhSaveSaetpoints(const char *fname, setpoints spts);
void GhDisplayAll(readings rs, setpoints sp);


//Lab10
alarmlimits GhSetAlarmLimits(void);
alarms *GhSetAlarms(alarms *head, alarmlimits alarmpt, readings rdata);
void GhDisplayAlarms(alarms *head);

//Lab11
int GhSetOneAlarm(alarm_e code, time_t atime, double value, alarms *head);
alarms *GhClearOneAlarm(alarm_e code, alarms *head);

#endif
