#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initTableWidget();
    void addTableWidgetItem(const QString &kode, const QString &nama, const QString &harga, const QString &jumlah);
    bool addItemToDatabase(const QString &kode, const QString &nama, const QString &harga, const QString &jumlah);
    void closeDatabase();
    bool loadBarangData();
    bool openDatabase();
private slots:
    void on_actionAdd_triggered();

    void on_actionDelete_triggered();

    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_cellChanged(int row, int column);

    void on_actionAbout_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    bool CellEdited=false;
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
