/*
Program passbox
*/

#include <Ticker.h>
Ticker ticker;

// digital pin D34 akan mendeteksi ada tidaknya logam, LED D5 akan mati (bernilai 0) jika sensor mendeteksi logam
int fr_door_sensor = 34;

// digital pin D32 akan mendeteksi ada tidaknya logam, LED D7 akan mati (bernilai 0) jika sensor mendeteksi logam
int rr_door_sensor = 32;

// digital pin D35 akan mendeteksi ada tidaknya tangan, LED D6 akan hidup (bernilai 1) jika sensor mendeteksi tangan
int fr_hand_sensor = 35;

// digital pin D33 akan mendeteksi ada tidaknya tangan, LED D8 akan hidup (bernilai 1) jika sensor mendeteksi tangan
int rr_hand_sensor = 33;

// setup pin D27 for fr_lock
int fr_lock = 27;

// setup pin D14 for rr_lock
int rr_lock = 14;

// fr hand sensor hanya boleh sekali, ketika sudah dibuka akan bernilai false
bool bool_fr_hand_sensor = true;
// fr hand sensor hanya boleh sekali, ketika sudah dibuka akan bernilai false
bool bool_rr_hand_sensor = true;

// indikator led
int fr_lamp = 19;
int fr_uv = 21;
int fr_go = 23;
int fr_wait = 22;
int rr_lamp = 2;
int rr_uv = 4;
int rr_go = 18;
int rr_wait = 5;

// lampu led dan uv AC
int uv = 12; 
int led = 13;

bool menutup_dari_depan = false;
bool menutup_dari_belakang = false;

int inisialisasi_pintu_depan = 0;

int count_waktu_tunggu_uv = 0;
int batas_count_waktu_tunggu_uv = 0;
int waktu_tunggu = 0;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); // baudrate untuk komunikasi serial

  // mengatur pin fr_door_sensor menjadi input:
  pinMode(fr_door_sensor, INPUT);

  // mengatur pin rr_door_sensor menjadi input:
  pinMode(rr_door_sensor, INPUT);

  // mengatur pin fr_hand_sensor menjadi input:
  pinMode(fr_hand_sensor, INPUT);

  // mengatur pin rr_hand_sensor menjadi input:
  pinMode(rr_hand_sensor, INPUT);

  // D27 -- D35 (fr_hand_sensor)
  pinMode(fr_lock, OUTPUT);

  // D14
  pinMode(rr_lock, OUTPUT);

  // indikator
  pinMode(fr_lamp, OUTPUT);
  pinMode(fr_uv, OUTPUT);
  pinMode(fr_go, OUTPUT);
  pinMode(fr_wait, OUTPUT);

  pinMode(rr_lamp, OUTPUT);
  pinMode(rr_uv, OUTPUT);
  pinMode(rr_go, OUTPUT);
  pinMode(rr_wait, OUTPUT);

  pinMode(uv, OUTPUT);
  pinMode(led, OUTPUT);

  ticker.attach(1,door_lock);

}


