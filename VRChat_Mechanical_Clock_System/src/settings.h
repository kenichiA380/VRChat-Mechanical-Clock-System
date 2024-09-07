//setting file loader/writer

#pragma once
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include "toml.h"
#include <ctype.h>
#include <io.h>
#define F_OK 0
#define access _access


typedef struct settings_t {
	char IP[128];
	unsigned short Port;
	unsigned short Beatrate;

	char yearParam[128];
	bool  yearParamEnabled;
	char monthParam[128];
	bool  monthParamEnabled;
	char dayParam[128];
	bool  dayParamEnabled;
	char dowParam[128];
	bool  dowParamEnabled;
	char moonParam[128];
	bool  moonParamEnabled;
	char hourParam[128];
	bool  hourParamEnabled;
	char minParam[128];
	bool  minParamEnabled;
	char secParam[128];
	bool  secParamEnabled;
	char msecParam[128];
	bool  msecParamEnabled;
    bool gmtParamEnabled;

};

static void error(const char* msg, const char* msg1)
{
	fprintf(stderr, "ERROR: %s%s\n", msg, msg1 ? msg1 : "");
	exit(1);
}

void writeDefaultSettings(char* filename)
{
    printf("Checking if setting file is available...\n");
    if (access(filename, F_OK) == 0) {
        printf("setting file found. continuing...\n");
        return 0;
    }
    else {
        FILE* fp;
        fp = fopen(filename, "w");
        if (fprintf(fp, "\
[settings]\n\
IP = \"127.0.0.1\"\n\
Port = 9000\n\
Beatrate = 8\n\
yearParam = \"AC_yr\"\n\
yearParamEnabled = false\n\
monthParam = \"AC_mt\"\n\
monthParamEnabled = false\n\
dayParam = \"AC_dy\"\n\
dayParamEnabled = false\n\
dowParam = \"AC_dw\"\n\
dowParamEnabled = false\n\
moonParam = \"AC_mp\"\n\
moonParamEnabled = false\n\
hourParam = \"AC_hh\"\n\
hourParamEnabled = true\n\
minParam = \"AC_mh\"\n\
minParamEnabled = true\n\
secParam = \"AC_sc\"\n\
secParamEnabled = true\n\
msecParam = \"AC_ms\"\n\
msecParamEnabled = false\n\
gmtParamEnabled = false\0") < 0)
        {
            fclose(fp);
            error("Writing failed!\n", "");
        }
        printf("success!\n");
        fclose(fp);
    }
}


struct settings_t loadsettings(char* filename)
{
    FILE* fp;
    char errbuf[200];
    struct settings_t loadedSettings;

