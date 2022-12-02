#include <QtWidgets/QApplication>
#include "snake.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	Snake window;

	window.setWindowTitle("Window");
	window.setWindowIcon(QIcon("Finch.jpg"));
	window.show();

	return a.exec();
}
