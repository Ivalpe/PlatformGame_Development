# PlatformGame

A 2D platformer game created using **Whimsy Hallow** assets.

[Link to Github](https://github.com/Ivalpe/PlatformGame_Development/tree/main)

## About

**PlatformGame** is a classic platformer featuring dynamic gameplay, various enemy types, and interactive environments. The game incorporates beautifully designed pixel art and animations to deliver an engaging experience.

### Credits
- **Tileset & Assets:** [Whimsy Hallow by Crumpaloo](https://crumpaloo.itch.io/whimsy-hallow)
- **Sound Effects:** [Dillon Becker](https://dillonbecker.itch.io/sdap)
- **Boss Sprite:** [Szadi art](https://szadiart.itch.io/2d-soulslike-character)
- **UI:** [Crusenho Agus Hennihuno](https://crusenho.itch.io/complete-ui-essential-pack)

---

## Controls

| Key       | Action                              |
|-----------|-------------------------------------|
| **A**     | Move Left                           |
| **D**     | Move Right                          |
| **Shift** | Run                                 |
| **Space** | Jump                                |
| **E**     | Shoot                               |
| **H**     | Show Help Menu                      |
| **F1**    | Start from Level 0                  |
| **F2**    | Start from Level 1                  |
| **F3**    | Start from Current Level            |
| **F4**    | Start from Boss Level               |
| **F5**    | Save Current Game State             |
| **F6**    | Load Previous Game State            |
| **F9**    | View Colliders                      |
| **F10**   | Toggle God Mode                     |
| **F11**   | Limit FPS to 30                     |

---

## Features

- **Player Mechanics**:  
  - Responsive controls with animations for idle, running, jumping, and death.
  - Shooting mechanics for combat.

- **Enemies**:  
  - Two types of enemies: ground-based and flying.  
  - Pathfinding system for both enemy types to chase the player.

- **Environment**:  
  - Fully interactive levels with precise collision detection.
  - Checkpoints activated by bonfires for progress saving.
  - Poison zones that instantly kill the player.

- **Camera**:  
  - Smooth camera system that follows the player dynamically.

- **Audio and Music**:  
  - Background music to enhance the atmosphere of each level.  
  - Sound effects for player actions (e.g., jumping, shooting) and enemy interactions.

- **Debug Mode**:  
  - A mode for testing, including tools for viewing colliders and enabling god mode.

- **Data Handling**:  
  - XML-based system to load maps, collisions, player configurations, and other settings.
