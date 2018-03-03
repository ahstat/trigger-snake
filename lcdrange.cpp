#include <QLCDNumber>
#include <QSlider>
#include <QVBoxLayout>

#include "LCDRange.h"

LCDRange::LCDRange(QWidget *parent) : QWidget(parent)
{
    lcd = new QLCDNumber(5);
    QVBoxLayout *layout = new QVBoxLayout;
    layout -> addWidget(lcd);
    setLayout(layout);
    lcd -> setSegmentStyle(QLCDNumber::Flat);
}

void LCDRange::setValue(int value)
{
    lcd -> display(value);
}
