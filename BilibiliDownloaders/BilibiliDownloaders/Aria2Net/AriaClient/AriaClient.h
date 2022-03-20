#pragma once

#include <QObject>

class AriaClient : public QObject
{
    Q_OBJECT

public:
    AriaClient(QObject *parent);
    ~AriaClient();
};
