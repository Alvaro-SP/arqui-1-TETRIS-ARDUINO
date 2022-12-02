//* ███╗░░░███╗░█████╗░░██████╗████████╗███████╗██████╗░
//* ████╗░████║██╔══██╗██╔════╝╚══██╔══╝██╔════╝██╔══██╗
//* ██╔████╔██║███████║╚█████╗░░░░██║░░░█████╗░░██████╔╝
//* ██║╚██╔╝██║██╔══██║░╚═══██╗░░░██║░░░██╔══╝░░██╔══██╗
//* ██║░╚═╝░██║██║░░██║██████╔╝░░░██║░░░███████╗██║░░██║
//* ╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═════╝░░░░╚═╝░░░╚══════╝╚═╝░░╚═╝
//TODO: - Controlar la pantalla LCD 16 x 2 para mostrar los distintos mensajes y estadísticas.
//TODO: - Recabará la información recibida de cada detección.
//TODO: - Manejará las listas de movimientos del modo manual.
//TODO: - Calculara los estadísticos.
//TODO: - Control de botones.
//TODO: - Contadores.
#include <Wire.h>
#include <LiquidCrystal.h>

//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀ VARIABLES MODO MANUAL  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
LiquidCrystal lcd(10, 9, 8, 7, 6, 5, 4); //(RS, RW, E, D4,D5, D6, D7)

//! BOTONES
int AVANZAR = 31;
int RETROCEDER = 32;
int IZQUIERDA = 33;
int DERECHA = 34;
int START = 35;       //* INICIA TODOS LOS MOVIMIENTOS
int PAUSA = 36;       //* PAUSA MODO MANUAL
int MODOAUTO = 37;    //* VARIABLES PARA ESCOGER LOS MODOS
int MODOMANUAL = 38;
int ENVIOtimeCommands = 0;
//! LISTA PARA GUARDAR LOS MOVIMIENTOS  0 1 2 3
int MOVEMENTS[20] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};


byte cara[] = {
  B00000,  B00000,  B01010,  B00000,  B00000,  B10001,  B01110,  B00000};
//? Matrices para las flechas que se muestran en el movimiento del carro
byte flechaArriba[] = {
  B00100,  B01110,  B10101,  B00100,  B00100,  B00100,  B00100,  B00100};
byte flechaAbajo[] = {
  B00100,  B00100,  B00100,  B00100,  B00100,  B10101,  B01110,  B00100};
byte flechaDerecha[] = {
  B01000,  B00100,  B00010,  B11111,  B11111,  B00010,  B00100,  B01000};
byte flechaIzquierda[] = {
  B00010,  B00100,  B01000,  B11111,  B11111,  B01000,  B00100,  B00010};
//Mensaje inicial en movimiento de izquierda a derecha

//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀ VARIABLES MODO AUTOMATICO  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
int listaPosicion1[30] = {0, 2, 0, 2, 2, 0, 3, 0, 2, 3, 3, 0,3,0,2,0,3,0,2,2,0,2,0,3,3,0,2,2,0,3}; //Rojo 
int listaPosicion2[19] = {0,3,0,2,0,3,0,2,2,0,2,0,3,3,0,2,2,0,3}; //Verde
int listaPosicion3[10] = {0,2,0,3,3,0,2,2,0,3}; //Azul
//! avanzar = 0
//! retroceder = 1
//! izquierda = 2
//! derecha = 3
int PRESENTCOLOR = 0;

//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
//* ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀ VARIABLES FLAGS Y CONTADORES ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀

int movIndex=0; //* indica indice que se estara EJECUTANDO modo manual.
unsigned long tiempo;
int estadoMaestro = 0; //Estado = 0 (Mensaje inicial de 10s) Estado = 1 (Mensaje de escogiendo de 3s) Estado = 2 (Escoger Modo)
int startMoves=0; //CONTADOR DE MOVIMIENTOS
<<<<<<< HEAD
//Matriz de bytes para dibujar la cara
byte cara[] = {
  B00000,  B00000,  B01010,  B00000,  B00000,  B10001,  B01110,  B00000};
//? Matrices para las flechas que se muestran en el movimiento del carro
byte flechaArriba[] = {
  B00100,  B01110,  B10101,  B00100,  B00100,  B00100,  B00100,  B00100};
byte flechaAbajo[] = {
  B00100,  B00100,  B00100,  B00100,  B00100,  B10101,  B01110,  B00100};
