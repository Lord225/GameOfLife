# GameOfLife

Implementacja GameOfLife z użyciem OpenGL.

Sterowanie:
* Kółko myszky: zoom
* Naciśnięcie kółka myszy: przesuwanie
* Spacja - Zmiana trybu (animacja/skokowy)
* Strzałka w lewo - zmiana prędkości w trybie animacji (w dół)
* Strzałka w prawko - zmiana prędkości w trybie animacji (w górę) / skok w trybie skokowym

Argumenty:
* --pixelsize [float] - zmiana rozmiaru pikseli (def. 10.0)
* --shaderpath [str] - ścieżka do folderu z shaderami (.frag, .geom) (def. ../../GameOfLife)
* --showsteptimes - flaga okreslajaca czy w konsoli powinny pojawiac sie informacje o czasach symulacji i czasie na ramke (def. false)
* --size [int] - rozmiar planszy (def. 1024, musi byc podzielne przez --threads)
* --threads [int] - ilość uruchamianych wątków przy symulacji (def. 8)

Przykład użycia
```
GameOfLife.exe --size 8196 --threads 8 --showsteptimes --shaderpath ../../GameOfLife
```
Skalowanie wydajności z ilością rdzeni (i7 7700k@4.5):
```
GameOfLife.exe --size 8196 --threads 1 --showsteptimes --shaderpath ../../GameOfLife 
sim: dt = 1312.55ms      frame: dt = 16.8034ms
sim: dt = 1415.22ms      frame: dt = 16.6497ms
sim: dt = 1348.98ms      frame: dt = 17.3721ms
sim: dt = 1351.17ms      frame: dt = 16.6758ms
--threads 2
sim: dt = 693.154ms      frame: dt = 16.7393ms
sim: dt = 720.934ms      frame: dt = 16.726ms
sim: dt = 670.227ms      frame: dt = 16.6596ms
sim: dt = 671.284ms      frame: dt = 16.64ms
--threads 4
sim: dt = 402.716ms      frame: dt = 16.6292ms
sim: dt = 409.949ms      frame: dt = 16.6844ms
sim: dt = 405.411ms      frame: dt = 16.6323ms
sim: dt = 397.172ms      frame: dt = 16.5453ms
--threads 8
sim: dt = 250.661ms      frame: dt = 33.256ms
sim: dt = 250.517ms      frame: dt = 16.6699ms
sim: dt = 243.597ms      frame: dt = 16.6676ms
sim: dt = 290.384ms      frame: dt = 16.4278ms
--threads 16
sim: dt = 269.047ms      frame: dt = 16.6297ms
sim: dt = 268.518ms      frame: dt = 16.6681ms
sim: dt = 270.637ms      frame: dt = 16.7101ms
sim: dt = 284.879ms      frame: dt = 25.2842ms
```
```
1  - 100%
2  - 195%
4  - 330%
8  - 539%
16 - 487%
```
Skalowanie wydajności z ilością rdzeni (R5 3600):
```
GameOfLife.exe --size 8196 --threads 1 --showsteptimes --shaderpath ../../GameOfLife 
sim: dt = 1102.22ms      frame: dt = 13.0700ms
--threads 4
sim: dt = 267.83ms       frame: dt = 16.6242ms
--threads 6
sim: dt = 219.663ms       frame: dt = 16.6242ms
--threads 8
sim: dt = 163.281ms      frame: dt = 16.356ms
--threads 12
sim: dt = 134.978ms      frame: dt = 17.4600ms
```
```
1  - 100%
4  - 270%
6  - 500%
8  - 678%
12 - 820%
```

