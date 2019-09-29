/* Author: Aneesh PA
 * 2019 Sept 27
 * Displays oven coking period wrt min. coking period
 * To be used along with scheduler.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>

#include <iostream>
#include <QtSql/QSqlError>
#include <QDateTime>
#include <QTimer>
#include <QObject>

// CHANGE_RELEASE
const QString chargeDb = "D:\\PA\\20190816\\openOPC\\openopc-toolkit\\OpenOPC-1.3.1.win32\\OpenOPC\\src\\charge_db.db";

// const QString chargeDb = "C:\\Users\\aapc-pa\\Desktop\\Reports\\db\\charge_db.db";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataTimer = new QTimer(this);
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));

    // x, y, w, h
    //setGeometry(300, 200, 1224, 690);
    setupOvenPlot();    // call  plot

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupOvenPlot(){
    corePlot(ui->customPlot);
    setWindowTitle("Oven Status");
    statusBar()->clearMessage();
    ui->customPlot->replot();
}

void MainWindow::corePlot(QCustomPlot *customPlot){
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    customPlot->setBackground(QBrush(gradient));

    // create empty bar chart objects: cPeriodBar, bPeriodBar, oPeriodBar: coking, balance and overrun;
    cPeriodBar = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    bPeriodBar = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    oPeriodBar = new QCPBars(customPlot->xAxis, customPlot->yAxis);

    cPeriodBar->setAntialiased(false); // gives more crisp, pixel aligned bar borders
    bPeriodBar->setAntialiased(false);
    oPeriodBar->setAntialiased(false);
    cPeriodBar->setStackingGap(1);
    bPeriodBar->setStackingGap(1);
    oPeriodBar->setStackingGap(1);

    // set names and colors:
    cPeriodBar->setName("Actual coking period");
    cPeriodBar->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    cPeriodBar->setBrush(QColor(0, 168, 140));


    bPeriodBar->setName("Balance coking period");
    bPeriodBar->setPen(QPen(QColor(111, 9, 176).lighter(170)));
    bPeriodBar->setBrush(QColor(111, 9, 176));

    oPeriodBar->setName("Coking period overrun");
    oPeriodBar->setPen(QPen(QColor(250, 170, 20).lighter(150)));
    oPeriodBar->setBrush(QColor(250, 170, 20));

    // stack bars on top of each other:
    bPeriodBar->moveAbove(cPeriodBar);
    oPeriodBar->moveAbove(bPeriodBar);

    // prepare x axis with oven nos
    QVector<double> ticks;
    QVector<QString> labels;
    ticks <<1<<2<<3<<4<<5<<6<<7<<8<<9<<10<<11<<12<<13<<14<<15<<16<<17<<18<<19<<20<<21<<22<<23<<24<<25<<26<<27<<28<<29<<30<<31<<32<<33<<34<<35<<36<<37<<38<<39<<40<<41<<42<<43<<44<<45<<46<<47<<48<<49<<50<<51<<52<<53<<54<<55<<56<<57<<58<<59<<60<<61<<62<<63<<64<<65<<66<<67<<68<<69<<70;

    labels << "141" << "142" << "143" << "144" << "145" << "146" << "147" << "148" <<"149" << "150" << "151" << "152" << "153" << "154" << "155" << "156" << "157" << "158" <<"159"<<"160" << "161" << "162" << "163" << "164" << "165" << "166" << "167" << "168" <<"169" << "170" << "171" << "172" << "173" << "174" << "175" << "176" << "177" << "178" <<"179"<< "180" << "181" << "182" << "183" << "184" << "185" << "186" << "187" << "188" <<"189" << "190" << "191" << "192" << "193" << "194" << "195" << "196" << "197" << "198" <<"199"<< "200" << "201" << "202" << "203" << "204" << "205" << "206" << "207" << "208" <<"209" <<"210";

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    customPlot->xAxis->setTicker(textTicker);
    customPlot->xAxis->setTickLabelRotation(45);
    customPlot->xAxis->setSubTicks(false);
    customPlot->xAxis->setTickLength(0, 4);

    customPlot->xAxis->setRange(0, 71);
    customPlot->xAxis->setBasePen(QPen(Qt::white));
    customPlot->xAxis->setTickPen(QPen(Qt::white));
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->xAxis->setLabel("ovens");

    // prepare y axis:
    customPlot->yAxis->setRange(-1, 25);
    customPlot->yAxis->setPadding(5); // left padding
    customPlot->yAxis->setLabel("coking duration in hours");
    customPlot->yAxis->setBasePen(QPen(Qt::white));
    customPlot->yAxis->setTickPen(QPen(Qt::white));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white));
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));


    // setup legend:
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    customPlot->legend->setBorderPen(Qt::NoPen);
    QFont legendFont = font();
    legendFont.setPointSize(8);
    customPlot->legend->setFont(legendFont);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    this->dataTimer->start(1000);  // called every second
}


void MainWindow::getCokingPeriod(){
    // read from charge time db_c
    QSqlDatabase db_c = QSqlDatabase::addDatabase("QSQLITE", "conn_c2");
    db_c.setDatabaseName(chargeDb);
    if (db_c.open()){
        // std::cout<<"db_c.opened";
    }
    QSqlQuery query_c(db_c);
    //to get potential CP
    QVariant record_c;
    QDateTime now = QDateTime::currentDateTime();

    int oven;

    for (int m=0; m<ovens.length(); ++m)
        {
        oven = int(this->ovens[m]);     // for each oven
        query_c.prepare("SELECT CHARGE_TIME FROM T_OVEN_CHARGE_TIME WHERE OVEN = :oven ORDER BY id DESC LIMIT 1");
        query_c.bindValue(":oven", oven);
        if (!query_c.exec()){
            // qDebug() << query_c.lastError().text();
            }
        // std::cout<<"accessing record_c"<<std::endl;
        while (query_c.next()){
            record_c = query_c.value(0);
            }
        QDateTime chargeTime = record_c.toDateTime();
        int cpHrs = (int(chargeTime.secsTo(now))/(60*60));   //cokingPeriod till now in hrs
        // std::cout<<cpHrs/60<<"\t";
        this->cokingPeriod.insert(m, cpHrs);      // save coking period in to the list
        // std::cout<<cokingPeriod.value(m)<<"\t";
        }
      db_c.close();
}


void MainWindow::getBalancePeriod(){
    // clear it
    for(int i=0; i<this->count; ++i){
        balancePeriod.insert(i,0);
    }

    for(int i=0; i<this->count; ++i){
        if( cokingPeriod.value(i) < this->minCokingPeriod){
            double t = this->minCokingPeriod - cokingPeriod.value(i);
            balancePeriod.insert(i,t);
            // std::cout<<"\n balancePeriod"<<balancePeriod.value(i)<<"\t";
        }
    }
}

// get over runs if any
void MainWindow::getOverRun(){
    // clear it
    for(int i=0; i<this->count; ++i){
        overRun.insert(i,0);
    }

    for(int i=0; i<this->count; ++i){
        if( cokingPeriod.value(i) > this->minCokingPeriod) {
            double t = cokingPeriod.value(i) - this->minCokingPeriod;
            overRun.insert(i,t);
            // std::cout<<"\n overRun"<<overRun.value(i)<<"\t";

        }
    }
}

void MainWindow::getLessThanMinCP(){
    // clear it
    for(int i=0; i<this->count; ++i){
        lessThanMinCP.insert(i,0);
    }

    for(int i=0; i<this->count; ++i){
        if( cokingPeriod.value(i) <= this->minCokingPeriod){
            double t = cokingPeriod.value(i);
            lessThanMinCP.insert(i, t);
        }
        else{
            lessThanMinCP.insert(i, this->minCokingPeriod);
        }
    }
}

void MainWindow::realtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 10) // at most update every 10s ; originally at most add point every 2 ms with 0.002
    {
        getCokingPeriod();      // gets actual coking period
        getBalancePeriod();     // gets balance part
        getOverRun();           // gets overrun
        getLessThanMinCP();     // gets plotting friends data for actual period less than min coking cases

        cPeriodBar->setData(xovens, lessThanMinCP);
        bPeriodBar->setData(xovens, balancePeriod);
        oPeriodBar->setData(xovens, overRun);
        ui->customPlot->replot();
        lastPointKey = key;
    }

}
