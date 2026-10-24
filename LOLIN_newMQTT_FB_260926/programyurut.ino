unsigned long milis100;
int degisecekno = -100;

int satirsayisip;

bool ifkiyas[5];
bool vevar[5];
bool ifsonuc;
uint8_t kiyassayisi;
String parantezici;
int aktifpinno;

u_int8_t susluparantezsayisi = 0;
String ptm;

bool standartprogram=false;
bool altProgramIptalBayragi = false; // Standart programdan gelen anlık DUR emri için

String PROGNOX;

void programrun() {
  standartprogram=true;
  satirsayisip = 0;
  String pdt = programdata;
  pdt.toUpperCase();
  if (pdt.indexOf("ACLTUT") < 0) ACLilanciyim = false;
  pdt = "";
  for (int i = 0; i <10; i++)  // İLK 10 SAYI 10 PİN İSİM VE DEĞERLERİNİ AL degis degdeg e yaz
  {
    if (programdata.substring(0, 1) == "\n") programdata = programdata.substring(1, programdata.length());  //arada programdata için temizlik yap
    degis[i] = pinname[i];
    bool yildizli;
    if (pinlabel[i].indexOf("*") + 1 == pinlabel[i].length()) yildizli = true;
    else yildizli = false;
    if (yildizli == false) degdeg[i] = PinState[i];
    else {
      if (PinState[i] == "1") degdeg[i] = "0";
      else degdeg[i] = "1";
    }

    /* firebase için
    if(i<6)
    {
      fbc[i] = "";
      fbcyol[i] = "";
      fbtd[i] = "";
      mqc[i] = "";
    }
     firebase son
    */
  }
  /*
  for (int i = 11; i < 21; i++) {
    degdeg[i] = "";
    degis[i] = "";
  }
  */

  if (programdata.length() < 4) return;

  // timer değişkenleri için
  if(milis100<millis()){
  for (int ii = 0; ii <10; ii++) {
     if (tmris[ii].length() > 0) {
        if (tmrdeg[ii].toFloat() > 0) {
        tmrdeg[ii] = String(tmrdeg[ii].toFloat() - 1);
      }
     }
  }
    milis100=millis()+100;
  }



  progmsg = "";

  programerrorlog = "";
  ptm = programdata;


  for (int q = 1; q < 200; q++) {
    satirsayisip += 1;
    if (ptm.length() < 3) break;
    String spacesil = ptm;
    spacesil.trim();
    if (spacesil.indexOf("\n") > 0 && spacesil.indexOf("\n") < 2) break;
    
    satirp = ptm.substring(0, ptm.indexOf("\n"));
    
    satirislem();
    
    if (ptm.indexOf("\n") > -1) ptm = ptm.substring(ptm.indexOf("\n") + 1, ptm.length());
    else break;
    
    //Serial.println("satirp "); Serial.println(satirp);
    yield();
  }



  // en son işlem burası //////////////////////////////////

  if (ACL.toInt() < 6) {
    if (eACL.toInt() != ACL.toInt()) {
      for (int sta = 0; sta <10; sta++) {
        if (acilseviyesi[sta] != " ") {
          String pinmodesta = pinmode[sta];
          pinmodesta.toUpperCase();
          if (pinmodesta.indexOf("OU") == 0) {
            if (ACL.toInt() > acilseviyesi[sta].toInt()) {
              acildeyim[sta] = true;
            } else acildeyim[sta] = false;
          }
        }
      }
      eACL = ACL;
    }
  }
standartprogram=false;
}

