#include <QLCDNumber>
 #include <QSlider>
 #include <QVBoxLayout>

 #include "lcdrange.h"

 LCDRange::LCDRange(QWidget *parent)
     : QWidget(parent)
 {
     lcd = new QLCDNumber(5);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(lcd);
     //layout->addWidget(slider);
     setLayout(layout);

lcd->setSegmentStyle(QLCDNumber::Flat);

     //setFocusProxy(slider);
 }

/*
 int LCDRange::value() const
 {
     return slider->value();
 }
*/
 void LCDRange::setValue(int value)
 {
     lcd->display(value);
 }
/*
 void LCDRange::setRange(int minValue, int maxValue)
 {
     if (minValue < 0 || maxValue > 99 || minValue > maxValue) {
         qWarning("LCDRange::setRange(%d, %d)\n"
                  "\tRange must be 0..99\n"
                  "\tand minValue must not be greater than maxValue",
                  minValue, maxValue);
         return;
     }
     slider->setRange(minValue, maxValue);
 }
*/
