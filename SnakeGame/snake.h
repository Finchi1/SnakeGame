#pragma once
#include <qwidget.h>
#include <qlist.h>
class Snake :
    public QWidget
{
public:
    Snake(QWidget* parent = nullptr);
private:
    static const int M_WIDTH = 300; // ������ �������
    static const int M_HEIGHT = 300; // ������ �������
    static const int M_DOT_PIXELS = 10; // ������ ������ �������� ������/������
    static const int M_DELAY = 140; // �������� �������
private:
    QImage i_head; // �������� ������
    QImage i_apple; // �������� ������
    QImage i_dot; // �������� ������ �������� ������ ������
    int timerID; // ID ������
private:
    QPoint m_head; // ���������� ������
    QPoint m_apple; // ���������� ������
    QList<QPoint> m_dots; // ������ � ������������ ���� ��������� ������
    QPoint m_tail_shadow; // ���������� ���������� �������� ������ ����� ������������, ������������ ��� ���������� ����� ��������� � ����� ������
private:
    bool m_up_direction; // �������� �����
    bool m_down_direction; // �������� ����
    bool m_right_direction; // ������� ������
    bool m_left_direction; // �������� �����
private:
    void loadImages(); // �������� ���� ��������
    void putApple(); // ��������� ���������� ������ �� ������� ����
    void moveHead(); // �������� ������ ������
    void moveTail(); // �������� ���� ������
    void eatApple(); // ���������� �������� ������� ������
    bool isGameOver(); // ��������� ����������� �� ����
private:
    void paintEvent(QPaintEvent* event);
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);
};

