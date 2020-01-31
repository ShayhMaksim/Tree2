#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml/QDomElement>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include "pCompany.h"
#include <QVector>
#include <QtXmlPatterns>
#include <algorithm>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <set>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

void ReadXML(QString file);
void Cancel();
void Return();
void Add();
int Delete();

void AddRoot(QString name,float count,float salary);
void AddChild(QTreeWidgetItem *parent,QString name_1, QString name_2,QString name_3,QString function,float salary);

private slots:
//void on_comboBox_activated(const QString &arg1);

void on_pushButton_5_clicked();
void on_pushButton_clicked();

private:
  Ui::MainWindow *ui;
  QString localPath;
  std::vector<std::shared_ptr<Department>> departments;
  std::multimap<std::shared_ptr<Department>,Person> members;
  std::vector<std::shared_ptr<std::multimap<std::shared_ptr<Department>,Person>>> controlSystem;
  std::map<QString,int> names;
  int departments_count=0;

};
#endif // MAINWINDOW_H
