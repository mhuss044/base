// Time related functions.

// Implement gluttimingFunc(millisecs, func*, ... );

void WaitForSecs(int secs)
{
    clock_t start;

    cout << INS << INS << "Wait (" << secs << "): ";


    start = clock();                                                    // Start num ticks

    for(int x = 0; x < secs; x++)                                       // Iterates secs times
    {
        while((clock() - start) < CLOCKS_PER_SEC);                      // Counts off seconds.
        cout << ". ";
        start = clock();
    }
}

void WaitForQSec(float qsecs)
{
    clock_t start;
    start = clock();                                                    // Start num ticks

    while((clock() - start) < (qsecs*float(CLOCKS_PER_SEC/4)));              // loop till time elapsed > #quarter seconds to wait
}

char* currentTime(char *specifiers = "%b %d, %Y; %Hh:%Mm:%Ss")          // Usually: "%b %d, %Y; %H:%M:%S\n"
{
    char *date;
    date = new char[90];

    struct tm *tim;
    time_t tim2;

    tim2 = time(NULL);
    tim = localtime(&tim2);

    strftime(date, 90,specifiers, tim);                                 // Get date.

    return date;
}

/*
// Only capable of watching 10 clocks at once.
class CWatchClock
{
public:
    clock_t times[10];

    CWatchClock();                                                       // Constructor: sets times to empty.
    int StartTime(void);                                                // Starts time, returns time ID.
    float StopTime(int *index);                                         // Stops time, returns #seconds elapsed.
    char* printTimeID(int *index);                                      // Stops time, returns string of time elapsed.
}MainGameWatch;

CWatchClock::CWatchClock()
{
    for(int x = 0; x < 10; x++)
        times[x] = NOVAL;
}

int CWatchClock::StartTime(void)
{
    int index = 0;

    while(index < 10)
    {
        if(times[index] == NOVAL)
            break;

        index++;
    }

    if(index > 9)                                                       // No more watches
    {
        CMainGameF.CMainGameLogFile << EXC << "ERROR (WatchClock): Requested watch when none available. clock_t times[] is full.";
        cout << EXC << "ERROR (WatchClock): Requested watch when none available. clock_t times[] is full.";
        WaitFor(1);

        // Return bogus time.
        return 0;
    }

    //Start clock
    times[index] = clock();

    return index;
}

float CWatchClock::StopTime(int *index)
{
    double timeElapsed;

    timeElapsed = double((clock() - times[*index])/CLOCKS_PER_SEC);

    times[*index] = NOVAL;                                              // Reset time.

    return timeElapsed;                                                 // Return num secs.
}

char* CWatchClock::printTimeID(int *index)
{
    double timeElapsed;
    char *time;
    time = new char[20];

    timeElapsed = double((clock() - times[*index])/CLOCKS_PER_SEC);

    times[*index] = NOVAL;                                              // Reset time.

    if(timeElapsed >= 60)
        sprintf(time, "%d minutes, %d seconds.", int((timeElapsed)/60), int(timeElapsed)%60 );
    else
        sprintf(time, "%0.2lf seconds.", timeElapsed );

    return time;
}
*/




