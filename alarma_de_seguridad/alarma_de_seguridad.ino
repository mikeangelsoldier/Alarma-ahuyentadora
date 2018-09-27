/************************************************
  Programa: Sistema de simulación de alarma de seguridad

  Autor: Miguel Ángel Ramírez Lira.

  Sistema que activa una alarma mientras se mantenga una puerta abierta (la cual se simula con un boton),
  y que al ser cerrada nuevamente se apaga por si sola (si se vuelve a presionar el boton nuevamente)

 ***********************************************/

#include <LowPower.h>//añadir al libreria para poner al arduino en bajo consumo de energia

/****************               CONSTANTES                     *******************/
#define BOTON_PUERTA 3                    //Botón que simula el cierre y apertura de una puerta
#define BOTON_TONO 2                      //Botón usado para cambiar al siguiente sonido de alarma
#define BOCINA 5                          //Bocina o buzzer de alarma
#define LED_ALARMA 13                     //Led usado para encender junto con la alarma
/*Los pines que se usarán para un diplay 7 segmentos, el cual se usará para mostrar el numero del tono de alarma actual:*/
#define DISPLAY_A 12                      //pin conectado a led A del display 7 segmentos
#define DISPLAY_B 11                      //pin conectado a led B del display 7 segmentos
#define DISPLAY_C 10                      //pin conectado a led C del display 7 segmentos
#define DISPLAY_D 9                       //pin conectado a led D del display 7 segmentos
#define DISPLAY_E 8                       //pin conectado a led E del display 7 segmentos
#define DISPLAY_F 7                       //pin conectado a led F del display 7 segmentos
#define DISPLAY_G 6                       //pin conectado a led G del display 7 segmentos

const int TIEMPO_DE_UMBRAL = 100;         //Constante que se usará para eliminar el rebote de la interrupcion

/*CONSTANTES usadas para identificar las frecuencias de las notas indicadas*/
long DO2 =      65.41,                     //Valores para las Notas DO,DO#,RE,RE#,MI,FA,SOL,SOL#,LA,LA#,SI en la segunda OCTAVA
     DOS2 =     69.30,
     RE2 =      73.42,
     RES2 =     77.78,
     MI2 =      82.41,
     FA2 =      87.31,
     FAS2 =     92.50,
     SOL2 =     98.00,
     SOLS2 =    103.83,
     LA2 =      110.00,
     LAS2 =     116.54,
     SI2 =      123.47,
     DO3 =      130.81,                     //Valores para las Notas DO,DO#,RE,RE#,MI,FA,SOL,SOL#,LA,LA#,SI en la tercer OCTAVA
     DOS3 =     138.59,
     RE3 =      146.83,
     RES3 =     155.56,
     MI3 =      164.81,
     FA3 =      174.61,
     FAS3 =     185.00,
     SOL3 =     196.00,
     SOLS3 =    207.65,
     LA3 =      220.00,
     LAS3 =     233.08,
     SI3 =      246.94,
     DO4 =      261.63,                     //Valores para las Notas DO,DO#,RE,RE#,MI,FA,SOL,SOL#,LA,LA#,SI en la cuarta OCTAVA
     DOS4 =     277.18,
     RE4 =      293.67,
     RES4 =     311.13,
     MI4 =      329.63,
     FA4 =      349.23,
     FAS4 =     369.99,
     SOL4 =     392.00,
     SOLS4 =    415.31,
     LA4 =      440.00,
     LAS4 =     466.16,
     SI4 =      493.88,
     DO5 =      523.25,                     //Valores para las Notas DO,DO#,RE,RE#,MI,FA,SOL,SOL#,LA,LA#,SI en la quinta OCTAVA
     DOS5 =     554.37,
     RE5 =      587.33,
     RES5 =     622.25,
     MI5 =      659.26,
     FA5 =      698.46,
     FAS5 =     739.99,
     SOL5 =     783.99,
     SOLS5 =    830.61,
     LA5 =      880.00,
     LAS5 =     932.33,
     SI5 =      987.77,
     PAU =      30000;

const int REDONDA =       4000,                           //Valores constantes de los tiempos musicales, se usarán para los delay, para simplicar su uso
          BLANCA =        2000,
          NEGRA =         1000,
          CORCHEA =       500,
          SEMICORCHEA =   250,
          FUSA =          125,
          SEMIFUSA =      62;