byte flechaDerecha[] = {
  B01000,  B00100,  B00010,  B11111,  B11111,  B00010,  B00100,  B01000};
byte flechaIzquierda[] = {
  B00010,  B00100,  B01000,  B11111,  B11111,  B01000,  B00100,  B00010};
=======
int FINMOVEMENTS =0; //BANDERA DE FINAL DE LOS MOVIMIENTOS
int contColores[4] = {0,0,0,0};
int ROJO=   0;   // si es 0 no hay flechas o direcciones disponibles
int VERDE=  0;
int AZUL=   0;
int NEGRO=   0;

int ENVIOvecesObstaculo = 0;
>>>>>>> ba92bc76f2b474629ae4947311b65bea5de013b2
void setup() {
  // Iniciamos I^2 C con Wire
  Wire.begin();
  Serial.begin(9600);
  // put your setup code here, to run once:
  for (int i=4; i<11; i++) pinMode(i, OUTPUT);
  lcd.begin(16, 2);
  lcd.createChar(0, cara);
  lcd.createChar(1, flechaArriba);
  lcd.createChar(2, flechaAbajo);
  lcd.createChar(3, flechaDerecha);
  lcd.createChar(4, flechaIzquierda);
  lcd.home();
  lcd.setCursor(1,0);
  lcd.write(byte(0));
  lcd.print("GRUPO #2 ACE1");
  lcd.setCursor(15,0);
  lcd.write(byte(0));
  lcd.setCursor(16, 0);
  lcd.print(" SECC A");
  tiempo = millis();
  pinMode(AVANZAR, INPUT);
  pinMode(RETROCEDER, INPUT);
  pinMode(IZQUIERDA, INPUT);
  pinMode(DERECHA, INPUT);
  pinMode(START, INPUT);
  pinMode(PAUSA, INPUT);
  pinMode(MODOMANUAL, INPUT);
  pinMode(MODOAUTO, INPUT);

}

//* █▀█ ▄▀█ █▄░█ ▀█▀ ▄▀█ █░░ █░░ ▄▀█     █░░ █▀▀ █▀▄
//* █▀▀ █▀█ █░▀█ ░█░ █▀█ █▄▄ █▄▄ █▀█     █▄▄ █▄▄ █▄▀
<<<<<<< HEAD

//Mensaje inicial en movimiento de izquierda a derecha
=======
//Matriz de bytes para dibujar la cara

>>>>>>> ba92bc76f2b474629ae4947311b65bea5de013b2
void move_left_right(){
  if(millis()-tiempo>10000){ //Conteo de 10s
    lcd.clear();
    tiempo = millis();
    estadoMaestro = 1;
  }else{
    for (int positionCounter = 0; positionCounter < 23; positionCounter++) {
      lcd.scrollDisplayRight();
      if(positionCounter == 23){
        break;
      }
      delay(325);
    }
  }
}

//Mostrar mensaje de "escogiendo modo" durante 3s
void displayEscogiendo(){
  if(millis()-tiempo>3000){
    lcd.clear();
    tiempo = millis();
    estadoMaestro = 2;
  }else{
      lcd.setCursor(0, 0);
      lcd.print("ESCOGIENDO MODO");
  }
}

//Mostrar las opciones que se pueden elegir, se sale de aquí hasta seleccionar una de las dos
void displayOpciones(){
  lcd.setCursor(0,0);
  lcd.print("<-AUTO|MANUAL->");
  //* ESPERANDO A QUE SE PRESIONEN LOS BOTONES PARA ENTRAR A ALGUN MODO :
  if(digitalRead(MODOMANUAL) == LOW){
    // Al entrar en este modo, se debe mostrar un mensaje en la pantalla LCD durante 5 segundos 
    // que se está iniciando el modo manual.
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Iniciando Modo");
    lcd.setCursor(0,1);
    lcd.print("Manual");
    delay(5000);
    lcd.clear();
    // MODO MANUAL ACTIVADO
    estadoMaestro=3;
  }else if(digitalRead(MODOAUTO) == LOW){
    // MODO AUTOMATICO ACTIVADO
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Iniciando Modo");
    lcd.setCursor(0,1);
    lcd.print("Automatico");
    delay(5000);    
    lcd.clear();
    estadoMaestro=4;
  }
}

