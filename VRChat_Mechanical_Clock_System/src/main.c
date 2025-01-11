
#include <stdio.h>
#include "time.h"
#include <sys/timeb.h>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "osc.h"
#include "settings.h"
#include "getTimes.h"
#include "MoonPhase.h"

static void ApplyButton();
static void applySettings();
int errorcode;
bool err = 0;
bool saved = 0;
bool zeroMsecAlign = false;
struct settings_t settings;
struct settings_t settingsbuf;

const char* SAVEFILE = "settings.conf";

//redefining here because railib conflicts with windows.h
struct SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} timedata;

char IPBoxText[16] = "127.0.0.1"; //IP input box
char setIP[16] =     "127.0.0.1"; //IP string buffer
char portBoxText[6] =   "9000";   //Port input box
unsigned short setPort = 9000;    //Port number buffer

char DestIPText[32] = "Destination IP:";
char DestPortText[32] = "Destination Port:";
char BeatrateText[64] = "Refresh rate:1~16Hz";
char currentSettingsLabelText[128] = "Current settings: ---";
char beatrateBoxText[5] = "8";    //Hz
unsigned short beatrate = 8;      //Hz
unsigned int   bph =      28800;  //beatrate*3600 (beat per hour)
char BPHLabelText[128] = "28800 BPH";