/******           ARREGLOS ESTATICOS CON FRECUENCIAS Y DURACION PARA LAS MELODIAS EXTRAS      *******/
const double MARCHA_IMPERIAL[] = {                        //Notas musicales (Tono o frecuencia) en orden para la canción "Marcha Imperial"
  SOL5, PAU, SOL5, PAU, SOL5, PAU,
  RES5, PAU, LAS5, PAU, SOL5, PAU,
  RES5, PAU, LAS5, PAU, SOL5, PAU
};

int TIEMPOS_MARCHA_IMPERIAL[] = {                         //Tiempos corrrespondientes para cada nota de la  "Marcha Imperial"
  CORCHEA, FUSA - 70, CORCHEA, FUSA - 70, CORCHEA, FUSA - 70,
  SEMICORCHEA, FUSA - 70, FUSA, FUSA - 70, CORCHEA, FUSA - 70,
  SEMICORCHEA, FUSA - 70, FUSA, FUSA - 70, NEGRA, SEMICORCHEA
};

double ROCKY[] = {                                        //Notas musicales en orden para la canción "ROCKY Balboa"
  MI3, PAU, SOL3, PAU, LA3, PAU,
  LA3, PAU, SI3, PAU, MI3, PAU
};

int TIEMPOS_ROCKY[] = {                                   //Tiempos corrrespondientes para cada nota de la  canción "ROCKY Balboa"
  SEMICORCHEA, SEMIFUSA , CORCHEA, SEMIFUSA + 20, BLANCA, CORCHEA,
  SEMICORCHEA, SEMIFUSA , CORCHEA, SEMIFUSA + 20, BLANCA, CORCHEA
};

double MISION_IMPOSIBLE[] = {                             //Notas musicales (Tono o frecuencia) en orden para la canción "Misión Impoible"
  MI3, PAU, MI3, PAU, FAS3, PAU, LA3,
  MI3, PAU, MI3, PAU, RE3, PAU, RES3,
  MI3, PAU, MI3, PAU, FAS3, PAU, LA3,
  MI3, PAU, MI3, PAU, RE3, PAU, RES3, PAU,
  SI3, PAU, LA3, PAU, SOL3, PAU,
  LA3, PAU, SOL3, PAU, FA3, PAU,
  SOL3, PAU, FA3, PAU, MI3, PAU, RE3, PAU, MI3, PAU
};

int TIEMPOS_MISION_IMPOSIBLE[] = {                      //Tiempos corrrespondientes para cada nota de la  canción "Misión Impoible"
  CORCHEA, FUSA, SEMICORCHEA, SEMICORCHEA, SEMICORCHEA + 20, FUSA - 50, CORCHEA,
  CORCHEA, FUSA, SEMICORCHEA, SEMICORCHEA, SEMICORCHEA + 20, FUSA - 50, CORCHEA,
  CORCHEA, FUSA, SEMICORCHEA, SEMICORCHEA, SEMICORCHEA + 20, FUSA - 50, CORCHEA,
  CORCHEA, FUSA, SEMICORCHEA, SEMICORCHEA, SEMICORCHEA + 20, FUSA - 50, CORCHEA, FUSA,
  FUSA, FUSA - 70, FUSA, FUSA - 70, NEGRA, FUSA + 20,
  FUSA, FUSA - 70, FUSA, FUSA - 70, NEGRA, FUSA + 20,
  FUSA, FUSA - 70, FUSA, FUSA - 70, NEGRA, FUSA + 40,      FUSA, FUSA - 30, FUSA, CORCHEA
};

double SEVEN_NATION_ARMY[] = {                          //Notas musicales (Tono o frecuencia) en orden para la canción "Seven Nation Army (de The White Stripes)"
  MI3, PAU, MI3, PAU, SOL3, PAU, MI3, PAU, RE3, PAU, DO3, PAU, SI2, PAU,
  MI3, PAU, MI3, PAU, SOL3, PAU, MI3, PAU, RE3, PAU, DO3, PAU, RE3, PAU, DO3, PAU, SI2, PAU
};

