unsigned long duration = 1000;
double frequency;

void play(String melody) {
if(melody.length()<1)return;
        //Serial.println("çaldım " + notalar);
    duration=(1000/(melody.length()));
    for (int ml = 0; ml < melody.length(); ml++) {
        char nota = melody.charAt(ml);  // Tek karakter al

        switch (nota) {
            case 'Y': frequency = 130.8; break;
            case 'U': frequency = 146.8; break;
            case 'I': frequency = 164.8; break;
            case 'O': frequency = 174.6; break;
            case 'H': frequency = 196;   break;
            case 'J': frequency = 220;   break;
            case 'K': frequency = 246.9; break;
            case 'C': frequency = 261.8; break;
            case 'D': frequency = 293.7; break;
            case 'E': frequency = 329.6; break;
            case 'F': frequency = 349.2; break;
            case 'G': frequency = 392;   break;
            case 'A': frequency = 440;   break;
            case 'B': frequency = 493.9; break;
            case '1': frequency = 523.3; break; // Özel mapping
            case '2': frequency = 698.5; break;
            case '3': frequency = 659.3; break;
            case '4': frequency = 587.3; break;
            case '5': frequency = 784;   break;
            case '6': frequency = 880;   break;
            case '7': frequency = 987.8; break;
            case '_': frequency = 0;     break; // Sessizlik
            default:  frequency = 0;     break;
        }

        if (frequency != 0) {
            tone(PIN_TONE, frequency, duration);
        }
        delay(duration);   // nota süresi kadar bekle
        noTone(PIN_TONE);  // sesi kes
    }
}
