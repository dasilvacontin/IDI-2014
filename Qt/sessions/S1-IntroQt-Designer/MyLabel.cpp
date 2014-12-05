#include <MyLabel.h>

void MyLabel::setUppercaseText(QString s) {
	s = s.toUpper();
	setText(s);
}
