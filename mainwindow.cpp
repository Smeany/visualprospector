#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QtEndian>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>
#include <QDesktopServices>
#include <QFileDialog>

QDataStream &operator<<(QDataStream &out, const VeinObject& v) {
   out << v.m_chunkX << v.m_chunkZ << v.veinType;
   return out;
}

QDataStream &operator>>(QDataStream &in, VeinObject &rhs) {
   in.readRawData(reinterpret_cast<char*>(&rhs), sizeof(rhs));
   return in;
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QTableWidget *tableWidget = new QTableWidget(this);

    tableWidget->setRowCount(3);
    tableWidget->setColumnCount(6);

    tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("X Chunk"));
    tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Z Chunk"));
    tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("X Coord"));
    tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("Z Coord"));
    tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("Vein"));
    tableWidget->setHorizontalHeaderItem(5,new QTableWidgetItem("is Depleted"));

    tableWidget->setColumnWidth(0,60);
    tableWidget->setColumnWidth(1,60);
    tableWidget->setColumnWidth(2,55);
    tableWidget->setColumnWidth(3,55);
    tableWidget->setColumnWidth(4,100);
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    for(int row = 0; row < 3; row++) {
        QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(QString::number(row)));
            tableWidget->setItem(row, 0, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(QString::number(row+1)));
            tableWidget->setItem(row, 1, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(QString::number(row*16)));
            tableWidget->setItem(row, 2, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(QString::number((row+1)*16)));
            tableWidget->setItem(row, 3, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg("apatite"));
            tableWidget->setItem(row, 4, newItem);
        newItem = new QTableWidgetItem("");
            tableWidget->setItem(row, 4, newItem);
    }

    ui->tabWidget->addTab(tableWidget,"Example");

    QJsonDocument doc = QJsonDocument::fromJson(veintypesLUT.toUtf8());
    QJsonObject obj = doc.object();
    foreach(const QString& key, obj.keys()) {
        QJsonValue value = obj.value(key);
        veinMap.insert(value.toInt(),key);
        //qDebug() << "Key = " << key << ", Value = " << value.toInt();
    }

    fd.setFileMode(QFileDialog::ExistingFiles);
    fd.setViewMode(QFileDialog::Detail);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButtonAddFiles_clicked() {
    QStringList fileNames;
    if (fd.exec())
        fileNames = fd.selectedFiles();
    if(!filesAdded && !fileNames.isEmpty()) {
        QWidget *example = ui->tabWidget->widget(0);
        ui->tabWidget->clear();
        delete example;
        filesAdded = true;
    }
    if(fileNames.size())
        processAddedFiles(fileNames);
}

QVector<WorldObject> MainWindow::parseToWorldObjects(const QStringList fileNames) {
    QVector<WorldObject> parsedWorlds;
    for(const QString &fn : fileNames) {
        QFile file(fn);

        if (!file.open(QIODevice::ReadOnly)) {
            qCritical() << "Failed to open file";
            continue;
        }

        // Read the binary data into a buffer
        QByteArray buffer = file.readAll();
        file.close();

        // Parse the binary data into DataObjects
        WorldObject world;
        QFileInfo fi(fn);
        const QString bn = fi.baseName();
        QString worldName = bn;
        if(worldNames.contains(bn)) {
            int i = 2;
            do {
                worldName = bn + "(" + QString::number(i) + ")";
            } while(worldNames.contains(worldName));
        }
        world.name = worldName;

        for (int i = 0; i < buffer.size(); i += (sizeof(VeinObject)-2)) {
            if (i + sizeof(VeinObject) - 2 > buffer.size()) {
                // Not enough data for another object
                break;
            }

            // Extract the values from the binary data, assuming big-endian byte order
            int firstInt = qFromBigEndian<qint32>(*reinterpret_cast<const qint32*>(buffer.data() + i));
            int secondInt = qFromBigEndian<qint32>(*reinterpret_cast<const qint32*>(buffer.data() + i + sizeof(qint32)));
            short shortValue = qFromBigEndian<qint16>(*reinterpret_cast<const qint16*>(buffer.data() + i + 2*sizeof(qint32)));
            qDebug() << "shortValue" << shortValue << veinMap[shortValue] << (shortValue | 0x8000);

            // Create a DataObject and add it to the list
            world.veins.append({firstInt, secondInt, shortValue});
        }


        if(world.veins.isEmpty())
            continue;

        parsedWorlds.append(world);
        worldNames.insert(worldName);
    }
    return parsedWorlds;
}

