# Sistema de alarma de seguridad "Alarma ahuyentadora"

## Descripción
***
El programa acciona una alarma la cual es activada al abrir una puerta (simulado al presionar un botón) y desactivada al cerrar la puerta (simulado al presionar nuevamente el botón) y visceversa.
El consumo de un arduino es algo que es precupante y mas si esta usando un medio de alimentación como baterias, por lo que es recomendable poner un arduino en estado de bajo consumo cuando este no este realizando ninguna tarea, por lo cual, este programa procura mantener en estado de consumo bajo al arduino cuando no esté realizando ninguna tarea. 
***

## Caracteristicas
***
Mantener al arduino en estado de consumo alto mientras se encuentre activada la alarma de seguridad y 
mantenrlo en estado de consumo bajo cuando se desactive la alarma 
El programa permite de forma interactiva elegir que tono se desea usar para la alarma, se tienen 3 tonos de alarma comunes y 4 melodias de alarma cortas de canciones como ("Marcha imperial", "Seven Nation Army" , "Rocky Balboa" y "Misión imposible")
***

## Datos del Alumno
**Instituto Tecnológico de León** 

**CARRERA:** Ingeniería en Sistemas Computacionales

**Practica 2: Sistema de alarma de seguridad**

**Materia:** Sistemas Programables

**Profesor:** Ing. Levy Rojas Carlos Rafael

**Alumno:** Miguel Ángel Ramírez Lira.

***

# Prerequisitos
## Programas necesarios📋

