#include <LiquidCrystal.h> // Inclusion de la bibliothèque LiquidCrystal pour gérer l'affichage sur écran LCD

// Initialisation de l'écran LCD avec les broches de connexion
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

const int pulsePin = 2;  // Définition de la broche de mesure du pouls
const int potPin = A4;   // Définition de la broche du potentiomètre pour compter les pas
const int tempPin = A1;  // Définition de la broche du capteur de température

volatile int bpm = 0; // Variable pour stocker la fréquence cardiaque
int stepCount = 0;    // Variable pour compter les pas
unsigned long lastUpdate = 0; // Variable pour stocker le temps de la dernière mise à jour
const long interval = 4000;   // Intervalle de mise à jour en millisecondes (4 secondes)
unsigned long lastBeatTime = 0; // Stocke le dernier temps enregistré du battement du cœur

// Définition des seuils de sécurité pour la fréquence cardiaque
const int bpmMax = 120; // Seuil maximal (tachycardie)
const int bpmMin = 50;  // Seuil minimal (bradycardie)

void setup() {
    lcd.begin(16, 2); // Initialisation de l'affichage LCD (16 colonnes, 2 lignes)
    lcd.print("Bracelet Actif"); // Affichage du message de démarrage
    delay(200); // Pause de 200ms avant de nettoyer l'écran
    lcd.clear(); // Nettoyage de l'écran LCD

    // Configuration des broches en entrée
    pinMode(pulsePin, INPUT);
    pinMode(potPin, INPUT);
    pinMode(tempPin, INPUT);

    // Attachement d'une interruption sur la détection du signal de pouls
    attachInterrupt(digitalPinToInterrupt(pulsePin), countBeat, RISING);
}

void loop() {
    // Lecture de la température brute et conversion en degrés Celsius
    int rawTemp = analogRead(tempPin);
    float temperature = (rawTemp * 5.0 * 100.0) / 1023.0;

    // Lecture de la valeur du potentiomètre pour détecter les pas
    int potValue = analogRead(potPin);
    if (potValue > 700) { // Si la valeur dépasse 700, un pas est compté
        stepCount++;
        delay(300); // Attente de 300ms pour éviter le comptage multiple du même pas
    }

    // Simulation de la fréquence cardiaque
    int pulseValue = digitalRead(pulsePin);
    if (pulseValue == HIGH) { // Si un battement est détecté
        bpm = random(50, 150); // Valeur aléatoire entre 50 et 150 BPM (simulation)
    }

    // Vérification si l'intervalle de temps est écoulé pour mise à jour des valeurs
    if (millis() - lastUpdate >= interval) {
        lastUpdate = millis(); // Mise à jour du dernier temps enregistré

        lcd.clear(); // Nettoyage de l'affichage LCD
        lcd.setCursor(0, 0); // Positionnement du curseur sur la première ligne
        lcd.print("BPM:"); lcd.print(bpm); // Affichage du BPM
        lcd.setCursor(8, 0);
        lcd.print("Pas:"); lcd.print(stepCount); // Affichage du nombre de pas

        lcd.setCursor(0, 1); // Positionnement du curseur sur la seconde ligne
        lcd.print("Temp:"); lcd.print(temperature, 1); // Affichage de la température avec 1 décimale
        lcd.print("C"); // Ajout de l'unité de température

        // Détection d'une anomalie sur la fréquence cardiaque
        if (bpm > bpmMax) { // Si le BPM dépasse le seuil maximal
            lcd.setCursor(8, 1);
            lcd.print("ALERTE BPM HAUT!"); // Affichage d'un message d'alerte
            Serial.println("⚠ ALERTE : BPM trop élevé !"); // Envoi d'une alerte série
        }

        if (bpm < bpmMin) { // Si le BPM est inférieur au seuil minimal
            lcd.setCursor(8, 1);
            lcd.print("ALERTE BPM BAS!"); // Affichage d'un message d'alerte
            Serial.println("⚠ ALERTE : BPM trop bas !"); // Envoi d'une alerte série
        }

        // Affichage des données sur le moniteur série
        Serial.print("BPM: "); Serial.print(bpm);
        Serial.print(" - Steps: "); Serial.print(stepCount);
        Serial.print(" - Temp: "); Serial.println(temperature);

        // Réinitialisation des valeurs après l'affichage
        bpm = 0;
        stepCount = 0;
    }
}

// Fonction de comptage des battements cardiaques via interruption
void countBeat() {
    unsigned long currentTime = millis();
    if (currentTime - lastBeatTime > 300) { // Filtrage des battements parasites
        bpm++; // Incrémentation du compteur de battements cardiaques
        lastBeatTime = currentTime; // Mise à jour du dernier battement détecté
    }
}