void satirislem() {
  String satiruppercase = satirp;
  satiruppercase.toUpperCase();



  //Serial.println(satiruppercase);
  // if için ///////////////////////////////////////////

  if (satiruppercase.indexOf("//") == 0) return;
  //Serial.println("SATIRIM:" + satiruppercase);

  //if (satiruppercase.indexOf("FBC ") == 0) firebasecihaztanitimi();  firebase için
  if (satiruppercase.indexOf("TMR ") == 0) timertanitimi();
  else if (satiruppercase.indexOf("DEG ") == 0) degiskentanitimi();
  else if (satiruppercase.indexOf("MQC ") == 0) mqttcihaztanitimi();
  else if (satiruppercase.indexOf("IF") == 0) gotoif();
  else if (satiruppercase.indexOf("RXTX") == 0 && !rxtxvar) rxtxsetup();
  else if (satiruppercase.indexOf("ACLTUT") == 0) ACLilanciyim = true;
  else if (satiruppercase.indexOf(";") > -1) {
    ifparantezdisi(satiruppercase, satirsayisip);
  } else return;
  // ????????????????????????????????????????????? sonra ne yaparız bilmem
}



void timertanitimi() {
  for (int ii = 0; ii <10; ii++) {
    if (satirp.indexOf("=") > 0) {

        if(tmris[ii] == satirp.substring(4, satirp.indexOf("=")))
        {
        break;
        }

        if (tmris[ii] == "" || tmris[ii] == "null") {
          String tmrstr = satirp;
          tmris[ii] = tmrstr.substring(4, tmrstr.indexOf("="));
          tmrdeg[ii] = tmrstr.substring(tmrstr.indexOf("=") + 1, tmrstr.indexOf(";"));
          break;
        }
    } else {
      progmsg += "TMR hatası. TMR TIMER1=10; şeklinde olmalı. satır no: " + (String)satirsayisip + " komut: " + satirp + "\n";
    }
  }
}



void degiskentanitimi() {
  Serial.println("degisken tanımına gelen satirp:" +  satirp );
  for (int ii = 11; ii < 21; ii++) {
    if (satirp.indexOf("=") > 0) {
      if (degis[ii] == "" || degis[ii] == "null" || degis[ii] == satirp.substring(4, satirp.indexOf("="))) {
        String degisdegdeg = satirp;
        degis[ii] = degisdegdeg.substring(4, degisdegdeg.indexOf("="));
        degdeg[ii] = degisdegdeg.substring(degisdegdeg.indexOf("=") + 1, degisdegdeg.indexOf(";"));
        Serial.print("[ii]:"  + String(ii) + " degis[ii]:" + degis[ii] + " degdeg[ii]:" + degdeg[ii]);
        break;
      }
    } else {
      progmsg += "DEG hatası. DEG SAYI1=1; şeklinde olmalı. satır no: " + (String)satirsayisip + " komut: " + satirp + "\n";
    }
  }
}



void mqttcihaztanitimi() {
  //Serial.println("FBCihaz tanıtımı");
  for (int ii = 0; ii < 6; ii++) {
    if (satirp.indexOf("=") > 5) {
      if (mqc[ii] == "" || mqc[ii] == "null" || mqc[ii] == satirp.substring(4, satirp.indexOf("="))) {
        String mqcvemqcyol = satirp;
        mqc[ii] = mqcvemqcyol.substring(4, mqcvemqcyol.indexOf("="));
        mqcyol[ii] = mqcvemqcyol.substring(mqcvemqcyol.indexOf("=") + 1, mqcvemqcyol.indexOf(";"));
        break;
      }
    } else {
      progmsg += "mqc hatası. mqc mq1=/yol/cihaz; şeklinde olmalı. satır no: " + (String)satirsayisip + " komut: " + satirp + "\n";
    }
  }
}



