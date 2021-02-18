# GameOfLife

Implementacja GameOfLife z użyciem OpenGL.

Sterowanie
* Kółko myszky: zoom
* Naciśnięcie kółka myszy: przesuwanie

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
