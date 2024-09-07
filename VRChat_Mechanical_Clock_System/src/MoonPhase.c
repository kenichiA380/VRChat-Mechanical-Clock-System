// Calculate the phase and position of the moon for a given date.
// The algorithm is simple and adequate for many purposes.
//
// This software was originally adapted to javascript by Stephen R. Schmitt
// from a BASIC program from the 'Astronomical Computing' column of Sky & Telescope,
// April 1994, page 84, written by Bradley E. Schaefer.
//
// Subsequently adapted from Stephen R. Schmitt's javascript to c++ for the Arduino
// by Cyrus Rahman, this work is subject to Stephen Schmitt's copyright:
//
// Copyright 2004 Stephen R. Schmitt
// You may use or modify this source code in any way you find useful, provided
// that you agree that the author(s) have no warranty, obligations or liability.  You
// must determine the suitability of this source code for your use.

//and edited by kenichiA380 to use in this project. licence are same as above.

#include <math.h>
#include "MoonPhase.h"

typedef struct MoonPhase {
	double jDate;
	double phase;
};

// Determine the Moon Phase and orbital positions for the specified time.
double calculateMoonPhase(time_t t) {
	double phase;
	long jDay = julianDay(t);
	double jDateFrac = julianDateFrac(t);

	// Calculate illumination (synodic) phase.
	// From number of days since new moon on Julian date MOON_SYNODIC_OFFSET
	// (1815UTC January 6, 2000), determine remainder of incomplete cycle.
	//
	// The calculation is convoluted because if we only have four-byte
	// floats on an older Arduino, we must avoid exceeding the available precision.
	// We can do this by subtracting the large integers first, then
	// dealing with the smaller partial days.
	//
	// With an 8-byte double this could simplified:
	//  Use (double) julianDate() to return the fractional Julian date
	//  and calculate the phase directly:
	//   jDate = julianDate(t);
	//   phase = (jDate - MOON_SYNODIC_OFFSET) / MOON_SYNODIC_PERIOD;
	//
	phase = ((jDay - 2451550L) - 0.26 + jDateFrac) / MOON_SYNODIC_PERIOD;
	phase -= floor(phase);
	return phase;
}

// Determine Julian day from Unix time.

static long julianDay(time_t t) {
	return ((long)((t / 86400.0L) + 0.5) + 2440587);
}


// Return the fractional Julian Date, e.g. fraction of day after 1200UTC.

static double julianDateFrac(time_t t) {
	double f = t / 86400.0L + 0.5;
	f -= floor(f);
	return (f);
}