bool paranteziciislem(String parantezi) {
  String sol = "";
  String sag = "";
  String solstate;

  bool ifsonucu = false;

  int op = 0;
  if (parantezi.indexOf("==") > -1) {
    op = 1;
    sol = parantezi.substring(0, parantezi.indexOf("=="));
    sag = parantezi.substring(parantezi.indexOf("==") + 2, parantezi.length());
  } else if (parantezi.indexOf(">=") > -1) {
    op = 2;
    sol = parantezi.substring(0, parantezi.indexOf(">="));
    sag = parantezi.substring(parantezi.indexOf(">=") + 2, parantezi.length());
  } else if (parantezi.indexOf(">") > -1) {
    op = 3;
    sol = parantezi.substring(0, parantezi.indexOf(">"));
    sag = parantezi.substring(parantezi.indexOf(">") + 1, parantezi.length());
  } else if (parantezi.indexOf("<=") > -1) {
    op = 4;
    sol = parantezi.substring(0, parantezi.indexOf("<="));
    sag = parantezi.substring(parantezi.indexOf("<=") + 2, parantezi.length());
  } else if (parantezi.indexOf("<") > -1) {
    op = 5;
    sol = parantezi.substring(0, parantezi.indexOf("<"));
    sag = parantezi.substring(parantezi.indexOf("<") + 1, parantezi.length());
  } else if (parantezi.indexOf("!=") > -1) {
    op = 6;
    sol = parantezi.substring(0, parantezi.indexOf("!="));
    sag = parantezi.substring(parantezi.indexOf("!=") + 2, parantezi.length());
  } else {
    programerrorlog += "if kiyaslama isareti uygun degil '==,>=,>,<,<=,!=' kullanin. Satir:" + String(satirsayisip) + "\n";
  }


  if (sol == "") { programerrorlog += "Kiyaslamada sol taraf bos! Satir:" + String(satirsayisip) + "\n"; }
  if (sag == "") { programerrorlog += "Kiyaslamada sag taraf bos! Satir:" + String(satirsayisip) + "\n"; }
  if (programerrorlog != "") return false;

  aktifpinno = -100;
     // Serial.print("sol " + sol + "\n");
  // 🎯 KRİTİK DÜZELTME: equalsIgnoreCase kullanarak harf uyuşmazlığı tamamen çözüldü
  if (sol.equalsIgnoreCase("ACL")) {
    solstate = ACL;
  } else {




    bool tempvar = false;
    bool humvar = false;
    if (sol.indexOf("t") > 0) tempvar = true;
    if (sol.indexOf("h") > 0) humvar = true;


    if (tempvar == false && humvar == false) {
      String SOLL = sol;
      SOLL.toUpperCase();

      for (int pinbul = 0; pinbul < 10; pinbul++) {
        //if(!standartprogram) Serial.println( String(pinbul) + " " + SOLL + " >< " + degis[pinbul] +  " degiskendegeri " + degdeg[pinbul]);

        if (SOLL == degis[pinbul]) {  // Büyük/küçük harf fark etmeksizin eşleşir
          solstate = degdeg[pinbul];
          aktifpinno = pinbul;
          break;
        }
      }

      for (int pb = 11; pb < 21; pb++) {
        //if(pb<14)Serial.println( String(pb) + " " + SOLL + " >< " + degis[pb] +  " degiskendegeri " + degdeg[pb]);

        if (SOLL == degis[pb]) {  // Büyük/küçük harf fark etmeksizin eşleşir
          solstate = degdeg[pb];
          if(SOLL == "PROGNO")PROGNOX=solstate;
          aktifpinno = -10;
          break;
        }
      }



    } else {
      if (tempvar == true || humvar == true) sol = sol.substring(0, sol.length() - 1);
      for (int pinbul = 0; pinbul <10; pinbul++) {
        String SOLL = sol;
        SOLL.toUpperCase();
        if (SOLL == (degis[pinbul])) {
          if (pinsignaltype[pinbul].indexOf("DHT") > -1) {
            if (tempvar == true) {
              if (degdeg[pinbul].indexOf("t") > -1) solstate = degdeg[pinbul].substring(1, degdeg[pinbul].indexOf("h"));
              else solstate = 200;
            } else {
              if (degdeg[pinbul].indexOf("h") > -1) solstate = degdeg[pinbul].substring(degdeg[pinbul].indexOf("h") + 1, degdeg[pinbul].length());
              else solstate = 200;
            }
            aktifpinno = pinbul;

            //Serial.println("SOLSTATE " + solstate);

            break;
          } else {
            programerrorlog += sol + " pininin, pinsignal type DHT değil. Satir No: " + String(satirsayisip) + " \n";
            return false;
          }
        }
      }
    }
  }

String* aktifTmrIsim = standartprogram ? tmris : utmris;
String* aktifTmrDegerDizi = standartprogram ? tmrdeg : utmrdeg; // Sizin orijinal diziniz

for (int ii = 0; ii < 11; ii++) {
  if (sol == aktifTmrIsim[ii]) {
    solstate = aktifTmrDegerDizi[ii];
    aktifpinno = -10;

    // 🎯 SİHİRLİ BAĞLANTI: 
    // O an eşleşen zamanlayıcı String hücre adresini (&) kilit pointer'ımıza eşitliyoruz!
    aktifTmrDeger = &aktifTmrDegerDizi[ii]; 
  }
}



  if (!sol.equalsIgnoreCase("ACL") && aktifpinno == -100) {
    programerrorlog += "if satır içi soldaki tanimli degil " + sol + " Satir:" + String(satirsayisip) + "\n";
    return false;
  }


  // Matematiksel Kıyaslamalar
  float solFloat = solstate.toFloat();
  float sagFloat = sag.toFloat();

  if (op == 1) {  // ==
    ifsonucu = (solFloat == sagFloat);
  } else if (op == 2) {  // >=
    ifsonucu = (solFloat >= sagFloat);
  } else if (op == 3) {  // >
    ifsonucu = (solFloat > sagFloat);
  } else if (op == 4) {  // <=
    ifsonucu = (solFloat <= sagFloat);
  } else if (op == 5) {  // <
    ifsonucu = (solFloat < sagFloat);
  } else if (op == 6) {  // !=
    ifsonucu = (solFloat != sagFloat);
  }

  /*
  Serial.print(F(" Kiyaslanacak: "));
  Serial.println(parantezi);
  Serial.print("SOLFLOAT ");
  Serial.print(solFloat);
  Serial.print("    SAĞFLOAT ");
  Serial.print(sagFloat);
  Serial.print(F(" -> Sonuc: "));
  Serial.println(ifsonucu ? F("TRUE") : F("FALSE"));
*/
  return ifsonucu;
}

