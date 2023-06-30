//_________________________________________________________
// Carpeta: IniciarSesion.ino
// Fecha: 29/06/2023
// Descripción: Control de iniciar sesión.
// Destino: Programa de Ingeniería de Sistemas.
// Autor: Laura Valentina Caicedo <lcaicedot@unicauca.edu.co> y Juan José Muñoz <jjmunozm@unicauca.edu.co>
// Excep. / Errores: Ninguno implementado.
// Copyright (c) Unicauca 2023. Todos los derechos reservados.
//________________________________________________________

#include <LiquidCrystal.h>
#include <Keypad.h>

void setupIniciarSesion() {
  lcd.begin(16, 2);
  lcd.print("Bienvenido");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ingrese clave");
  lcd.setCursor(4, 1);

  // sets the redPin to be an output
  pinMode(ledRojo, OUTPUT);
  // sets the greenPin to be an output
  pinMode(ledVerde, OUTPUT);
  // sets the bluePin to be an output
  pinMode(ledAzul, OUTPUT); 
}

//Iniciar sesión, retorna 1 si se ingresó correctamente, retorna 2 si se hicieron 3 intentos fallidos
int login() {
  char key = keypad.getKey();
  int centinela = 0;
    verificadorPV == 0;
  verificadortecla == 0;
  if (key && count != 3){
    password[contador] = key;
    contador = contador + 1;
    lcd.print("*");
    if(contador == 4){
        if(password[0]==clave[0] && password[1]==clave[1] && password[2]==clave[2] && password[3]==clave[3]){
          delay(1000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Contrasena");
          lcd.setCursor(0, 1);
          lcd.print("Correcta");
          delay(1000);
          color(0,255, 0); // Led verde
          delay(1000);
          lcd.clear();
          count = 3;
          bandera = true;
          centinela = 1;
        }else{
          delay(1000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Contrasena");
          lcd.setCursor(0, 1);
          lcd.print("Incorrecta");
          count = count + 1;
          delay(1000);
          color(255, 0, 0); // Led rojo
          delay(1000); 
          lcd.clear();
          if(count < 3){
              lcd.setCursor(0, 0);
              lcd.print("Ingrese clave");
              lcd.setCursor(4, 1);
          }
          bandera = false;
        }
        contador = 0;
      }
      if(count == 3 && !bandera){
        centinela = 2;
      }
    }
    return centinela;
  }

//Sistema bloqueado
  void sistemaBloqueado() {
  unsigned long startTime = millis();  // Obtener el tiempo de inicio
  while (millis() - startTime < 5000) {  // Continuar durante 20 segundos
    lcd.setCursor(0, 0);
    lcd.print("Sistema");
    lcd.setCursor(0, 1);
    lcd.print("bloqueado");
    delay(1000);
    color(0, 0, 255);  // Led azul
  }
    lcd.clear();
  }

//Seleccionar tecla (*)
int seleccionarTecla() {
  char tecla = keypad.getKey();
  lcd.setCursor(0, 0);
  lcd.print("Cargando...");
  lcd.setCursor(4, 1);
    if (tecla == '*') {
      verificadortecla = 1;
      delay(2000);
    }
    return verificadortecla;
  }
  void color (unsigned char red, unsigned char green, unsigned char blue) 
  {
  analogWrite(ledRojo, red);
  analogWrite(ledAzul, blue);
  analogWrite(ledVerde, green);
  }