#include <DHT.h>
#define decodificadorDA  2
#define decodificadorDB  3
#define decodificadorDC  4
#define decodificadorDD  5
#define decodificadorUA  6
#define decodificadorUB  7
#define decodificadorUC  8
#define decodificadorUD  9
const int pinEnviarSonda = 10;
const int pinLed = 11;
const int pinBoton = 12;
#define PIN_DHT 13       // Pin de datos conectado al pin 13 de Arduino
#define TIPO_DHT DHT11   // Tipo de sensor DHT que se utiliza
DHT dht(PIN_DHT, TIPO_DHT);
const int pinReceptorSonda = A0;
int estadoBoton = 0;
int ultimoEstadoBoton = 0;
int modoVisualizacion = 0;
int temperatura;
int valorMostrado;
int valorParaMostrar;
int bitsLlegada;
float voltajeResistencias;
float voltajeLlegada;
float resistenciaSuelo;
int resistenciaSonda = 10000;
const double resistenciaTierraSeca = 125184;
const double resistenciaTierraHumeda = 24263;
float humedadInstante;
int intHumedadInstante;
int arregloMuestras[5];
int promedioHumedad;
void setup() {
  Serial.begin(9600); 
  delay(200);
   // Inicializar pines
  pinMode(pinBoton, INPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(pinEnviarSonda, OUTPUT);
  pinMode(pinReceptorSonda, INPUT);
  pinMode(decodificadorDA, OUTPUT);
  pinMode(decodificadorDB, OUTPUT);
  pinMode(decodificadorDC, OUTPUT);
  pinMode(decodificadorDD, OUTPUT);
  pinMode(decodificadorUA, OUTPUT);
  pinMode(decodificadorUB, OUTPUT);
  pinMode(decodificadorUC, OUTPUT);
  pinMode(decodificadorUD, OUTPUT);
  // Inicializar sensor DHT11
  dht.begin();
}
void loop() {  
  estadoBoton = digitalRead(pinBoton);
  if (estadoBoton == HIGH){
    if (valorMostrado==0){
      valorParaMostrar = temperaturaSensor();
    }
    else {
      valorParaMostrar = humedadSonda();
    }
  }
  else{
    digitalWrite(pinLed, HIGH);
    delay(500);
    if (valorMostrado==0){
      valorMostrado = 1;
      valorParaMostrar = humedadSonda();
    }
    else {
      valorParaMostrar = temperaturaSensor();
      valorMostrado = 0;
    }
    digitalWrite(pinLed, LOW);
  }
  mostrarEnDisplaySim(valorParaMostrar);
  delay(1000);
}

int humedadSonda(){
  digitalWrite(pinEnviarSonda,HIGH);
  int bitsSondaCasera = analogRead(pinReceptorSonda);
  digitalWrite(pinEnviarSonda,LOW);
  float voltajeSonda = bitsSondaCasera * (5./1023.);
  float resistenciaSuelo = resistenciaSonda * ((voltajeSonda)/(5.-voltajeSonda));
  if (resistenciaSuelo > resistenciaTierraSeca){
    resistenciaSuelo = 125184;
  }
  float humedad = ((resistenciaTierraSeca - resistenciaSuelo) / (resistenciaTierraSeca - resistenciaTierraHumeda)) * 100.0; //Rseca =125184  Rhumeda=4595 
  humedad = constrain(humedad, 0, 100);
  if (humedad > 99){ humedad = 99}
  return humedad;
  delay(1000);
}
int temperaturaSensor(){
  return 15+(10*abs(dht.readTemperature()));
}
void mostrarEnDisplaySim(int numero){
  int unidades = numero % 10;
  int decenas = numero / 10;
  digitalWrite(decodificadorUA, unidades & 0b0001);
  digitalWrite(decodificadorUB, (unidades >> 1) & 0b0001);
  digitalWrite(decodificadorUC, (unidades >> 2) & 0b0001);
  digitalWrite(decodificadorUD, (unidades >> 3) & 0b0001);
  digitalWrite(decodificadorDA, decenas & 0b0001);
  digitalWrite(decodificadorDB, (decenas >> 1) & 0b0001);
  digitalWrite(decodificadorDC, (decenas >> 2) & 0b0001);
  digitalWrite(decodificadorDD, (decenas >> 3) & 0b0001);
}