void gotoif() {

  String satiruppercase = satirp;
  satiruppercase.toUpperCase();

  String sat = satirp;
  if (sat.indexOf("(") < 1) { programerrorlog += "if ten sonra ( işareti olmalı. Satır:" + (String)satirsayisip + "\n"; }
  if (sat.indexOf(")") < 1) { programerrorlog += "if te ) kapatılmamış." + (String)satirsayisip + "\n"; }
  if (programerrorlog != "") return;
  String parantezici = sat.substring(sat.indexOf("(") + 1, sat.indexOf(")"));
  parantezici.replace(" ", "");
  int kes = 0;
  int kesv = 0;
  int kesy = 0;
  if (parantezici.indexOf("&&") > -1) kesv = parantezici.indexOf("&&");
  if (parantezici.indexOf("||") > -1) kesy = parantezici.indexOf("||");

  if (kesv == 0 && kesy == 0) kes = 0;
  else if (kesv == 0 && kesy > 0) kes = kesy;
  else if (kesv > 0 && kesy == 0) kes = kesv;
  else if (kesv > 0 && kesy > 0) {
    if (kesv < kesy) kes = kesv;
    else kes = kesy;
  }


  //Serial.print("kes >>>>: ");      Serial.println(kes);
  ifsonuc = false;
  if (kes == 0) ifsonuc = paranteziciislem(parantezici);
  else {

    bool sonis = false;
    ifsonuc = false;
    for (int j = 1; j < 5; j++) {
      kes = 0;
      kesv = 0;
      kesy = 0;
      vevar[j] = false;
      ifkiyas[j] = false;
      if (parantezici.indexOf("&&") > -1) kesv = parantezici.indexOf("&&");
      if (parantezici.indexOf("||") > -1) kesy = parantezici.indexOf("||");
      if (kesv == 0 && kesy == 0) kes = 0;
      else if (kesv == 0 && kesy > 0) {
        kes = kesy;
      } else if (kesv > 0 && kesy == 0) {
        kes = kesv;
        vevar[j] = true;
      } else if (kesv > 0 && kesy > 0) {
        if (kesv < kesy) {
          kes = kesv;
          vevar[j] = true;
        }
        if (kesv > kesy) {
          kes = kesy;
          vevar[j] = false;
        }
      }


      String isyap;
      if (kes > 0) {
        isyap = parantezici.substring(0, kes);
        parantezici = parantezici.substring(kes + 2, parantezici.length());
      } else {
        isyap = parantezici;
        sonis = true;
      }

      ifkiyas[j] = paranteziciislem(isyap);

      kiyassayisi = j;
      if (sonis == true) break;
    }


    for (int ki = 1; ki < kiyassayisi; ki++) {

      if (vevar[ki] == true) {
        if (ifkiyas[ki + 1] == true && ifkiyas[ki] == true) ifkiyas[ki + 1] = true;
        else ifkiyas[ki + 1] = false;
      } else {
        if (ifkiyas[ki + 1] == true || ifkiyas[ki] == true) ifkiyas[ki + 1] = true;
        else ifkiyas[ki + 1] = false;
      }
      ifsonuc = ifkiyas[ki + 1];
    }
  }


  if (ifsonuc == true) {
    ifparantezdisi(satiruppercase, satirsayisip);
  } else if(standartprogram==true){
    if (satiruppercase.indexOf("{") > -1) {
      susluparantezsayisi += 1;
      if (satiruppercase.indexOf("}") > -1) susluparantezsayisi -= 1;
    }

    if (susluparantezsayisi > 0) { //süslü parantez arama başlangıcı
      satirsayisip -= 1;

      for (int q = satirsayisip; q < 200; q++) {
        if (ptm.indexOf("\n") > -1) ptm = ptm.substring(ptm.indexOf("\n") + 1, ptm.length());
        satirp = ptm.substring(0, ptm.indexOf("\n"));
        satirsayisip += 1;
        if (satirp.indexOf("{") > -1) {
          susluparantezsayisi += 1;
        }
        if (satirp.indexOf("}") > -1) {
          susluparantezsayisi -= 1;
        }

        if (ptm.length() < 1) {
          susluparantezsayisi = 0;
          break;
        }

        if (susluparantezsayisi == 0) {
          break;  // son süslüparantezi } 
        }
      }
    }
  }
}





