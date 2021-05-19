# COP290-A2-GameDev

A Pacman-like maze game using SDL2. Ghosts are heuristic-driven with varying degree of randomness based on difficulty parameter. To build and run the game:-

```
make compile_game
make run_game
```

Following is a screenshot from the game.\
![Pacman Game](/assets/cover.jpg "Game still")

## TO-DO

### Single-player

- [x] Random maze generation.
- [x] Tiling the maze.
- [x] Rendering multiple images in same window. (SDL texture manipulation) (Using sprites for animation of pacman)
- [x] Event-driven programming. (IO control to detect keyboard presses)
- [x] Collision detection. (for walls and ghost collisions)
- [x] Add sound effects and font-styles.
- [x] AI-driven ghosts that depend on difficulty.

### Multi-player

- [x] Add client-server architecture. (if possible add a matchmaking system) - Done partially.
- [ ] Design a different maze game for 2 players.
- [x] Warn users when connection is weak.
- [ ] (optional) AI player that takes moves based on heuristic.
