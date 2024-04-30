#include "platform.h"
#include "ui_platform.h"

platform::platform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::platform)
{
    ui->setupUi(this);

    setWindowTitle("cim platform");

    mainLayout = new QGridLayout;
    ipmanagerLayout = new QVBoxLayout;
    ipLayout = new QHBoxLayout;
    ipbtnLayout = new QHBoxLayout;
    startBtn = new QPushButton("开始搜索");
    stopBtn = new QPushButton("终止搜索");
    localipLabel = new QLabel("本机地址：");
    ipBox = new QComboBox(this);
    devicetable = new QTableWidget(20,5);
    deviceLayout = new QVBoxLayout;

    ipLayout->addWidget(localipLabel);
    ipLayout->addWidget(ipBox);
    ipLayout->addStretch();

    ipbtnLayout->addWidget(startBtn);
    ipbtnLayout->addWidget(stopBtn);
    ipbtnLayout->addStretch();

    ipmanagerLayout->addLayout(ipLayout);
    ipmanagerLayout->addLayout(ipbtnLayout);

    QStringList headerText;
    headerText<<"序号"<<"设备IP"<<"设备类型"<<"SN码"<<"软件版本";  //表头标题用QStringList来表示
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
    devicetable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    devicetable->clearContents();
    devicetable->verticalHeader()->hide();

    deviceLayout->addWidget(devicetable);

    mainLayout->addLayout(ipmanagerLayout,0,0);
    mainLayout->addLayout(deviceLayout,1,0);

    this->setLayout(mainLayout);
    this->resize(1500,800);
}

platform::~platform()
{
    delete ui;
}