int TIEMPOS_SEVEN_NATION_ARMY[] = {                     //Tiempos corrrespondientes para cada nota de la canción "Seven Nation Army"
  NEGRA, SEMIFUSA + 10, SEMICORCHEA, SEMIFUSA + 20, SEMICORCHEA, SEMIFUSA + 20, SEMICORCHEA + 50,
  SEMIFUSA + 30, SEMICORCHEA, FUSA, NEGRA, SEMIFUSA + 20, NEGRA, CORCHEA,
  NEGRA, SEMIFUSA + 10, SEMICORCHEA, SEMIFUSA + 20, SEMICORCHEA, SEMIFUSA + 20, SEMICORCHEA + 50,
  SEMIFUSA + 30, SEMICORCHEA, FUSA, SEMICORCHEA, SEMIFUSA + 20, SEMICORCHEA, SEMIFUSA + 20, SEMICORCHEA, SEMIFUSA + 20, NEGRA, CORCHEA
};


/****************         VARIABLES         *******************/
long contadorTiempo = 0;                    //Variable usada en la interrupcion para almacenar el valor de millis()
boolean alarmaApagada = true;               //variable que cambia de estado cada que se presiona el boton
int frecuencia = 0;                         //Variable usada para guardar de grados a radianes, obtenido con funcion SENO
double valorSeno = 0;                       //Variable usada para guardar la frecuencia resultante de la suma con la variable frecuencia
volatile boolean botonPresionado = false;   // Variable usada por la interrupción "interrupcionAlarma"
volatile int tonoActual = 1;                // Variable usada por la interrupción "interrupcionTono"


void setup() {

  //modo de los pines necesarios
  pinMode(BOTON_PUERTA, INPUT_PULLUP);
  pinMode(BOTON_TONO, INPUT_PULLUP);
  pinMode(LED_ALARMA, OUTPUT);
  pinMode(DISPLAY_A, OUTPUT);
  pinMode(DISPLAY_B, OUTPUT);
  pinMode(DISPLAY_C, OUTPUT);
  pinMode(DISPLAY_D, OUTPUT);
  pinMode(DISPLAY_E, OUTPUT);
  pinMode(DISPLAY_F, OUTPUT);
  pinMode(DISPLAY_G, OUTPUT);
  
  //  Las interrupciones a utilizar:
  attachInterrupt(digitalPinToInterrupt(BOTON_PUERTA), interrupcionAlarma, RISING);   //rising es el modo de disparo que activara la interrupcion
  attachInterrupt(digitalPinToInterrupt(BOTON_TONO),interrupcionTono , RISING);   //rising es el modo de disparo que activara la interrupcion
  
  Serial.begin(9600);

  
}

