float t;
int h;
void updatehcsr() {
  for (int x = 0; x < pinsayisi; x++) {

    if (pinsignaltype[x] == "HCE") {
      hcsr04loop(x);
    }
  }
}


void updateinput() {  //degisenler="";
  for (int x = 0; x < pinsayisi; x++) {

    if (pinmode[x] == "INP") {
      if (pinsignaltype[x] == "ANG") {
        //delay(5);
        int analogokunandeger = analogRead(getGPIOPin(pinname[x]));
        if (pinlabel[x].indexOf("DC") > -1) {
          float dcv;
          if (analogokunandeger < 200) {
            dcv = analogokunandeger * 0.015;

          } else if (analogokunandeger >= 200 && analogokunandeger < 231) {
            dcv = 2.80 + ((analogokunandeger - 200) / 18.5);

          } else if (analogokunandeger >= 231 && analogokunandeger < 250) {
            dcv = 4.52 + ((analogokunandeger - 231) / 14.4);

          } else if (analogokunandeger >= 250 && analogokunandeger < 260) {
            dcv = 5.50 + ((analogokunandeger - 250) / 14.4);

          } else if (analogokunandeger >= 260 && analogokunandeger < 331) {
            dcv = 6.47 + ((analogokunandeger - 260) / 15.6);

          } else {
            analogokunandeger = analogokunandeger - 180;
            dcv = analogokunandeger * 0.0737;
          }
          //Serial.print("değer: "); Serial.print(analogokunandeger); Serial.print(" Volt karşılığı: ");Serial.println(dcv);

          PinState[x] = dcv;
        } else PinState[x] = analogokunandeger;
      }

      if (pinsignaltype[x] == "DIG") {
        //delay(5);
        PinState[x] = digitalRead(getGPIOPin(pinname[x]));
      }

      if (pinsignaltype[x] == "HCE") {
        hcsr04loop(x);
      }

      if (pinsignaltype[x].indexOf("DHT") > -1 && DHTA[x] != NULL) {
        dhtsayac -= 1;
        if (dhtsayac < 1 || dhtsayac == 0) {

          //Serial.println("DHTDONGUSU");
          dhtsayac = 5;
          //DHT
          delay(30);
          t = DHTA[x]->readTemperature();  // boş iken  celcius // f = DHTA.readTemperature(true); fahrenayt
          delay(30);
          h = DHTA[x]->readHumidity();
          delay(30);
          //Serial.print("t: ") ;Serial.println(t);
          //Serial.print("h: ") ;Serial.println(h);
          // Check if any reads failed and exit early (to try again).
          if (isnan(h) || isnan(t))  // || isnan(f)
          {
            pinayarerrorlog = "DHT sensor okunamadı";
            dhtsayac == 1;
            // FAKE bilgi gönder

            PinState[x] = "t200h200";
            //Serial.println("DHT okunamadı.");

          } else {

            // sıcaklık hatası düzelt
            float tvehata = t + pinval[x].toFloat();
            String ts = (String)tvehata;

            // nem hatası düzelt
            int hvehata = h + pinmaxvalue[x].toInt();
            String hs = (String)hvehata;

            // kısa veri gönder
            if (pinminvalue[x] == "0") ts = ts.substring(0, ts.length() - 1);
            if (pinminvalue[x] == "0") hs = hs.substring(0, hs.length() - 1);

            if (pinminvalue[x] == "1") ts = ts.substring(0, ts.length() - 3);
            if (pinminvalue[x] == "1") hs = hs.substring(0, hs.length() - 1);

            PinState[x] = "t" + ts + "h" + hs;
            /*
                                        Serial.print(("Nem: "));
                                        Serial.print(hs);
                                        Serial.print(("0%  Sıcaklık: "));
                                        Serial.print(ts);
                                        Serial.print((" C"));
                                        Serial.println(".");
                                        */
          }
          //Serial.println("DHTDONGUSU-çıkış");
        }
      }

      if (ePinState[x] != PinState[x]) {
        // ------------------------------------------------=================
        // 1. DURUM: SERVO MOTOR KONTROLÜ (ÇIKIŞ - OUTPUT)
        // ------------------------------------------------=================
  if (servovar[x] && myservo[x] != NULL) 
  {
    // HANGİ DEĞERİN GELDİĞİNİ GÖRMEK İÇİN DEBUG LOGU EKLEYİN:
    Serial.printf_P(PSTR("[DEBUG] Servo tetiklendi! Gelen string: '%s'\n"), PinState[x].c_str());

    int aciDeperi = constrain(PinState[x].toInt(), 0, 180); 
    
    Serial.printf_P(PSTR("[DEBUG] Motora yazilan aci: %d\n"), aciDeperi);

    myservo[x]->write(aciDeperi); 
    
    psco = true; 
    ePinState[x] = PinState[x]; 
  }
        // ------------------------------------------------=================
        // 2. DURUM: SENSÖR VE GİRİŞ SİNYAL TİPLERİ (GİRİŞ - INPUT)
        // ------------------------------------------------=================
        else if (pinsignaltype[x].indexOf("DH") > -1) {
          psci = true;  // Giriş değişti bayrağı
          ePinState[x] = PinState[x];
        } else if (pinsignaltype[x] == "HCE" && PinState[x].toInt() > 2) {
          psci = true;
          ePinState[x] = PinState[x];
        } else if (pinsignaltype[x] == "ANG") {
          if (pinlabel[x].indexOf("DC") > -1) {
            float epinstatef = ePinState[x].toFloat();
            float pinstatef = PinState[x].toFloat();

            if (epinstatef != pinstatef) {
              psci = true;
              ePinState[x] = PinState[x];
            }
          } else {
            int epinstate = ePinState[x].toInt();
            int pinstate = PinState[x].toInt();
            if (abs(epinstate - pinstate) > 2) {
              psci = true;
              ePinState[x] = PinState[x];
            }
          }
        }
        // Diğer standart dijital giriş/çıkış durumları için genel karar:
        else {
          // Eğer bu genel else bloğu bir röle çıkışını tetikliyorsa buraya da psco = true diyebilirsiniz,
          // buton gibi bir giriş ise psci = true kalabilir.
          psci = true;
          ePinState[x] = PinState[x];
        }
      }
    }
  }
}

