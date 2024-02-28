#include "mainwindow.h"
#include "ui_mainwindow.h"


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

// Создание QList с ключами
int MainWindow::getKey(QString keyText) {
    if (keyText.isEmpty())
        return 0;

    for (QChar letter : keyText) {
        if(!letter.isDigit()) {
            return 0;
        }
    }

    return keyText.toInt();
}

// Шрифт цезаря
QString MainWindow::toCeasar(QString text, int key) {
    QString encryptedText;

    for (QChar letter : text) {
        int index = ALPHABET.indexOf(letter.toUpper());
        if (index != -1) {
            int newKey = (index + key);

            while (newKey < 0){
                newKey += ALPHABET.length();
            }

            encryptedText += ALPHABET[newKey % ALPHABET.length()];
        } else {
            return "";
        }
    }

    return encryptedText;
}

void MainWindow::encrypt() {
    // Дополнительные переменные
    QString keyText = ui->keysToCrypt->text();
    QString text = ui->textUnencrypted->text();

    // Проверка на заполненность
    if (text == "" or keyText == "")
        return;

    // Создание int с ключами
    int key = getKey(keyText);

    // Применение шифрования Цезаря
    QString result = toCeasar(text.toUpper(), key);

    calculateFrequancy(text, result);

    // Вывод
    ui->messageEncrypted->setText(result);
}

void MainWindow::unencrypt() {
    // Дополнительные переменные
    QString keyText = ui->keysToUnencrypt->text();
    QString text = ui->textEncrypted->text();

    // Проверка на заполненность
    if (text == "" or keyText == "")
        return;

    // Создание int с ключами
    int key = getKey(keyText);

    // Применение шифрования Цезаря
    QString result = toCeasar(text.toUpper(), -key);

    calculateFrequancy(result, text);

    // Вывод
    ui->messageUnencrypted->setText(result);
}

void MainWindow::calculateFrequancy(QString unencrytedText, QString encrytedText) {
    unencryptedMap.clear();
    for (QChar letter: unencrytedText) {
        if (!unencryptedMap.contains(letter)) {
            unencryptedMap.insert(letter, 1);
        }
        else{
            unencryptedMap[letter]++;
        }
    }

    QString result1  = "";
    for (auto i = unencryptedMap.begin(); i != unencryptedMap.end(); ++i) {
        result1 += QString("\'") + i.key() + "\': " + QString::number(i.value()) + " ";
    }
    ui->frequencyUnencrypted->setText(result1);

    encryptedMap.clear();
    for (QChar letter: encrytedText) {
        if (!encryptedMap.contains(letter)) {
            encryptedMap.insert(letter, 1);
        }
        else{
            encryptedMap[letter]++;
        }
    }

    QString result2 = "";
    for (auto i = encryptedMap.begin(); i != encryptedMap.end(); ++i) {
        result2 += QString("\'") + i.key() + "\': " + QString::number(i.value()) + " ";
    }
    ui->frequencyEncrypted->setText(result2);
}

void MainWindow::on_btnEncrypt_clicked()
{
    encrypt();
}


void MainWindow::on_btnUnencrypt_clicked()
{
    unencrypt();
}

