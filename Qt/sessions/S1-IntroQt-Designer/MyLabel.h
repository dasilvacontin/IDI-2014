#include <QLabel>
#include <QString>

class MyLabel: public QLabel
{

	Q_OBJECT
	
	public:
	MyLabel(QString text, QFrame*& f) : QLabel (text, f) {

	}

	public slots:
	void setUppercaseText (QString s);

};
