#include "mainwindow.h"
#include "ui_mainwindow.h"



float getMeanSalary(std::vector<Person> arg){
  float result=0;
  for(int i=0;i<arg.size();i++)
    {
      result+=arg.at(i).salary;
    }
  return result/arg.size();
}


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::ReadXML(QString filename)
{
    std::vector<Person> pack;
    Person current_p;
    Department current_dep;
    QFile file(filename);

     int key=0;
     bool keyblock[]={false,false,false,false,false};
     QString department_name;
    if (file.open(QIODevice::ReadOnly)){
         QXmlStreamReader xml(&file);
         while(!xml.atEnd()){
             xml.readNext();
             qDebug() <<xml.tokenString()
                    <<xml.name()
                   <<xml.text();
          QXmlStreamAttributes attributes = xml.attributes();


          if ((xml.tokenString()=="StartElement")&&(xml.name().toString()=="department")){
          if (attributes.hasAttribute("name"))
                  department_name = attributes.value("name").toString();

            }

          if ((xml.tokenString()=="EndElement")&&(xml.name().toString()=="department"))
            {
              current_dep.name=department_name;
              current_dep.count=pack.size();
              current_dep.salary=getMeanSalary(pack);

              if (names.find(department_name)==names.end())
                {
                  departments.push_back(std::make_shared<Department>(std::move(current_dep)));
                  names.insert(std::make_pair(department_name,departments_count));
                  departments_count++;
                }

              else if (names.find(department_name)!=names.end())
                {
                  for(int i=0;i<departments.size();i++)
                    {
                      if(departments.at(i)->name==department_name)
                        {
                          departments.at(i)->salary=(departments.at(i)->salary*departments.at(i)->count+current_dep.salary*current_dep.count)/(departments.at(i)->count+current_dep.count);
                          departments.at(i)->count+=current_dep.count;
                        }
                    }
                }

              for(int i=0;i<current_dep.count;i++)
                {
                  auto arg1=names[department_name];
                  members.insert(std::make_pair(departments.at(arg1),pack[i]));
                }
              pack.clear();

            }

             if ((xml.tokenString()=="StartElement")&&(xml.name().toString()==keywords[key])){
                 keyblock[key]=true;
               }

             if ((xml.tokenString()=="EndElement")&&(xml.name().toString()==keywords[key])){
                 keyblock[key]=false;
                 if (key!=4) {
                     ++key;
                   }
                 else {
                     key=0;
                     pack.push_back(current_p);
                   }
               }

             if (xml.tokenString()=="Characters") {
                 switch(key){
                   case 0:
                     if (keyblock[0]==true){
                         current_p.employee[0]=xml.text().toString();
                       }
                     break;
                   case 1:
                     if (keyblock[1]==true){
                         current_p.employee[1]=xml.text().toString();
                       }
                     break;
                   case 2:
                     if (keyblock[2]==true){
                         current_p.employee[2]=xml.text().toString();
                       }
                     break;
                   case 3:
                     if (keyblock[3]==true){
                         current_p.function=xml.text().toString();
                       }
                     break;
                   case 4:
                     if (keyblock[4]==true){
                         current_p.salary=xml.text().toFloat();
                       }
                     break;
                   }
               }
           }
         if (xml.hasError()){
             qDebug()<<"Error:"<<xml.errorString();
           }
         file.close();
      }
    //    controlSystem.push_back(members);
    controlSystem.push_back(std::make_shared<std::multimap<std::shared_ptr<Department>,Person>>(members));
}



void MainWindow::on_pushButton_5_clicked()
{
  // Выбор целевого каталога для скачивания
  auto fileName = QFileDialog::getOpenFileName(this,
                                               tr("Open Xml"), 0 , tr("XML files (*.xml)"));
  localPath=fileName;
  if (localPath!="")
    {
  ui->label_5->setText("Complete");
  ui->lineEdit->clear();
  ui->lineEdit_2->clear();
  ui->lineEdit_3->clear();
  ui->lineEdit_4->clear();
  ui->lineEdit_5->clear();
  ui->lineEdit_6->clear();
  ui->lineEdit->setDisabled(true);
  ui->lineEdit_2->setDisabled(true);
  ui->lineEdit_3->setDisabled(true);
  ui->lineEdit_4->setDisabled(true);
  ui->lineEdit_5->setDisabled(true);
  ui->lineEdit_6->setDisabled(true);
    }

}


void MainWindow::on_pushButton_clicked()
{
    ui->treeWidget_2->clear();
    if (ui->label_5->text()=="Complete"){
        ReadXML(localPath);
      } else {
          auto first_name=ui->lineEdit->text();
          auto name=ui->lineEdit_5->text();
          auto last_name=ui->lineEdit_6->text();
          auto function=ui->lineEdit_3->text();
          auto salary=ui->lineEdit_2->text();
          auto department_name=ui->lineEdit_4->text();

          Person pack;
          pack.employee[0]=first_name;
          pack.employee[1]=name;
          pack.employee[2]=last_name;
          pack.function=function;
          pack.salary=salary.toFloat();

          Department current_dep;
          current_dep.name=department_name;
          current_dep.count=1;
          current_dep.salary=salary.toFloat();
          if (names.find(department_name)==names.end())
            {
              departments.push_back(std::make_shared<Department>(std::move(current_dep)));
              names.insert(std::make_pair(department_name,departments_count));
              departments_count++;
            }
          else if(names.find(department_name)!=names.end())
            {
              for(int i=0;i<departments.size();i++)
                {
                  if(departments.at(i)->name==department_name)
                    {
                      /*
                       * Неправильно рассчитывается средняя сумма
                       */
                      departments.at(i)->salary=(departments.at(i)->salary*departments.at(i)->count+current_dep.salary*current_dep.count)/(departments.at(i)->count+current_dep.count);
                      departments.at(i)->count+=current_dep.count;
                    }
                }
            }
             auto arg1=names[department_name];
             members.insert(std::make_pair(departments.at(arg1),pack));
             controlSystem.push_back(std::make_shared<std::multimap<std::shared_ptr<Department>,Person>>(members));
      }
    for(int i=0;i<departments.size();i++){
        AddRoot(departments[i]->name,departments[i]->count,departments[i]->salary);
        auto arg=members.equal_range(departments[i]);
        for(auto j =arg.first; j!=arg.second;j++)
          {
                    AddChild(ui->treeWidget_2->topLevelItem(i),
                                  j->second.employee[0],
                                  j->second.employee[1],
                                  j->second.employee[2],
                                  j->second.function,
                                  j->second.salary);
          }
      }
    localPath="";
    ui->label_5->text()="";
    ui->lineEdit->setDisabled(false);
    ui->lineEdit_2->setDisabled(false);
    ui->lineEdit_3->setDisabled(false);
    ui->lineEdit_4->setDisabled(false);
    ui->lineEdit_5->setDisabled(false);
    ui->lineEdit_6->setDisabled(false);
}

void MainWindow::AddRoot(QString name,float count,float salary)
{

            QTreeWidgetItem *item=new QTreeWidgetItem(ui->treeWidget_2);
            item->setText(0,name);
            item->setText(1,QString::number(count));
            item->setText(2,QString::number(salary));
            ui->treeWidget_2->addTopLevelItem(item);

}

void MainWindow::AddChild(QTreeWidgetItem *parent,QString name_1, QString name_2,QString name_3,QString function,float salary)
{
  QTreeWidgetItem *item=new QTreeWidgetItem(parent);
  item->setText(0,name_1+name_2+name_3);
  item->setText(1,function);
  item->setText(2,QString::number(salary));
  parent->addChild(item);
}
