//CÓDIGO FINAL

//Medición de flujo y volumen
volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
int PinSensor = 2;    //Sensor conectado en el pin 2
float factor_conversion = 7.5; //para convertir de frecuencia a caudal
float volumen = 0;
long dt = 0; //variación de tiempo por cada bucle
long t0 = 0; //millis() del bucle anterior
String caudal1 = "";
String volumen1 = "";

//---Función que se ejecuta en interrupción---
void ContarPulsos ()
{
  NumPulsos++;  //incrementamos la variable de pulsos
}

//---Función para obtener frecuencia de los pulsos---
int ObtenerFrecuencia()
{
  int frecuencia;
  NumPulsos = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(1000);   //muestra de 1 segundo
  noInterrupts(); //Desabilitamos las interrupciones
  frecuencia = NumPulsos; //Hz(pulsos por segundo)
  return frecuencia;
}

//Mostrar en pantalla LCD
#include <LiquidCrystal.h>
//Crear el objeto LCD con los números correspondientes (rs, en, d4, d5, d6, d7)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup()
{
  Serial.begin (9600);
  lcd.begin(16, 2); // Inicializar el LCD con el número de  columnas y filas del LCD
  pinMode(PinSensor, INPUT);
  attachInterrupt(0, ContarPulsos, RISING); //(Interrupcion 0(Pin2),funcion,Flanco de subida)
  t0 = millis();
}

void loop ()
{
  float frecuencia = ObtenerFrecuencia(); //obtenemos la Frecuencia de los pulsos en Hz
  float caudal_L_m = frecuencia / factor_conversion; //calculamos el caudal en L/min
  dt = millis() - t0; //calculamos la variación de tiempo
  t0 = millis();
  volumen = volumen + (caudal_L_m / 60) * (dt / 1000); // volumen(L)=caudal(L/s)*tiempo(s)
  //---Enviar datos al LCD---
  // Cursor en la primera posición de la 1° fila
  lcd.setCursor(0, 0);
  lcd.print("Caudal:");
  lcd.print(caudal_L_m, 3);
  lcd.print(" L/m");
  // Cursor en la primera posición de la 2° fila
  lcd.setCursor(0, 1);
  lcd.print("Volumen:");
  lcd.print(volumen, 2); //3 decimales
  lcd.print(" L ");
  caudal1 = String(caudal_L_m, 3); //3 decimales
  volumen1 = String(volumen, 2); //2 decimales
  Serial.print(caudal1 + "/" + volumen1 + "/"); // enviar caudal1 y volumen1 en un solo paquete Serial
  delay(200);
}