//Mostrar los movimientos del modo manual en LCD
void moveType(){
  int contador = 0;
  for(int i=movIndex; i<20; i++){
    lcd.setCursor(contador,0);
    if(MOVEMENTS[i]==0){
      lcd.write(byte(1));
    }
    if(MOVEMENTS[i]==1){
      lcd.write(byte(2));
    }
    if(MOVEMENTS[i]==2){
      lcd.write(byte(4));
    }
    if(MOVEMENTS[i]==3){
      lcd.write(byte(3));
    }
    contador = contador + 2;
  }
  for(int i=movIndex; i<20; i++){
    lcd.scrollDisplayRight();
    delay(300);
  }
}

//* █▀▄▀█ █▀█ █▀▄ █▀█   █▀▄▀█ ▄▀█ █▄░█ █░█ ▄▀█ █░░
//* █░▀░█ █▄█ █▄▀ █▄█   █░▀░█ █▀█ █░▀█ █▄█ █▀█ █▄▄
void SendMovements(){
  for(int i = 0; i<20;i++){
    Wire.write(MOVEMENTS[i]);
  }
}
int conta_instrucciones = 0;
void ModoManual(){
  contColores[0]=0;
  contColores[1]=0;
  contColores[2]=0;
  contColores[3]=0;
  //! RECIBIENDO INSTRUCCIONES
  
  if(startMoves <= 19){ //* en este caso si la bandera de iniciar a guardar Movimientos en array se activa:
    if(digitalRead(AVANZAR)==LOW){          //! 0
      //? FALTA TIEMPO EN QUE VA A AVANZAR
      Serial.println("BOTON AVANZAR 10 seg por defecto");
      // for (int i=4; i<11; i++) pinMode(i, OUTPUT);
      // lcd.begin(16, 2);
      // lcd.createChar(1, flechaArriba);
      lcd.setCursor(1,0);
      lcd.write(byte(1));
      MOVEMENTS[startMoves] = 0;
      startMoves++;
      conta_instrucciones++;
      lcd.setCursor(0,1);
      lcd.print("NO. INSTRU: ");
      lcd.setCursor(13,1);
      lcd.print(conta_instrucciones);
      delay(200);
    }
    else if(digitalRead(RETROCEDER)==LOW){  //! 1
      //? FALTA TIEMPO EN QUE VA A AVANZAR
      Serial.println("BOTON RETRO 10 seg por defecto");
      //for (int i=4; i<11; i++) pinMode(i, OUTPUT);
      //lcd.begin(16, 2);
      //lcd.createChar(1, flechaAbajo);
      lcd.setCursor(1,0);
      lcd.write(byte(2));
      MOVEMENTS[startMoves] = 1;
      startMoves++;
      conta_instrucciones++;
      lcd.setCursor(0,1);
      lcd.print("NO. INSTRU: ");
      lcd.setCursor(13,1);
      lcd.print(conta_instrucciones);
      delay(200);
    }
    else if(digitalRead(IZQUIERDA)==LOW){   //! 2
      Serial.println("BOTON IZQ 10 seg por defecto");
      //? ANGULO
      lcd.setCursor(1,0);
      lcd.write(byte(4));
      MOVEMENTS[startMoves] = 2;
      startMoves++;
      conta_instrucciones++;
      lcd.setCursor(0,1);
      lcd.print("NO. INSTRU: ");
      lcd.setCursor(13,1);
      lcd.print(conta_instrucciones);      
      delay(200);
    }
    else if(digitalRead(DERECHA)==LOW){     //! 3
      //? ANGULO
      Serial.println("BOTON DER 10 seg por defecto");
      lcd.setCursor(1,0);
      lcd.write(byte(3));
      MOVEMENTS[startMoves] = 3;
      startMoves++;
      conta_instrucciones++;
      lcd.setCursor(0,1);
      lcd.print("NO. INSTRU: ");
      lcd.setCursor(13,1);
      lcd.print(conta_instrucciones);
      delay(200);
    }
    else if(digitalRead(START)==LOW){       //! START
      Serial.println("BOTON START");
      delay(200);
      //* Al presionar START debe estar ya llena la LISTA MOVEMENTS
      //* y envio el banderazo de que puede iniciar la ejecucion
      Wire.beginTransmission(1);
      Wire.write(1); //!FLAG
      Wire.endTransmission();
    }

  }
  else{
    //* Aqui al llegar a tener los 20 movimientos, procedo a enviar la lista de MOVEMENTS al slave.
    //! ir a estado enviar array al esclavo
    Wire.beginTransmission(1);
    Wire.write(0);                    //* ENVIO LOS MOVEMENTS
    SendMovements();                  //? ENVIA LISTA MOVEMENTS
    Wire.endTransmission();
    startMoves=0;
  }

}


