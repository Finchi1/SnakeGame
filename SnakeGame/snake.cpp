#include "snake.h"
#include <qpainter.h>
#include <QKeyEvent>
#include <qfontmetrics.h>
#include <qpainterpath.h>
#include <random>

Snake::Snake(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedSize(M_WIDTH, M_HEIGHT); // установим фиксированный размер окна
	this->setStyleSheet("background-color: black"); // установим черный цвет фона
	// Задаем начальные координаты головы змейки
	m_head.setX(2);
	m_head.setY(2);
	// Добавляем два элемента тела змейки и задаем им начальные координаты
	m_dots.push_back(QPoint(1, 2));
	m_dots.push_back(QPoint(0, 2));
	// Задаем начальное направление змейки
	m_up_direction = false;
	m_down_direction = false;
	m_right_direction = true;
	m_left_direction = false;
	loadImages(); // загружаем картинки
	putApple(); // разместим яблоко на игровом поле случайным образом
	timerID = startTimer(M_DELAY); // запускаем таймер и записываем его ID
}
// Загружаем картинки и задаем им ширину по 10 пикселей каждая (высота подгоняется по ширине автоматически)
void Snake::loadImages()
{
	i_head = QImage("head.png");
	i_head = i_head.scaledToWidth(10);
	i_apple = QImage("apple.png");
	i_apple = i_apple.scaledToWidth(10);
	i_dot = QImage("dot.png");
	i_dot = i_dot.scaledToWidth(10);
}
// Размещаем яблоко на игровом поле случайным образом
void Snake::putApple()
{
	bool no_snake = false;
	int counter = 0;
	int x, y;
	// Проверяем чтобы яблоко не размещалось на теле или голове змейки
	while (!no_snake)
	{
		// В качестве генератора случайных чисел используем вихрь Мерсена для получения координат
		x = std::mt19937(std::random_device{}())() % (M_WIDTH / M_DOT_PIXELS);
		y = std::mt19937(std::random_device{}())() % (M_HEIGHT / M_DOT_PIXELS);
		// Проверяем чтобы яблоко не размещалось на голове змейки
		if (m_head.x() != x || m_head.y() != y)
			// Проверяем чтобы яблоко не размещалось на теле змейки
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
	// Устанавливаем полученные случайные координаты для яблока
	m_apple.setX(x);
	m_apple.setY(y);
}

// Обрабатываем движение головы змейки в разных направлениях, для уменьшения повторяемости кода хвост обрабатываем отдельно
void Snake::moveHead()
{
	m_tail_shadow = m_dots.at(m_dots.size() - 1); // сохраняем координаты последнего элемента хвоста перед перемещением
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

// Обрабатываем движение хвоста в след за головой змейки (отдельно для уменьшения повторяемости кода)
void Snake::moveTail()
{
	int x = m_head.x();
	int y = m_head.y();
	for (int i = m_dots.size() - 1; i > 0; i--)
		m_dots.replace(i, QPoint(m_dots.at(i - 1).x(), m_dots.at(i - 1).y()));
	m_dots.replace(0, QPoint(x, y));
}
// Проверяем съела ли змейка яблоко, если да, то добалвяем один элемент к змейке на координаты m_tail_shadow
void Snake::eatApple()
{
	if (m_head.x() == m_apple.x() && m_head.y() == m_apple.y())
	{
		m_dots.push_back(m_tail_shadow);
		putApple();
	}
}

// Проверяем закончилась ли игра
bool Snake::isGameOver()
{
	// Если вышли за коодинаты поля
	if (m_head.x() < 0 || m_head.x() >= 30 || m_head.y() < 0 || m_head.y() >= 30)
		return true;
	// Если змейка врезалась сама в себя
	for (auto& dot : m_dots)
		if (dot.x() == m_head.x() && dot.y() == m_head.y())
			return true;
	return false;
}

void Snake::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event); // указываем компилятору на неиспользуемую переменную
	QPainter* painter = new QPainter(this);
	// Если игра не закончена
	if (!isGameOver())
	{
		painter->drawImage(m_head * M_DOT_PIXELS, i_head); // рисуем голову змейки
		painter->drawImage(m_apple * M_DOT_PIXELS, i_apple); // рисуем яблоко
		// Рисуем все элементы хвоста
		for (auto& dot : m_dots)
			painter->drawImage(dot * M_DOT_PIXELS, i_dot);
	}
	// Если игра закончилась
	else
	{
		killTimer(timerID); // выключаем таймер
		painter->setRenderHint(QPainter::Antialiasing); // устанавливаем сглаживание для красивого отображения текста
		// Получаем высоту и ширину виджета
		qreal width = this->width();
		qreal heigth = this->height();
		QString text("Game Over"); // устанавливаем текст
		QFont font("Times New Roman", 35); // устанавливаем шрифт
		QFontMetrics metrics(font); // измерение шрифта
		qreal text_width = metrics.horizontalAdvance(text); // измеряем ширину текста в текущем шрифте
		painter->setPen(QPen(QColor("red"))); // устанавливаем красное перо
		painter->setBrush(QBrush(QColor("white"))); // устанавливаем белую кисть
		// Создаем патч и добавляем в него текст с координатами и шрифтом
		QPainterPath textpath;
		textpath.addText(width / 2 - text_width / 2, heigth / 2, font, text);
		painter->drawPath(textpath); // рисуем патч
	}
}

void Snake::timerEvent(QTimerEvent* event)
{
	Q_UNUSED(event);
	moveHead(); // двигаем голову змейки каждый тик таймера
	eatApple(); // проверяем сЪела ли змейка яблоко
	this->repaint(); // перерисовываем виджет каждый тик
}

// Обработчик события нажатия клавиш для изменения направления движения головы змейки
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