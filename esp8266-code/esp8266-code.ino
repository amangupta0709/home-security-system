#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "AES.h"
#include "Base64.h"

const char *ssid = "JioFiber-4G";   //Enter SSID
const char *password = "Vinod123$"; //Enter Password
//const char* host = "192.168.29.230"; //edit the host adress, ip address etc.
//String url = "";
AES aes;

void setup(void)
{
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    //Serial.print("*");
  }

  //Serial.println("");
  //Serial.println("WiFi connection Successful");
  //Serial.print("The IP Address of ESP8266 Module is: ");
  //Serial.print(WiFi.localIP()); // Print the IP address
}

void loop()
{
  if (Serial.available())
  {
    String IncomingString = Serial.readString();
    postDataToServer(IncomingString);
  }
  // postDataToServer();
  delay(1000);
}

void postDataToServer(String data)
{

  // Serial.println("Posting JSON data to server...");

  if (WiFi.status() == WL_CONNECTED)
  {

    HTTPClient http;
    WiFiClient client;

    http.begin(client, "http://192.168.29.230:3000/");
    http.addHeader("Content-Type", "text/plain");

    // String data = "2233";
    String encryptedData = aes_encrypt(data);

    int httpCode = http.POST(encryptedData);
    String response = http.getString();

    // Serial.println(httpCode);
    Serial.println(response);

    //    http.end();
  }
  else
  {
    Serial.println("Error in wifi connection");
  }
}

uint8_t getrnd()
{
  uint8_t really_random = *(volatile uint8_t *)0x3FF20E44;
  return really_random;
}

// Generate a random initialization vector
void gen_iv(byte *iv)
{
  for (int i = 0; i < N_BLOCK; i++)
  {
    iv[i] = (byte)getrnd();
  }
}

// Our AES key. Note that is the same that is used on the Node-Js side but as hex bytes.
byte key[] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

// The unitialized Initialization vector
byte my_iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

String aes_encrypt(String msg)
{
  char b64data[200];
  byte cipher[1000];
  byte iv[N_BLOCK];

  // Our message to encrypt. Static for this example.
  //String msg = "{\"data\":{\"value\":300}, \"SEQN\":700 , \"msg\":\"IT WORKS!!\" }";

  aes.set_key(key, sizeof(key)); // Get the globally defined key
  gen_iv(my_iv);                 // Generate a random IV

  // Print the IV
  base64_encode(b64data, (char *)my_iv, N_BLOCK);
  // Serial.println(" IV b64: " + String(b64data));

  String IVb64 = String(b64data);

  // Serial.println(" Message: " + msg);

  int b64len = base64_encode(b64data, (char *)msg.c_str(), msg.length());
  // Serial.println(" Message in B64: " + String(b64data));
  // Serial.println(" The lenght is:  " + String(b64len));

  // For sanity check purpose
  //base64_decode( decoded , b64data , b64len );
  //Serial.println("Decoded: " + String(decoded));

  // Encrypt! With AES128, our key and IV, CBC and pkcs7 padding
  aes.do_aes_encrypt((byte *)b64data, b64len, cipher, key, 128, my_iv);

  // Serial.println("Encryption done!");

  // Serial.println("Cipher size: " + String(aes.get_size()));

  base64_encode(b64data, (char *)cipher, aes.get_size());
  // Serial.println("Encrypted data in base64: " + String(b64data));

  // Serial.println("Done...");
  return String(b64data) + " : " + IVb64;
}

//void aes_decrypt(String recieved)
//{
//  String b64data = "";
//  int i;
//  for(i=0;recieved[i]!=' ',i++){
//    b64data += recieved[i];
//  }
//  i+=3;
//  String IV_base64 = "";
//  while(i!=recieved.length()){
//    IV_base64 += recieved[i];
//    i++;
//  }
//  char data_decoded[300];
//  char iv_decoded[300];
//  byte out[300];
//  char temp[300];
//  b64data.toCharArray(temp, 300);
//  base64_decode(data_decoded, temp, b64data.length());
//  IV_base64.toCharArray(temp, 300);
//  base64_decode(iv_decoded, temp, IV_base64.length());
//  aes.do_aes_decrypt((byte *)data_decoded, aes.get_size(), out, key, 128, (byte *)iv_decoded);
//  char message[msg.length()];
//  base64_decode(message, (char *)out, aes.get_size());
//  for (int i = 0; i < aes.get_size(); i++)
//  {
//    char curChar = (char)message[i];
//    if (curChar != '}')
//      temp[i] = curChar;
//    else
//    {
//      temp[i] = curChar;
//      temp[i+1] = '\0';
//      break;
//    }
//  }
//  String result = String((char *)temp);
//  Serial.println(result);
//}