//äÚïåÉ^ÉìÉÅÉìêHÇ◊ÇΩÇ¢

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int WinMain()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    const int screenWidth = 432;
    const int screenHeight = 504;

    SetConfigFlags(FLAG_WINDOW_UNDECORATED);//hides Windows default window away
    InitWindow(screenWidth, screenHeight, "VRChat Mechanical Clock OSC Client"); //better name? anyone?

    GuiLoadStyle("style_terminal.rgs");       //cool lookin' theme

    // Mechanical Clock OSC Client: controls initialization
    //----------------------------------------------------------------------------------

    writeDefaultSettings(SAVEFILE);    //create settings file if doesn't exist
    settings = loadsettings(SAVEFILE); //load settings file to struct settings
    applySettings();                          //applying settings

    const char* WindowBox000Text = "VRChat Mechanical Clock OSC Client V0.0.3";//seriously can anyone come up with a better name please?
    const char* Label007Text = "Hz";
    const char* Label010Text = "Parameter Settings";
    const char* Label011Text = "/avatar/parameters/";
    const char* YearCheckText = "Year";
    const char* MonthCheckText = "Month";
    const char* DayCheckText = "Day";
    const char* DowCheckText = "Day of week";
    const char* MoonCheckText = "Moonphase";
    const char* HourCheckText = "Hour";
    const char* MinCheckText = "Minute";
    const char* SecCheckText = "Second";
    const char* MillisCheckText = "Millisecond";
    const char* gmtCheckText = "GMT(24H)";
    const char* applyButtonText = "Apply Settings";
    const char* Line031Text = "\0";
    const char* Line009Text = "\0";

    char StartBtnText[128] = "Start";
    char StatusLabelText[128] = "Sending:";
    //bool WindowBox000Active = true;

    bool IPBoxEditMode = false;
    bool portBoxEditMode = false;
    bool beatrateBoxEditMode = false;
    unsigned short setBeat = 6;
    /*
    bool YearCheckChecked = false;
    bool MonthCheckChecked = false;
    bool DayCheckChecked = false;
    bool DowCheckChecked = false;
    bool MoonCheckChecked = false;
    bool HourCheckChecked = true;
    bool MinCheckChecked = true;
    bool SecCheckChecked = true;
    bool MillisCheckChecked = false;
    bool gmtCheckChecked = false;
    */
    bool yearParamEditMode = false;
    bool monthParamEditMode = false;
    bool dayParamEditMode = false;
    bool dowParamEditMode = false;
    bool moonParamEditMode = false;
    bool hourParamEditMode = false;
    bool minParamEditMode = false;
    bool secParamEditMode = false;
    bool msecParamEditMode = false;
    bool StartBtnActive = false;

    // General variables
    Vector2 mousePosition = { 0 };
    Vector2 windowPosition = { 500, 200 };
    Vector2 panOffset = mousePosition;
    bool dragWindow = false;

    SetWindowPosition(windowPosition.x, windowPosition.y);

    bool exitWindow = false;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    //Clock
    //timedata = getTimes();

    //Timer
    clock_t oldTime, newTime;
    oldTime = clock();
    newTime = 0;

    float year_hand = 0;
    float month_hand = 0;
    float day_hand = 0;
    float dow_hand = 0;
    float moon_hand = 0;
    float hours_hand = 0;
    float minutes_hand = 0;
    float seconds_hand = 0;
    float mseconds_hand = 0;

    

    //osc initialization
    initOSC(9001, setPort, setIP, errorcode);
    printf("%s:%d, initOSC returned %d\n", setIP, setPort, errorcode);

    sprintf_s(currentSettingsLabelText, sizeof(currentSettingsLabelText), 
        "Current Setting : %s:%d/%dHz", setIP, setPort, beatrate);


    // Main game loop
    while (!exitWindow && !WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mousePosition = GetMousePosition();

        //check if clicked cross button (close window)
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !dragWindow)
        {
            if (CheckCollisionPointRec(mousePosition, (Rectangle) { 0, 0, screenWidth, 20 }))
            {
                dragWindow = true;
                panOffset = mousePosition;
            }
        }

        if (dragWindow)
        {
            windowPosition.x += (mousePosition.x - panOffset.x);
            windowPosition.y += (mousePosition.y - panOffset.y);

            SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragWindow = false;
        }
        //----------------------------------------------------------------------------------

        //refresh and send osc packets when reached each beat

        if (beatrate <= 0 || beatrate > 16) return 1;

        //StartButton ON
        if (StartBtnActive && !err) 
        {       
            oldTime = clock();
            strcpy(StartBtnText, "Stop");

            time_t time_; //for moon phase
            time(&time_);
           
            while (!zeroMsecAlign)
            {
               timedata = getTimes();
               if (timedata.wMilliseconds == 0) zeroMsecAlign = true;
            }
           
            if (oldTime >= newTime + 1000 / beatrate && zeroMsecAlign) //check beat
            {

                timedata = getTimes();//get times

                //hand position float calculations
                // ---------------------------------------------------------------------------------------------------------------
                //animation range 0~any frame. 3.6deg/frame per year.
                year_hand = (timedata.wYear % 2000 * 0.01f); 
                //animation range 0~any frame, 30deg/frame per month.
                month_hand = (timedata.wMonth * 0.0833f);    
                //animation range 0~93-100 frame, 11.61deg/frame per day.
                day_hand = (timedata.wDay * 0.03f);          
                //animation range 0~70-100 frame, 51.42deg/frame per day.
                dow_hand = (timedata.wDayOfWeek * 0.1f);
                //animation range 0-100 frame, 360 deg for full circle.
                moon_hand = calculateMoonPhase(time_);
                //animation range 0~60-100 frame, 6deg/frame per sec.
                seconds_hand = (timedata.wSecond * 0.01f) + (timedata.wMilliseconds * 0.00001f);
                //animation range 0~60-100 frame, 6deg/frame per min.
                minutes_hand = timedata.wMinute * 0.01f + (timedata.wSecond * 0.000166f) + (timedata.wMilliseconds * 0.0000001f);
                //animation range 0~60-100 frame, 30deg/frame per hour.
                if (!settings.gmtParamEnabled)  hours_hand = (0.05f * (timedata.wHour % 12) + (0.00083f * timedata.wMinute));
                //!!GMT!! animation range 0~60-100 frame, 15deg/frame per hour.
                else hours_hand = (0.025f * (timedata.wHour) +(0.000415f * timedata.wMinute));
                //animation range 0-100 frame, 0.36deg/frame per millisec.
                mseconds_hand = (timedata.wMilliseconds * 0.001f);
                //----------------------------------------------------------------------------------------------------------------

                //show current time as status
                sprintf_s(StatusLabelText, sizeof(StatusLabelText), "Sending: %d/%02d/%02d %s %02d:%02d:%02d.%03d Moon:%.2f\
",timedata.wYear, timedata.wMonth, timedata.wDay, convertDow(timedata.wDayOfWeek), timedata.wHour, timedata.wMinute, timedata.wSecond, timedata.wMilliseconds, moon_hand);

                //pass parameter name to OSC
                sprintf_s(settingsbuf.yearParam, sizeof(settingsbuf.yearParam),   "/avatar/parameters/%s", settings.yearParam);
                sprintf_s(settingsbuf.monthParam, sizeof(settingsbuf.monthParam), "/avatar/parameters/%s", settings.monthParam);
                sprintf_s(settingsbuf.dayParam, sizeof(settingsbuf.dayParam),     "/avatar/parameters/%s", settings.dayParam);
                sprintf_s(settingsbuf.dowParam, sizeof(settingsbuf.dowParam),     "/avatar/parameters/%s", settings.dowParam);
                sprintf_s(settingsbuf.moonParam, sizeof(settingsbuf.moonParam),   "/avatar/parameters/%s", settings.moonParam);
                sprintf_s(settingsbuf.hourParam, sizeof(settingsbuf.hourParam),   "/avatar/parameters/%s", settings.hourParam);
                sprintf_s(settingsbuf.minParam, sizeof(settingsbuf.minParam),     "/avatar/parameters/%s", settings.minParam);
                sprintf_s(settingsbuf.secParam, sizeof(settingsbuf.secParam),     "/avatar/parameters/%s", settings.secParam);
                sprintf_s(settingsbuf.msecParam, sizeof(settingsbuf.msecParam),   "/avatar/parameters/%s", settings.msecParam);

                //check if each parameter is enabled and send OSC message
                if (settings.yearParamEnabled) printf("%d\n", sendOSCMessageFloat(settingsbuf.yearParam, year_hand));
                if (settings.monthParamEnabled) printf("%d\n", sendOSCMessageFloat(settingsbuf.monthParam, month_hand));
                if (settings.dayParamEnabled) printf("%d\n", sendOSCMessageFloat(settingsbuf.dayParam, day_hand));
                if (settings.dowParamEnabled) printf("%d\n", sendOSCMessageFloat(settingsbuf.dowParam, dow_hand));
                if (settings.moonParamEnabled) printf("%d\n", sendOSCMessageFloat(settingsbuf.moonParam, moon_hand));
                if (settings.hourParamEnabled) printf("%d\n", sendOSCMessageFloat(settingsbuf.hourParam, hours_hand));
                if (settings.minParamEnabled) printf("%d\n", sendOSCMessageFloat(settingsbuf.minParam, minutes_hand));
                if (settings.secParamEnabled) printf("%d\n", sendOSCMessageFloat(settingsbuf.secParam, seconds_hand));
                if (settings.msecParamEnabled)printf("%d\n", sendOSCMessageFloat(settingsbuf.msecParam, mseconds_hand));
                
                /* Doesnt seem to work with bundles???
                mobundle bundle = { 0 };
                if(HourCheckChecked) bundleOSCMessage("/avatar/parameters/AC_hh", ",f", hours_hand, bundle);
                if(MinCheckChecked) bundleOSCMessage("/avatar/parameters/AC_mh", ",f", minutes_hand, bundle);
                if(SecCheckChecked) bundleOSCMessage("/avatar/parameters/AC_sc", ",f", seconds_hand, bundle);
                
                sendOSCBundle(bundle);
                */
                newTime = oldTime;
            }
        }
        else //StartButton OFF
        {
            //GuiUnlock();
            strcpy(StartBtnText,"Start");
            strcpy(StatusLabelText, "Status: Stopped");
            zeroMsecAlign = false; //reset 0 millisec alignment
        }


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        // raygui: controls drawing
        //----------------------------------------------------------------------------------
            exitWindow = GuiWindowBox((Rectangle) { 0, 0, screenWidth, screenHeight }, WindowBox000Text);
            GuiLabel((Rectangle) { 24, 48, 168, 24 }, DestIPText);
            GuiLabel((Rectangle) { 24, 72, 168, 24 }, DestPortText);
            GuiLabel((Rectangle) { 24, 96, 168, 24 }, BeatrateText);
            if (GuiTextBox((Rectangle) { 192, 48, 216, 24 }, IPBoxText, 128, IPBoxEditMode)) IPBoxEditMode = !IPBoxEditMode;
            if (GuiTextBox((Rectangle) { 192, 72, 216, 24 }, portBoxText, 128, portBoxEditMode)) portBoxEditMode = !portBoxEditMode;
            if (GuiTextBox((Rectangle) { 192, 96, 48, 24 }, beatrateBoxText, 128, beatrateBoxEditMode)) beatrateBoxEditMode = !beatrateBoxEditMode;
            GuiLabel((Rectangle) { 240, 96, 16, 24 }, Label007Text);
            if (!checkBeatrateString(beatrateBoxText))
            {
                sprintf_s(BPHLabelText, sizeof(BPHLabelText), "%d BPH", (atoi(beatrateBoxText)*3600) );
                strcpy(BeatrateText, "Refresh rate:1~16Hz");
            }
            else strcpy(BeatrateText, "INVALID BEATRATE!!!");

            GuiLabel((Rectangle) { 264, 96, 144, 24 }, BPHLabelText);
            GuiLine((Rectangle) { 24, 152, 384, 24 }, Line009Text);
            GuiLabel((Rectangle) { 24, 176, 120, 24 }, Label010Text);
            GuiLabel((Rectangle) { 272, 176, 136, 24 }, Label011Text);

            if (!StartBtnActive)
            {
                GuiCheckBox((Rectangle) { 24, 200, 24, 24 }, YearCheckText, & settings.yearParamEnabled);
                GuiCheckBox((Rectangle) { 24, 224, 24, 24 }, MonthCheckText, & settings.monthParamEnabled);
                GuiCheckBox((Rectangle) { 24, 248, 24, 24 }, DayCheckText, & settings.dayParamEnabled);
                GuiCheckBox((Rectangle) { 24, 272, 24, 24 }, DowCheckText, & settings.dowParamEnabled);
                GuiCheckBox((Rectangle) { 24, 296, 24, 24 }, MoonCheckText, & settings.moonParamEnabled);
                GuiCheckBox((Rectangle) { 24, 320, 24, 24 }, HourCheckText, & settings.hourParamEnabled);
                GuiCheckBox((Rectangle) { 24, 344, 24, 24 }, MinCheckText, & settings.minParamEnabled);
                GuiCheckBox((Rectangle) { 24, 368, 24, 24 }, SecCheckText, & settings.secParamEnabled);
                GuiCheckBox((Rectangle) { 24, 392, 24, 24 }, MillisCheckText, & settings.msecParamEnabled);
                GuiCheckBox((Rectangle) { 144, 320, 24, 24 }, gmtCheckText, & settings.gmtParamEnabled);
                /*
                settings.yearParamEnabled = YearCheckChecked;
                settings.monthParamEnabled = MonthCheckChecked;
                settings.dayParamEnabled = DayCheckChecked;
                settings.dowParamEnabled = DowCheckChecked;
                settings.moonParamEnabled = MoonCheckChecked;
                settings.hourParamEnabled = HourCheckChecked;
                settings.minParamEnabled = MinCheckChecked;
                settings.secParamEnabled = SecCheckChecked;
                settings.msecParamEnabled = MillisCheckChecked;
                settings.gmtParamEnabled = gmtCheckChecked;
                */
            }
            else
            {
                if(settings.yearParamEnabled) GuiLabel((Rectangle) { 24, 200, 120, 24 }, YearCheckText);
                if(settings.monthParamEnabled) GuiLabel((Rectangle) { 24, 224, 120, 24 }, MonthCheckText);
                if(settings.dayParamEnabled) GuiLabel((Rectangle) { 24, 248, 120, 24 }, DayCheckText);
                if(settings.dowParamEnabled) GuiLabel((Rectangle) { 24, 272, 120, 24 }, DowCheckText);
                if(settings.moonParamEnabled) GuiLabel((Rectangle) { 24, 296, 120, 24 }, MoonCheckText);
                if(settings.hourParamEnabled) GuiLabel((Rectangle) { 24, 320, 120, 24 }, HourCheckText);
                if(settings.minParamEnabled) GuiLabel((Rectangle) { 24, 344, 120, 24 }, MinCheckText);
                if(settings.secParamEnabled) GuiLabel((Rectangle) { 24, 368, 120, 24 }, SecCheckText);
                if(settings.msecParamEnabled) GuiLabel((Rectangle) { 24, 392, 120, 24 }, MillisCheckText);
                if(settings.gmtParamEnabled) GuiLabel((Rectangle) { 144, 320, 120, 24 }, gmtCheckText);
            }

            if (GuiTextBox((Rectangle) { 272, 200, 136, 24 }, settings.yearParam, 128, yearParamEditMode)   && !StartBtnActive) yearParamEditMode = !yearParamEditMode;
            if (GuiTextBox((Rectangle) { 272, 224, 136, 24 }, settings.monthParam, 128, monthParamEditMode) && !StartBtnActive) monthParamEditMode = !monthParamEditMode;
            if (GuiTextBox((Rectangle) { 272, 248, 136, 24 }, settings.dayParam, 128, dayParamEditMode)     && !StartBtnActive) dayParamEditMode = !dayParamEditMode;
            if (GuiTextBox((Rectangle) { 272, 272, 136, 24 }, settings.dowParam, 128, dowParamEditMode)     && !StartBtnActive) dowParamEditMode = !dowParamEditMode;
            if (GuiTextBox((Rectangle) { 272, 296, 136, 24 }, settings.moonParam, 128, moonParamEditMode)   && !StartBtnActive) moonParamEditMode = !moonParamEditMode;
            if (GuiTextBox((Rectangle) { 272, 320, 136, 24 }, settings.hourParam, 128, hourParamEditMode)   && !StartBtnActive) hourParamEditMode = !hourParamEditMode;
            if (GuiTextBox((Rectangle) { 272, 344, 136, 24 }, settings.minParam, 128, minParamEditMode)     && !StartBtnActive) minParamEditMode = !minParamEditMode;
            if (GuiTextBox((Rectangle) { 272, 368, 136, 24 }, settings.secParam, 128, secParamEditMode)     && !StartBtnActive) secParamEditMode = !secParamEditMode;
            if (GuiTextBox((Rectangle) { 272, 392, 136, 24 }, settings.msecParam, 128, msecParamEditMode)   && !StartBtnActive) msecParamEditMode = !msecParamEditMode;
            GuiLine((Rectangle) { 24, 424, 384, 16 }, Line031Text);
            if (err) StartBtnActive = false;
            GuiToggle((Rectangle) { 24, 456, 72, 24 }, StartBtnText, & StartBtnActive);
            GuiLabel((Rectangle) { 120, 456, 288, 24 }, StatusLabelText);
            if (GuiButton((Rectangle) { 24, 128, 120, 24 }, applyButtonText)) ApplyButton();
            GuiLabel((Rectangle) { 160, 128, 256, 24 }, currentSettingsLabelText);
        //----------------------------------------------------------------------------------
        EndDrawing();
    }
    //save settings before exiting.
    strcpy(settings.IP, setIP);
    settings.Port = setPort;
    settings.Beatrate = beatrate;
    savesettings(settings, SAVEFILE);
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(125);        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

