#ifndef PCOMPANY_H
#define PCOMPANY_H
#include <QString>
#include <QVector>
#include <memory>

struct Department;

struct Person{
  QString employee[3];
  QString function;
  float salary;
};

/*
 * нужны завернуть в векторы
*/

struct Department{
  QString name;
  float count=0;
  float salary;
};



static QString keywords[]={"surname","name","middleName","function","salary"};

#endif // PCOMPANY_H
