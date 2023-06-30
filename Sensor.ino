//_________________________________________________________
// Carpeta: Sensor.ino
// Fecha: 29/06/2023
// Descripción: Control de sensor hall.
// Destino: Programa de Ingeniería de Sistemas.
// Autor: Laura Valentina Caicedo <lcaicedot@unicauca.edu.co> y Juan José Muñoz <jjmunozm@unicauca.edu.co>
// Excep. / Errores: Ninguno implementado.
// Copyright (c) Unicauca 2023. Todos los derechos reservados.
//________________________________________________________

// Pin digital utilizado para la lectura del sensor lineal Hall 
const int hallSensorDigitalPin = 8; 

void setupSensor() {
  pinMode(hallSensorDigitalPin, INPUT);  // Configurar el pin digital del sensor lineal Hall como entrada
  pinMode(LED_BUILTIN, OUTPUT);          // Configurar el pin del LED integrado como salida
  Serial.begin(9600);                    // Iniciar la comunicación serial a 9600 baudios
}

//Activa el sensor
void sensorPV() {
  // Leer el valor digital del sensor lineal Hall
  int digitalValue = digitalRead(hallSensorDigitalPin);       

  if (digitalValue == 1) {
    // Encender el LED integrado si algo se acerca al sensor y retorna 1
    digitalWrite(LED_BUILTIN, HIGH);  
    verificadorPV = 1;
  } 
  if (digitalValue == 0) {
    // Apagar el LED integrado si no se detecta nada y retorna 2
    digitalWrite(LED_BUILTIN, LOW);   
    verificadorPV = 2;
  }
}