static void ApplyButton()
{
    bool ipError = 0;
    bool portError = 0;
    bool beatError = 0;

    // TODO: Implement control logic
    printf("Apply button clicked!\n");

    //check if IP is valid
    if (!checkIP(IPBoxText))
    {
        strcpy(setIP, IPBoxText);
        strcpy(DestIPText, "Destination IP:");
        ipError = 0;
    }
    else 
    {
        strcpy(DestIPText, "INVALID IP!!!");
        ipError = 1;
    }

    //check if Port is valid
    if (checkPortString(portBoxText) == 0)
    {
        setPort = atoi(portBoxText);
        strcpy(DestPortText, "Destination Port:");
        portError = 0;
    }
    else 
    {
        strcpy(DestPortText, "INVALID PORT!!!");
        portError = 1;
    }

    //check if beatrate is valid
    if (!checkBeatrateString(beatrateBoxText))
    {
        beatrate = atoi(beatrateBoxText);
        strcpy(BeatrateText, "Refresh rate:1~16Hz");
        bph = beatrate * 3600;
        beatError = 0;
    }
    else
    {
        strcpy(BeatrateText, "INVALID BEATRATE!!!");
        beatError = 1;
    }


    if (ipError || portError || beatError)
        err = true;
    else
    {
        sprintf_s(currentSettingsLabelText, sizeof(currentSettingsLabelText),
            "Current Setting : %s:%d/%dHz", setIP, setPort, beatrate);
        initOSC(9001, setPort, setIP, errorcode);
        printf("%s:%d, initOSC returned %d\n", setIP, setPort, errorcode);
        err = false;
    }
        
}

