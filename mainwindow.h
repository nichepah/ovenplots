#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include <QTimer>
#include <QObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupOvenPlot();
    void corePlot(QCustomPlot *customPlot);
    void getCokingPeriod();
    void getBalancePeriod();
    void getOverRun();
    void getLessThanMinCP();    // get coking period less than min cp; for better plotting


private slots:
  void realtimeDataSlot();

private:
    Ui::MainWindow *ui;
    double minCokingPeriod = 17;    // in hrs
    int count = 70;                 // no. of ovens
    QTimer *dataTimer;                // for repeated calls to replot

    QCPBars *cPeriodBar;
    QCPBars *bPeriodBar;
    QCPBars *oPeriodBar;


    QVector<double> ovens =   {    141, 142, 143, 144, 145, 146, 147, 148, 149,
                              150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
                              160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
                              170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
                              180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
                              190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                              200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
                              201
                              };

    // for better plotting; avoid that translation
    QVector<double> xovens =   {   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                                   20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                                   40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                                   60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70
                               };


    // Vector: actual coking period so far in hrs, including overruns;
    QVector<double> cokingPeriod =  {
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                                   };

    // Vector: minCokingPeriod-cokingPeriod in hrs for those less than minCokingPeriod
    QVector<double> balancePeriod =  {
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                                   };

    // Vector: period over cooked in hrs
    QVector<double> overRun =       {
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                                   };

    // Vector: period less than minCokingPeriod; for better plotting
    QVector<double> lessThanMinCP = {
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                                   };

};

#endif // MAINWINDOW_H
