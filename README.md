# Projet LH ooq

Code Arduino pour faire réagir une bande de LED selon une distance calculée par un capteur à ultrason.
Lorsqu'un individu s'approche, les Leds deviennent de plus en plus rouge.
Lorsqu'aucune détection n'est faite, la bande clignote selon une valeur de couleur prise au hasard.

## Démarrage


### Matériel

* Capteur à ultra-son HC-SR04
* Bande de Leds Adafruit NeoPixels

### Montage

La détection se fait par interruption sur un timer. Ne pas attacher la Neo_Pixels sur la broche PMW gérée par le Timer utilisé par la bibliothèque NewPing. Cette broche dépendant du type de carte utilisé, se référer aux spécifications.

* Timer2 pour Arduino Uno / Mega
* Timer4 pour Arduino Leonardo / Teensy


### Pré-requis

Les bibliothèques suivantes doivent être installées dans l'IDE Arduino : 

* [NewPing](https://bitbucket.org/teckel12/arduino-new-ping/downloads/) pour le fonctionnement du capteur à ultra-son 
* [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) pour la bande de LED

### Paramétrage

Variables disponibles pour ajustement :

* NUMPIXELS nombre de leds à allumer sur la bande
* MAX_DISTANCE distance en cm à partir de laquelle une détection est faite
* pingSpeed fréquence d'envoi du ping par le capteur à ultra_son (exemple : 50ms serait 20 envois par seconde)


## Effectuer les tests

* Vérifier la détection d'approche -> la bande est de plus en plus rouge
* Vérifier la remise à zéro lorsque l'éloignement est supérieur à MAX_DISTANCE (effacement de la dernière luminosité enregistrée)
* Vérifer le déclenchement du clignotement losqu'il n'y a plus de détection faite