*    El Arduino IDE el cual se puede descargar de la pagina oficial: [Pagina de Arduino](https://www.arduino.cc/en/Main/Software)  
*    Git ya sea en linux o windows en caso de querer clonar el repositorio.

### Instalación de GIT
para instalar git solo se tiene que abrir una terminal e introducir el siguiente comando
```
$ apt-get install git
```

## Como probar el programa
### clonar el repositorio
Una vez que se tenga instalado Git en nuestra computadora, se debe:
Abrir la terminal, dirigirse a un directorio donde se desee clonar el repositorio y ejecutar el siguiente comando
```
$ git clone git://github.com/mikeangelsoldier/Alarma-ahuyentadora.git
```
Y es todo ya tendras clonado el repositorio en tu directorio.

### Abrir archivo .ino desde el IDE arduino
Desde el IDE de arduino abrir el archivo alarma_de_seguridad.ino, es importante que el archivo este contenido en una carpeta con el mismo nombre ya que arduino no lo abrirá si no esta en esta forma, en este caso el archivo ya se encuentra dentro de la carpeta "alarma_de_seguridad"

Armar el circuito tal como se muestra en la seccion de **Vista del circuito armado** y con los siguientes materiales

## Materiales: 🔧

*    1 Protoboard.
*    1 Arduino UNO (o cualquier otro).
*    2 Push Button.
*    4 resistencias (330 ohms).
*    1 LED
*    1 Buzzer
*    1 Display 7 segmentos.
*    17 cables aproximadamente (7 cables para conexion con display, 2 cables largos para cada botón, 1 cable para conectar el buzzer, 1 cable para conectar el LED, cables para conectar a tierra y a voltaje los componentes)



## Diagrama del circuito.
Diagrama de circuito: 
![alt text](https://github.com/mikeangelsoldier/Alarma-ahuyentadora/blob/master/imagenes/diagrama.png "Imagen Circuito")




## Vista del circuito armado:
Este es el resultado al armar el circuito mostrado en el diagrama de arriba, se puede observar que el display muestra un número, ese número es del tono de alarma que se esta seleccionado en ese momento

Circuito Fisico prueba 1: 
![alt text](https://github.com/mikeangelsoldier/Alarma-ahuyentadora/blob/master/imagenes/imagen1.jpg "Circuito armado con tono 3 de alarma")	


Circuito Fisico prueba 2: 
![alt text](https://github.com/mikeangelsoldier/Alarma-ahuyentadora/blob/master/imagenes/imagen2.jpg "Circuito armado con tono 1 de alarma")	

***

# Conceptos técnicos
Si quieres saber como funciona el programa es necesario conocer algo de teoria para conocer que existe detrás de la magia:

## Interrupciones
Una interrupcion es una función que al ejecutarse deja a un lado la secuencia o el flujo normal de un programa para atender a lo que se encuentre dentro de esta. Existen 4 tipos de eventos que pueden generar una interrupción:
* RISING, ocurre en el flanco de subida de LOW a HIGH.
* FALLING, ocurre en el flanco de bajada de HIGH a LOW.
* CHANGING, ocurre cuando el pin cambia de estado (rising + falling).
* LOW, se ejecuta continuamente mientras está en estado LOW.

### pines que pueden generar interrupciones
  En nuestro caso el arduino UNO cuenta con 2 pines que generan interruciones:
    PIN 2 (Int 0)
    PINT 3 (Int 1)
    
   En este programa se usan esos dos pines, uno para el botón que activa la alarma y otro que permite cambiar el tono de alarma a otro.
    
    
### ISR
Es una función asociada a una interrupcion 

#### como crear una interrupción
la siguiente instruccion permite definir una funcion asociada a la interrupcion que genere el pin especificado dentro de la intrucción **digitalPinToInterrupt(pin)**, se indica despues la funcion que manejará lo que se hará una vez que se detecte la interrupción y el modo de del evento que generará la interrupción:
```
attachInterrupt(digitalPinToInterrupt(pin), ISR, mode);
```
Como ya se mencionó, en este programa las interrupciones se usaron para cambiar el tono de alarma y para activar o desactivar esta.

## Como funciona un Buzzer
Un Un zumbador es un dispositivo que  consigue generar un sonido al vibrar en la frecuencia de la señal eléctrica que recibe. El valor en frecuencia de la señal determinará el tono en el que suene el piezoeléctrico. Es un sonido de muy baja calidad pero vamos a poder generar notas y con ello música.

### Generar un sonido en un buzzer
```
tone(pin,  frecuencia)
```
esa instrucción recibe el pin donde sde debe conectar el BUZZER, cabe mencionar que se debe verificar que el pin sea de tipo PWM, el otro parametro es la frecuencia la cual puede variar dependiendo del sonido que deseemos generar.

### Como hacer música
Es muy simple, lo unico que se requiere es tener establecidos los valores de las frecuancias que generan las notas musicales de esta forma:
```
long DO=523.25, //definimos las frecuencias de las notas
        DoS=554.37,
        RE=587.33,
        RES=622.25,
        MI=659.26,
        FA=698.46,
        FAS=739.99,
        SOL=783.99,
        SOLS=830.61,
        LA=880,
        LAS=932.33,
        SI=987.77,
        RE2=1174.66,
        FAS2=1479.98,
        PAU=30000; //pausa

```
Esto nos será de gran ayuda porque lo unico de lo que nos preocuparemos sera en colocar las notas en orden, solo tendremos que poner la variable y no el valor de esta.


Enseguida debemos de colocar en un arreglo las notas en el orden correcto para que se pueda interpretar la melodia asi como colocar en otro arreglo los valores para los tiempos en los que sonará cada nota:
```
int d=1000, e=500, m=250, c=125; //definimos los tiempos de doble, entera, media y cuarta
```
```
int melodia[] = {SI, RE2, SI, RE2, PAU, SI, LA, SI, SI, SI, PAU, RE2, SI, SI, PAU, RE2, PAU, RE2, RE2, PAU, SI, RE2, SI, RE2, PAU,
SI, LA, SI, SI, SI, PAU, RE2, SI, SI, PAU, RE2, PAU, RE2, RE2, PAU, RE2, PAU, SI, RE2, PAU, SI, RE2, PAU, SI, RE2,
PAU, SI, PAU, RE2, SI, LA, SI, PAU, RE2,PAU, SI, RE2, PAU, SI, RE2, PAU, SI, RE2, PAU, SI, PAU, RE2, SI, LA, SI,
PAU, FAS2, PAU, RE2, FAS2, PAU, RE2, FAS2, PAU, RE2 ,FAS2, PAU, RE2, FAS2, PAU, RE2, LA, SOLS, SI, PAU, FAS2, PAU,
RE2, FAS2, PAU, RE2, FAS2, PAU, RE2, FAS2, PAU, RE2, FAS2, PAU, RE2, LA, SI, SI
};
```
```
int duracionNota[] = {m, m, m, m, e, c, m, m, c, m, e, m, m, m, e, m, m, m, m, d, m, m, m, m, e, c, m, m, c, m, e, m, m, m, e, m, m, m, m, d,
m, m, m, m, m, m, m, m, m, m, m, m, c, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m,
m, m, m, m, m, m, m, m, m, m, m, m, c, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m,
};
```
Ahora lo unico que hace falta es recorrer el arreglo de nuestras notas musicales a la par con el arreglo de los tiempos para interpretar la melodia, Dentro de un ciclo se recorren los arreglos y se obtienen los valores en cada posición que indican la  frecuencia y la duración de esta, y mandamos dichos valores al Buzzer para que genere los sonidos correspondientes:
```
for (int notaActual = 0; notaActual < 112; notaActual++) {  //va a repetir las 112 notas
tone(11, melodia[notaActual]); //da el tono a la frecuencia de la nota en ese momento
delay(duracionNota[notaActual]);//se mantiene con la nota el tiempo definido para esa nota
noTone(11); //finaliza la melodía
}
```
#### como crear una canción que te guste
Lo que tienes que hacer es buscar la partitura en internet, te aparecerá el nombre de cada nota, si conoces algo de música te será muy fácil identificar las notas necesarias asi como los tiempos de estas mismas, lo que tienes que hacer es crear un arreglo donde coloques en el orden necesario las notas a ejcutar y en otro arreglo los tiempos de cada nota, tal como se mostro lectura arriba, y por ultimo crear un arreglo para ejecutar la melodia.
Se deben hacer varias pruebas para colocar las notas en su tiempo correcto, poco a poco podrás acomodar los tiempos y hacer que la melodia se escuche tal como se debe de escuchar, obvio con un sonido no muy bueno, pero aún asi suena muy bien para ser solo un buzzer con el que se ejecutarán las melodias.

Si quieres conocer mas sobre como hacer música con un buzzer o mas valores de frecuencias para las notas musicales puedes encontrar buena información en la sigueinte pagina 
[Crear música con arduino usando un Buzzer](https://juegosrobotica.es/musica-con-arduino/)

***

# Autor ✒️

* **Miguel Ángel Ramírez Lira** - *Trabajo general* - [mikeangelsoldier](https://github.com/mikeangelsoldier/)