void loop() {


  if (botonPresionado) {//La variable cambia a TRUE y FALSE cada que se presiona el BOTON_PUERTA para simular la apertura y cierre de la puerta
    switch (tonoActual) {
      case 1:
        mostrarNumTono(0, 1, 1, 0, 0, 0, 0);          //si se reproduce tono 1 se envian señales a los segmentos del display para mostrar el numero 1
        tonoAlarma(1);
        break;
      case 2:
        mostrarNumTono(1, 1, 0, 1, 1, 0, 1);          //si se reproduce tono 2 se envian señales a los segmentos del display para mostrar el numero 2
        tonoAlarma(2);
        break;
      case 3:
        mostrarNumTono(1, 1, 1, 1, 0, 0, 1);          //si se reproduce tono 3 se envian señales a los segmentos del display para mostrar el numero 3
        tonoAlarma(3);
        break;
      case 4:
        mostrarNumTono(0, 1, 1, 0, 0, 1, 1);          //si se reproduce tono 4 se envian señales a los segmentos del display para mostrar el numero 4
        melodiaAlarma(ROCKY, TIEMPOS_ROCKY, 12);       //se indica la melodia para alarma, con sus notas, la duración (tiempos) de cada nota y el tamaño de melodia)
        break;
      case 5:
        mostrarNumTono(1, 0, 1, 1, 0, 1, 1);          //si se reproduce tono 5 se envian señales a los segmentos del display para mostrar el numero 5
        melodiaAlarma(MARCHA_IMPERIAL, TIEMPOS_MARCHA_IMPERIAL, 18);   //se indica la melodia para alarma, con sus notas, la duración (tiempos) de cada nota y el tamaño de melodia)
        break;
      case 6:
        mostrarNumTono(1, 0, 1, 1, 1, 1, 1);          //si se reproduce tono 6 se envian señales a los segmentos del display para mostrar el numero 6
        melodiaAlarma(MISION_IMPOSIBLE, TIEMPOS_MISION_IMPOSIBLE, 51);//se indica la melodia para alarma, con sus notas, la duración (tiempos) de cada nota y el tamaño de melodia)
        break;
      case 7:
        mostrarNumTono(1, 1, 1, 0, 0, 0, 0);          //si se reproduce tono 7 se envian señales a los segmentos del display para mostrar el numero 7
        melodiaAlarma(SEVEN_NATION_ARMY, TIEMPOS_SEVEN_NATION_ARMY, 32);//se indica la melodia para alarma, con sus notas, la duración (tiempos) de cada nota y el tamaño de melodia)
        break;
    }
  } else {                                            //Si el botón se vuelve a presionar se debe de apagar la alarma y los LED del display 7 segmentos
    //puede darse el caso que un tono de alarma sea muy largo y por eso se debe de esperar a que termine el sonido para reproducir tono que fin:
    if (!alarmaApagada) {                         //Solo si la alarma no se ha apagado aun reproducirá el tono de apagado
      tone(BOCINA, 600.25, 250);
      delay(250);
      noTone(BOCINA);
      delay(300);
      tone(BOCINA, 987.77, 400);
      delay(400);
      noTone(BOCINA);
      alarmaApagada = true;                             //Indicamos que ya alarma termino de reproducirse o escucharse
      mostrarNumTono(0, 0, 0, 0, 0, 0, 0);
      LowPower.powerDown(SLEEP_FOREVER,ADC_OFF,BOD_OFF);// coloca al arduino en bajo consumo de energia, vuelve a estado normal con una interrupción
    }

    //apagamos cualquier sonido que pudiera haber continuado asi como apagar el display:
    noTone(BOCINA);
    digitalWrite(LED_ALARMA, LOW);
    mostrarNumTono(0, 0, 0, 0, 0, 0, 0);
    
  }
}

void interrupcionAlarma() {
  /*Este método es usado cuando se activa la interrupcion al presionar el BOTON_PUERTA para activar la alarma
    (El botón simula que se abre una puerta al presionarlo, y nuevamente al volver a presionarlo simula el cierre de la puerta)*/
    

  /*Condicional para eliminar el rebote de la interrupción:   */
  if (millis() > contadorTiempo + TIEMPO_DE_UMBRAL) {
    contadorTiempo = millis();
    Serial.println("Se presiono el boton");

    //se hace un cambio del valor de variable botonPresionado (de true a false y visceversa)
    if (botonPresionado) {
      botonPresionado = false;
    } else {
      botonPresionado = true;
      alarmaApagada = false;
    }
  }
}

void interrupcionTono() {
  /*Este metodo se usara cuando se active la interrupcion al presionar el BOTON_TONO
  Se incrementa el valor de la variable tonoActual cada que se presiona BOTON_TONO, 
  esto para indicar el siguiente tono a reproducir
  Solo hay 7 tonos disponibles (3 alarmas y 4 melodias), 
  cuando llega al valor 7 se asigna el valor 1 para volver a continuar con el primer tono e ir pasando al siguiente tono
  */
  if (millis() > contadorTiempo + TIEMPO_DE_UMBRAL) {
    contadorTiempo = millis();
    Serial.println("Se presiono el boton de tono");

    if (tonoActual == 7) {
      tonoActual = 1;
    } else {
      tonoActual++;
    }
  }
}

