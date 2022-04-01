# Windows laptop

My good old POS windows laptop.

`g++ -O2 -Wall main.cpp -o sandpile.exe`

Study example for code changes.

## Baseline

#define gsize uint8_t  
#define pilehalf 128  

shift: 17  
grains: 131072  
grid_X: 12000  
grid_Y: 12000  
grid_size: 144000000  
2^17
- 131072 grains placed
- Time 51
- 5857:6143 5857:6143
- 286 286


### So much faster!

#define gsize uint16_t  
#define pilehalf 32768   

shift: 17  
grains: 131072  
grid_X: 12000  
grid_Y: 12000  
grid_size: 144000000  
2^17
- 131072 grains placed
- Time 11
- 5857:6143 5857:6143
- 286 286

shift: 18  
grains: 262144   
grid_X: 12000  
grid_Y: 12000  
grid_size: 144000000  
2^18
- 262144 grains placed
- Time 59
- 5803:6197 5803:6197
- 394 394

shift: 19  
grains: 524288  
grid_X: 12000  
grid_Y: 12000  
grid_size: 144000000  
2^19
- 524288 grains placed
- Time 276
- 5725:6275 5725:6275
- 550 550

### Trying to inline the topple function. 

It looks faster. Need more testing

shift: 17  
grains: 131072  
grid_X: 12000  
grid_Y: 12000  
grid_size: 144000000  
2^17
- 131072 grains placed
- Time 12
- 5857:6143 5857:6143
- 286 286

shift: 18  
grains: 262144  
grid_X: 12000  
grid_Y: 12000  
grid_size: 144000000  
2^18
- 262144 grains placed
- Time 53
- 5803:6197 5803:6197
- 394 394

shift: 19  
grains: 524288  
grid_X: 12000  
grid_Y: 12000  
grid_size: 144000000  
2^19
- 524288 grains placed
- Time 249
- 5725:6275 5725:6275
- 550 550