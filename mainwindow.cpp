#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "ui_dialog.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QtDebug>
#include <QTableWidgetItem>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>

//Joesdarian Ignatius - 2440021086 - LB40

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initTableWidget();
    openDatabase();
}


void MainWindow::initTableWidget()
{
    QStringList headers;
    headers <<"Kode "<< "Name   " << "Harga Satuan  " << "Jumlah ";

    ui->tableWidget->setColumnCount(headers.length());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::addTableWidgetItem(const QString &kode, const QString &nama, const QString &harga, const QString &jumlah)
{
    CellEdited = false;
    int nRowCount = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(nRowCount+1);
    ui->tableWidget->setItem(nRowCount, 0, new QTableWidgetItem(kode));
    ui->tableWidget->setItem(nRowCount, 1, new QTableWidgetItem(nama));
    ui->tableWidget->setItem(nRowCount, 2, new QTableWidgetItem(harga));
    ui->tableWidget->setItem(nRowCount, 3, new QTableWidgetItem(jumlah));
}

bool MainWindow::openDatabase()
{
    QString dbPath = QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).filePath("database.db");
    QFile::copy(":/database.db",dbPath);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    bool dbConnect = db.open();

    QSqlQuery query;
     query.exec("create table database_table"
                "(id integer primary key, "
                "kode varchar(5) , "
                "nama varchar(20), "
                "harga varchar(255) , "
                "jumlah varchar(255)) ");
    if (dbConnect)
    {
        qDebug() << "Database: Connected";
        loadBarangData();
    }
    else
    {
        qDebug() << "Error: Failed to Connect";
    }
}

void MainWindow::closeDatabase()
{
    db.close();
}

bool MainWindow::addItemToDatabase(const QString &kode, const QString &nama, const QString &harga, const QString &jumlah)
{

    QSqlQuery qry(db);
      qry.prepare("INSERT INTO database_table (kode,nama,harga,jumlah) VALUES (:kode,:nama,:harga,:jumlah);");
      qry.bindValue(":kode", kode);
      qry.bindValue(":nama", nama);
      qry.bindValue(":harga", harga);
      qry.bindValue(":jumlah", jumlah);
      bool fResult = qry.exec();
      if (!fResult)
          qDebug() << "InsertData Error: " <<
                      qry.lastError().text();
      return fResult;
}

bool MainWindow::loadBarangData()
{

    QSqlQuery qry("SELECT kode,nama,harga,jumlah FROM database_table",
                     db);

        while (qry.next())
        {
            QString kode = qry.value("kode").toString();
            QString nama  = qry.value("nama").toString();
            QString harga = qry.value("harga").toString();
            QString jumlah = qry.value("jumlah").toString();
            qDebug() << kode << ", "<< nama << ", " << harga << ", " << jumlah;
            addTableWidgetItem(kode,nama,harga,jumlah);
        }
        return true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAdd_triggered()
{
    Dialog add;
    if (add.exec() == QDialog::Accepted)
    {
        QString kode = add.ui->lineEditKode->text();
        QString nama = add.ui->lineEditNama->text();
        QString harga = add.ui->lineEditHarga->text();
        QString jumlah = add.ui->lineEditJumlah->text();
        addItemToDatabase(kode,nama,harga,jumlah);
        addTableWidgetItem(kode,nama,harga,jumlah);

        qDebug() << "Accepted: " << kode << "," << nama << "," <<harga << "," << jumlah;
    }
}

void MainWindow::on_actionDelete_triggered()
{
    int Result = QMessageBox::information(this, "Confirmation",
                             "Are you sure you want to delete all item?",
                             QMessageBox::Yes|QMessageBox::No,
                             QMessageBox::No);
    if (Result==QMessageBox::Yes)
    {
        int Current = ui->tableWidget->currentRow();
        ui->tableWidget->removeRow(Current);
    }
}


void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    CellEdited = true;
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    if (CellEdited)
    {
        qDebug("cellChanged %d, %d", row, column);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,"StockBarang","2440021086-Joesdarian Ignatius");
}


void MainWindow::on_actionExit_triggered()
{
    closeDatabase();
    close();
}

