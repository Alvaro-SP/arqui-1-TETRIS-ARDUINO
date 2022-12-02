//*  ░██████╗██╗░░░░░░█████╗░██╗░░░██╗███████╗
//*  ██╔════╝██║░░░░░██╔══██╗██║░░░██║██╔════╝
//*  ╚█████╗░██║░░░░░███████║╚██╗░██╔╝█████╗░░
//*  ░╚═══██╗██║░░░░░██╔══██║░╚████╔╝░██╔══╝░░
//*  ██████╔╝███████╗██║░░██║░░╚██╔╝░░███████╗
//*  ╚═════╝░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚══════╝

//TODO: - Mover el automóvil
//TODO: - Detección de obstáculos
//TODO: - Detección de colores

//* PROGRAMACION DE MODO MANUAL
/*  Al entrar en este modo, se debe mostrar un mensaje en la pantalla LCD durante 5 segundos 
    que se está iniciando el modo manual. En este modo el automóvil recibirá todas sus 
    instrucciones desde el centro de control, el número de instrucciones que recibirá queda a 
    discreción del usuario, pero no puede ser menos de 20 movimientos, así como también el 
    tiempo que se moverá hacia delante, hacia atrás y los grados que girara hacia la derecha e 
    izquierda, teniendo en cuenta que los movimientos hacia adelante y atrás deben durar al 
    menos 10 segundos y debe mostrarse en la pantalla LCD el movimiento que hace y los 
    movimientos restantes, este modo debe correrse dentro de las paredes exteriores del laberinto. 
    Este modo deberá de detectar cuando encuentre un obstáculo y pausar sus movimientos, el 
    automóvil únicamente puede salir de esta pausa al presionar un botón definido por el grupo y 
    detectar que no tenga ningún obstáculo adelante para seguir con su movimiento hacia 
    adelante, para detectar un obstáculo en su movimiento hacia atrás se deberá usar un sensor de 
    tope o fin de carrera el cual detectara el obstáculo y entrara en el mismo estado de pausa, del 
    cual se podrá salir al igual que como se mencionó anteriormente presionando un botón 
    especifico y que el sensor del obstáculo de atrás no este activo. Cuando se encuentre en el 
    estado de pausa se debe mostrar un mensaje en la pantalla LCD indicándolo. También deberá
    de leer los colores del suelo y al momento de ser leído deberá mostrarse en la pantalla LCD el 
    color que se leyó. Al finalizar todos los movimientos guardados debe de mostrarse un mensaje 
    indicándole al usuario que ha finalizado la secuencia.
*/
#include <Wire.h>

//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀ VARIABLES MODO MANUAL  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

//* ░░░░░░░░░░░░░░░░░░░░░░░░░░ MOVIMIENTOS ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
int MOVEMENTS[20] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
//* -░░░░░░░░░░░░░░░░░░░░░░░░    MOTORES   ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
int IZQUIERDO_F = 12; //motor izquierdo
int IZQUIERDO_B = 11; //motor izquierdo
int DERECHO_F = 22; //motor derecho
int DERECHO_B = 23; //motor derecho
int FLAGPOSICION=-1;
//* ░░░░░░░░░░░░░░░░░░░░░░░░░ ULTRASONICOS ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
int OJOSFRONT_T = 2; //FRONTAL TIENE SU TRIGGER Y SU ECHO
int OJOSFRONT_E = 3;
int OJOSBACK_T = 4;  //DE ATRAS TIENE SU TRIGGER Y SU ECHO
int OJOSBACK_E = 5;

int DISTANCIAFRONT = 0;  // distancia que hay de un objeto al frente del carro
int DISTANCIABACK = 0;    // distancia que hay de un objeto atras del carro

//* ░░░░░░░░░░░░░░░░░░░░░░ SENSOR DE COLOR ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//! Pines para el sensor de color RGB
const int S0 = 6;
const int S1 = 7;
const int S2 = 8;
const int S3 = 9;
const int salidaTCS = 10;

// *Variables necesarias para los pulsos de salida del sensor
int redPulse;    //Rojo
int greenPulse;  //Verde
int bluePulse;   //Azul
// *Contadores para las repeticiones de los colores, se deben mostrar desde el maestro
// *Banderas que se utilizan para saber el color que esta activo en una fila del laberinto
int contColores[4] = {0,0,0,0};        // rosado    negro   verdeOscuro   Gris    Cafe    amarillo     azul   naranja   celeste   verdeClaro
//! avanzar = 0
//! retroceder = 1
//! izquierda = 2
//! derecha = 3
int listaPosicion1[30] = {0, 2, 0, 2, 2, 0, 3, 0, 2, 3, 3, 0,3,0,2,0,3,0,2,2,0,2,0,3,3,0,2,2,0,3}; //Rojo 
int listaPosicion2[19] = {0,3,0,2,0,3,0,2,2,0,2,0,3,3,0,2,2,0,3}; //Verde
int listaPosicion3[10] = {0,2,0,3,3,0,2,2,0,3}; //Azul
  //* 0    1      2     3
  //* rojo verde  azul  negro
