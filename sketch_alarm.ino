
#include <RTClib.h>

RTC_DS3231 rtc;

String ALARMAS[] = {"17:48:00"};
int ALARMAS_DURACION = 5; // segundos

String MINI_ALARMAS[] = {};
int MINI_ALARMAS_DURACION = 2; // segundos

int RELE_PIN = 13;

void setup() {
  Serial.begin(9600);
  
  pinMode(RELE_PIN, OUTPUT);

  delay(1000);

  if(!rtc.begin()) {
    Serial.println("Error de conn");
    while(1);
  }
   
  // Si se ha perdido la corriente, fijar fecha y hora
  if (rtc.lostPower()) {
      // Fijar a fecha y hora de compilacion
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      
      // Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
      // rtc.adjust(DateTime(2016, 1, 21, 3, 0, 0));
  }
}

void loop() {
   String timeNow = (getActualHour());
   Serial.println(timeNow);
   int dayOfWeek = getActualDayOfWeek();
   Serial.println(dayOfWeek);    
   checkearAlarmas(timeNow, dayOfWeek);
   checkearMiniAlarmas(timeNow, dayOfWeek);
     
   delay(500);
}


void checkearAlarmas(String timeNow, int dayOfWeek) 
{
  int alarmsSize = sizeof(ALARMAS);
  for (int x=0; x < alarmsSize; x++){
    if (ALARMAS[x] == timeNow && dayOfWeek != 0 && dayOfWeek != 1){
      sonarTimbre(ALARMAS_DURACION);
    }
  }
}

void checkearMiniAlarmas(String timeNow, int dayOfWeek)
{
  int alarmsSize = sizeof(MINI_ALARMAS);
  for (int x=0; x < alarmsSize; x++){
    if (MINI_ALARMAS[x] == timeNow && dayOfWeek != 0 && dayOfWeek != 1){
      sonarTimbre(MINI_ALARMAS_DURACION);
    }
  }
}

/**
 * Suena el timbre, duracion en Segs
 */
void sonarTimbre(int duracionSegs)
{
  digitalWrite(RELE_PIN, HIGH);
  delay(duracionSegs * 1000);
  digitalWrite(RELE_PIN, LOW);
}

/**
 * Retorna hora actual en Formato hh:mm:ss
 */
String getActualHour()
{
  DateTime date = rtc.now();  
  char time[30];
  sprintf(time, "%02d:%02d:%02d", date.hour(), date.minute(), date.second());

  return time;
}

/**
 * Retorna el actual dia de la semana
 * 0: Domingo
 * 1: Lunes
 * 2: Martes
 * 3: Miercoles
 * 4: Jueves
 * 5: Viernes
 * 6: Sabado
 */
int getActualDayOfWeek() {
  DateTime date = rtc.now();  
  int day = date.dayOfTheWeek();
  return day;
}
