#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Crear instancia de MFRC522

Servo microservo9g; // Crear instancia del servomotor

int led_verde = 6; // Pin del LED verde
int led_rojo = 7; // Pin del LED rojo

// UID de la tarjeta permitida
String uidPermitido = "390E37B3"; // Cambia esto por el UID de tu tarjeta

void setup() {
  pinMode(led_verde, OUTPUT); // Establecer pin del LED verde como salida
  pinMode(led_rojo, OUTPUT); // Establecer pin del LED rojo como salida
  
  microservo9g.attach(3); // Conectar el servomotor al pin 3
  microservo9g.write(0); // Posición inicial del servomotor
  
  Serial.begin(9600); // Iniciar comunicación serial
  SPI.begin(); // Iniciar bus SPI
  mfrc522.PCD_Init(); // Iniciar MFRC522
  
  Serial.println("Acerca tu tarjeta al lector...");
  Serial.println();
}

void loop() {
  // Buscar nuevas tarjetas
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Seleccionar una de las tarjetas
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // Mostrar UID en el monitor serial
  Serial.print("UID de la tarjeta: ");
  String contenido = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    contenido.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    contenido.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensaje: ");
  contenido.toUpperCase();
  
  if (contenido == uidPermitido) { // Comparar UID con el UID permitido
    Serial.println("Acceso autorizado");
    Serial.println();
    
    // Encender el LED verde y mover el servomotor
    digitalWrite(led_verde, HIGH);
    digitalWrite(led_rojo, LOW);
    microservo9g.write(90); // Girar el servomotor 90 grados
    delay(2000);
    microservo9g.write(0); // Volver a la posición inicial
    digitalWrite(led_verde, LOW);
    
  } else {
    Serial.println("Acceso denegado");
    Serial.println();
    
    // Encender el LED rojo
    digitalWrite(led_verde, LOW);
    for (int i = 0; i < 3; i++) { // Parpadear el LED rojo tres veces
      digitalWrite(led_rojo, HIGH);
      delay(200);
      digitalWrite(led_rojo, LOW);
      delay(200);
    }
  }
  delay(1000); // Pequeño retraso antes de la siguiente lectura
}