int listaTiempo1[30] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int listaTiempo2[19] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int listaTiempo3[10] = {0,0,0,0,0,0,0,0,0,0};   
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀ ENVIO AL MAESTRO CONSTANTEMENTE  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
unsigned long tiempoInicio;//tiempo en que muestra punteo luego de perder
//* ░░░░░░░░░░░░░░░░░░░░░░ MANUAL ░░░░░░░░░░░░░░░░░░░░░░
int ENVIOvecesObstaculo = 0; // VECES QUE ENCUENTRA UN OBSTACULO
unsigned long ENVIOtimeCommands = 0; // TIEMPO PARA EJECUTAR COMANDOS
int FINMOVEMENTS =0; // 0 no terminado     1 terminado

//* ░░░░░░░░░░░░░░░░░░░░░░ AUTOMATICO ░░░░░░░░░░░░░░░░░░░░░░
int CantTopes = 0;


//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀ VARIABLES MODO AUTOMATICO  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
int PAST = -1;
int FUTURE = -1;
<<<<<<< HEAD
int PASTAUTO = -1;
int FUTUREAUTO = -1;



=======
// int PASTAUTO = -1;
// int FUTUREAUTO = -1;
int PRESENTCOLOR = -1;
int FLAGFIRSTPOSITION = 0;

int movIndex = 0;
>>>>>>> ba92bc76f2b474629ae4947311b65bea5de013b2


//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀ VARIABLES FLAGS Y CONTADORES ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
int PAUSARESUME = 0; // 0 = resume   1 = pausa //  al iniciar no hay cambios sigue en resume
int flagModo = -1;
int t;
unsigned long startTime; //tiempo auxiliar
int flagMoviendose = 0; // 0 no se mueve
//? variables para estadisticas
int ContVecesGiro = 0; // veces en que va a girar el carro izquierda o derecha.
int TimeofMove = 0;// tiempo en que el carro permanece en movimiento
int PasilloSinSalida = 0; // veces en que entra a pasillo sin salida


void setup() {
  startTime= millis();
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(0x01);                //*SE IDENTIFICA COMO ESCLAVO 1, COMO SE DECLARO EN EL MAESTRO
  Wire.onReceive(evento);   //* sirve para setear que va a recibir desde el maestro.
  Wire.onRequest(Peticion);
  pinMode(IZQUIERDO_F,OUTPUT);
  pinMode(IZQUIERDO_B, OUTPUT);
  pinMode(DERECHO_F,OUTPUT);
  pinMode(DERECHO_B, OUTPUT);
  pinMode(OJOSFRONT_T, OUTPUT);
  pinMode(OJOSFRONT_E, INPUT);
  pinMode(OJOSBACK_T, OUTPUT);
  pinMode(OJOSBACK_E, INPUT);
  pinMode(DISTANCIAFRONT, INPUT);
  pinMode(salidaTCS,INPUT);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,HIGH);
}

