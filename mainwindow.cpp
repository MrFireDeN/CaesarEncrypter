#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Иницилизация таблицы
    QStringList columnLabels = {
        "Буква\nотрытого\nтекста",
        "Позиция\nбуквы\nв алфавите",
        "Буква\nв кодово\nслове",
        "Величина\nсдвига",
        "Новая\nпозиция\nбуквы",
        "Буква\nшифртекста"
    };
    ui->tableWidget->setHorizontalHeaderLabels(columnLabels);
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

// Создание блоков
QStringList MainWindow::getBlocks(QString text, int blockLength) {
    QStringList blockList;

    // Заполнение блоков
    for (int i = 0; i < text.length(); i += blockLength) {
        blockList.append(text.mid(i, blockLength));
    }

    // Заполнение последнего блока пробелами
    while (blockList.last().length() < blockLength) {
        blockList.last().append(' ');
    }

    return blockList;
}

bool MainWindow::isValid(QString text, QString key) {
    // Проверка на пустоту
    if (text.isEmpty() or key.isEmpty())
        return false;

    // Проверка текста
    for (QChar letter : text) {
        if (!ALPHABET.contains(letter))
            return false;
    }

    // Проверка ключа
    for (QChar letter : key) {
        if (!ALPHABET.contains(letter))
            return false;
    }

    return true;
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

// Шрифт цезаря
QStringList MainWindow::toCeasarShift(QStringList blocks, QString key, int k = 1) {
    QStringList encryptedBlocks;
    QString encryptedText;

    for (QString block : blocks) {
        encryptedText.clear();
        for (int i = 0; i < block.length(); ++i) {
            int textIndex = ALPHABET.indexOf(block[i].toUpper());
            int keyIndex = ALPHABET.indexOf(key[i]);

            int newIndex = textIndex + keyIndex * k;

            while (newIndex < 0){
                newIndex += ALPHABET.length();
            }

            encryptedText += ALPHABET[newIndex % ALPHABET.length()];
        }

        encryptedBlocks.append(encryptedText);
    }

    return encryptedBlocks;
}

void MainWindow::encrypt() {
    // Дополнительные переменные
    QString key = ui->keysToCrypt->text();
    QString text = ui->textUnencrypted->text();

    qDebug() << 1;
    // Проверка на заполненность
    if (!isValid(text, key))
        return;
    qDebug() << 2;

    // Создание блоков
    QStringList blocks = getBlocks(text, key.length());

    // Применение шифрования Цезаря
    QStringList encryptedBlocks = toCeasarShift(blocks, key);

    QString result;
    for (QString block : encryptedBlocks) {
        result += block;
    }

    calculateFrequancy(text, result);
    fillTableData(blocks, encryptedBlocks, key);

    // Вывод
    ui->messageEncrypted->setText(result);
}

void MainWindow::unencrypt() {
    // Дополнительные переменные
    QString key = ui->keysToUnencrypt->text();
    QString text = ui->textEncrypted->text();

    qDebug() << 1;
    // Проверка на заполненность
    if (!isValid(text, key))
        return;
    qDebug() << 2;

    // Создание блоков
    QStringList blocks = getBlocks(text, key.length());

    // Применение шифрования Цезаря
    QStringList unencryptedBlocks = toCeasarShift(blocks, key, -1);

    QString result;
    for (QString block : unencryptedBlocks) {
        result += block;
    }

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

void MainWindow::fillTableData(QStringList blocks, QStringList encryptedBlocks, QString key) {
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(blocks.length() * key.length());

    for (int i = 0; i < blocks.length(); ++i) {
        for (int j = 0; j < key.length(); ++j) {
            ui->tableWidget->setItem(i*key.length()+j, 0, new QTableWidgetItem(blocks[i][j]));
            ui->tableWidget->setItem(i*key.length()+j, 1, new QTableWidgetItem(QString::number(ALPHABET.indexOf(blocks[i][j]))));
            ui->tableWidget->setItem(i*key.length()+j, 2, new QTableWidgetItem(key[j]));
            ui->tableWidget->setItem(i*key.length()+j, 3, new QTableWidgetItem(QString::number(ALPHABET.indexOf(key[j]))));
            ui->tableWidget->setItem(i*key.length()+j, 4, new QTableWidgetItem(QString::number(ALPHABET.indexOf(encryptedBlocks[i][j]))));
            ui->tableWidget->setItem(i*key.length()+j, 5, new QTableWidgetItem(encryptedBlocks[i][j]));
        }
    }
}

void MainWindow::on_btnEncrypt_clicked()
{
    encrypt();
}


void MainWindow::on_btnUnencrypt_clicked()
{
    unencrypt();
}

