#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>

// Forward declarations
void specialKeys(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void update(int value);
void display();

// Game State
float ballX = 400, ballY = 300;
float ballSpeedX = 5, ballSpeedY = 5;
float paddleX = 350, paddleY = 20;
int score = 0;
bool gameOver = false;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float BALL_RADIUS = 15;
const float PADDLE_WIDTH = 100;
const float PADDLE_HEIGHT = 20;

struct Particle {
    float x, y;
    float speedX, speedY;
    float life;
    float size;
};

std::vector<Particle> particles;
float glowIntensity = 0.0f;
bool scoreIncreased = false;

// Modern color palette
GLfloat bgColor[3] = {0.08f, 0.08f, 0.12f};
GLfloat paddleColor[3] = {0.0f, 0.7f, 1.0f};
GLfloat ballColor[3] = {1.0f, 0.3f, 0.2f};
GLfloat accentColor[3] = {0.2f, 0.8f, 0.4f};

void createParticles(float x, float y) {
    for(int i = 0; i < 20; i++) {
        Particle p = {
            x, y,
            (rand()%100 - 50)/10.0f,
            (rand()%100 - 50)/10.0f,
            1.0f,
            static_cast<float>(rand()%5 + 2)
        };
        particles.push_back(p);
    }
}

void drawText(float x, float y, const char* text, bool centered = false) {
    if(centered) {
        int width = 0;
        for(const char* c = text; *c; c++)
            width += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c);
        x -= width/2;
    }
    
    glRasterPos2f(x, y);
    for(const char* c = text; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void drawCircle(float cx, float cy, float r, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i < segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        glVertex2f(cx + r*cosf(theta), cy + r*sinf(theta));
    }
    glEnd();
}

void drawPaddle() {
    // Main paddle
    glColor3fv(paddleColor);
    glBegin(GL_QUADS);
    glVertex2f(paddleX, paddleY);
    glVertex2f(paddleX + PADDLE_WIDTH, paddleY);
    glVertex2f(paddleX + PADDLE_WIDTH, paddleY + PADDLE_HEIGHT);
    glVertex2f(paddleX, paddleY + PADDLE_HEIGHT);
    glEnd();

    // Glow effect
    if(glowIntensity > 0) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.7f, 1.0f, glowIntensity*0.3f);
        glBegin(GL_QUADS);
        float glowSize = 15.0f * glowIntensity;
        glVertex2f(paddleX - glowSize, paddleY - glowSize);
        glVertex2f(paddleX + PADDLE_WIDTH + glowSize, paddleY - glowSize);
        glVertex2f(paddleX + PADDLE_WIDTH + glowSize, paddleY + PADDLE_HEIGHT + glowSize);
        glVertex2f(paddleX - glowSize, paddleY + PADDLE_HEIGHT + glowSize);
        glEnd();
        glDisable(GL_BLEND);
    }
}

void drawParticles() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for(auto& p : particles) {
        glColor4f(accentColor[0], accentColor[1], accentColor[2], p.life);
        glPointSize(p.size);
        glBegin(GL_POINTS);
        glVertex2f(p.x, p.y);
        glEnd();
    }
    glDisable(GL_BLEND);
}

void display() {
    glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw background grid
    glColor4f(1,1,1,0.1f);
    glLineWidth(1);
    glBegin(GL_LINES);
    for(int i = 50; i < WINDOW_WIDTH; i += 50) {
        glVertex2f(i, 0);
        glVertex2f(i, WINDOW_HEIGHT);
    }
    for(int i = 50; i < WINDOW_HEIGHT; i += 50) {
        glVertex2f(0, i);
        glVertex2f(WINDOW_WIDTH, i);
    }
    glEnd();

    // Draw particles
    drawParticles();

    // Draw Ball with glow
    glColor3f(ballColor[0], ballColor[1], ballColor[2]);
    drawCircle(ballX, ballY, BALL_RADIUS, 32);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1.0f, 0.3f, 0.2f, 0.3f);
    drawCircle(ballX, ballY, BALL_RADIUS*1.5, 32);
    glDisable(GL_BLEND);

    // Draw Paddle
    drawPaddle();

    // UI Elements
    glColor3fv(accentColor);
    drawText(20, WINDOW_HEIGHT - 40, "SCORE", false);
    char scoreText[50];
    sprintf(scoreText, "%04d", score);
    glColor3f(1,1,1);
    drawText(20, WINDOW_HEIGHT - 70, scoreText);

    if(gameOver) {
        glColor4f(0,0,0,0.7f);
        glBegin(GL_QUADS);
        glVertex2f(0,0);
        glVertex2f(WINDOW_WIDTH,0);
        glVertex2f(WINDOW_WIDTH,WINDOW_HEIGHT);
        glVertex2f(0,WINDOW_HEIGHT);
        glEnd();

        glColor3fv(accentColor);
        drawText(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 40, "GAME OVER", true);
        glColor3f(1,1,1);
        drawText(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 20, "Press R to restart", true);
    }

    glutSwapBuffers();
}

void update(int value) {
    // Update particles
    for(auto& p : particles) {
        p.x += p.speedX;
        p.y += p.speedY;
        p.life -= 0.02f;
        p.speedY -= 0.1f;
    }
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Particle& p) { return p.life <= 0; }), particles.end());

    // Update game state
    if(!gameOver) {
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        // Wall collisions
        if(ballX+BALL_RADIUS > WINDOW_WIDTH || ballX-BALL_RADIUS < 0) {
            ballSpeedX = -ballSpeedX;
            createParticles(ballX, ballY);
        }
            
        // Ceiling collision
        if(ballY+BALL_RADIUS > WINDOW_HEIGHT) {
            ballSpeedY = -ballSpeedY;
            createParticles(ballX, ballY);
        }
            
        // Paddle collision
        if(ballY-BALL_RADIUS < paddleY+PADDLE_HEIGHT && 
           ballY+BALL_RADIUS > paddleY &&
           ballX > paddleX && 
           ballX < paddleX+PADDLE_WIDTH) {
            ballSpeedY = -ballSpeedY * 1.05f;
            score += 10;
            scoreIncreased = true;
            glowIntensity = 1.0f;
            createParticles(ballX, ballY);
        }
        
        // Game Over
        if(ballY-BALL_RADIUS < 0)
            gameOver = true;
    }

    // Animate glow
    if(glowIntensity > 0) glowIntensity -= 0.1f;
    if(scoreIncreased) {
        ballColor[0] = 1.0f;
        ballColor[1] = 0.6f;
        scoreIncreased = false;
    } else {
        ballColor[0] = fmax(1.0f, ballColor[0] - 0.02f);
        ballColor[1] = fmin(0.3f, ballColor[1] + 0.02f);
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if(key == 'r' || key == 'R') {
        gameOver = false;
        ballX = WINDOW_WIDTH/2;
        ballY = WINDOW_HEIGHT/2;
        score = 0;
        ballSpeedX = 5;
        ballSpeedY = 5;
        particles.clear();
    }
}

void specialKeys(int key, int x, int y) {
    float speed = 20;
    switch(key) {
        case GLUT_KEY_LEFT:
            paddleX = fmax(0, paddleX - speed);
            break;
        case GLUT_KEY_RIGHT:
            paddleX = fmin(WINDOW_WIDTH-PADDLE_WIDTH, paddleX + speed);
            break;
    }
}

void init() {
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Modern Bouncing Ball");
    
    init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    
    glutMainLoop();
    return 0;
}