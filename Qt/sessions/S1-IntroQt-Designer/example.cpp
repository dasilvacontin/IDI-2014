#include <QApplication>
#include <QPushButton>
#include <QFrame>

int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	QFrame F(0, NULL);
	QPushButton *hello = new QPushButton("shizzle my fizzle", &F);
	a.connect(hello, SIGNAL(clicked()), &F, SLOT(close()));
	F.show();
	return a.exec();
}
