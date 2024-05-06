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
#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QCheckBox>
#include <QLineEdit>
#include <QProgressBar>
#include "servernetwork.h"

namespace Ui {
class platform;
}

#define TIMEOUT 1*1000


// 定义设备信息结构
struct DeviceInfo {
    QString ipAddress;
    QString sn;
    QString v_software;
    bool isChecked;
    // 添加其他设备信息字段
};


class platform : public QWidget
{
    Q_OBJECT

public:
    explicit platform(QWidget *parent = nullptr);
    ~platform();
    void init_platform();
    void init_udp_client();
    void dataSend();
    bool initFTP();

public slots:
    void startBtn_clicked();
    void stopBtn_clicked();
    void startupdate_clicked();
    void handleTimerEvent();
    void dataReceived();

private:
    QTabWidget  *tabWidget;
    QWidget     *tab1;
    QLabel      *promptLabel;
    QVBoxLayout *mainlayout;
    QVBoxLayout *devupdateLayout;
    QHBoxLayout *updateLayout;
    QHBoxLayout *ipLayout;
    QHBoxLayout *ipbtnLayout;
    QVBoxLayout *ipmanagerLayout;
    QVBoxLayout *deviceLayout;
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QComboBox   *ipBox;
    QLabel      *localipLabel;
    QTableWidget *devicetable;
    QTableWidgetItem *headerItem;
    QLabel      *userLabel;
    QLineEdit   *userEdit;
    QLabel      *pwdLabel;
    QLineEdit   *pwdEdit;
    QHBoxLayout *userLayout;
    QLabel      *updateLabel;
    QLineEdit   *fileEdit;
    QPushButton *checkfilesBtn;
    QHBoxLayout *fileLayout;
    QPushButton *startupdateBtn;
    QPushButton *stopupdateBtn;
    QHBoxLayout *btnLayout;
    QVBoxLayout *operationLayout;

private:
    Ui::platform *ui;
    serverNetwork servernet;
    QTimer *search_Timer;
    int timeCount;
    int port;
    QUdpSocket *udpSocket;
    QHostAddress *groupAddress;
    bool searchflag;
    QMap<QCheckBox*, DeviceInfo> devicesMap;
    QString filePath;
    QString user;
    QString password;
};

#endif // PLATFORM_H