static void applySettings()
{
    bool ipError = 0;
    bool portError = 0;
    bool beatError = 0;

    //check if IP is valid
    if (!checkIP(settings.IP))
    {
        strcpy(IPBoxText, settings.IP);
        strcpy(setIP, settings.IP);
        strcpy(DestIPText, "Destination IP:");
        ipError = 0;
    }
    else
    {
        ipError = 1;
    }

    //check if Port is valid
    if (checkPortInt(settings.Port) == 0)
    {
        sprintf_s(portBoxText, sizeof(portBoxText), "%d", settings.Port);
        setPort = settings.Port;
        strcpy(DestPortText, "Destination Port:");
        portError = 0;
    }
    else
    {
        portError = 1;
    }

    //check if beatrate is valid
    if (!checkBeatrateInt(settings.Beatrate))
    {
        sprintf_s(beatrateBoxText, sizeof(beatrateBoxText), "%d", settings.Beatrate);
        beatrate = settings.Beatrate;
        strcpy(BeatrateText, "Refresh rate:1~16Hz");
        bph = beatrate * 3600;
        beatError = 0;
    }
    else
    {
        beatError = 1;
    }


    if (ipError || portError || beatError)
        err = true;
    else
    {
        sprintf_s(currentSettingsLabelText, sizeof(currentSettingsLabelText),
            "Current Setting : %s:%d/%dHz", setIP, setPort, beatrate);
        err = false;
    }

}