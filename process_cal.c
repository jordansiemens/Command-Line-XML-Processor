#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_LINE_LEN 200
#define MAX_EVENTS 1000

/*
    Function: main
    Description: represents the entry point of the program.
    Inputs: 
        - argc: indicates the number of arguments to be passed to the program.
        - argv: an array of strings containing the arguments passed to the program.
    Output: an integer describing the result of the execution of the program:
        - 0: Successful execution.
        - 1: Erroneous execution.
*/

struct Time{
  int hour;
  int min;
  char flag[3];
};


struct Event
  {
  char description[MAX_LINE_LEN];
  char timezone[MAX_LINE_LEN];
  char location[MAX_LINE_LEN];
  int day, month, year;
  char dayof[MAX_LINE_LEN];
  struct Time start;
  struct Time end;

};

char* string_value(char *line){

  char* start;  
  start = strchr(line, '>');
  start++;
  char* value = strtok(start,"<");
  

  return value;

}

const char* check_flag(int hr){
  char* flag;
  if (hr>=12){
    flag = "PM";
  }
  else {
    flag = "AM";
  }
  return flag;
}

struct Time time_value(char *line){
  struct Time temp_time;
  temp_time.hour = atoi(strtok(string_value(line),":"));
  temp_time.min = atoi(strtok(NULL,":"));
  strcpy(temp_time.flag,check_flag(temp_time.hour));

  return temp_time;
}


void getInformation(char *line,struct Event *event){
  
    if(strstr(line,"<description>")){
      strcpy(event->description,string_value(line));    
    }
    if(strstr(line,"<timezone>")){
      strcpy(event->timezone,string_value(line));
    } 
    if(strstr(line,"<location>")){
      strcpy(event->location,string_value(line));
  }
    if(strstr(line,"<day>")){
      event->day = atoi(string_value(line));
    }
    if(strstr(line,"<month>")){
      event->month = atoi(string_value(line));
    }
    if(strstr(line,"<year>")){
      event->year = atoi(string_value(line));
    }
    if(strstr(line,"<dweek>")){
      strcpy(event->dayof,string_value(line));
    }
    if(strstr(line,"<start>")){
      event->start=time_value(line);
    }
    if(strstr(line,"<end>")){
      event->end=time_value(line);
    }

}  

int parse_file(char* file_name,struct Event *event_arr){

  char line[MAX_LINE_LEN];

  FILE *fp;
  fp = fopen(file_name,"r");

  if (fp == NULL){
    printf("Error, file does not exist");
    exit(0);
  }
  
  int i = 0;
  
  while (fgets(line,MAX_LINE_LEN,fp) != NULL){

    if(strstr(line,"</event>")){
      i++;
    } 
    getInformation(line,&event_arr[i]);
  }
  fclose(fp);

  return i;
}

int compare(const void *e1, const void *e2) {

  struct Event *event1 = (struct Event *)e1;
  struct Event *event2 = (struct Event *)e2;

  if (event1->year < event2->year){
    return -1;
  }
  if (event1->year == event2->year && event1->month < event2->month){
    return -1;
  }
  if (event1->year == event2->year && event1->month == event2->month && event1->day < event2->day){
    return -1;
  }
  if (event1->year == event2->year && event1->month == event2->month && event1->day == event2->day &&
    event1->start.hour < event2->start.hour) return -1;
  
  if (event1->year == event2->year && event1->month == event2->month && event1->day == event2->day &&
    event1->start.hour == event2->start.hour && event1->start.min < event2->start.min) return -1;

  else {
    return 1;
  }
}

int smaller_than(struct Event *event1,int year, int month, int day){ // Checks if date is smaller than second date
  if (event1->year < year) return 0;
  if (event1->year == year && event1->month < month) return 0;
  if (event1->year == year && event1->month == month && event1->day <= day) return 0;

  return 1;
  
}

int equals(struct Event *event1, struct Event *event2){
  if (event1->day == 0 || event2->day == 0)return 1;
  if (event1->year == event2->year && 
      event1->month == event2->month &&
      event1->day == event2->day){

    return 0;
  }
  else {
    return 1;
  }
}

void print_event(struct Event *event){

  if (event->start.hour > 12){
    event->start.hour = event->start.hour - 12;
  }
  if (event->end.hour > 12){
    event->end.hour = event->end.hour -12;
  }

  printf("%02d:%02d %s to %02d:%02d %s: %s {{%s}} | %s", // Event formatting
  event->start.hour,
  event->start.min,
  event->start.flag,
  event->end.hour,
  event->end.min,
  event->end.flag,
  event->description,
  event->location,
  event->timezone);

}

void output_events(char start[], char end[],int num_of_events, struct Event *event_arr){

  int syear = atoi(strtok(start,"/"));
  int smonth = atoi(strtok(NULL, "/"));
  int sday = atoi(strtok(NULL,"/"));

  int eyear = atoi(strtok(end,"/"));
  int emonth = atoi(strtok(NULL, "/"));
  int eday = atoi(strtok(NULL,"/"));


  const char *months[13] = {
    "NULL",
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"};

  qsort(event_arr, num_of_events, sizeof(struct Event), compare);

  int x = 0; // Counter for events in a row

  for (int i = 0; i<num_of_events;i++){

    if (smaller_than(&event_arr[x],syear,smonth,sday-1) == 1 && smaller_than(&event_arr[x],eyear,emonth,eday) == 0){
      
      printf("%s %d, %d (%s)\n",
      months[event_arr[x].month],
      event_arr[x].day,
      event_arr[x].year,
      event_arr[x].dayof);

      int length = strlen(months[event_arr[x].month]) + sizeof(event_arr[x].day) + sizeof(event_arr[x].year) + strlen(event_arr[x].dayof);

      for (int i =0;i<(length+4);i++) printf("-"); // Print dashes to match length of string (4 is length of unused characters)
      printf("\n"); // Print new line
       
      do {

        print_event(&event_arr[x]);

        printf("\n");
        x++;


      } while(equals(&event_arr[x-1],&event_arr[x]) == 0);
      

      printf("\n"); //Prints gap in between dates
      
      if (x == num_of_events){
        break; // Breaks loop
      } 
      
    }
    else {
      x++;
    }
  }

}

int main(int argc, char *argv[])
{
  struct Event event_arr[MAX_EVENTS];

  char* startdate = strchr(argv[1], '=');
  startdate++;
  char* enddate = strchr(argv[2],'=');
  enddate++;
  char* file_name = strchr(argv[3],'=');
  file_name++;

  int num_of_events = parse_file(file_name,event_arr);

  output_events(startdate,enddate,num_of_events,event_arr);


  exit(0);
}
