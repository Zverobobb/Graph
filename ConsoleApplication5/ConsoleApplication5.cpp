#include "freeglut.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <string>

#define EDGES  58       // Количество ребер
#define NODES  41       // Количество вершин
#define PI     3.141592 // Число Пи
#define RADIUS 30       // Радиус вершин в пикселях
#define WID    5        // Тощина ребер в пикселях

using namespace std;

const int height = 1000; // Высота окна, пиксель
const int width = 2000;  // Ширина окна, пиксель

float node[NODES][2];    // Координаты вершин

int edge[EDGES][3] = {
                        { 1,  2,  24 },
                        { 2,  3,  10 },
                        { 3,  4,  17 },
                        { 4,  7,  44 },
                        { 4,  5,  49 },
                        { 5,  6,  23 },
                        { 6,  7,  2  },
                        { 7,  10, 10 },
                        { 7,  8,  12 },
                        { 7,  14, 36 },
                        
                        { 7,  9,  5  },
                        { 10, 32, 5  },
                        { 32, 36, 5  },
                        { 32, 37, 7  },
                        { 32, 38, 5  },
                        { 32, 39, 9  },
                        { 8,  11, 14 },
                        { 8,  12, 3  },
                        { 14, 15, 15 },
                        { 8,  13, 5  },

                        { 15, 19, 7  },
                        { 15, 20, 7  },
                        { 15, 21, 12 },
                        { 14, 17, 32 },
                        { 17, 22, 13 },
                        { 17, 23, 14 },
                        { 17, 24, 14 },
                        { 17, 25, 31 },
                        { 9,  26, 3  },
                        { 9,  27, 3  },

                        { 9,  28, 3  },
                        { 13, 18, 10 },
                        { 11, 18, 8  },
                        { 12, 18, 9  },
                        { 23, 30, 20 },
                        { 24, 30, 20 },
                        { 25, 30, 43 },
                        { 22, 30, 14 },
                        { 30, 33, 8  },
                        { 16, 30, 5  },

                        { 21, 29, 6  },
                        { 20, 29, 5  },
                        { 19, 29, 5  },
                        { 18, 34, 3  },
                        { 29, 33, 7  },
                        { 33, 34, 6  },
                        { 31, 34, 3  },
                        { 34, 35, 15 },
                        { 35, 36, 6  },
                        { 36, 37, 24 },

                        { 37, 38, 34 },
                        { 38, 39, 12 },
                        { 39, 40, 34 },
                        { 40, 41, 7  },
                        { 14, 16, 6  },
                        { 26, 31, 3  },
                        { 27, 31, 3  },
                        { 28, 31, 3  }
};

void display();
void draw();
void changeSize(int w, int h);
void mouse(int button, int status, int x, int y);
void mouseMove(int x, int y);
void timer(int i);
void RenderString(float x, float y, float r, float g, float b, void* font, string text);


int nMouseX, nMouseY;
int pMouseX, pMouseY;
bool pushMouse = false;
int minNode = 0;


int main(int argc, char** argv)
{
    glutInit(&argc, argv);  // Инициализаци графической библиотеки

    vector< pair<int, int> > adj[58];

    for (int i = 0; i < EDGES; i++)
    {
        adj[edge[i][0]].push_back({ edge[i][1], edge[i][2]});
    }

    int j = 0;
    for (int i = 0; i < NODES; i++)
    {
        if (i % 8 == 0 && i != 0) { j++; }
        node[i][0] = (float)(i % 8) / 4.0f - 1.0f + 1.0f / 8.0f;
        node[i][1] = (float)(j % 8) / 4.0f - 1.0f + 1.0f / 8.0f;
    }

    draw();

    return 0;
}


