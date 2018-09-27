/************************************************
  Programa: Sistema de simulación de alarma de seguridad

  Autor: Miguel Ángel Ramírez Lira.

  Sistema que activa una alarma mientras se mantenga una puerta abierta (la cual se simula con un boton),
  y que al ser cerrada nuevamente se apaga por si sola (si se vuelve a presionar el boton nuevamente)

 ***********************************************/



/****************         CONSTANTES         *******************/
#define BOTON_PUERTA 3                      //Botón que simula el cierra y apertura de una puerta
#define BOCINA 5                            //Bocina o buzzer de alarma
const int TIEMPO_DE_UMBRAL = 100;           //Constante que se usará para eliminar el rebote de la interrupcion

/****************         VARIABLES         *******************/
long contadorTiempo = 0;
boolean alarmaApagada = true;           //variable que cambia de estado cada que se presiona el boton
volatile boolean botonPresionado = false;   // Variable usada por la interrupción "interrupcionAlarma" 



void setup() {
  pinMode(BOTON_PUERTA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BOTON_PUERTA), interrupcionAlarma, RISING);   //rising es el modo de disparo que activara la interrupcion
  Serial.begin(9600);
}

void loop() {

  if (botonPresionado) {
    sonarAlarma();
  } else {
    if (!alarmaApagada) {     //Solo si la alarma no se ha apagado aun reproducirá el tono de apagado
      tone(BOCINA, 600.25, 250);
      delay(250);
      noTone(BOCINA);
      delay(300);
      tone(BOCINA, 987.77, 400);
      delay(400);
      noTone(BOCINA);
      alarmaApagada = true;
    }

    noTone(BOCINA);               
  }


}

void interrupcionAlarma() {
  /*Este método es usado cuando se activa la interrupcion al presionar el botón para activar la alarma 
  (El botón simula que se abre una puerta al presionarlo, y nuevamente al volver a presionarlo simula el cierre de la puerta)*/

  /*Condicional para eliminar el rebote de la interrupción:   */
  if (millis() > contadorTiempo + TIEMPO_DE_UMBRAL) {
    contadorTiempo = millis();
    Serial.println("Se presiono el boton");

    if (botonPresionado) {
      botonPresionado = false;
    } else {
      botonPresionado = true;
      alarmaApagada = false;
    }
  }
}



void sonarAlarma() {
      tone(BOCINA, 600);   // inicia el zumbido con una frecuencia de 600 hz
      delay(200);
      noTone(BOCINA);
      delay(200);
}


