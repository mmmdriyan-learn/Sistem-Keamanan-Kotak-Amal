//include library CTBot
#include "CTBot.h";

//variabel untuk bot telegram
CTBot myBot;

//konfigurasi koneksi ke wifi
String ssid = "raynetwork";
String pass = "Lasjaw99";

//variabel token dan id telegram;
String token = "5323863907:AAHPMSsLS6Kufp85h3nTvWmSsJH6eyblgIo";
const int id = 1347245848;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  delay(3000);
  Serial.println("Memulai telegram bot. koneksi ke wifi:");

  //koneksi ke wifi
  myBot.wifiConnect(ssid, pass);

  //set token telegram
  myBot.setTelegramToken(token);

  //cek koneksi wifi
  if(myBot.testConnection())
  Serial.println("koneksi berhasil");
  else
  Serial.println("koneksi gagal");
  myBot.sendMessage(id,"Sistem Keamanan terhubung ke Bot");
}

void loop() {
  TBMessage msg;
  String data = "";
  //selama data serial ada
  while (Serial.available() > 0) {
    //ambil data serial, masukan ke variabel
    data += char(Serial.read());
  }
  //membuang spasi data
  data.trim();
  //tampilkan di serial monitor
  Serial.println(data);
  myBot.sendMessage(id, data);
  delay(500);
  
}
