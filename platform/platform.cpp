#include "platform.h"
#include "ui_platform.h"
#include <QMessageBox>

platform::platform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::platform)
{
    ui->setupUi(this);



    ipLayout = new QHBoxLayout;
    localipLabel = new QLabel("本机地址：");
    ipBox = new QComboBox();
    ipLayout->addWidget(localipLabel);
    ipLayout->addWidget(ipBox);
    ipLayout->addStretch();

    ipbtnLayout = new QHBoxLayout;
    startBtn = new QPushButton("开始搜索");
    stopBtn = new QPushButton("终止搜索");
    ipbtnLayout->addWidget(startBtn);
    ipbtnLayout->addWidget(stopBtn);
    ipbtnLayout->addStretch();

    ipmanagerLayout = new QVBoxLayout;
    ipmanagerLayout->addLayout(ipLayout);
    ipmanagerLayout->addLayout(ipbtnLayout);

    userLabel = new QLabel("用户名称：");
    userEdit = new QLineEdit();
    pwdLabel = new QLabel("用户密码：");
    pwdEdit = new QLineEdit();
    userLayout = new QHBoxLayout;
    userLayout->addWidget(userLabel);
    userLayout->addWidget(userEdit);
    userLayout->addWidget(pwdLabel);
    userLayout->addWidget(pwdEdit);
    userLayout->addStretch();

    updateLabel = new QLabel("升级文件：");
    fileEdit = new QLineEdit();
    checkfilesBtn = new QPushButton("选择..");
    fileLayout = new QHBoxLayout;
    fileLayout->addWidget(updateLabel);
    fileLayout->addWidget(fileEdit);
    fileLayout->addWidget(checkfilesBtn);
    fileLayout->addStretch();

    startupdateBtn = new QPushButton("开始升级");
    stopupdateBtn = new QPushButton("全部终止");
    btnLayout = new QHBoxLayout;
    btnLayout->addWidget(startupdateBtn);
    btnLayout->addWidget(stopupdateBtn);
    btnLayout->addStretch();

    operationLayout = new QVBoxLayout;
    operationLayout->addLayout(userLayout);
    operationLayout->addLayout(fileLayout);
    operationLayout->addLayout(btnLayout);

    updateLayout = new QHBoxLayout;
    updateLayout->addLayout(ipmanagerLayout);
    updateLayout->addLayout(operationLayout);

    devicetable = new QTableWidget(20,6);
    QStringList headerText;
    headerText<<"序号"<<"设备IP"<<"设备类型"<<"SN码"<<"软件版本"<<"升级状态";  //表头标题用QStringList来表示
    devicetable->setColumnCount(headerText.count());//列数设置为与 headerText的行数相等
    for (int i=0;i<devicetable->columnCount();i++)//列编号从0开始
    {
        headerItem=new QTableWidgetItem(headerText.at(i)); //新建一个QTableWidgetItem， headerText.at(i)获取headerText的i行字符串
        QFont font=headerItem->font();//获取原有字体设置
        font.setBold(true);//设置为粗体
        font.setPointSize(12);//字体大小
        headerItem->setFont(font);//设置字体
        devicetable->setHorizontalHeaderItem(i,headerItem); //设置表头单元格的Item
    }
    devicetable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    devicetable->horizontalHeader()->setStretchLastSection(true);
    devicetable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    devicetable->clearContents();
    devicetable->verticalHeader()->hide();
    devicetable->setAlternatingRowColors(1);
    devicetable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    devicetable->setSelectionMode(QAbstractItemView::NoSelection);

    promptLabel = new QLabel("[准备搜索]");
    promptLabel->setStyleSheet("QLabel {color:green}");

    deviceLayout = new QVBoxLayout;
    deviceLayout->addWidget(devicetable);
    deviceLayout->addWidget(promptLabel);

    devupdateLayout = new QVBoxLayout;

    devupdateLayout->addLayout(updateLayout);
    devupdateLayout->addLayout(deviceLayout);

    QStringList list_ip = servernet.Getip();
    for (auto item : list_ip)
    {
        ipBox->addItem(item,item);
    }
    connect(startBtn,&QPushButton::clicked,this,&platform::startBtn_clicked);
    connect(stopBtn,&QPushButton::clicked,this,&platform::stopBtn_clicked);
    connect(startupdateBtn,&QPushButton::clicked,this,&platform::startupdate_clicked);

    init_platform();

    tabWidget = new QTabWidget();

    tab1 = new QWidget();

    tab1->setLayout(devupdateLayout);

    QIcon icon1("../icon/device.png");

    tabWidget->addTab(tab1, icon1, "设备升级");

    mainlayout = new QVBoxLayout();

//    //新建第二个页面的部件
//    QLabel *label = new QLabel("Hello Qt");
//    QIcon icon2("../icon/device.png");
//    tabWidget->addTab(label, icon2, "Tab2");

    mainlayout->addWidget(tabWidget);

    this->setLayout(mainlayout);
    this->resize(1000,500);
    this->setWindowTitle("cim platform");
}

platform::~platform()
{
    delete ui;
}

void platform::init_platform()
{
    devicesMap.clear();
    DeviceInfo deviceInfo;
    timeCount = 0;
    search_Timer = new QTimer();
    search_Timer->setInterval(TIMEOUT);
    connect(search_Timer, &QTimer::timeout, this, &platform::handleTimerEvent);
//    QCheckBox *checkBox = new QCheckBox();
//    checkBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    QWidget *checkBoxWidget = new QWidget();
//    QHBoxLayout *layout = new QHBoxLayout(checkBoxWidget);
//    layout->addWidget(checkBox);
//    layout->setContentsMargins(0, 0, 0, 0);
//    checkBoxWidget->setLayout(layout);
//    devicetable->setCellWidget(0, 0, checkBoxWidget);
    //devicetable->setItem(0,0,indexItem);
    //devicetable->setItem(0,1,ipItem);

//    deviceInfo.ipAddress = "ipv4Address";
//    deviceInfo.isChecked = checkBox->isChecked();
//    devicesMap.insert(checkBox,deviceInfo);
//    QObject::connect(checkBox, &QCheckBox::stateChanged, [&](int state)
//    {
//        devicesMap[checkBox].isChecked = (state == Qt::Checked);
//    });
}

