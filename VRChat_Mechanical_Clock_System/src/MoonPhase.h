#define MoonPhaseFXP_h

#include <time.h>

#define MOON_SYNODIC_PERIOD   29.530588853   // Period of moon cycle in days.
#define	MOON_SYNODIC_OFFSET   2451550.26     // Reference cycle offset in days.
#define MOON_DISTANCE_PERIOD  27.55454988    // Period of distance oscillation
#define MOON_DISTANCE_OFFSET  2451562.2
#define MOON_LATITUDE_PERIOD  27.212220817   // Latitude oscillation
#define	MOON_LATITUDE_OFFSET  2451565.2
#define MOON_LONGITUDE_PERIOD 27.321582241   // Longitude oscillation
#define	MOON_LONGITUDE_OFFSET 2451555.8

double jDate;
double phase;	      // 0 - 1, 0.5 = full

double calculateMoonPhase(time_t);
static long julianDay(time_t t);
static double julianDateFrac(time_t t);
