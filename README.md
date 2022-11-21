# [_2D Pinball by Fuim Enjoyers_](https://github.com/Fuim-Enjoyers-Studios/Pinball-Game)

## Description

A 2D Pinball to test our knowledge in Box2D we created a pinball game, emulating one of the many that can be found on the internet.

## Key Features

- Get as many points as you can
- Each time you bounce a planet without touching the walls, the following bounce score increases
- Flying ships across the map
- A star wars theme
- Physics of the game by Box2D
 
## Personal Experience

During the creation of the pinball we found some bugs and issues. A bug for example is that a chain can't be roatated, and we didn't know that. So we struggled to know why our flippers didn't rotate. Finally we found out that we should use rectangles in the flippers so they can rotate.
Then, some issues we had are for example, to blit the textures of the text for the score numbers. We have a module that blits in screen a string, but not integers, so there is a function to translate the numbers into string so it can be printed. We had some issues that it can't be float, it must be int. And the size of the string should be 10 allways, so if we wanted to print 3 numbers for example, and the size was 3, error.
Also to clear the debug balls (the ones created with the the debug key "1"), we had the issue that we cleared the list without destroying the fixtures before, so the fixtures remained in the map.
We had a lot of issues like those, but the ones we wrote here are the ones that annoyed us the most.
 
## Controls

- Left Arrow - Left flipper
- Right Arrow - Right flipper
- Escape - Ingame to return to menu screen
- 1 - Create debug circles
- Q - Inside the game to close the game
- F1 - Draw all colliders and ability to move the ball with your mouse
- F2 - FPS toggle so it can be edited (F1 must be activated)
- F3 - GRAVITY COEFFICIENT toggle so it can be edited (F1 must be activated)
- F4 - BOUNCE COEFFICIENT toggle so it can be edited (F1 must be activated)
- Tab - Draw or hide layout
- Up Arrow - Increase coefficient (FPS, GRAVITY COEFFICIENT OR BOUNCE COEFFICIENT)
- Down Arrow - Reduce coefficient (FPS, GRAVITY COEFFICIENT OR BOUNCE COEFFICIENT)
- P - Pause Game
- R - Restart 


## Developers

 - Luis González - Coding, Art and Level Design - [_punto16_](https://github.com/punto16)
 - Pau Vivas - Coding, Art and Level Design - [_Paules23_](https://github.com/Paules23)
 - Alex Delgado - Coding, Art and Level Design - [_faelion_](https://github.com/faelion)
 - Francisco Ferreros - Coding, Art and Level Design - [_kikofp02_](https://github.com/kikofp02)

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.