//* █▀▄▀█ █▀█ █▀▄ █▀█   ▄▀█ █░█ ▀█▀ █▀█ █▀▄▀█ ▄▀█ ▀█▀ █ █▀▀ █▀█
//* █░▀░█ █▄█ █▄▀ █▄█   █▀█ █▄█ ░█░ █▄█ █░▀░█ █▀█ ░█░ █ █▄▄ █▄█

void ModoAuto(){  //! AUN PENDIENTE
  contColores[0]=0;
  contColores[1]=0;
  contColores[2]=0;
  contColores[3]=0;
  Wire.beginTransmission(1);    //INDICA A QUE ESCLAVO TRANSMITIR
  Wire.write(2);                //* MODO AUTOMATICO
  // Wire.write(1);                //ENVIA EL DISPOSIIVO
  // Wire.write(1);                //ENVÍA EL VALOR DE LA VARIABLE ESTADO DEL DISPOSITIVO
  Wire.endTransmission();
}

//!██████████████████ ESTADISTICAS ██████████████████
void estadisticasManual(){
  //* Numero correcto de veces que lee un color

  lcd.clear();
  lcd.setCursor(2,0); // columna , fila.
  lcd.print("ESTADISTICAS");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0); // columna , fila.
  lcd.print("NO. COLORES");
  delay(2000);

  lcd.clear();

  lcd.setCursor(0,0); // columna , fila.
  lcd.print("R:");
  lcd.setCursor(2,0);
  lcd.print(ROJO);

  lcd.setCursor(5,0);
  lcd.print("V:");
  lcd.setCursor(7,0);
  lcd.print(VERDE);

  lcd.setCursor(0,1);
  lcd.print("A:");
  lcd.setCursor(2,1);
  lcd.print(AZUL);

  lcd.setCursor(5,1);
  lcd.print("N:");
  lcd.setCursor(7,1);
  lcd.print(NEGRO);
  delay(7000);

  //* Veces que encuentra un obstaculo
  lcd.clear();
  lcd.setCursor(0,0); // columna , fila.
  lcd.print("NO. OBSTACULOS");
  lcd.setCursor(5,1);
  lcd.print(ENVIOvecesObstaculo);
  delay(5000);

  //* Tiempo de resolucion ambos modos
  lcd.clear();
  lcd.setCursor(0,0); // columna , fila.
  lcd.print("TIEMPO. SOLVE");
  lcd.setCursor(5,1);
  lcd.print(ENVIOtimeCommands);
  delay(5000);

}



int obtenerDistancia(){
  int temp = 0;
  for(int i=0; i<sizeof(contColores); i++){
    temp = temp+(contColores[i] * 5); //NUMERO DE MARCAS QUE LEYO EN TODO SU CAMINO.
    //! MULTIPLIQUE POR 5 CENTIMETROS PERO NO SE EL ESPACIO DE CADA TIRA EN LABERINTO
  }

  
  return temp;
}


