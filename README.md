# 🎮 Bouncing Ball Game (OpenGL + GLUT)

A simple yet modern **Bouncing Ball Game** implemented in **C++ with OpenGL/GLUT**.  
The game features smooth animations, glowing paddle effects, particle explosions, and a stylish UI with a modern color palette.

---

## 🚀 Features
- 🔴 **Ball Physics** – realistic bouncing mechanics with speed increase on paddle hit  
- 🟦 **Paddle Control** – move paddle left & right using arrow keys  
- ✨ **Particle Effects** – explosions when the ball hits walls/paddle  
- 🌟 **Glow Effects** – paddle glows dynamically when the ball bounces  
- 🎨 **Modern UI** – stylish grid background, glowing ball, and clean score display  
- 💀 **Game Over Screen** – press `R` to restart anytime  

---

## 🎮 Controls
- ⬅️ **Left Arrow** → Move paddle left  
- ➡️ **Right Arrow** → Move paddle right  
- 🔄 **R** → Restart the game after Game Over  

---

## 🖥️ Installation & Run

### Prerequisites
- C++ compiler (g++, clang++, MSVC, etc.)
- OpenGL and GLUT libraries installed

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install freeglut3-dev
g++ main.cpp -lGL -lGLU -lglut -o BouncingBall
./BouncingBall
````

### Windows (MinGW + FreeGLUT)

1. Download and install [FreeGLUT](http://freeglut.sourceforge.net/).
2. Compile with:

```bash
g++ main.cpp -lfreeglut -lopengl32 -lglu32 -o BouncingBall.exe
./BouncingBall.exe
```

### macOS

```bash
brew install freeglut
g++ main.cpp -framework OpenGL -framework GLUT -o BouncingBall
./BouncingBall
```

---

## 📂 Project Structure

```
📁 Bouncing-Ball-Game
 ┣ 📜 main.cpp          # Game source code
 ┣ 📜 modern_game       # Extra/alternate implementation
 ┣ 📜 game              # Supporting file
 ┣ 📜 README.md         # Project documentation
```

---

## 📝 Future Enhancements

* Multiple difficulty levels
* Sound effects
* High score system
* Power-ups & bonuses

---

## 🤝 Contributing

Feel free to fork this repository and submit pull requests with improvements.

---