void ifparantezdisi(String satiruppercase, int satirsayisip) {
  String yapilacaklar;
  String yapilacakis;
  int yapilacaklarislemsayisi;
  String parantezdisisol;
  String parantezdisisag;
  //String

  satiruppercase.toUpperCase();
  yapilacaklar = satiruppercase;

  if (yapilacaklar.indexOf("{") > -1) {
    //Serial.print("ptm:");Serial.println(ptm);
    yapilacaklar = yapilacaklar.substring(ptm.indexOf("{") + 1, yapilacaklar.indexOf("}"));
    //  Serial.print("yapilacaklar:");
    //  Serial.println(yapilacaklar);
    yapilacaklar.replace("\n", "");
  }


  // boşlukları sil
  if (yapilacaklar.indexOf(";") > -1) {



    for (int i = 0; i <10; i++) {

      if (yapilacaklar.indexOf(" ") == 0) {
        for (int j = 0; j < 200; j++) {
          yapilacaklar = yapilacaklar.substring(1, yapilacaklar.length());
          if (yapilacaklar.indexOf(" ") != 0) break;
        }
      }  //baştaki boşlukları sil


      yapilacakis = yapilacaklar.substring(0, yapilacaklar.indexOf(";"));
      if (yapilacakis.indexOf("{") == 0) yapilacakis = yapilacakis.substring(1, yapilacakis.length());
      yapilacaklarislemsayisi = i;

      // tam burada yapılacak hesabına git
      String yais = yapilacakis;
      yais.toUpperCase();
          if(yais.indexOf("BUZ>")==0)
          {
            String pns=yais.substring(yais.indexOf(">")+1,yais.length());
            play(pns);
            return;
          }

      if (yais.indexOf("MQTT:") == 0) {
        if (habp > 0) {

          int hane = 0;
          if (yapilacakis.indexOf("=") > -1) {

            String mqyoltmp = yapilacakis.substring(5, yapilacakis.indexOf("="));

            for (int ii = 0; ii < 6; ii++) {

              if (mqc[ii] == mqyoltmp) {
                mqyoltmp = mqcyol[ii];
                break;
              }
              if (mqc[ii] == "" || mqc[ii] == "null") break;
            }

            String mdegisenlertmp = yapilacakis.substring(yapilacakis.indexOf("=") + 1, yapilacakis.length());
            String degisenmqtmp = YOL + "/" + esphostname + "=" + mdegisenlertmp + ",";

            String ACLtmp = "";
            if (degisenmqtmp.indexOf("ACL:") > -1) {
              ACLtmp = degisenmqtmp.substring(degisenmqtmp.indexOf("ACL:") + 4, degisenmqtmp.length());
              if (ACLtmp.indexOf(",") > 0) ACLtmp = ACLtmp.substring(0, ACLtmp.indexOf(","));
              if (ACLtmp.indexOf(";") > 0) ACLtmp = ACLtmp.substring(0, ACLtmp.indexOf(";"));
              if (ACLtmp.toInt() != ACL.toInt()) { ACL = ACLtmp; }
            }



            for (int y = 1; y < 6; y++) {

              if (mqyoltmp == mqyol[y]) {
                if (degisenmq[y] != "" && degisenmqtmp == degisenmq[y]) {

                  return;
                } else {
                  hane = y;
                  break;
                }
              }

              if (mqyol[y] == "") {
                hane = y;
                break;
              }
            }

            if (habp == 1 || habp == 2) mqttsend(mqyoltmp, degisenmqtmp);
            mqyol[hane] = mqyoltmp;
            degisenmq[hane] = degisenmqtmp;
            delay(1);
          }
        }
      } else if (yais.indexOf("HTTP:") == 0) {
        htyolla = yapilacakis.substring(0, yapilacakis.indexOf(";"));
        httpgonder();
        delay(50);
      } else
       { 
        yapilacakis.replace(" ","");
        if(yapilacakis.indexOf("}") != 0) yap(yapilacakis, yapilacaklarislemsayisi);
       }
      
      yapilacaklar = yapilacaklar.substring(yapilacaklar.indexOf(";") + 1, yapilacaklar.length());
      if (yapilacaklar.length() < 1) { break; }
    }
  } else {
    if (yapilacaklar.length() > 3) {
      programerrorlog += (String)satirsayisip + ". satırda / satır sonunda ';' unutulmuş.\n";
      return;
    }
  }
}

