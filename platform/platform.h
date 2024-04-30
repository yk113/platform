#ifndef PLATFORM_H
#define PLATFORM_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QLabel>
#include <QHeaderView>
#include <QTableWidgetItem>

namespace Ui {
class platform;
}

class platform : public QWidget
{
    Q_OBJECT

public:
    explicit platform(QWidget *parent = nullptr);
    ~platform();

private:
    QGridLayout *mainLayout;
    QHBoxLayout *ipLayout;
    QHBoxLayout *ipbtnLayout;
    QVBoxLayout *ipmanagerLayout;
    QVBoxLayout *deviceLayout;
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QComboBox *ipBox;
    QLabel *localipLabel;
    QTableWidget *devicetable;

private:
    Ui::platform *ui;
    QTableWidgetItem *headerItem;
};

#endif // PLATFORM_H
