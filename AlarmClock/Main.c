#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include <time.h>
//todo:
//Replace system("cls"); for more cross platform friendly
//Add clock tick while typing in time for more live usage
typedef enum { false, true } bool;

time_t start, stop;

void clearscr(void)
{
#ifdef _WIN32
    system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    system("clear");
//add some other OSes here if needed
#else
    #error "OS not supported."
#endif
}

void print_time(int time_array[])/*Prints current time from array*/
{
    int i;

     for(i = 0; i <= 2; i++) /*Prints in format HH:MM:SS*/
        {
            if (time_array[i]<10) printf("0");

            printf("%d",time_array[i]);

            if(i>=2) break;

            printf(":");
        }
}

int compareArrays(int array_a[], int array_b[], int array_size)/*compares two arrays if equal return 1 else 0*/
{
  int i;
  for(i = 0; i <= array_size; i++) {
    if (array_a[i] != array_b[i]) return 0;
  }
  return 1;
}

void entertime(int present_time[])
{
    int temp_time; //holds hhmmss for later use
    int user_time;
    while(true)//Loops untill time is entered
    {
    temp_time = 0;
    user_time = 0;

        printf("Enter the time: \n");

    char *p, s[100]; //prevents non-integer inputs
    while (fgets(s, sizeof(s), stdin)) {

         user_time = strtol(s, &p, 10);
        if (p == s || *p != '\n') {
                clearscr();
                printf("Thats not a valid time!\n");
        }
        else break;

    }


        temp_time = user_time/10000;/*Checks if entered time hh is < 24*/
        clearscr();
        if(temp_time<24 && temp_time >= 0)
        {
        present_time[0] = temp_time;
        user_time = user_time - (temp_time * 10000);
        temp_time = user_time/100;

            if (temp_time<60 && temp_time >= 0) /*Checks if entered time mm is < 60*/
            {
            present_time[1] = temp_time;
            user_time = user_time - (temp_time * 100);

                if(temp_time<60 && temp_time >= 0) /*Checks if entered time ss is < 60*/
                {
                present_time[2] = user_time;
                break;/*When the time is valid, continue program*/
                }
                else printf("Thats not a valid time!\n");
            }
            else printf("Thats not a valid time!\n");
        }
        else printf("Thats not a valid time!\n");

    }//End of while loop
}

void realtime(int array_current[])/*Checks the time elapsed from first user input*/
{
    int i;
    int elapsed_array[2] = {0};
    int temp;
    time(&stop);
    temp = difftime(stop, start);
    time(&start);/*Starts time again for next elapsed time*/

    /*transfers elapsed time into a array from sec to hhmmss for later integration*/
    elapsed_array[0] = temp/3600;
    if (elapsed_array[0]>0)
    {
    temp = temp - elapsed_array[0]*3600;
    }
    elapsed_array[1] = temp/60;

    if (elapsed_array[0]>0)
    {
    temp = temp - elapsed_array[1]*60;
    }
    elapsed_array[2] = temp;


    /*transfers elapsed time into current time array*/
    for(i=0; i<=2; i++)
    {
        array_current[i]= array_current[i]+ elapsed_array[i];
    }
    if(array_current[2]>59)
    {
    array_current[2] = array_current[2] - 60;
    array_current[1]++;
    }
    if(array_current[1]>59)
    {
    array_current[1]= array_current[1] - 60;
    array_current[0]++;
    }
    if(array_current[0]>24)
    {
    array_current[1]= array_current[1] - 24;
    }
    return;
}

void countdown(int alarm_time[], int current_time[])/*current time, countdown and alarm time*/
{
int time_c = 0;
int temp = 0;
int time_a = 0;
int count_array[2] = {0};
int i;

/*time it took to get here*/
realtime(current_time);
/*----------------*/

/*time difference between current time and alarm*/
    time_c = current_time[2];
    temp = current_time[1] * 60;
    time_c = time_c + temp;
    temp = current_time[0] * 3600;
    time_c = time_c + temp;

    time_a = alarm_time[2];
    temp = alarm_time[1] * 60;
    time_a = time_a + temp;
    temp = alarm_time[0] * 3600;
    time_a = time_a + temp;

    temp = time_a - time_c;

    if(temp < 0) temp = 86400 + temp; // outside this day


/*puts the difference into countdown array hhmmss*/
    count_array[0] = temp/3600;
    if (count_array[0]>0)
    {
        temp = temp - count_array[0]*3600;
    }
    count_array[1] = temp/60;
    if (count_array[1]>0)
    {
        temp = temp - count_array[1]*60;
    }
     count_array[2] = temp;


    if (compareArrays(alarm_time, current_time, 2) == false)/*checks if array is equal before dowhile loop*/
    {
        do
        {
            clearscr();
            printf("Current time: ");
            print_time(current_time);
            printf("\nAlarm time: ");
            print_time(alarm_time);

            printf("\n\nAlarm will ring  in: ");
            print_time(count_array);
            /*Countup*/
            if(current_time[2]<59)
            {
            current_time[2]++;
            }
            else
            {
            current_time[2] = 0;
                if(current_time[1]<59)
                {
                current_time[1]++;
                }
                else
                {
                current_time[1] = 0;
                    if (current_time[0]<23)current_time[0]++;
                    else current_time[0] = 0;
                }
            }
            /*----------*/
            /*Countdown*/
            if(count_array[2]>0)
            {
                count_array[2]--;
            }
            else if(count_array[1]>0)
            {
                count_array[1]--;
                count_array[2] + 59;
            }
            else if(count_array[0]> 0)
            {
                count_array[0]--;
                count_array[1] + 59;
            }

            /*-----------*/
        Sleep(1000);/*1 sec between each loop*/
        }while (compareArrays(alarm_time, current_time, 2) == false);/*Checks if current time = alarm*/
    }

clearscr();
printf("Alarm?!\n");
printf("Press Any Key to Continue\n");
getchar();

return;
}

void clock_loop(int time_for_alarm[], int present_time[])
{

    clearscr();

    printf("The current time is: ");

    print_time(present_time);

    printf("\nAt what time do you want the alarm to ring?\n");

    entertime(time_for_alarm);

    countdown(time_for_alarm, present_time);

    realtime(present_time);


    return;
}

void first_input(int present_time[]) //First user time input
{
    printf("What is the current time?\n");
    clearscr();
    entertime(present_time);

    time(&start);/*Time from entering current time till stop*/

    system("cls");

    printf("The current time is: ");

    print_time(present_time);
    return;
}

int main()
{
int time_for_alarm[3]= {0};
int present_time[3] = {0};

    first_input(present_time);

    char response;
    do {
        clock_loop(time_for_alarm, present_time);

        printf("Press Y/y to set another alarm");

        response = getch();

    } while(response =='y' || response =='Y'); // Clock loop


return 0;
}