void yap(String yapilacakis, int islemno) {

  String noktalivirgulekadar = yapilacakis;
  String dsol = noktalivirgulekadar.substring(0, noktalivirgulekadar.indexOf("="));
  if (dsol.indexOf("{") > -1) dsol = dsol.substring(dsol.indexOf("{") + 1, dsol.length());
  dsol.toUpperCase();
  String dsag = noktalivirgulekadar.substring(noktalivirgulekadar.indexOf("=") + 1, noktalivirgulekadar.indexOf(";"));
      aktifpinno = -100;
 

String* aktifTmrIsim = standartprogram ? tmris : utmris;
String* aktifTmrDeger = standartprogram ? tmrdeg : utmrdeg;

for (int ii = 0; ii <10; ii++) {
  // void yap içinde aranan sol taraf değişkeni 'dsol'dur
  if (dsol == aktifTmrIsim[ii]) { 
    
    // 🎯 KRİTİK DÜZELTME: Geçici solstate yerine doğrudan dizinin hafızasına yazıyoruz!
    aktifTmrDeger[ii] = dsag; 
    
    return; // Atama yapıldı, fonksiyondan güvenle çıkabiliriz
  }
}

  
  
  if (dsol == "ACL") {
    ACL = dsag;
    return;
  }


// void yap(String yapilacakis, int islemno) fonksiyonunun içi:
if (dsol == "DUR") {
  // 🎯 GÜVENLİK SİGORTASI: Eğer alt program zaten çalışmıyorsa (false ise) 
  // boşu boşuna işlem yapma, log basma ve fonksiyonu hemen terk et!
  if (altDosyaCalisiyor == false) {
    return; 
  }

  // Eğer alt program o sırada AKTİF olarak çalışıyorsa kesme işlemlerini yap:
  altProgramIptalBayragi = true; 
  ualtprogdur();                 
  
  dsol = ""; 
  satirp = ""; 
  yield();
  return; 
}


if (dsol == "TX>") {
  String bilesen_ani = dsag;
  bilesen_ani.trim();

  String altSenaryoYolu = "/_" + bilesen_ani + ".txt"; 

  if (LittleFS.exists(altSenaryoYolu)) {
    Serial.printf("[uprog-tetik] Alt senaryo bulundu: %s. Baslatiliyor...\n", bilesen_ani.c_str());
    
    if (ualt_dosya) ualt_dosya.close(); 
    ualt_dosya = LittleFS.open(altSenaryoYolu, "r");
    if (ualt_dosya) {
      altDosyaCalisiyor = true;
      aktifAltDosyaAdi = bilesen_ani;
      
      // 🎯 SİHİRLİ DOKUNUŞ: Ana dosyanın while döngüsünü anında kır!
      // Böylece kod uprogram.txt okumayı bırakıp hemen alt dosyaya dallanacaktır.
      return; 
    }
  } 
  else {
    txsend(dsag);
    return;
  }
}


  for (int indis = 0; indis < 21; indis++)  // her islem için döngü
  {

    if (indis >= 0 && indis < 10){
    if (dsol == degis[indis]) {
      degdeg[indis] = dsag;
      PinState[indis] = dsag;
      if (indis < 10 && ePinState[indis] != PinState[indis]) psco = true;
      progmsg += (String)satirsayisip + " satır>" + yapilacakis + " degisti.<br>";
      aktifpinno= -10;
    }
    }

    if (indis >= 10 && indis < 21)  // degiskenleri tut
    {
      if (dsol == degis[indis]) {
        degdeg[indis] = dsag;
        progmsg += (String)satirsayisip + " satır>" + yapilacakis + " degisti.<br>";
      aktifpinno = -10;
      }
    }

    // 0-10  PİN     11-20 degisken
  }

  if (aktifpinno == -100) {
    programerrorlog += "tanimli olmayan değişken var: " + dsol + " Satir:" + String(satirsayisip) + "\n";
  }

}

// --- 1. BAĞIMSIZ ZAMAN EKSİLTME MOTORU ---
void timerGeriSay() {
  if (millis() >= milis100) {
    // 1. STANDART PROGRAM TİMER'LARI (100ms'de bir 1 eksiltir)
    for (int ii = 0; ii < MAX_TIMERS; ii++) {
      if (tmris[ii].length() > 0) {
        float standartVal = tmrdeg[ii].toFloat();
        if (standartVal > 0) {
          tmrdeg[ii] = String(standartVal - 1);
        }
      }
    }

    // 2. UZUN METRAJ PROGRAM TİMER'LARI (100ms'de bir 1 eksiltir)
    for (int ii = 0; ii < MAX_TIMERS; ii++) {
      if (utmris[ii].length() > 0) {
        float uzunmetrajVal = utmrdeg[ii].toFloat();
        if (uzunmetrajVal > 0) {
          utmrdeg[ii] = String(uzunmetrajVal - 1);
        }
      }
    }
    milis100 = millis() + 100;
  }
}
