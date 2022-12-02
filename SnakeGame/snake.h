#pragma once
#include <qwidget.h>
#include <qlist.h>
class Snake :
    public QWidget
{
public:
    Snake(QWidget* parent = nullptr);
private:
    static const int M_WIDTH = 300; // ширина виджета
    static const int M_HEIGHT = 300; // высота виджета
    static const int M_DOT_PIXELS = 10; // размер одного элемента змейки/яблока
    static const int M_DELAY = 140; // задержка таймера
private:
    QImage i_head; // картинка головы
    QImage i_apple; // картинка яблока
    QImage i_dot; // картинка одного элемента хвоста змейки
    int timerID; // ID тймера
private:
    QPoint m_head; // координаты головы
    QPoint m_apple; // координаты яблока
    QList<QPoint> m_dots; // список с координатами всех элементов хвоста
    QPoint m_tail_shadow; // координаты последнего элемента хвоста перед перемещением, используется для добавления новых элементов в хвост змейки
private:
    bool m_up_direction; // движение вверх
    bool m_down_direction; // движение вниз
    bool m_right_direction; // двиение вправо
    bool m_left_direction; // движение влево
private:
    void loadImages(); // загрузка всех картинок
    void putApple(); // случайное размещение яблока на игровом поле
    void moveHead(); // движение головы змейки
    void moveTail(); // движение тела змейки
    void eatApple(); // обработчик съедания змейкой яблока
    bool isGameOver(); // проверяем закончилась ли игра
private:
    void paintEvent(QPaintEvent* event);
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);
};