void loop() {
  // put your main code here, to run repeatedly:

  // baca input pin fr_door_sensor:
  int bacaan_fr_door_sensor = digitalRead(fr_door_sensor); // LED D5 akan mati (bernilai 0 / keadaan ditutup) jika sensor mendeteksi logam

  // baca input pin rr_door_sensor:
  int bacaan_rr_door_sensor = digitalRead(rr_door_sensor); // LED D7 akan mati (bernilai 0 / keadaan ditutup) jika sensor mendeteksi logam

  // baca input pin fr_hand_sensor:
  int bacaan_fr_hand_sensor = digitalRead(fr_hand_sensor); // LED D6 akan hidup (bernilai 1) jika sensor mendeteksi tangan

  // baca input pin rr_hand_sensor:
  int bacaan_rr_hand_sensor = digitalRead(rr_hand_sensor); // LED D8 akan hidup (bernilai 1) jika sensor mendeteksi tangan

  if (bacaan_fr_door_sensor && !bacaan_rr_door_sensor) {
    // pada saat terbuka dari depan, jika fr door sensor terbuka maka D19 fr_lamp, D23 fr_go, D5 rr_wait, dan D2 rr_lamp menyala
    digitalWrite(fr_lamp, HIGH);
    digitalWrite(rr_lamp, HIGH);
    digitalWrite(rr_wait, HIGH);
    digitalWrite(fr_go, HIGH);

    digitalWrite(fr_wait, LOW);
    digitalWrite(fr_uv, LOW);
    digitalWrite(rr_uv, LOW);
    digitalWrite(rr_go, LOW);

    digitalWrite(led, HIGH);
    digitalWrite(uv, LOW);

    menutup_dari_depan = false;
    menutup_dari_belakang = false;

    // if (bacaan_fr_hand_sensor) {
    //   if ((batas_count_waktu_tunggu_uv  - count_waktu_tunggu_uv) < 3) {
    //     if (inisialisasi_pintu_depan < 3) {
    //       // tanda bisa nyeting lampu go kedip2
    //       inisialisasi_pintu_depan = inisialisasi_pintu_depan + 1;
    //       digitalWrite(fr_go, LOW);
    //       delay(100);     
    //     } else {
    //       digitalWrite(fr_go, HIGH);
    //       delay(100);
    //       count_waktu_tunggu_uv = count_waktu_tunggu_uv + 1;
    //       batas_count_waktu_tunggu_uv = batas_count_waktu_tunggu_uv + 1;
    //       digitalWrite(fr_go, LOW);
    //       delay(100);
    //     }
    //   } else {
    //     waktu_tunggu = count_waktu_tunggu_uv;
    //     Serial.println(waktu_tunggu);
    //     digitalWrite(fr_go, HIGH);
    //     delay(100);
    //   }
    // } else {
    //   digitalWrite(fr_go, HIGH);
    //   delay(100);
    //   batas_count_waktu_tunggu_uv = batas_count_waktu_tunggu_uv + 1;
    //   digitalWrite(fr_go, LOW);
    //   delay(100); 
    // }


  } else if (!bacaan_fr_door_sensor && bacaan_rr_door_sensor) {
    // pada saat salah satu pintu terbuka, jika rr door sensor terbuka maka D2 rr_lamp, D18 rr_go, D22 fr_wait, dan D19 fr_lamp menyala
    digitalWrite(rr_lamp, HIGH);
    digitalWrite(rr_go, HIGH);
    digitalWrite(fr_wait, HIGH);
    digitalWrite(fr_lamp, HIGH);
    digitalWrite(led, HIGH);

    digitalWrite(fr_go, LOW);
    digitalWrite(rr_wait, LOW);
    digitalWrite(rr_uv, LOW);
    digitalWrite(fr_uv, LOW);

    digitalWrite(uv, LOW);

    menutup_dari_depan = false;
    menutup_dari_belakang = false;
    
  } else if (!bacaan_rr_door_sensor && !bacaan_fr_door_sensor) {
    // pada saat semua pintu tertutup (menutup dari pintu depan), fr_go, fr_uv, rr_go, dan rr_uv menyala
    if (menutup_dari_depan) {
      digitalWrite(fr_go, HIGH);
      digitalWrite(rr_go, HIGH);
      digitalWrite(fr_uv, HIGH);
      digitalWrite(rr_uv, HIGH);
      digitalWrite(uv, HIGH);

      digitalWrite(fr_lamp, LOW);
      digitalWrite(rr_wait, LOW);
      digitalWrite(rr_lamp, LOW);
      digitalWrite(fr_wait, LOW);
      
      digitalWrite(led, LOW);

      delay(2400000); // atur, aslinya 40 menit
      // count_waktu_tunggu_uv = 0;
      // batas_count_waktu_tunggu_uv = 0;
      // waktu_tunggu = 0;

      digitalWrite(uv, LOW);
      digitalWrite(fr_uv, LOW);
      digitalWrite(rr_uv, LOW);

      menutup_dari_depan = false;
      menutup_dari_belakang = false;

    } 
    if(menutup_dari_belakang) { //  menutup_dari_belakang
      // pada saat semua pintu tertutup (menutup dari pintu belakang), fr_go dan rr_go menyala
      digitalWrite(fr_go, HIGH);
      digitalWrite(rr_go, HIGH);
      
      digitalWrite(fr_lamp, LOW);
      digitalWrite(rr_wait, LOW);
      digitalWrite(rr_lamp, LOW);
      digitalWrite(fr_wait, LOW);
      digitalWrite(fr_uv, LOW);
      digitalWrite(rr_uv, LOW);

      digitalWrite(uv, LOW);
      digitalWrite(led, LOW);

      menutup_dari_depan = false;
      menutup_dari_belakang = false;
    }

  } else {
    // membuka semua
    digitalWrite(uv, LOW);
    digitalWrite(led, LOW);
      
    // pada saat kedua pintu terbuka muncul lampu emergensi
    digitalWrite(fr_lamp, HIGH);
    digitalWrite(fr_go, HIGH);
    digitalWrite(rr_wait, HIGH);
    digitalWrite(rr_lamp, HIGH);
    digitalWrite(rr_uv, HIGH);

    digitalWrite(fr_uv, HIGH);
    digitalWrite(rr_lamp, HIGH);
    digitalWrite(rr_go, HIGH);
    digitalWrite(fr_wait, HIGH);
    digitalWrite(fr_lamp, HIGH);

    delay(500);

    digitalWrite(fr_lamp, LOW);
    digitalWrite(fr_go, LOW);
    digitalWrite(rr_wait, LOW);
    digitalWrite(rr_lamp, LOW);
    digitalWrite(rr_uv, LOW);

    digitalWrite(fr_uv, LOW);
    digitalWrite(rr_lamp, LOW);
    digitalWrite(rr_go, LOW);
    digitalWrite(fr_wait, LOW);
    digitalWrite(fr_lamp, LOW);

  }

  if (bacaan_fr_hand_sensor && bool_fr_hand_sensor && bool_rr_hand_sensor && !bacaan_fr_door_sensor) {
    bool_fr_hand_sensor = false; // sudah membuka (hanya untuk satu kali)
    delay(500);
    digitalWrite(fr_lock, HIGH);
    delay(50);
    digitalWrite(fr_lock, LOW);
  } 

  if (bacaan_rr_hand_sensor && bool_rr_hand_sensor && bool_fr_hand_sensor && !bacaan_rr_door_sensor) {
    bool_rr_hand_sensor = false;
    delay(500);
    digitalWrite(rr_lock, HIGH);
    delay(50);
    digitalWrite(rr_lock, LOW);
  }

  // ketika pintu fr ditutup
  if (bacaan_fr_door_sensor && !bacaan_rr_door_sensor) {
    menutup_dari_depan = true;
    bool_fr_hand_sensor = false; // tidak dapat dibuka lagi karena sudah terbuka
    delay(3000);
  } else {
    // keadaan menutup
    bool_fr_hand_sensor = true;
  }

  // ketika pintu rr ditutup
  if (bacaan_rr_door_sensor && !bacaan_fr_door_sensor) {
    menutup_dari_belakang = true;
    bool_rr_hand_sensor = false; // tidak dapat dibuka lagi karena sudah terbuka
    delay(3000);
  } else {
    // keadaan menutup
    bool_rr_hand_sensor = true;
  }

  delay(1000);

}

void door_lock() {
  // ESP.rest*`
  ``+art();
  if (digitalRead(fr_hand_sensor)) {
    bool_fr_hand_sensor = false; // sudah membuka (hanya untuk satu kali)
    delay(500);
    digitalWrite(fr_lock, HIGH);
    delay(50);
    digitalWrite(fr_lock, LOW);
  } else if (digitalRead(rr_hand_sensor)) {
    bool_rr_hand_sensor = false; // sudah membuka (hanya untuk satu kali)
    delay(500);
    digitalWrite(rr_lock, HIGH);
    delay(50);
    digitalWrite(rr_lock, LOW);
  }
}