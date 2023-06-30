//_________________________________________________________
// Carpeta: Monitoreo.ino
// Fecha: 29/06/2023
// Descripción: Control de monitoreo (temperatura, humedad y luz).
// Destino: Programa de Ingeniería de Sistemas.
// Autor: Laura Valentina Caicedo <lcaicedot@unicauca.edu.co> y Juan José Muñoz <jjmunozm@unicauca.edu.co>
// Excep. / Errores: Ninguno implementado.
// Copyright (c) Unicauca 2023. Todos los derechos reservados.
//________________________________________________________

#include "AsyncTaskLib.h"
#include "DHTStable.h"

#define DEBUG(a) Serial.print(millis()); Serial.print(": "); Serial.println(a);
#define DHT11_PIN       2

DHTStable DHT;


const int photocellPin = A0;
int outputValue = 0;

//Leer luz
void leerFotoresistor(void)
{
  outputValue = analogRead(photocellPin);
  Luz = outputValue;
}

//Leer temperatura y luz
void leerTemperatura(void)
{
  int chk = DHT.read11(DHT11_PIN);

  
  humedad = DHT.getHumidity();
  temperatura = DHT.getTemperature();

  delay(3000);

  if(temperatura >= 15.0f){
    verificadorTemp = 1;
  }else{
    verificadorTemp = 2;
  }
}