void MainWindow::processAddedFiles(const QStringList fileNames) {
    QFileInfo fi(fileNames.first());
    pathHint = fi.absoluteDir().absolutePath();

    QVector<WorldObject> newWorlds = parseToWorldObjects(fileNames);
    worlds.append(newWorlds);
    //add them to tabs
    for(const WorldObject &w : newWorlds) {
        QTableWidget *tableWidget = addWorldToTable(w);
        ui->tabWidget->addTab(tableWidget,w.name);
    }
}

QTableWidget *MainWindow::addWorldToTable(const WorldObject &world) {
    QTableWidget *tableWidget = new QTableWidget(this);

    tableWidget->setRowCount(static_cast<int>(world.veins.size()));
    tableWidget->setColumnCount(6);

    tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("X Chunk"));
    tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Z Chunk"));
    tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("X Coord"));
    tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("Z Coord"));
    tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("Vein"));
    tableWidget->setHorizontalHeaderItem(5,new QTableWidgetItem("is Depleted"));

    tableWidget->setColumnWidth(0,60);
    tableWidget->setColumnWidth(1,60);
    tableWidget->setColumnWidth(2,55);
    tableWidget->setColumnWidth(3,55);
    tableWidget->setColumnWidth(4,100);
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    int row = 0;
    for(const VeinObject &v : world.veins) {
        QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(QString::number(v.m_chunkX)));
            tableWidget->setItem(row, 0, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(QString::number(v.m_chunkZ)));
            tableWidget->setItem(row, 1, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(QString::number(v.m_chunkX*16)));
            tableWidget->setItem(row, 2, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(QString::number(v.m_chunkZ*16)));
            tableWidget->setItem(row, 3, newItem);
        newItem = new QTableWidgetItem(tr("%1").arg(veinMap[(v.veinType < 0) ? v.veinType & 0x7FFF : v.veinType].mid(8)));
            tableWidget->setItem(row, 4, newItem);
        newItem = new QTableWidgetItem((v.veinType < 0) ? "Depleted" : "");
            tableWidget->setItem(row, 5, newItem);
        row++;
    }

    return tableWidget;
}

void MainWindow::on_tabWidget_tabCloseRequested(int index) {
    QString worldName = ui->tabWidget->tabText(index);
    if(worldNames.contains(worldName))
        worldNames.remove(worldName);
    ui->tabWidget->removeTab(index);

    for(int i = static_cast<int>(worlds.size())-1; i >= 0; i--) {
        if(worlds[i].name != worldName) continue;
        worlds.remove(i);
        break;
    }
}

void MainWindow::on_pushButton_2_clicked() {
    mergedWorld.veins.clear();

    //bool is isDepleted state, true if depleted vein..
    QMap<QPair<int,int>, bool> set;
    for(const WorldObject &w : worlds) {
        for(const VeinObject &v : w.veins) {
            QPair<int,int> coords({v.m_chunkX,v.m_chunkZ});
            if(set.contains(coords)) {
                if( v.veinType < 0 && !set[coords]) {
                    set[coords] = true;
                    for(VeinObject &v_merged : mergedWorld.veins) {
                        if(v.m_chunkX == v_merged.m_chunkX &&
                                v.m_chunkZ == v_merged.m_chunkZ) {
                            v_merged.veinType = v.veinType;
                            break;
                        }
                    }
                }
                continue;
            }
            set.insert(coords,(v.veinType < 0) ? true : false);
            mergedWorld.veins.append(v);
        }
    }

    QTableWidget *tableWidget = addWorldToTable(mergedWorld);
    QWidget *old = ui->centralwidget->layout()->itemAt(3)->widget();
    ui->centralwidget->layout()->replaceWidget(old,tableWidget);
    delete old;
}

void MainWindow::on_pushButton_3_clicked() {
    QUrl fileUrl = QFileDialog::getSaveFileUrl(this, tr("Save As"));
    if(fileUrl.isEmpty()) return;

    QFile file(fileUrl.toLocalFile());
    if(!file.open(QIODevice::WriteOnly))
        qCritical() << "Couldn't save file";
    QDataStream s(&file);
    s.setByteOrder(QDataStream::BigEndian);
    for(const VeinObject &v : mergedWorld.veins)
        s << v;
    QFileInfo fi(file);
    QDesktopServices::openUrl(QUrl::fromLocalFile(fi.absoluteDir().absolutePath()));
}
