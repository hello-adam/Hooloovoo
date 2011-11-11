#ifndef INPUTRECEIVER_H
#define INPUTRECEIVER_H

#include <QKeyEvent>

class InputReceiver
{
public:
    explicit InputReceiver();
    ~InputReceiver();

    virtual void handleKeyEvent(QKeyEvent *ke) = 0;

signals:

public slots:

};

#endif // INPUTRECEIVER_H
