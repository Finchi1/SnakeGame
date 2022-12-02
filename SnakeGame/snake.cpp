#include "snake.h"
#include <qpainter.h>
#include <QKeyEvent>
#include <qfontmetrics.h>
#include <qpainterpath.h>
#include <random>

Snake::Snake(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedSize(M_WIDTH, M_HEIGHT); // ��������� ������������� ������ ����
	this->setStyleSheet("background-color: black"); // ��������� ������ ���� ����
	// ������ ��������� ���������� ������ ������
	m_head.setX(2);
	m_head.setY(2);
	// ��������� ��� �������� ���� ������ � ������ �� ��������� ����������
	m_dots.push_back(QPoint(1, 2));
	m_dots.push_back(QPoint(0, 2));
	// ������ ��������� ����������� ������
	m_up_direction = false;
	m_down_direction = false;
	m_right_direction = true;
	m_left_direction = false;
	loadImages(); // ��������� ��������
	putApple(); // ��������� ������ �� ������� ���� ��������� �������
	timerID = startTimer(M_DELAY); // ��������� ������ � ���������� ��� ID
}
// ��������� �������� � ������ �� ������ �� 10 �������� ������ (������ ����������� �� ������ �������������)
void Snake::loadImages()
{
	i_head = QImage("head.png");
	i_head = i_head.scaledToWidth(10);
	i_apple = QImage("apple.png");
	i_apple = i_apple.scaledToWidth(10);
	i_dot = QImage("dot.png");
	i_dot = i_dot.scaledToWidth(10);
}
// ��������� ������ �� ������� ���� ��������� �������
void Snake::putApple()
{
	bool no_snake = false;
	int counter = 0;
	int x, y;
	// ��������� ����� ������ �� ����������� �� ���� ��� ������ ������
	while (!no_snake)
	{
		// � �������� ���������� ��������� ����� ���������� ����� ������� ��� ��������� ���������
		x = std::mt19937(std::random_device{}())() % (M_WIDTH / M_DOT_PIXELS);
		y = std::mt19937(std::random_device{}())() % (M_HEIGHT / M_DOT_PIXELS);
		// ��������� ����� ������ �� ����������� �� ������ ������
		if (m_head.x() != x || m_head.y() != y)
			// ��������� ����� ������ �� ����������� �� ���� ������
			for (auto& dot : m_dots)
			{
				if (dot.x() != x || dot.y() != y)
					counter++;
				else
					break;
			}
		if (counter == m_dots.size())
			no_snake = true;
	}
	// ������������� ���������� ��������� ���������� ��� ������
	m_apple.setX(x);
	m_apple.setY(y);
}

// ������������ �������� ������ ������ � ������ ������������, ��� ���������� ������������� ���� ����� ������������ ��������
void Snake::moveHead()
{
	m_tail_shadow = m_dots.at(m_dots.size() - 1); // ��������� ���������� ���������� �������� ������ ����� ������������
	if (m_up_direction)
	{
		int y = m_head.y();
		moveTail();
		y--;
		m_head.setY(y);
	}
	else if (m_down_direction)
	{
		int y = m_head.y();
		moveTail();
		y++;
		m_head.setY(y);
	}
	else if (m_left_direction)
	{
		int x = m_head.x();
		moveTail();
		x--;
		m_head.setX(x);
	}
	else
	{
		int x = m_head.x();
		moveTail();
		x++;
		m_head.setX(x);
	}
}

// ������������ �������� ������ � ���� �� ������� ������ (�������� ��� ���������� ������������� ����)
void Snake::moveTail()
{
	int x = m_head.x();
	int y = m_head.y();
	for (int i = m_dots.size() - 1; i > 0; i--)
		m_dots.replace(i, QPoint(m_dots.at(i - 1).x(), m_dots.at(i - 1).y()));
	m_dots.replace(0, QPoint(x, y));
}
// ��������� ����� �� ������ ������, ���� ��, �� ��������� ���� ������� � ������ �� ���������� m_tail_shadow
void Snake::eatApple()
{
	if (m_head.x() == m_apple.x() && m_head.y() == m_apple.y())
	{
		m_dots.push_back(m_tail_shadow);
		putApple();
	}
}

// ��������� ����������� �� ����
bool Snake::isGameOver()
{
	// ���� ����� �� ��������� ����
	if (m_head.x() < 0 || m_head.x() >= 30 || m_head.y() < 0 || m_head.y() >= 30)
		return true;
	// ���� ������ ��������� ���� � ����
	for (auto& dot : m_dots)
		if (dot.x() == m_head.x() && dot.y() == m_head.y())
			return true;
	return false;
}

void Snake::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event); // ��������� ����������� �� �������������� ����������
	QPainter* painter = new QPainter(this);
	// ���� ���� �� ���������
	if (!isGameOver())
	{
		painter->drawImage(m_head * M_DOT_PIXELS, i_head); // ������ ������ ������
		painter->drawImage(m_apple * M_DOT_PIXELS, i_apple); // ������ ������
		// ������ ��� �������� ������
		for (auto& dot : m_dots)
			painter->drawImage(dot * M_DOT_PIXELS, i_dot);
	}
	// ���� ���� �����������
	else
	{
		killTimer(timerID); // ��������� ������
		painter->setRenderHint(QPainter::Antialiasing); // ������������� ����������� ��� ��������� ����������� ������
		// �������� ������ � ������ �������
		qreal width = this->width();
		qreal heigth = this->height();
		QString text("Game Over"); // ������������� �����
		QFont font("Times New Roman", 35); // ������������� �����
		QFontMetrics metrics(font); // ��������� ������
		qreal text_width = metrics.horizontalAdvance(text); // �������� ������ ������ � ������� ������
		painter->setPen(QPen(QColor("red"))); // ������������� ������� ����
		painter->setBrush(QBrush(QColor("white"))); // ������������� ����� �����
		// ������� ���� � ��������� � ���� ����� � ������������ � �������
		QPainterPath textpath;
		textpath.addText(width / 2 - text_width / 2, heigth / 2, font, text);
		painter->drawPath(textpath); // ������ ����
	}
}

void Snake::timerEvent(QTimerEvent* event)
{
	Q_UNUSED(event);
	moveHead(); // ������� ������ ������ ������ ��� �������
	eatApple(); // ��������� ����� �� ������ ������
	this->repaint(); // �������������� ������ ������ ���
}

// ���������� ������� ������� ������ ��� ��������� ����������� �������� ������ ������
void Snake::keyPressEvent(QKeyEvent* event)
{
	int key = event->key();
	if (key == Qt::Key_Up && !m_down_direction)
	{
		m_up_direction = true;
		m_right_direction = false;
		m_left_direction = false;
	}
	if (key == Qt::Key_Down && !m_up_direction)
	{
		m_down_direction = true;
		m_right_direction = false;
		m_left_direction = false;
	}
	if (key == Qt::Key_Right && !m_left_direction)
	{
		m_right_direction = true;
		m_up_direction = false;
		m_down_direction = false;
	}
	if (key == Qt::Key_Left && !m_right_direction)
	{
		m_left_direction = true;
		m_up_direction = false;
		m_down_direction = false;
	}
}