/*
void updatefbvirtual()
{
                for (int fbg=0;fbg<6;fbg++)
                {
                  if(fbc[fbg].length()>0)
                  {
                    //Serial.println("fbcyol: " + fbcyol[fbg]);
                    //Serial.println("fbtd[fbg] " + fbtd[fbg] + "     efbtd[fbg]" +efbtd[fbg] );
                    if(fbtd[fbg]!=efbtd[fbg])
                    {
                      fbbaskacihazagonder(fbcyol[fbg], fbtd[fbg], fbg);
                    }
                  }
                }
}
*/

void updateoutput() {

  for (int x = 0; x < pinsayisi; x++) {

    int mod = bitRead(GPE, 5);

    if (mod == 1) {
      //Serial.println("Pin D1 şu an modu: OUTPUT");
    } else {
      //Serial.println("Pin D1 şu an modu: INPUT");
    }


    if (pinmode[x] == "OUT") {
      String pinstatesakla;

      if (acildeyim[x] == true) {
        pinstatesakla = PinState[x];
        PinState[x] = acildeger[x];
      }

      if (pinsignaltype[x] == "PWM") {
        int PWMdegerint = PinState[x].toInt();
        Outpwm(pinname[x], PWMdegerint);
      }
      if (pinsignaltype[x] == "DIG") {
        bool yildizli;
        if (pinlabel[x].indexOf("*") + 1 == pinlabel[x].length()) yildizli = true;
        else yildizli = false;
        if (PinState[x] == "0" || PinState[x] == "") {
          if (yildizli == false) digitalWrite(getGPIOPin(pinname[x]), LOW);
          else digitalWrite(getGPIOPin(pinname[x]), HIGH);
        } else if (PinState[x] == "1" || PinState[x] == "ON") {
          if (yildizli == false) digitalWrite(getGPIOPin(pinname[x]), HIGH);
          else digitalWrite(getGPIOPin(pinname[x]), LOW);
        }
        //Serial.println("\npinmod: >>");
      }


      if (ePinState[x] != PinState[x]) {
        Serial.println("Farklı olan e: " + pinname[x] + " " + ePinState[x] + " >> " + PinState[x]);

        if (pinmode[x] == "OUT") {
          psco = true;
          //pindurumrecyap=true;
          ePinState[x] = PinState[x];
          //degisenler += pinname[x] + ":" + PinState[x]+ "|" + pinlabel[x] + ",";
        }
      }

      if (acildeyim[x] == true) {
        PinState[x] = pinstatesakla;
      }
    }
    /*
              if(pindurumrecyap==true)
              {
                dosyayazpindurum();
              }
*/
    pinuygula();
  }

  /*
      if(degisenler!="" && degisenler!= edegisenler){
        if(htServerip.length()>3)sendserver80(htServerip, "8080", degisenler);

      }*/
}