void platform::init_udp_client()
{
    udpSocket = new QUdpSocket(this);
    int lastIndexDot = ipBox->currentText().lastIndexOf('.');
    if (lastIndexDot != -1) {
        QString newIpAddress = ipBox->currentText().left(lastIndexDot + 1) + "255";
        qDebug() << newIpAddress;
        groupAddress = new QHostAddress(newIpAddress);
        udpSocket->joinMulticastGroup(*groupAddress);
        connect(udpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));

        port = 5555;
        searchflag = false;

        bool result = udpSocket->bind(port,QUdpSocket::ShareAddress);
        if(!result)
        {
            QMessageBox::information(this,tr("error"),tr("udp socket create error!"));
            return;
        }
    }
    else
    {
        QMessageBox::information(this,tr("error"),tr("ip is error!"));
    }

}

bool platform::initFTP()
{
    //判断有没有填写URL地址和用户名、密码
    if (fileEdit->text().isEmpty()) {
        QMessageBox::critical(NULL, tr("Error"), "URL地址不能为空");
        return false;
    }
    else if (userEdit->text().isEmpty()) {
        QMessageBox::critical(NULL, tr("Error"), "user不能为空");
        return false;
    }
    else if (pwdEdit->text().isEmpty()) {
        QMessageBox::critical(NULL, tr("Error"), "密码不能为空");
        return false;
    }
    else {
        filePath = fileEdit->text();
        user = userEdit->text();
        password = pwdEdit->text();
        return true;
    }
}

void platform::handleTimerEvent()
{
    timeCount++;
    if((timeCount <= 10)&&(searchflag == false))
    {
        udpSocket->writeDatagram("Hello from UDP client",sizeof("search"),*groupAddress,port);
    }
    else
    {
        if(searchflag == false)
        {
            QMessageBox::information(this,tr("error"),tr("search fail!"));
        }
        promptLabel->setText("[搜索结束]");
        timeCount = 0;
        search_Timer->stop();
        delete udpSocket;
        delete groupAddress;
    }
}

void platform::startBtn_clicked()
{
   init_udp_client();
   search_Timer->start();
   promptLabel->setText("[搜索中...]");
}

void platform::stopBtn_clicked()
{
    promptLabel->setText("[搜索终止]");
    timeCount = 0;
    search_Timer->stop();
    delete udpSocket;
    delete groupAddress;
}


void platform::dataReceived()
{
    int index = 0;
    DeviceInfo deviceInfo;
    while (udpSocket->hasPendingDatagrams()){
         QByteArray receData;//创建接收字节流
         QHostAddress peerAddr;//接收地址
         quint16 peerPort;//接收端口
         quint16 size=udpSocket->pendingDatagramSize();//读取接收udp数据大小
         receData.resize(size);//重设接收字节流大小
         receData.prepend("NOT_ME");
         udpSocket->readDatagram(receData.data(),receData.size(),&peerAddr,&peerPort);//读取以太网发送端的udp数据

         if(receData.startsWith("NOT_ME"))
         {
             //qDebug() <<"receData:"<<receData.data();

             QStringList parts = peerAddr.toString().split(QRegExp(":"),QString::SkipEmptyParts);

             if (!parts.isEmpty()) {
                 devicesMap.clear();
                 QString ipv4Address = parts.last(); // 假设 IPv4 地址是最后一个部分
                 searchflag = true;
                 index++;
                 QString strindex = QString::number(index);
                 QTableWidgetItem *indexItem = new QTableWidgetItem(strindex);
                 indexItem->setTextAlignment(Qt::AlignCenter);
                 QTableWidgetItem *ipItem = new QTableWidgetItem(ipv4Address);

                 QCheckBox *checkBox = new QCheckBox();
                 checkBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                 QWidget *checkBoxWidget = new QWidget();
                 QHBoxLayout *layout = new QHBoxLayout(checkBoxWidget);
                 layout->addWidget(checkBox);
                 layout->setContentsMargins(0, 0, 0, 0);
                 checkBoxWidget->setLayout(layout);
                 devicetable->setCellWidget((index-1), 0, checkBoxWidget);
                 devicetable->setItem((index-1),0,indexItem);
                 devicetable->setItem((index-1),1,ipItem);

                 deviceInfo.ipAddress = ipv4Address;
                 deviceInfo.isChecked = false;
                 devicesMap.insert(checkBox,deviceInfo);
                 QObject::connect(checkBox, &QCheckBox::stateChanged, [&](int state) {
                     devicesMap[checkBox].isChecked = (state == Qt::Checked);
                 });
             }
         }
     }
}

void platform::startupdate_clicked()
{
    //if(initFTP())
    {
        QMap<QCheckBox*, DeviceInfo>::const_iterator it;
        qDebug() << "count: " << devicesMap.count();
        for (it = devicesMap.constBegin(); it != devicesMap.constEnd(); ++it)
        {
            DeviceInfo deviceInfo = it.value();
            qDebug() << "IP Address: " << deviceInfo.ipAddress;
            qDebug() << "Is Checked: " << deviceInfo.isChecked;
            // 这里可以做更多的操作
        }
    }
}

