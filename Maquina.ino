//_________________________________________________________
// Carpeta: Maquina.ino
// Fecha: 29/06/2023
// Descripción: Control general de la máquina de estados y ejecución principal.
// Destino: Programa de Ingeniería de Sistemas.
// Autor: Laura Valentina Caicedo <lcaicedot@unicauca.edu.co> y Juan José Muñoz <jjmunozm@unicauca.edu.co>
// Excep. / Errores: Ninguno implementado.
// Copyright (c) Unicauca 2023. Todos los derechos reservados.
//________________________________________________________


#include "StateMachineLib.h"
#include "AsyncTaskLib.h"
#include <LiquidCrystal.h>
#include <Keypad.h>


//Configuración LCD
LiquidCrystal lcd(12, 11, 10, 6, 5, 4);

//Configuración del teclado
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {35, 37, 39, 41}; 
byte colPins[KEYPAD_COLS] = {43, 45, 47, 49};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

// Definición de estados de la máquina
enum State
{
  IniciarSesion = 0,
  Monitoreo = 1,
  Alarma = 2,
  SistemaBloqueado = 3,
  Sensor = 4
};

// Definición de transiciones de la máquina
enum Input
{
  contraseniaCorrecta = 0,
  tempMayor32 = 1,
  timeOut5 = 2,
  tempMayor32Time5 = 3,
  timeOut20 = 4,
  event = 5,
  timeOut15 = 6,
  timeOut25 = 7,
  tecla = 8,
  contraseniaIncorrecta = 9,
  desconocido = 10,
};

// Definicioónes de variables 
int verificador = 0;
unsigned long startTime = 0;
unsigned long tiempoInicio = 0;
unsigned long start = 0;
unsigned long duracionTimeout5 = 5000; 
unsigned long duracionTimeout20 = 2500; 
unsigned long elapsedTime = 0;
extern int verificadorAlarma = 0;
extern int verificadorBloqueo = 0;
extern int temperatura = 0;
extern int humedad = 0; 
extern int verificadorTemp = 0;
extern int verificadorPV = 0;
extern int verificadortecla = 0;
extern int Luz = 0;
extern void leerTemperatura(void);
extern void leerFotoresistor(void);
char clave [] = "1006";
int contador = 0;
char password [4];
int count = 0;
boolean bandera = true;
int ledRojo = 16;
int ledVerde = 18;
int ledAzul = 19;

//Tareas asincronas
AsyncTask asyncTask(2000, true, leerFotoresistor);
AsyncTask asyncTask1(4000, true, leerTemperatura);


// Crear la máquina de estados
StateMachine stateMachine(5, 10);

// Almacena la última entreda
Input input;

// Concifugración de las transiciones de los estados de la máquina
void setupStateMachine()
{
  // Transiciones
  stateMachine.AddTransition(IniciarSesion, Monitoreo, []() { return input == contraseniaCorrecta; });
  stateMachine.AddTransition(IniciarSesion, SistemaBloqueado, []() { return input == contraseniaIncorrecta; });

  stateMachine.AddTransition(Monitoreo, Alarma, []() { return input == tempMayor32; });
  stateMachine.AddTransition(Alarma, Monitoreo, []() { return input == timeOut5; });

  stateMachine.AddTransition(Monitoreo, Sensor, []() { return input == timeOut25; });
  stateMachine.AddTransition(Sensor, Monitoreo, []() { return input == timeOut15; });

  stateMachine.AddTransition(Alarma, SistemaBloqueado, []() { return input == tempMayor32Time5; });

  stateMachine.AddTransition(SistemaBloqueado, Sensor, []() { return input == timeOut20; });
  stateMachine.AddTransition(Sensor, SistemaBloqueado, []() { return input == event; });

  stateMachine.AddTransition(Sensor, IniciarSesion, []() { return input == tecla; });

  // Acciones  de entrada
  stateMachine.SetOnEntering(IniciarSesion, inputIniciarSesion);
  stateMachine.SetOnEntering(Monitoreo, inputMonitoreo);
  stateMachine.SetOnEntering(Alarma, inputAlarma);
  stateMachine.SetOnEntering(SistemaBloqueado, inputSistemaBloqueado);
  stateMachine.SetOnEntering(Sensor, inputSensor);

  //Acciones de salida
  stateMachine.SetOnLeaving(IniciarSesion, []() { Serial.println("Leaving Iniciar sesión"); });
  stateMachine.SetOnLeaving(Monitoreo, []() {Serial.println("Leaving Monitoreo"); });
  stateMachine.SetOnLeaving(Alarma, []() {Serial.println("Leaving Alarma"); });
  stateMachine.SetOnLeaving(SistemaBloqueado,outPutSistemaBloqueado );
  stateMachine.SetOnLeaving(Sensor, []() {Serial.println("Leaving Sensor"); });
}

void setup() 
{
  Serial.begin(9600);

  Serial.println("Starting State Machine...");
  setupStateMachine();  
  Serial.println("Start Machine Started");
  // Estado inicial
  stateMachine.SetState(IniciarSesion, false, true);
}