int obtenerMedia(){
  int ret = 0;
  int c = 0;
  switch (PRESENTCOLOR)
    {   //! LISTAPOSICIONES : ==>
                        //! avanzar = 0
                        //! retroceder = 1
                        //! izquierda = 2
                        //! derecha = 3
      case 1:
          for(int i=0; i<sizeof(listaPosicion1); i++){
            if(listaPosicion1[i] == 2 || listaPosicion1[i] == 3){
              ret = ret+90;
              c++;
            }
          }
          break;
      case 2:
          for(int i=0; i<sizeof(listaPosicion2); i++){
            if(listaPosicion2[i] == 2 || listaPosicion2[i] == 3){
              ret = ret+90;
              c++;
            }
          }
          break;
      case 3:
          for(int i=0; i<sizeof(listaPosicion3); i++){
            if(listaPosicion3[i] == 2 || listaPosicion3[i] == 3){
              ret = ret+90; 
              c++;
            }
          }
          break;
    }
    return ret/c;
}
int obtenerModa(){
  int z = 0;
  int d = 0;
   switch (PRESENTCOLOR)
    {   //! LISTAPOSICIONES : ==>
                        //! avanzar = 0
                        //! retroceder = 1
                        //! izquierda = 2
                        //! derecha = 3
      case 1:
          for(int i=0; i<sizeof(listaPosicion1); i++){
            if(listaPosicion1[i] == 3){
              d++;
            }
            if(listaPosicion1[i] == 2 ){
              z++;
            }
          }
          break;
      case 2:
          for(int i=0; i<sizeof(listaPosicion2); i++){
            if(listaPosicion1[i] == 3){
              d++;
            }
            if(listaPosicion1[i] == 2 ){
              z++;
            }
          }
          break;
      case 3:
          for(int i=0; i<sizeof(listaPosicion3); i++){
            if(listaPosicion1[i] == 3){
              d++;
            }
            if(listaPosicion1[i] == 2 ){
              z++;
            }
          }
          break;
    }
    int x;
    if(d<z){
      x = 1;
    }else{
      x =0;
    }
    return x;
}
int CanDer(){
  int d = 0;
   switch (PRESENTCOLOR)
    {   //! LISTAPOSICIONES : ==>
                        //! avanzar = 0
                        //! retroceder = 1
                        //! izquierda = 2
                        //! derecha = 3
      case 1:
          for(int i=0; i<sizeof(listaPosicion1); i++){
            if(listaPosicion1[i] == 3){
              d++;
            }
          }
          break;
      case 2:
          for(int i=0; i<sizeof(listaPosicion2); i++){
            if(listaPosicion1[i] == 3){
              d++;
            }
          }
          break;
      case 3:
          for(int i=0; i<sizeof(listaPosicion3); i++){
            if(listaPosicion1[i] == 3){
              d++;
            }
          }
          break;
    }
    return d;
}

int CanIz(){
  int z = 0;
  int d = 0;
   switch (PRESENTCOLOR)
    {   //! LISTAPOSICIONES : ==>
                        //! avanzar = 0
                        //! retroceder = 1
                        //! izquierda = 2
                        //! derecha = 3
      case 1:
          for(int i=0; i<sizeof(listaPosicion1); i++){
            if(listaPosicion1[i] == 2 ){
              z++;
            }
          }
          break;
      case 2:
          for(int i=0; i<sizeof(listaPosicion2); i++){
            if(listaPosicion1[i] == 2 ){
              z++;
            }
          }
          break;
      case 3:
          for(int i=0; i<sizeof(listaPosicion3); i++){
            if(listaPosicion1[i] == 2 ){
              z++;
            }
          }
          break;
    }
    return z;
}

void estadisticasAuto(){
  lcd.clear();
  lcd.setCursor(2,0); // columna , fila.
  lcd.print("ESTADISTICAS");
  delay(3000);
  
  //* • Distancia total recorrida (Generada a través del maestro dependiendo de los datos enviados en la lectura de marcas de color, modo automático) .
  lcd.clear();
  lcd.setCursor(0,0); // columna , fila.
  lcd.print("DISTANCIA: "); 
  lcd.setCursor(5,1);
  int retDis = obtenerDistancia();
  lcd.print(retDis);
  lcd.setCursor(9,1);
  lcd.print("cms.");
  delay(5000);


  //* • Media y Moda de los ángulos que el automóvil gira (2 modos).
  lcd.clear();
  lcd.setCursor(0,0); // columna , fila.
  lcd.print("MEDIA:");
  lcd.setCursor(1,1);
  int retMedia = obtenerMedia();
  lcd.print(retMedia);
  lcd.setCursor(9,0);
  lcd.print("MODA:");
  lcd.setCursor(0,1);
  int retModa = obtenerModa();  //1 a la derecha
  if(retModa == 1){
    lcd.print("90° a derecha");
  }else{
    lcd.print("90° a izquierda");
  }

  delay(6000);

  //* • El tiempo que el automóvil estuvo en movimiento (se enviará una vez al final, 2 modos).

  //* • Las veces que el automóvil llega a un pasillo sin salida (Modo automático).
  lcd.clear();
  lcd.setCursor(0,0); // columna , fila.
  lcd.print("PASILLO NO SALIDA");
  lcd.setCursor(1,1);
  lcd.print("0 VECES");
  delay(5000);
  //* • La cantidad y dirección de los giros realizados (Modo automático).
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CANTIDAD G. IZQ.");
  int ci =CanIz();
  int cd =CanDer();
  lcd.setCursor(0,1);
  lcd.print(ci);
  lcd.setCursor(3,1);
  lcd.print("Giros <-");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("CANTIDAD G. DER");
  lcd.setCursor(0,1);
  lcd.print(cd);
  lcd.setCursor(3,1);
  lcd.print("Giros ->");
  delay(2000);
  

  //* • El número de marcas de cada color que se leen (2 modos)

  lcd.clear();
  lcd.setCursor(0,0); // columna , fila.
  lcd.print("NO. COLORES");
  delay(2000);

  lcd.clear();

  lcd.setCursor(0,0); // columna , fila.
  lcd.print("R:");
  lcd.setCursor(2,0);
  lcd.print(ROJO);

  lcd.setCursor(5,0);
  lcd.print("V:");
  lcd.setCursor(7,0);
  lcd.print(VERDE);

  lcd.setCursor(0,1);
  lcd.print("A:");
  lcd.setCursor(2,1);
  lcd.print(AZUL);

  lcd.setCursor(5,1);
  lcd.print("N:");
  lcd.setCursor(7,1);
  lcd.print(NEGRO);
  delay(7000);

  //* Tiempo de resolucion ambos modos
}

