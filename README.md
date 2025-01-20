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
| **F8**    | View Button Colliders               |
| **F9**    | View Colliders                      |
| **F10**   | Toggle God Mode                     |
| **F11**   | Limit FPS to 30                     |

---

## Features

- **Player Mechanics**:  
  - Responsive controls with animations for idle, running, jumping, and death.  
  - Shooting mechanics for combat.  
  - Health recovery via hearts, collectible items, and special power-ups.

- **Enemies**:  
  - Two types of enemies: ground-based and flying.  
  - Pathfinding system for both enemy types to chase the player.

- **Environment**:  
  - Fully interactive levels with precise collision detection.  
  - Checkpoints activated by bonfires for saving progress.  
  - Poison zones that instantly kill the player.  
  - Teleportation between bonfires of any level for faster movement.

- **Bosses**:  
  - A level 3 with a final boss featuring unique attack patterns.

- **Endgame**:  
  - A final scene after defeating the boss and crossing the gate.

- **Death Screen**:  
  - A dedicated death screen showing player stats and options to restart.

- **HUD**:  
  - HUD elements for the intro, title screen, player status, pause menu, death screen, and victory screen.  
  - Timer displaying elapsed gameplay time.

- **Camera**:  
  - Smooth camera system that follows the player dynamically.

- **Audio and Music**:  
  - Background music for the intro screen, levels, boss fight, and final scene.  
  - Sound effects for player actions (e.g., jumping, shooting) and enemy interactions.  
  - Audio for level transitions, user interface actions, enemies, bosses, and item pickups.

- **Debug Mode**:  
  - A mode for testing, including tools for viewing colliders and enabling god mode.

- **Data Handling**:  
  - XML-based system for loading maps, collisions, player configurations, items, enemies, and saved game data.

- **Optimization**:  
  - **Frustum Culling**: Prevents rendering of entities outside the screen to improve performance.  
  - **Lazy Updates**: Ensures entities outside the screen are not updated, reducing unnecessary processing.  
  - **Pathfinding Optimization**: Limits the pathfinding range for enemies, preventing excessive resource use during AI calculations.  
  - Performance monitoring via Tracy.

- **Main Menu**:  
  - A main menu with options for starting a new game, loading a saved game, adjusting settings, viewing credits, and exiting.  
  - The settings menu features sliders for adjusting music and sound effects volumes, and a checkbox for toggling fullscreen mode.

## Fixes from Previous Releases

- **Enemy Movement**:  
  - Dynamic movement for enemies to make the game world feel more alive.