void tonoAlarma(int numAlarma) {
  /*Método usado para indicar cual tono de alarma se reproducirá,
  se recibe la variable numAlarma la cual indica el tono a reproducir, 
  cada tono tiene diferente frecuencia y duración establecida
  */
  
  switch (numAlarma) {
    case 1:                       //reproducir tono 1 para alarma
      tone(BOCINA, 1000);   
      digitalWrite(LED_ALARMA, HIGH);
      delay(200);
      noTone(BOCINA);
      digitalWrite(LED_ALARMA, LOW);
      delay(200);
      break;
    case 2:                       //reproducir tono 2 para alarma
      tone(BOCINA, 1000);   
      digitalWrite(LED_ALARMA, HIGH);
      delay(200);
      noTone(BOCINA);
      digitalWrite(LED_ALARMA, LOW);
      delay(200);
      tone(BOCINA, 800);  
      digitalWrite(LED_ALARMA, HIGH);
      delay(100);
      noTone(BOCINA);
      digitalWrite(LED_ALARMA, LOW);
      delay(100);
      tone(BOCINA, 200);   
      digitalWrite(LED_ALARMA, HIGH);
      delay(200);
      noTone(BOCINA);
      digitalWrite(LED_ALARMA, LOW);
      delay(200);
      break;
    case 3:                       //reproducir tono 3 para alarma
      for (int i = 0; i < 180; i++) {
        //pasar de grados a radianes
        valorSeno = (sin(i * (3.1412 / 180)));

        //generar Tono o frecuencia
        frecuencia = 10 + (int(valorSeno * 1000));
        tone(BOCINA, frecuencia);
        delay(2);
      }
      break;
  }
}


void melodiaAlarma(double notas[], int tiempos[], int tamano) {
  /*Método usado para indicar cual melodia de alarma se reproducirá,
  se reciben los arreglos de notas [] (las frecuencias), 
  los arreglos de los tiempos[] (que corresponden a cada nota), 
  y el total de notas a reproducir (tamaño de la melodia).
  Los arreglos de notas[] y tiempos [] estan relacionados 1 a 1, es decir a cada nota le correspondie un tiempo del otro arreglo
  Este método recorrre ambos arreglos, reproduciendo en la bocina la frecuencia indicada con el tiempo indicado ().
  */
  int tamanoMelodia = tamano;

  for (int notaActual = 0; notaActual < tamanoMelodia; notaActual++) {  //va a repetir las n notas
    tone(BOCINA, notas[notaActual]);        //da el tono a la frecuencia de la nota en ese momento
    delay(tiempos[notaActual]);             //se mantiene con la nota el tiempo definido para esa nota
    noTone(BOCINA);                         //finaliza esa nota para reproducir la sig nota de la melodia
  }
}


void mostrarNumTono(int a, int b, int c, int d, int e, int f, int g) {
  /*
  Este método se utiliza para pasar los valores '1' o '0' en cada parametro recibido,
  los parametros corresponden a las conexiones a,b,c,d,e,f,g del display de 7 segmentos
  Se se recibe un valor entero '1' se mandará una señal baja a el pin que conecta a la conexion indicada en el display
  Se se recibe un valor entero '0' se mandará una señal alta a el pin que conecta a la conexion indicada en el display
  Esto se verifica para cada parametro recibido en ese orden, lo cual permite encender los segmentos 
  de tal forma que se vizualiza el número decimal en el display el que indica el tono usado como alarma en el momento
  (son 7 tonos en total que se pueden usar uno a la vez como alarma (3 tonos y 4 melodias como alarmas))
  */
  if (a == 1) {                 //comprobar que señal enviar al segmento a del display;
    digitalWrite(DISPLAY_A, LOW);
  } else {
    digitalWrite(DISPLAY_A, HIGH);
  }

  if (b == 1) {                 //comprobar que señal enviar al segmento b del display;
    digitalWrite(DISPLAY_B, LOW);
  } else {
    digitalWrite(DISPLAY_B, HIGH);
  }

  if (c == 1) {                 //comprobar que señal enviar al segmento c del display;
    digitalWrite(DISPLAY_C, LOW);
  } else {
    digitalWrite(DISPLAY_C, HIGH);
  }

  if (d == 1) {                 //comprobar que señal enviar al segmento d del display;
    digitalWrite(DISPLAY_D, LOW);
  } else {
    digitalWrite(DISPLAY_D, HIGH);
  }

  if (e == 1) {                 //comprobar que señal enviar al segmento e del display;
    digitalWrite(DISPLAY_E, LOW);
  } else {
    digitalWrite(DISPLAY_E, HIGH);
  }

  if (f == 1) {                 //comprobar que señal enviar al segmento f del display;
    digitalWrite(DISPLAY_F, LOW);
  } else {
    digitalWrite(DISPLAY_F, HIGH);
  }

  if (g == 1) {                 //comprobar que señal enviar al segmento g del display;
    digitalWrite(DISPLAY_G, LOW);
  } else {
    digitalWrite(DISPLAY_G, HIGH);
  }

}