void draw()
{
    glutInitDisplayMode(GLUT_DOUBLE);   // Определение параметров окна отрисовки
    glutInitWindowSize(width, height);  // Задание размеров окна (ВхШ), пиксель
    glutInitWindowPosition(200, 200);   // Задание положения окна от левого верхнего угла экрана (ШхВ), пиксель

    glutCreateWindow("Graph");           // Задание названия окна

    glClearColor(0, 0, 0, 0);           // Задание чёрного цвета фона окна (красный, зелёный, синий, прозрачность)
    glutReshapeFunc(changeSize);        // Задание функции изменения размеров окна
    glutDisplayFunc(display);           // Задание функции отрисовки

    glutMouseFunc(mouse);               // Подключение обработчика событий иыши
    glutMotionFunc(mouseMove);          // Подключение функции движения мыши
    glutTimerFunc(100, timer, 0);       // Подключение функции постоянной отрисовки

    glutMainLoop();                     // Запуск функции отрисовки
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                 // Задание параметров буферу отрисовки

    glColor3f(1.0f, 0.0f, 1.0f);
    glLineWidth(WID);
    glBegin(GL_LINES);
    for (int i = 0; i < EDGES; i++)
    {
        glVertex2f(node[edge[i][0] - 1][0], node[edge[i][0] - 1][1]);
        glVertex2f(node[edge[i][1] - 1][0], node[edge[i][1] - 1][1]);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glPointSize(RADIUS);
    glBegin(GL_POINTS);
    for (int i = 0; i < NODES; i++)
    {
        glVertex2f(node[i][0], node[i][1]);
    }
    glEnd();

    for (int i = 0; i < EDGES; i++)
    {
        RenderString( (node[edge[i][1] - 1][0] + node[edge[i][0] - 1][0]) / 2.0f, (node[edge[i][1] - 1][1] + node[edge[i][0] - 1][1]) / 2.0f, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_TIMES_ROMAN_24, to_string(edge[i][2]));
    }

    for (int i = 0; i < NODES; i++)
    {
        RenderString(node[i][0] - 0.01f, node[i][1] - 0.01f, 0.0f, 0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, to_string(i + 1));
    }

    glutSwapBuffers();                // Отрисовка буфера
}

void RenderString(float x, float y, float r, float g, float b, void* font, string text)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);

    glutBitmapString(font, (const unsigned char*)text.c_str());
}

void changeSize(int w, int h)
{
    if (h == 0) h = 1;              // Предупреждение деления на ноль
    float ratio = 1.0 * w / h;      // Определение отношения ширины к высоте
    glMatrixMode(GL_PROJECTION);    // Задание типа матрицы "проекция"
    glLoadIdentity();               // Обновление матрицы
    glOrtho(0, w, 0, h, 0, 0);      // Задание границ окна
}

void mouse(int button, int status, int x, int y)
{
    nMouseX = x;                                    // Запоминается текущее положения мыши
    nMouseY = y;
    if (button == 0) {
        if (status == 0) {
            pMouseX = nMouseX;                      // Если правая кнопка мыши нажата, фиксируетя нажатие и запоминаются её координаты
            pMouseY = nMouseY;
            for (int i = 0; i < NODES; i++)
            {
                float asd1 = sqrt(pow((float)pMouseX / width * 2 - 1 - node[i][0], 2) + pow(-((float)pMouseY / height * 2 - 1) - node[i][1], 2));
                float asd2 = sqrt(pow((float)pMouseX / width * 2 - 1 - node[minNode][0], 2) + pow(-((float)pMouseY / height * 2 - 1) - node[minNode][1], 2));
                if (asd1 < asd2)
                {
                    minNode = i;
                }
            }
            pushMouse = true;
        }
        else pushMouse = false;                     // Если правая кнопка мыши отпущена, фиксируетя это событие
    }
}

void mouseMove(int x, int y)
{
    nMouseX = x;                    // Запоминается текущее положение мыши
    nMouseY = y;
    if (pushMouse == true) {
        node[minNode][0] += (float)(nMouseX - pMouseX) / width * 2;
        pMouseX = nMouseX;
        node[minNode][1] -= (float)(nMouseY - pMouseY) / height * 2;
        pMouseY = nMouseY;
        display();                  // Отрисовка окна
    }
}

void timer(int i) {
    display();
    glutTimerFunc(10, timer, 0);  // Окно отрисовывается принудительно каждые 10 миллисекунд (100 кадров в секунду)
}
