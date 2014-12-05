#include <QApplication>
#include <QPushButton>
int main(int argc, char **argv)
{
	QApplication a(argc,argv);
	QPushButton hello("Hello Qt!", 0);
	hello.resize(100, 30);
	hello.show();
	return a.exec();
}
