#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;

struct snake {
    snake();

    int x, y;
    snake *next, *prev;
};

int v = 50, cw = 1000, ch = 1000, l = 1, t = 15, cycle = 0, vx = v, vy = 0, ax = cw / 2 + 2, ay = ch / 2 + 2;
bool p = true, turn = true;
snake *head;
snake *tail;
snake *s;


int rand_uns(int min, int max) {
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    static std::default_random_engine e(seed);
    static std::uniform_int_distribution<int> d(min, max);
    return d(e);
}

snake *add(snake *t) {
    snake *n = new snake;
    l++;
    n->x = t->x;
    n->y = t->y;
    n->prev = t;
    t->next = n;
    return n;
}

void checkCollision() {
    if (head->x > cw || head->x < 0 || head->y > ch || head->y < 0) {
        cout << "Your score is " << l << ". Congratulations!";
        exit(0);
    }
    s = head;
    s = s->next->next;
    for (int i = 3; i < l; i++) {
        s = s->next;
        if (head->x == s->x && head->y == s->y) {
            cout << "Your score is " << l << ". Congratulations!";
            exit(0);
        }
    }
}

bool checkLocation() {
    s = head;
    if (s->x == ax && s->y == ay) {
        return true;
    }
    for (int i = 1; i < l; i++) {
        s = s->next;
        if (s->x == ax && s->y == ay)
            return true;
    }
    return false;
}

void food() {
    ax = rand_uns(0, cw / 50 - 1) * 50 + 2;
    ay = rand_uns(0, ch / 50 - 1) * 50 + 2;
    while (checkLocation()) {
        ax = rand_uns(0, cw / 50 - 1) * 50 + 2;
        ay = rand_uns(0, ch / 50 - 1) * 50 + 2;
    }
}

void eat() {
    if (head->x == ax && head->y == ay) {
        for (int i = 0; i < 3; i++)
            tail = add(tail);
        food();
    }
}

void reshape(int w, int h) {
    cw = w;
    ch = h;
    glViewport(0, 0, w, h);
    glClearColor(0.7, 0.7, 0.7, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() {
    int r = 50, a = r - 4, d = a - 4, dx, dy;
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1, 1, 1);
    for (int i = 0; i <= cw - r; i += r)
        for (int j = 0 + i % (r * 2); j <= ch - r; j += 2 * r) {
            glBegin((GL_QUADS));
            glVertex2d(i, j);
            glVertex2d(i + r, j);
            glVertex2d(i + r, j + r);
            glVertex2d(i, j + r);
            glEnd();
        }
    glColor3f(1, 0, 0);
    glBegin((GL_QUADS));
    glVertex2d(ax + 4, ay + 4);
    glVertex2d(ax + d, ay + 4);
    glVertex2d(ax + d, ay + d);
    glVertex2d(ax + 4, ay + d);
    glEnd();
    glColor3f(0, 0, 0);
    s = head;
    glBegin((GL_QUADS));
    glVertex2d(s->x, s->y);
    glVertex2d(s->x + a, s->y);
    glVertex2d(s->x + a, s->y + a);
    glVertex2d(s->x, s->y + a);
    glEnd();
    for (int i = 1; i < l; i++) {
        s = s->next;
        dx = s->prev->x - s->x;
        dy = s->prev->y - s->y;
        if (dx > 0 || dy > 0) {
            glBegin((GL_QUADS));
            glVertex2d(s->x, s->y);
            glVertex2d(s->x + a + dx, s->y);
            glVertex2d(s->x + a + dx, s->y + a + dy);
            glVertex2d(s->x, s->y + a + dy);
            glEnd();
        } else if (dx < 0) {
            glBegin((GL_QUADS));
            glVertex2d(s->x + a, s->y);
            glVertex2d(s->x + a + dx, s->y);
            glVertex2d(s->x + a + dx, s->y + a);
            glVertex2d(s->x + a, s->y + a);
            glEnd();
        } else if (dy < 0) {
            glBegin((GL_QUADS));
            glVertex2d(s->x, s->y + a);
            glVertex2d(s->x + a, s->y + a);
            glVertex2d(s->x + a, s->y + dy + a);
            glVertex2d(s->x, s->y + dy + a);
            glEnd();
        }
    }
    glutSwapBuffers();
}

void timf(int value) {
    if (!p) {
        cycle++;
        if (cycle == 10) {
            checkCollision();
            eat();
            cycle = 0;
            tail->x = head->x;
            tail->y = head->y;
            head->prev = tail;
            tail->next = head;
            head = tail;
            tail = tail->prev;
            head->prev = NULL;
            head->x += vx;
            head->y += vy;
            glutPostRedisplay();
            turn = true;
        }
    }
    glutTimerFunc(t, timf, 0);
}

void mouse(int button, int state, int x, int y) {
    if (state == 1)
        p = !p;
}

void key(unsigned char k, int x, int y) {
    if (!p && turn) {
        if (k == 'w' && vy == 0) {
            vx = 0;
            vy = v;
            turn = false;
        }
        if (k == 's' && vy == 0) {
            vx = 0;
            vy = -v;
            turn = false;
        }
        if (k == 'a' && vx == 0) {
            vy = 0;
            vx = -v;
            turn = false;
        }
        if (k == 'd' && vx == 0) {
            vy = 0;
            vx = v;
            turn = false;
        }
    }
}

int main(int argc, char *argv[]) {
    head = new snake;
    head->x = 2;
    head->y = ch - 50 + 2;
    tail = add(head);
    tail = add(tail);
    head->x += vx;
    food();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(cw, ch);
    glutInitWindowPosition(500, 50);
    glutCreateWindow("Snake");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(t, timf, 0);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}