
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    int getKey(QString);
    bool isValid(QString, QString);
    QStringList getBlocks(QString, int);
    QString toCeasar(QString, int);
    QStringList toCeasarShift(QStringList, QString, int);

    void encrypt();
    void unencrypt();

    void calculateFrequancy(QString, QString);
    void fillTableData(QStringList, QStringList, QString);

    void on_btnEncrypt_clicked();
    void on_btnUnencrypt_clicked();

private:
    Ui::MainWindow *ui;

    QMap<QChar, int> unencryptedMap;
    QMap<QChar, int> encryptedMap;

    const QString ALPHABET = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ ";
};

#endif // MAINWINDOW_H
