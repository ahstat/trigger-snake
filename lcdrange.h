#ifndef LCDRANGE_H
#define LCDRANGE_H

#include <QWidget>

class QLCDNumber;
class QSlider;

class LCDRange : public QWidget
{
    Q_OBJECT

    public:
        LCDRange(QWidget *parent = 0);

    public slots:
        void setValue(int value);

    signals:

    private:
        QLCDNumber *lcd;
};

#endif