//! ██████████████████ SENSORES ULTRASONICOS ██████████████████
//!CONTROLA EL SENSOR ULTRASONICO FRONTAL
void UltraFront(){
  int d = 0;
  digitalWrite(OJOSFRONT_T, LOW);  //* ESTOS digitalWrites practicamente lo que hacen es:
  delayMicroseconds(2);              //* tomar senales del sensor.
  digitalWrite(OJOSFRONT_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(OJOSFRONT_T, LOW);
  t = pulseIn(OJOSFRONT_E, HIGH);
  d = round(t*0.01715); //* ESTA ES LA DISTANCIA QUE HAY ENTRE UN OBJETO HASTA EL CARRO
  //TODO: esta distancia es de ir probando sino queda muy lejos el carro
  if (d<=5){   //* PARA ESTE CASO VALIDO QUE SEA MENOR A 5 CM
    DISTANCIAFRONT = d; //*ASIGNO A VARIABLE GLOBAL DEL FRENTE POR SI ES MENOR A 13
    // EN ESTE MOMENTO DEBE PAUSAR EL CARRO NO IMPORTANDO EN QUE ESTADO ESTE
    // SINO CHOCA Y BOOM XD
    //CarStop();
    ENVIOvecesObstaculo++;
  }else{
    DISTANCIAFRONT = 0; // * SI LA DISTANCIA ES MAYOR ENTONCES 0 CLAVOS Y PODEMOS CONTINUAR
  }
}

void UltraBack(){
  int d = 0;
  digitalWrite(OJOSBACK_T, LOW);  //* ESTOS digitalWrites practicamente lo que hacen es:
  delayMicroseconds(2);              //* tomar senales del sensor.
  digitalWrite(OJOSBACK_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(OJOSBACK_T, LOW);
  t = pulseIn(OJOSBACK_E, HIGH);
  d = round(t*0.01715); //* ESTA ES LA DISTANCIA QUE HAY ENTRE UN OBJETO HASTA EL CARRO
  //TODO: esta distancia es de ir probando sino queda muy lejos el carro
  if (d<=5){   //* PARA ESTE CASO VALIDO QUE SEA MENOR A 5 CM
    DISTANCIAFRONT = d; //*ASIGNO A VARIABLE GLOBAL DEL FRENTE POR SI ES MENOR A 13
    // EN ESTE MOMENTO DEBE PAUSAR EL CARRO NO IMPORTANDO EN QUE ESTADO ESTE
    // SINO CHOCA Y BOOM XD
    //CarStop();
  }else{
    DISTANCIAFRONT = 0; // * SI LA DISTANCIA ES MAYOR ENTONCES 0 CLAVOS Y PODEMOS CONTINUAR
  }
}


//!██████████████████ METODOS DE COLORES ██████████████████

//Función para comparar los pulsos y determinar el tipo de color que es
void colorType(){
  if((redPulse>=75 && redPulse<=93) && (greenPulse>=222 && greenPulse<=232) && (bluePulse>=140 && bluePulse<=160)){
    Serial.println("ROJO");
    contColores[0] = contColores[0] + 1;
<<<<<<< HEAD

    PASTAUTO = FUTUREAUTO;
    FUTUREAUTO = 1;
=======
    // FUTUREAUTO = 2;
    // PASTAUTO = 3;
    PRESENTCOLOR = 1;
>>>>>>> ba92bc76f2b474629ae4947311b65bea5de013b2
  }
  if((redPulse>=248 && redPulse<=260) && (greenPulse>=210 && greenPulse<=220) && (bluePulse>=175 && bluePulse<=187)){
    Serial.println("VERDE");
    contColores[1] = contColores[1] + 1;
<<<<<<< HEAD

    PASTAUTO = FUTUREAUTO;
    FUTUREAUTO = 2;
=======
    // FUTUREAUTO = 3;
    // PASTAUTO = 1;
    PRESENTCOLOR = 2;
>>>>>>> ba92bc76f2b474629ae4947311b65bea5de013b2
  }
  if((redPulse>=194 && redPulse<=245) && (greenPulse>=170 && greenPulse<=195) && (bluePulse>=80 && bluePulse<=115)){
    Serial.println("AZUL");
    contColores[2] = contColores[2] + 1;
<<<<<<< HEAD
    FUTURE = 1;

    PASTAUTO = FUTUREAUTO;
    FUTUREAUTO = 3;
=======
    // PASTAUTO = 2;
    // FUTUREAUTO = 1;
    PRESENTCOLOR = 3;
>>>>>>> ba92bc76f2b474629ae4947311b65bea5de013b2
  }
  if((redPulse>=325 && redPulse<=340) && (greenPulse>=402 && greenPulse<=425) && (bluePulse>=280 && bluePulse<=300)){
    Serial.println("NEGRO");
    contColores[3] = contColores[3] + 1;
    PAST = 1 ;
  }




/*Rojo    -> verde   ->    azul  ->
   1          2             3
  p = 3      p = 1        p = 2
  f = 2      f=3          f=1
  
  
  //!     1 ->   3
        if()
  */



}
//Establecer fotodiodos para cada uno de los filtros

void fotodiodos(){
  //Para el color rojo
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  redPulse = pulseIn(salidaTCS, LOW);
  Serial.print(" R= "); Serial.print(redPulse);
  //644
  delay(100);

  //Para el color verde
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  greenPulse = pulseIn(salidaTCS, LOW);
  Serial.print(" V= "); Serial.print(greenPulse);
  delay(100);

  //Para el color azul
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  bluePulse = pulseIn(salidaTCS, LOW);
  Serial.print(" A= "); Serial.print(bluePulse);
  delay(100);

  colorType();
}

//!██████████████████ MOVIMIENTO CARRO ██████████████████
void PausarMovimientos(){
  ENVIOtimeCommands=ENVIOtimeCommands+20;
  // interrupts();
  while(digitalRead(PAUSARESUME)!=LOW){ // al detectar un LOW sale y ...
    //no hace nada :v
    Serial.println("Aki se queda modo tieso (PAUSA) xd");
  }
  // noInterrupts();
}
int tiempoAvanza = 25000;
// void CarAvanza(){
//     ENVIOtimeCommands=ENVIOtimeCommands+10;
//     Serial.println("AVANZANDO Manual");

<<<<<<< HEAD

void CarStop(){
  //! SE APAGAN LOS MOTORES CUANDO EL ULTRASONICO DEL FRENTE DETECTE OBJETO
  digitalWrite(DERECHO_B, LOW);   //motor DC derecho
  digitalWrite(DERECHO_F, LOW);
  digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
  digitalWrite(IZQUIERDO_F, LOW);
}


void CarAvanza(){
  // * millis () = da el tiempo en milisegundos desde que se encendió la tarjeta Arduino
  // * startTime + 10000  =  los 10 segundos que se debe avanzar
  // * millis() = para el caso es el tiempo que va pasando antes de los 10 segundos futuros
  //? mientras mi marca de tiempo + 10 segundos sea mayor al tiempo que va pasando seguira avanzando el carro
  while(startTime+10000 > millis()){ // osea cuando millis llegue a los 10 segundos ya no avanza
    // ACTIVO ambos motores hacia adelante
    digitalWrite(DERECHO_B, LOW);   //motor DC derecho
    digitalWrite(DERECHO_F, HIGH);
    digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
    digitalWrite(IZQUIERDO_F, HIGH);
    //! AL DETECTAR UN OBJETO ENTONCES VA AL ESTADO DE PAUSA:
    if(DISTANCIAFRONT!=0){
      flagMoviendose=0; // lo detengo porque ya no se mueve
      PausarMovimientos();
    }
  }
  CarStop();
  startTime = 0;
=======
//     startTime=millis();
//     tiempoAvanza = millis()+250000;

//     while(startTime < tiempoAvanza){
//       Serial.println(startTime);
//       Serial.println(millis()-startTime);
//       digitalWrite(DERECHO_B, LOW);   //motor DC derecho
//       digitalWrite(DERECHO_F, HIGH);
//       digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
//       digitalWrite(IZQUIERDO_F, HIGH);
//       startTime = startTime+ 1000;
//     }

//     //Serial.println("Entra al else");
//     digitalWrite(DERECHO_B, LOW);   //motor DC derecho
//     digitalWrite(DERECHO_F, LOW);
//     digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
//     digitalWrite(IZQUIERDO_F, LOW);
//     //! AL DETECTAR UN OBJETO ENTONCES VA AL ESTADO DE PAUSA:

// }

// void CarRetrocede(){
//   ENVIOtimeCommands=ENVIOtimeCommands+10;
//   Serial.println("RETROCEDIENDO Manual");
//   while(startTime < millis()+250000){
//     // ACTIVO ambos motores hacia atras
//     digitalWrite(DERECHO_B, HIGH);   //motor DC derecho
//     digitalWrite(DERECHO_F, LOW);
//     digitalWrite(IZQUIERDO_B, HIGH); //motor DC izquierdo
//     digitalWrite(IZQUIERDO_F, LOW);
//     startTime = startTime+ 1000;
//   }
//   digitalWrite(DERECHO_B, LOW);   //motor DC derecho
//   digitalWrite(DERECHO_F, LOW);
//   digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
//   digitalWrite(IZQUIERDO_F, LOW);
//   //startTime = 0;
// }

// void CarIzquierda(){
//   ENVIOtimeCommands=ENVIOtimeCommands+2.8;
//   Serial.println("GIRO IZQUIERDA Manual");
//   while(startTime < millis()+110000){
//     // para la izquierda DESACTIVO motor izquierdo y activo adelante motor derecho
//     digitalWrite(DERECHO_B, LOW);   //motor DC derecho
//     digitalWrite(DERECHO_F, LOW);
//     digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
//     digitalWrite(IZQUIERDO_F, HIGH);
//     startTime = startTime+ 1000;
//   }
//   digitalWrite(DERECHO_B, LOW);   //motor DC derecho
//   digitalWrite(DERECHO_F, LOW);
//   digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
//   digitalWrite(IZQUIERDO_F, LOW);
// }

// void CarDerecha(){
//   ENVIOtimeCommands=ENVIOtimeCommands+2.8;
//   Serial.println("GIRO DERECHA Manual");
//   // para la derecha DESACTIVO motor derecho y activo adelante motor izquierdo
//   while(startTime < millis()+110000){
//     // para la izquierda DESACTIVO motor izquierdo y activo adelante motor derecho
//     digitalWrite(DERECHO_B, LOW);   //motor DC derecho
//     digitalWrite(DERECHO_F, HIGH);
//     digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
//     digitalWrite(IZQUIERDO_F, LOW);
//     startTime = startTime+ 1000;
//   }
//   digitalWrite(DERECHO_B, LOW);   //motor DC derecho
//   digitalWrite(DERECHO_F, LOW);
//   digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
//   digitalWrite(IZQUIERDO_F, LOW);
// }


// tiempoInicio=millis();
int AVANZAINS = 0;
int RETROCEDEINS = 0;
int IZQUIERDAINS = 0;
int DERECHAINS = 0;
//? █▀▄▀█ █▀█ █▀▄ █▀█   █▀▄▀█ ▄▀█ █▄░█ █░█ ▄▀█ █░░
//? █░▀░█ █▄█ █▄▀ █▄█   █░▀░█ █▀█ █░▀█ █▄█ █▀█ █▄▄
//!██████████████████ EJECUCION DE 20 MOVIMIENTOS ██████████████████
// void gotoMovements(){
//   FLAGPOSICION = 0;
  //* ACTIVO NOINTERRUPTS PARA CUANDO SE VAYA A EJECUTAR LAS INSTRUCCIONES NO HAYAN CLAVOS CON LOS BOTONES 
  //noInterrupts();
  // for(int i = 0; i<20;i++){ //solo son 20 movements
  //   switch (MOVEMENTS[i])
  //   {
  //     movIndex = i;
  //     case 0:
  //         tiempoInicio=millis();
  //         AVANZAINS = 1;
  //         delay(12000);
  //         break;
  //     case 1:
  //         //startTime= millis();
  //         // Serial.print("Car Retrocede");
  //         // CarRetrocede();
  //         // delay(3000);
  //         tiempoInicio=millis();
  //         RETROCEDEINS = 1;
  //         delay(12000);
  //         break;
  //     case 2:
  //         //startTime= millis();
  //         // Serial.print("Car Izq");
  //         // CarIzquierda();
  //         // delay(3000);
  //         tiempoInicio=millis();
  //         IZQUIERDAINS = 1;
  //         delay(12000);
  //         break;
  //     case 3:
  //         //startTime= millis();
  //         // Serial.print("Car Der");
  //         // CarDerecha();
  //         // delay(3000);
  //         tiempoInicio=millis();
  //         DERECHAINS = 1;
  //         delay(12000);
  //         break;
  //   }
  
  
  // }
  
  
  //! TERMINAN LOS 20 MOVIMIENTOS

  //* DESACTIVO NOINTERRUPTS PARA REGRESAR A NORMALIDAD
  //interrupts();
// }


//? █▀▄▀█ █▀█ █▀▄ █▀█   ▄▀█ █░█ ▀█▀ █▀█ █▀▄▀█ ▄▀█ ▀█▀ █ █▀▀ █▀█
//? █░▀░█ █▄█ █▄▀ █▄█   █▀█ █▄█ ░█░ █▄█ █░▀░█ █▀█ ░█░ █ █▄▄ █▄█




void CarAvanzaAUTO(int time){
  ENVIOtimeCommands=ENVIOtimeCommands+10;
    Serial.println("AVANZANDO AUTOMATICO");
    while(startTime < millis()+time){
      Serial.println(startTime);
      Serial.println(millis()-startTime);
      digitalWrite(DERECHO_B, LOW);   //motor DC derecho
      digitalWrite(DERECHO_F, HIGH);
      digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
      digitalWrite(IZQUIERDO_F, HIGH);
      startTime = startTime+ 1000;
    }
    //Serial.println("Entra al else");
    digitalWrite(DERECHO_B, LOW);   //motor DC derecho
    digitalWrite(DERECHO_F, LOW);
    digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
    digitalWrite(IZQUIERDO_F, LOW);
    //! AL DETECTAR UN OBJETO ENTONCES VA AL ESTADO DE PAUSA:
>>>>>>> ba92bc76f2b474629ae4947311b65bea5de013b2

}

void CarRetrocedeAUTO(int time){
  ENVIOtimeCommands=ENVIOtimeCommands+10;
  Serial.println("RETROCEDIENDO AUTOMATICO");
  while(startTime < millis()+time){
    // ACTIVO ambos motores hacia atras
    digitalWrite(DERECHO_B, HIGH);   //motor DC derecho
    digitalWrite(DERECHO_F, LOW);
    digitalWrite(IZQUIERDO_B, HIGH); //motor DC izquierdo
    digitalWrite(IZQUIERDO_F, LOW);
    startTime = startTime+ 1000;
  }
<<<<<<< HEAD
  CarStop();
  startTime = 0;
}

void CarIzquierda(){
  while(startTime+2500 > millis()){
=======
  digitalWrite(DERECHO_B, LOW);   //motor DC derecho
  digitalWrite(DERECHO_F, LOW);
  digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
  digitalWrite(IZQUIERDO_F, LOW);
  //startTime = 0;
}

void CarIzquierdaAUTO(int time){
  ENVIOtimeCommands=ENVIOtimeCommands+2.8;
  Serial.println("GIRO IZQUIERDA AUTOMATICO");
  while(startTime < millis()+time){
    // para la izquierda DESACTIVO motor izquierdo y activo adelante motor derecho
    digitalWrite(DERECHO_B, LOW);   //motor DC derecho
    digitalWrite(DERECHO_F, LOW);
    digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
    digitalWrite(IZQUIERDO_F, HIGH);
    startTime = startTime+ 1000;
  }
  digitalWrite(DERECHO_B, LOW);   //motor DC derecho
  digitalWrite(DERECHO_F, LOW);
  digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
  digitalWrite(IZQUIERDO_F, LOW);
}

void CarDerechaAUTO(int time){
  ENVIOtimeCommands=ENVIOtimeCommands+2.8;
  Serial.println("GIRO DERECHA AUTOMATICO");
  // para la derecha DESACTIVO motor derecho y activo adelante motor izquierdo
  while(startTime < millis()+time){
>>>>>>> ba92bc76f2b474629ae4947311b65bea5de013b2
    // para la izquierda DESACTIVO motor izquierdo y activo adelante motor derecho
    digitalWrite(DERECHO_B, LOW);   //motor DC derecho
    digitalWrite(DERECHO_F, HIGH);
    digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
    digitalWrite(IZQUIERDO_F, LOW);
    startTime = startTime+ 1000;
  }
<<<<<<< HEAD
  CarStop();
  startTime = 0;
}

void CarDerecha(){
  while(startTime+2500 > millis()){
    // para la derecha DESACTIVO motor derecho y activo adelante motor izquierdo
    digitalWrite(DERECHO_B, LOW);   //motor DC derecho
    digitalWrite(DERECHO_F, LOW);
    digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
    digitalWrite(IZQUIERDO_F, HIGH);
  }
  CarStop();
  startTime = 0;
}







//? █▀▄▀█ █▀█ █▀▄ █▀█   █▀▄▀█ ▄▀█ █▄░█ █░█ ▄▀█ █░░
//? █░▀░█ █▄█ █▄▀ █▄█   █░▀░█ █▀█ █░▀█ █▄█ █▀█ █▄▄
//!██████████████████ EJECUCION DE 20 MOVIMIENTOS ██████████████████
void gotoMovements(){
  //* ACTIVO NOINTERRUPTS PARA CUANDO SE VAYA A EJECUTAR LAS INSTRUCCIONES NO HAYAN CLAVOS CON LOS BOTONES 
  noInterrupts();
  for(int i = 0; i<20;i++){ //solo son 20 movements
    //! CADA ITERACION (MOVIMIENTO) VOY A PREPARAR LA PETICION POR PARTE DEL MASTER DE LA ACCION QUE SE HACE
    switch (MOVEMENTS[i])
=======
  digitalWrite(DERECHO_B, LOW);   //motor DC derecho
  digitalWrite(DERECHO_F, LOW);
  digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
  digitalWrite(IZQUIERDO_F, LOW);
}

void elegirDireccion(int dir, int time){
  switch (dir)
>>>>>>> ba92bc76f2b474629ae4947311b65bea5de013b2
    {
      case 0:
          CarAvanzaAUTO(time);
          delay(3000);
          break;
      case 1:
          CarRetrocedeAUTO(time);
          delay(3000);
          break;
      case 2:
          CarIzquierdaAUTO(time);
          delay(3000);
          break;
      case 3:
          CarDerechaAUTO(time);
          delay(3000);
          break;
    }

}


<<<<<<< HEAD
//? █▀▄▀█ █▀█ █▀▄ █▀█   ▄▀█ █░█ ▀█▀ █▀█ █▀▄▀█ ▄▀█ ▀█▀ █ █▀▀ █▀█
//? █░▀░█ █▄█ █▄▀ █▄█   █▀█ █▄█ ░█░ █▄█ █░▀░█ █▀█ ░█░ █ █▄▄ █▄█
int DISTANCIAAUX = 0;
void UltraFrontEscaneo(){
  int d = 0;
  digitalWrite(OJOSFRONT_T, LOW);  //* ESTOS digitalWrites practicamente lo que hacen es:
  delayMicroseconds(2);              //* tomar senales del sensor.
  digitalWrite(OJOSFRONT_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(OJOSFRONT_T, LOW);
  t = pulseIn(OJOSFRONT_E, HIGH);
  d = round(t*0.01715); //* ESTA ES LA DISTANCIA QUE HAY ENTRE UN OBJETO HASTA EL CARRO
  //TODO: esta distancia es de ir probando sino queda muy lejos el carro
  if (d >= 20){   //* PARA ESTE CASO VALIDO QUE SEA MAYOR A 15 CM
    DISTANCIAFRONT = d; //*ASIGNO A VARIABLE GLOBAL DEL FRENTE POR SI ES MENOR A 13
    // EN ESTE MOMENTO DEBE PAUSAR EL CARRO NO IMPORTANDO EN QUE ESTADO ESTE
    // SINO CHOCA Y BOOM XD
    CarStop();
    ENVIOvecesObstaculo++;
  }else{
    DISTANCIAFRONT = 0; // * SI LA DISTANCIA ES MAYOR ENTONCES 0 CLAVOS Y PODEMOS CONTINUAR
  }
}
void ESCANEO(){
  while(startTime+5000 > millis()){
    UltraFrontEscaneo
    // para la derecha DESACTIVO motor derecho y activo adelante motor izquierdo
    digitalWrite(DERECHO_B, LOW);   //motor DC derecho
    digitalWrite(DERECHO_F, LOW);
    digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
    digitalWrite(IZQUIERDO_F, HIGH);
  }
  CarStop();
}


void StartModoAuto(){
  while ((PAST != 1) && (FUTURE != 1)) {
    if(DISTANCIAFRONT != 0){
      ESCANEO();
    }
  }
=======
void MlistaPosicion1(){
  for(int i=0; i<sizeof(listaPosicion1); i++){
    elegirDireccion(listaPosicion1[i] , listaTiempo1[i]);
  
  }
}
void MlistaPosicion2(){
  for(int i = 0; i<sizeof(listaPosicion2);i++){
    elegirDireccion(listaPosicion2[i], listaTiempo2[i]);
  }
}
void MlistaPosicion3(){
  for(int i = 0; i<sizeof(listaPosicion3);i++){
      elegirDireccion(listaPosicion3[i], listaTiempo3[i]);

  }
}


void initColorType(){
  if(FLAGFIRSTPOSITION == 0){
    FLAGFIRSTPOSITION = 1;
    if(PRESENTCOLOR == 1){ //Rojo
      MlistaPosicion1();
    }else if(PRESENTCOLOR == 2){ //VERDE
      MlistaPosicion2();
    }else if(PRESENTCOLOR == 3){ //Azul
      MlistaPosicion3();
    }
  }
}

void StartModoAuto(){
  initColorType();

>>>>>>> ba92bc76f2b474629ae4947311b65bea5de013b2
}

//!██████████████████ TIPO CONTROLADOR ██████████████████
//!██████████████████ ACCIONES AL RECIBIR INSTRUCCION DE MAESTRO██████████████████
void evento(){
  int dispositivo = -1;
  int estado = -1;

  //! MODO MANUAL
  //*ESTO ES TIPO CONTROLADOR QUE ESTA VIENDO QUE ES LO QUE LE MANDA EL MASTER:
  //* MODO MANUAL
  if(Wire.available()){
    dispositivo = Wire.read();
    if(dispositivo == 0){    //* RECIBO LOS MOVEMENTS
      flagModo = 0;
      //TODO: necesito ver si es necesario hacer el Wire.read() antes de jalar la matriz.
      Serial.print("[ ");
      for(int i=0; i<20; i++)
      {
        MOVEMENTS[i] = Wire.read(); //* AQUI ES DONDE VOY RECIBIENDO Y GUARDANDO EN MOVEMENTS DESDE EL MASTER
        Serial.print(MOVEMENTS[i]);
        if((i+1)<20){
          Serial.print(",");
        }
      }
      Serial.println(" ]");
    }
    else if (dispositivo == 1){    //* RECIBE BANDERAZO DE START (INICIA LOS 20 MOVEMENTS)
      flagMoviendose=1;
      // gotoMovements();
      FLAGPOSICION = 0;
      FINMOVEMENTS = 1;
      flagMoviendose=0;
      //! TERMINA DE MOVERSE DAR AVISO FIN DE MODO MANUAL (MOVEMENTS EJECUTADOS)
    }
    //! MODO AUTOMATICO
    else if(dispositivo == 2){
      flagModo = 1;
      flagMoviendose=1;
      StartModoAuto();
    }
    else if(dispositivo == 3){

    }
    else{
      //! AGREGAREMOS LAS VARIABLES NECESARIAS DEPENDIENDO LOS ESTADOS QUE USAREMOS
    }

  }

}


//!█████████████████████████████ LO QUE RETORNA AL MASTER █████████████████████████████
void Peticion(){
  //! para enviar usamos el Wire.write(envio);
  if(flagModo == 0){ //! Datos para enviar en el modo manual
    // • El número de veces que se lee un color de una marca del suelo.
    for(int i = 0; i<4; i++){
      Wire.write(contColores[i]);
    }
    // • Las veces que el automóvil se encuentra con un obstáculo.
    Wire.write(ENVIOvecesObstaculo);
    // • El tiempo que el automóvil se tarda en ejecutar todos los comandos ingresados
    Wire.write(ENVIOtimeCommands);
    // VARIABLE DEL MOVIMIENTO DIRECCION
    Wire.write(ENVIOtimeCommands);
    // VARIABLE FLAG DE FIN DE MODO MANUAL
    Wire.write(FINMOVEMENTS);
  }else if (flagModo == 1){ //! Datos para enviar en el modo automático
    // El dato leído a través del sensor de color y las marcas del suelo.
    // • La cantidad de topes que encontró en la resolución.
    // Wire.write(CantTopes);

    //!EVNIO LA BANDERA DE DONDE SE COLOCO EL CARRO MODO AUTOMATICO
    Wire.write(PRESENTCOLOR);
  }


}




void MoverDireccion(){
  
  if(AVANZAINS == 1){
    if(millis()-tiempoInicio>10000){
      digitalWrite(DERECHO_B, LOW);   //motor DC derecho
      digitalWrite(DERECHO_F, LOW);
      digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
      digitalWrite(IZQUIERDO_F, LOW);
      AVANZAINS=0;
      FLAGPOSICION++;
      Serial.print("-----INSTRUCCION: ");
      Serial.println(FLAGPOSICION);
    }else{
      Serial.println("AVANZA");
      digitalWrite(DERECHO_B, LOW);   //motor DC derecho
      digitalWrite(DERECHO_F, HIGH);
      digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
      digitalWrite(IZQUIERDO_F, HIGH);
    }
  }else if (RETROCEDEINS == 1){
    if(millis()-tiempoInicio>10000){
      digitalWrite(DERECHO_B, LOW);   //motor DC derecho
      digitalWrite(DERECHO_F, HIGH);
      digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
      digitalWrite(IZQUIERDO_F, HIGH);
      RETROCEDEINS=0;
      FLAGPOSICION++;
      Serial.print("-----INSTRUCCION: ");
      Serial.println(FLAGPOSICION);
    }else{
      Serial.println("RETROCEDE");
      digitalWrite(DERECHO_B, HIGH);   //motor DC derecho
      digitalWrite(DERECHO_F, LOW);
      digitalWrite(IZQUIERDO_B, HIGH); //motor DC izquierdo
      digitalWrite(IZQUIERDO_F, LOW);
    }
  }else if (IZQUIERDAINS == 1){
    if(millis()-tiempoInicio>2500){
      digitalWrite(DERECHO_B, LOW);   //motor DC derecho
      digitalWrite(DERECHO_F, LOW);
      digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
      digitalWrite(IZQUIERDO_F, HIGH);
      IZQUIERDAINS=0;
      FLAGPOSICION++;
      Serial.print("-----INSTRUCCION: ");
      Serial.println(FLAGPOSICION);
    }else{
      Serial.println("IZQUIERDA");
      digitalWrite(DERECHO_B, LOW);   //motor DC derecho
      digitalWrite(DERECHO_F, LOW);
      digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
      digitalWrite(IZQUIERDO_F, HIGH);
    }
  }else if (DERECHAINS == 1){
    if(millis()-tiempoInicio>2500){
      digitalWrite(DERECHO_B, LOW);   //motor DC derecho
      digitalWrite(DERECHO_F, HIGH);
      digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
      digitalWrite(IZQUIERDO_F, LOW);
      DERECHAINS=0;
      FLAGPOSICION++;
      Serial.print("-----INSTRUCCION: ");
      Serial.println(FLAGPOSICION);
    }else{
      Serial.println("DERECHA");
      digitalWrite(DERECHO_B, LOW);   //motor DC derecho
      digitalWrite(DERECHO_F, HIGH);
      digitalWrite(IZQUIERDO_B, LOW); //motor DC izquierdo
      digitalWrite(IZQUIERDO_F, LOW);
    }
  }
}

//!███████████████████████████████ MAIN █████████████████████████████████████████
void loop() {
  //! DENTRO DEL LOOP NECESITAMOS QUE ESTEN CORRIENDO LOS SENSORES DE MANERA CICLICA
  //* primero ponemos a correr o ejecutar los ojos (ultrasonicos)
      // * el ultrasonico de ATRAS solo se usa en modo MANUAL
  if(flagMoviendose == 1){ // si se mueve el carro activo sensor color
    fotodiodos();
    UltraFront();
    UltraBack();
  }
<<<<<<< HEAD
}
=======
  if(FLAGPOSICION>=0 || FLAGPOSICION<20 ){
    switch (MOVEMENTS[FLAGPOSICION])
    {
      case 0:
          tiempoInicio=millis();
          AVANZAINS = 1;
          MoverDireccion();
          break;
      case 1:
          tiempoInicio=millis();
          RETROCEDEINS = 1;
          MoverDireccion();
          break;
      case 2:
          tiempoInicio=millis();
          IZQUIERDAINS = 1;
          MoverDireccion();
          break;
      case 3:
          tiempoInicio=millis();
          DERECHAINS = 1;
          MoverDireccion();
          break;
     
    }
  
  }
  
  
  //UltraFront();
  //UltraBack();

}
>>>>>>> ba92bc76f2b474629ae4947311b65bea5de013b2
