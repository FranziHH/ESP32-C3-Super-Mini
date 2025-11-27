# ESP32-C3-Super-Mini


## ESP32-C3-Super-Mini WiFi-Test

Der Sketch ist wie auf der Website veröffentlicht nicht funktionsfähig.
Deshalb habe ich den Sketch lauffähig hier zur Verfügung gestellt.

Am Einfachsten ist es, mit vsCode + PlatformIO zu nutzen.
Funktioniert auch in der Arduino IDE  - ist aber umständlicher.


#### Mein Test Aufbau

<img src="/images/IMG_20251127_103856.jpg" height="400px" title="Test Aufbau 1"> 

<img src="/images/IMG_20251127_103928.jpg" height="400px" title="Test Aufbau 2"> 


## Original Anleitung (YouTube) und Sketch

[YouTube: WLAN Empfang am ESP32 C3 super mini mit einfacher Modifikation verbessern, Tips zur Optimierung.](https://www.youtube.com/watch?v=LwBawTlJZ4s)

[Homepage: Sketch WLAN Test](https://goesenswelt.de/wlan/)

## Probleme mit WiFi

Es gibt einige Boards, bei denen das WiFi gar nicht funktioniert.
Hier gibt es einen Lösungsansatz.

[Arduino Forum: Keine WLAN-Verbindung mit ESP32-C3 Super Mini](https://forum.arduino.cc/t/no-wifi-connect-with-esp32-c3-super-mini/1324046/22)


##### Das Wichtigste:
Die Antenne so weit wie möglich an den Rand des Boards verschieben.
Sie kann ruhig überstehen. Die Antenne sollte nur sicher angelötet sein.

Einige User haben die Antenne mit kurzen Drähten nach Außen verlagert.


## Serial Port

Ich habe es nicht zuverlässig hinbekommen, Ausgaben im seriellen Monitor zu bekommen.
Vor allem Boot Messages usw bekommt man auf die Art nicht angezeigt.

Um vernünftige Ausgaben zu bekommen, habe ich den internen UART an einen USB-TTL Serial Converter angeschlossen.

PIN20: TX vom Adapter
PIN21: RX vom Adapter

Die Darstellung auf dem Bild stimmt nur dann, wenn die Anschlüsse des Adapters vertauscht sind.
Falls keine Kommunikation möglich ist, versuchsweise diese beiden Anschlüsse vertauschen.

Auch das Flashen der Firmware ist über diesen Anschluss möglich

[AZ-Delivery: USB auf Seriell Adapter mit CH340](https://www.az-delivery.de/products/usb-auf-seriell-adapter-mit-ch340)

<img src="/images/IMG_20250510_152911.png" height="400px" title="USB auf Seriell Adapter"> 

#### Gehäuse für diesen Adapter

[Thingiverse: Gehäuse](https://www.thingiverse.com/thing:7034979)

<img src="/images/IMG_20250510_152911.jpg" height="400px" title="Gehäuse"> 


#### Produktbilder vom Verkäufer

<img src="/images/ESP32-C3-Super-Mini-Pinout-f.png" height="400px" title="ESP32 C3 Super Mini Pinout"> 

<img src="/images/ESP32-C3-Super-Mini-Serial-Port.png" height="400px" title="ESP32 C3 Super Mini Serial Port">