    // 1. Read and parse toml file
    fp = fopen(filename, "r");
    if (!fp) {
        error("cannot open file", strerror(errno));
    }

    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!conf) {
        error("cannot parse - ", errbuf);
    }

    // 2. Traverse to a table.
    toml_table_t* settings = toml_table_in(conf, "settings");
    if (!settings) {
        error("missing [settings]", "");
    }

    // 3. Extract values
    //IP----------------------------------------------------------------------
    toml_datum_t IP = toml_string_in(settings, "IP");
    if (!IP.ok) {
        error("cannot read IP", "");
    }
    printf("IP read success.\n");
    strcpy(loadedSettings.IP, IP.u.s);

    //Port----------------------------------------------------------------------
    toml_datum_t Port = toml_int_in(settings, "Port");
    if (!Port.ok) {
        error("cannot read Port", "");
    }
    printf("Port read success.\n");
    loadedSettings.Port = Port.u.i;
    

    //Beatrate----------------------------------------------------------------------
    toml_datum_t Beatrate = toml_int_in(settings, "Beatrate");
    if (!Beatrate.ok) {
        error("cannot read Beatrate", "");
    }
    printf("Beatrate read success.\n");
    loadedSettings.Beatrate = Beatrate.u.i;

    //yearParam----------------------------------------------------------------------
    toml_datum_t yearParam = toml_string_in(settings, "yearParam");
    if (!yearParam.ok) {
        error("cannot read yearParam", "");
    }
    toml_datum_t yearParamEnabled = toml_bool_in(settings, "yearParamEnabled");
    if (!yearParamEnabled.ok)
    {
        error("cannot read yearParamEnabled", "");
    }
    strcpy(loadedSettings.yearParam, yearParam.u.s);
    loadedSettings.yearParamEnabled = yearParamEnabled.u.b;


    //monthParam----------------------------------------------------------------------
    toml_datum_t monthParam = toml_string_in(settings, "monthParam");
    if (!monthParam.ok) {
        error("cannot read monthParam", "");
    }
    toml_datum_t monthParamEnabled = toml_bool_in(settings, "monthParamEnabled");
    if (!monthParamEnabled.ok)
    {
        error("cannot read monthParamEnabled", "");
    }
    strcpy(loadedSettings.monthParam, monthParam.u.s);
    loadedSettings.monthParamEnabled = monthParamEnabled.u.b;


    //dayParam-------------------------------------------------------------------------
    toml_datum_t dayParam = toml_string_in(settings, "dayParam");
    if (!dayParam.ok) {
        error("cannot read dayParam", "");
    }
    toml_datum_t dayParamEnabled = toml_bool_in(settings, "dayParamEnabled");
    if (!dayParamEnabled.ok)
    {
        error("cannot read dayParamEnabled", "");
    }
    strcpy(loadedSettings.dayParam, dayParam.u.s);
    loadedSettings.dayParamEnabled = dayParamEnabled.u.b;


    //dowParam-------------------------------------------------------------------------
    toml_datum_t dowParam = toml_string_in(settings, "dowParam");
    if (!dowParam.ok) {
        error("cannot read dowParam", "");
    }
    toml_datum_t dowParamEnabled = toml_bool_in(settings, "dowParamEnabled");
    if (!dowParamEnabled.ok)
    {
        error("cannot read dowParamEnabled", "");
    }
    strcpy(loadedSettings.dowParam, dowParam.u.s);
    loadedSettings.dowParamEnabled = dowParamEnabled.u.b;


    //moonParam------------------------------------------------------------------------
    toml_datum_t moonParam = toml_string_in(settings, "moonParam");
    if (!moonParam.ok) {
        error("cannot read moonParam", "");
    }
    toml_datum_t moonParamEnabled = toml_bool_in(settings, "moonParamEnabled");
    if (!moonParamEnabled.ok)
    {
        error("cannot read moonParamEnabled", "");
    }
    strcpy(loadedSettings.moonParam, moonParam.u.s);
    loadedSettings.moonParamEnabled = moonParamEnabled.u.b;


    //hourParam------------------------------------------------------------------------
    toml_datum_t hourParam = toml_string_in(settings, "hourParam");
    if (!hourParam.ok) {
        error("cannot read hourParam", "");
    }
    toml_datum_t hourParamEnabled = toml_bool_in(settings, "hourParamEnabled");
    if (!hourParamEnabled.ok)
    {
        error("cannot read hourParamEnabled", "");
    }
    strcpy(loadedSettings.hourParam, hourParam.u.s);
    loadedSettings.hourParamEnabled = hourParamEnabled.u.b;


    //minParam-------------------------------------------------------------------------
    toml_datum_t minParam = toml_string_in(settings, "minParam");
    if (!minParam.ok) {
        error("cannot read minParam", "");
    }
    toml_datum_t minParamEnabled = toml_bool_in(settings, "minParamEnabled");
    if (!minParamEnabled.ok)
    {
        error("cannot read minParamEnabled", "");
    }
    strcpy(loadedSettings.minParam, minParam.u.s);
    loadedSettings.minParamEnabled = minParamEnabled.u.b;


    //secParam-------------------------------------------------------------------------
    toml_datum_t secParam = toml_string_in(settings, "secParam");
    if (!secParam.ok) {
        error("cannot read secParam", "");
    }
    toml_datum_t secParamEnabled = toml_bool_in(settings, "secParamEnabled");
    if (!secParamEnabled.ok)
    {
        error("cannot read secParamEnabled", "");
    }
    strcpy(loadedSettings.secParam, secParam.u.s);
    loadedSettings.secParamEnabled = secParamEnabled.u.b;


    //msecParam------------------------------------------------------------------------
    toml_datum_t msecParam = toml_string_in(settings, "msecParam");
    if (!msecParam.ok) {
        error("cannot read msecParam", "");
    }
    toml_datum_t msecParamEnabled = toml_bool_in(settings, "msecParamEnabled");
    if (!msecParamEnabled.ok)
    {
        error("cannot read msecParamEnabled", "");
    }
    strcpy(loadedSettings.msecParam, msecParam.u.s);
    loadedSettings.msecParamEnabled = msecParamEnabled.u.b;

    //gmtParam
    toml_datum_t gmtParamEnabled = toml_bool_in(settings, "gmtParamEnabled");
    if (!gmtParamEnabled.ok)
    {
        error("cannot read gmtParamEnabled", "");
    }
    loadedSettings.gmtParamEnabled = gmtParamEnabled.u.b;


    // 4. Free memory
    free(IP.u.s);
    //free(Port.u.i);
    //free(Beatrate.u.i);
    free(yearParam.u.s);
    free(monthParam.u.s);
    free(dayParam.u.s);
    free(dowParam.u.s);
    free(moonParam.u.s);
    free(hourParam.u.s);
    free(minParam.u.s);
    free(secParam.u.s);
    free(msecParam.u.s);
    toml_free(conf);
    return loadedSettings;
}

