//_________________________________________________________
// Carpeta: Alarma.ino
// Fecha: 29/06/2023
// Descripción: Control de la alarma.
// Destino: Programa de Ingeniería de Sistemas.
// Autor: Laura Valentina Caicedo <lcaicedot@unicauca.edu.co> y Juan José Muñoz <jjmunozm@unicauca.edu.co>
// Excep. / Errores: Ninguno implementado.
// Copyright (c) Unicauca 2023. Todos los derechos reservados.
//________________________________________________________

#define BEZ_SENSOR 3
// Variable para almacenar el tiempo de inicio
unsigned long tiempoIni; 

void activarAlarma() {
  // Guardar el tiempo de inicio
  tiempoIni = millis(); 
  while (millis() - tiempoIni < 5000) {
    // Reproducir el tono de alarma durante 250 ms
    tone(BEZ_SENSOR, 262, 250); 
    // Pausa de 250 ms entre tonos
    delay(250); 
    // Detener la reproducción del tono
    noTone(BEZ_SENSOR);
    // Pausa de 250 ms entre tonos
    delay(250); 
  }
}

void desactivarAlarma() {
  noTone(BEZ_SENSOR);
}