void PeticionManual(){

  //! RECIBO COLORES
  Wire.requestFrom(1, 8);
  //! aqui habia un while lo cual esta malo porque se acaba todos los bytes y solo son 4 + los de abajo
  // while(Wire.available()){

    
  // }

  if(Wire.available()){
    int recibido = Wire.read();
    if(recibido != 255){
      ROJO= Wire.read();
      VERDE= Wire.read();
      AZUL= Wire.read();
      NEGRO = Wire.read();
      Serial.print("PETICION");
      Serial.print(ROJO);
      Serial.print(VERDE);
      Serial.print(AZUL);
      Serial.print(NEGRO);
    }
    Serial.print("ESTO SE VA A ENCICLAR DADO QUE ES LA PETICION CONSTANTE: ")
    //! RECIBO NO OBSTACULOS
    ENVIOvecesObstaculo = Wire.read();
    Serial.print(ENVIOvecesObstaculo);
    //! RECIBO TIEMPO DE RESOLUCION
    ENVIOtimeCommands = Wire.read();
    Serial.print(ENVIOtimeCommands);
    //! VARIABLE DEL MOVIMIENTO DIRECCION
    movIndex = Wire.read();
    Serial.print(movIndex);
    //! VARIABLE QUE MARCA EL FINAL DE LOS 20 MOVIMIENTOS
    FINMOVEMENTS = Wire.read();
    Serial.print(FINMOVEMENTS);
  }
}

void PeticionAuto(){
  Serial.print("ESTO SE VA A ENCICLAR DADO QUE ES LA PETICION CONSTANTE: ")
   //! RECIBO SOLO LA BANDERA DE QUE POSICION INICIO QUEMADAS
  Wire.requestFrom(1, 1);
  if (Wire.available()){
    PRESENTCOLOR = Wire.read();
  }
}
//!██████████████████████████████ MAIN ██████████████████████████████
void loop() {
  
  if(estadoMaestro == 0){           //* INICIO DEL LCD
    move_left_right();
  }else if(estadoMaestro == 1){     //* ESTADO ESCOGER EL TIPO
    displayEscogiendo();
  }else if(estadoMaestro == 2){     //* ESTADO OPCIONES DEL DISPLAY
    displayOpciones();
  }else if(estadoMaestro == 3){     //* ESTADO MANUAL
    ModoManual();
   // PeticionManual();
    
  }else if(estadoMaestro == 4){     //* ESTADO AUTOMATICO
    ModoAuto();
    estadisticasAuto();
    PeticionAuto();
  }else if(estadoMaestro == 5){
    estadisticasManual();
  }

  //! SERA LAS PETICIONES QUE SE RECIBIRAN CONSTANTEMENTE DESDE EL ESCLAVO
  
}





//! VALIDACIONES DE LOS IF EN EL SLAVE (CONTROLADOR)
      //!   0 <== RECIBE ARRAY MOVEMENTS
      //!   1 <== BANDERA INICIO MODO MANUAL
      //!   2 <== BANDERA INICIO MODO AUTOMATICO
      //!   3 <== 
      //!   4 <== 
      //!   5 <== 
      //!   6 <== estado de iniciar las instrucciones en el ESCLAVO