int checkASCII(char* string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (!isascii(string[i]))
            return 1;
    }
    return 0;
}

int savesettings(struct settings_t settings, char* filename)
{
    printf("saving config... ");
    FILE* fp;
    fp = fopen(filename, "w");
    if (fprintf(fp, "\
[settings]\n\
IP = \"%s\"\n\
Port = %d\n\
Beatrate = %d\n\
yearParam = \"%s\"\n\
yearParamEnabled = %s\n\
monthParam = \"%s\"\n\
monthParamEnabled = %s\n\
dayParam = \"%s\"\n\
dayParamEnabled = %s\n\
dowParam = \"%s\"\n\
dowParamEnabled = %s\n\
moonParam = \"%s\"\n\
moonParamEnabled = %s\n\
hourParam = \"%s\"\n\
hourParamEnabled = %s\n\
minParam = \"%s\"\n\
minParamEnabled = %s\n\
secParam = \"%s\"\n\
secParamEnabled = %s\n\
msecParam = \"%s\"\n\
msecParamEnabled = %s\n\
gmtParamEnabled = %s", 
settings.IP, 
settings.Port, 
settings.Beatrate, 
settings.yearParam,
settings.yearParamEnabled ? "true" : "false",
settings.monthParam,
settings.monthParamEnabled ? "true" : "false",
settings.dayParam,
settings.dayParamEnabled ? "true" : "false", 
settings.dowParam,
settings.dowParamEnabled ? "true" : "false", 
settings.moonParam,
settings.moonParamEnabled ? "true" : "false", 
settings.hourParam,
settings.hourParamEnabled ? "true" : "false", 
settings.minParam,
settings.minParamEnabled ? "true" : "false", 
settings.secParam,
settings.secParamEnabled ? "true" : "false", 
settings.msecParam,
settings.msecParamEnabled ? "true" : "false",
settings.gmtParamEnabled ? "true" : "false\0") < 0)
    {
        fclose(fp);
        error("writing failed!\n", "");
    }
    printf("success!\n");
    fclose(fp);
}