void loop() 
{
  // Leer la entrada del usuario
  input = static_cast<Input>(readInput());
  // Actualizar el estado de la máquina
  stateMachine.Update();
}

// Función auxiliar que lee la entrada del usuario
int readInput()
{
  int currentInput = stateMachine.GetState(); 

    switch (currentInput) {

    //Caso Iniciar sesión  
    case IniciarSesion:  

    //Llama a la función login y cambia a el estado Monitoreo si la contraseña es correcta, sino a el estado SistemaBloqueado si la contraseña es incorrecta 
    verificador = login();
    if(verificador == 1){
      return Input :: contraseniaCorrecta;
    }
    if(verificador == 2){
      return Input :: contraseniaIncorrecta;
    }
    if(verificador == 0){
      return Input :: desconocido;
    }
      break;

    //Caso monitoreo temperatura, humedad y luz  
    case Monitoreo:

    //Iniciar a tomar el tiempo de ejeución de este caso
    start = millis();

    //Actualiza las tareas
    asyncTask.Update();
    asyncTask1.Update();

    //Verifica si han transcurrido por lo menos 2.5 segundos y cambia a el estado Sensor
    if(millis() - start >= duracionTimeout20){
      return Input :: timeOut25;
    }

    //Verifica si la tempertaura es mayor a 32, detiene las tareas y cambia a el estado Alarma
    if(verificadorTemp == 1){
      asyncTask.Stop();
      asyncTask1.Stop();
      return Input :: tempMayor32;
    }
      break;

    //Caso alarma  
    case Alarma:

    //Iniciar a tomar el tiempo de ejeución de este caso
    tiempoInicio = millis();
    //Se activa y la alarma 
    verificadorAlarma++;
    activarAlarma();
    startTime = micros();

    //La alarma suena 4 veces durante 5 segundos y cambia a el estado Monitoreo
    if(verificadorAlarma >= 0 && verificadorAlarma < 4){
     desactivarAlarma();
      return Input :: timeOut5;
    }

    //Se apaga la alarma después de sonar duranta 20 segundos, se comprueba que la temperatura fue mayor a 32 y cambia a el estado Sistema Bloqueado
    if(verificadorAlarma == 4 && verificadorTemp == 1){
      desactivarAlarma();
      elapsedTime = 0;
      startTime = 0;
      stateMachine.Update();
      verificadorAlarma = 0;
      return Input :: tempMayor32Time5;
    }
      break;

    //Caso sistema bloqueado  
    case SistemaBloqueado:

    //Cambia a el estado Sensor
    return Input::timeOut20;
      break;

    //Caso sensor    
    case Sensor:
    //Llamar al método para verificar si se oprime el caracter * y cambia a el estado IniciarSesion
      verificadortecla = seleccionarTecla();
      delay(1000);
      sensorPV();
      lcd.clear();
      if(verificadortecla == 1){
        return Input::tecla;
      }

      //Verifica si se acerca algo al sensor y cambia a el estado SistemaBloqueado
      if(verificadorPV == 1){
        lcd.setCursor(0, 0);
        lcd.print("Objeto ");
        lcd.setCursor(0, 1);
        lcd.print("detectado");
        delay(1000);
        lcd.clear();
        return Input::event;
      }

      //Verifica si han pasado 1.5 segundos y no se detecta nada en el sensor para cambiar a el estado Monitoreo
      if(verificadorPV == 2 && millis() - tiempoInicio >= 1500){
        lcd.setCursor(0, 0);
        lcd.print("Objeto ");
        lcd.setCursor(0, 1);
        lcd.print("no detectado");
        delay(1000);
        lcd.clear();
        return Input::timeOut15;
      }
      break;

    default:
      break;
    }
}


//Acción de entrada en el estado IniciarSesion
void inputIniciarSesion(){
  contador = 0;
  count = 0;
  Serial.println("Starting Iniciar sesion");
  setupIniciarSesion();
}

//Acción de entrada en el estado Monitoreo
void inputMonitoreo(){
  Serial.println("Starting monitoreo");
  lcd.setCursor(0, 0);
  lcd.print("Monitoreando...");
  delay(1000);
  lcd.clear();

  //Leer temperatura, humedad y luz
  leerFotoresistor();
  leerTemperatura();

  asyncTask.Start();
  asyncTask1.Start();
}

//Acción de entrada en el estado Alarma
void inputAlarma(){
  Serial.println("Starting alarma");

}

//Acción de entrada en el estado SistemaBloqueado
void inputSistemaBloqueado(){
    Serial.println("Starting sistema bloqueado");
    sistemaBloqueado();
}

//Acción de entrada en el estado Sensor
void inputSensor(){
  Serial.println("Starting Sensor");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor...");
  lcd.setCursor(0, 1);
  lcd.print("puertas y ventanas");
  delay(1000);
  lcd.clear();
  setupSensor();
  tiempoInicio = millis();
  verificadorPV == 0;
  verificadortecla == 0;
}

//Acción de salida del estado SistemaBloqueado
void outPutSistemaBloqueado(){
  Serial.println("Leaving sistema bloqueado");
}
