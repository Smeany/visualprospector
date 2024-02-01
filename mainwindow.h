#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QFileDialog>
#include <QTableWidget>
#include <QDataStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct VeinObject {
    int m_chunkX;
    int m_chunkZ;
    short veinType;
};

struct WorldObject {
    QString name;
    QVector<VeinObject> veins;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonAddFiles_clicked();

    void on_tabWidget_tabCloseRequested(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    bool filesAdded = false;
    Ui::MainWindow *ui;
    QMap<int,QString> veinMap;
    QFileDialog fd;

    QString pathHint;
    QSet<QString> worldNames;
    QVector<WorldObject> worlds;
    WorldObject mergedWorld;

    void processAddedFiles(const QStringList fileNames);
    QVector<WorldObject> parseToWorldObjects(QStringList fileNames);
    QTableWidget *addWorldToTable(const WorldObject &world);
};

const QString veintypesLUT = "{"
    "\"ore.mix.uranium\": 32,"
    "\"ore.mix.tungstate\": 22,"
    "\"ore.mix.richnuclear\": 65,"
    "\"ore.mix.sulfur\": 11,"
    "\"ore.mix.ross128ba.Tungstate\": 85,"
    "\"ore.mix.diamond\": 26,"
    "\"ore.mix.lapis\": 30,"
    "\"ore.mix.molybdenum\": 21,"
    "\"ore.mix.ross128.Forsterit\": 81,"
    "\"ore.mix.coppertin\": 38,"
    "\"ore.mix.oilsand\": 33,"
    "\"ore.mix.europa\": 67,"
    "\"ore.mix.olivine\": 27,"
    "\"ore.mix.osmium\": 47,"
    "\"ore.mix.tungstenirons\": 59,"
    "\"ore.mix.ross128.Roquesit\": 78,"
    "\"ore.mix.terraaer\": 36,"
    "\"ore.mix.coal\": 4,"
    "\"ore.mix.manganese\": 24,"
    "\"ore.mix.desh\": 49,"
    "\"ore.mix.ross128ba.tib\": 84,"
    "\"ore.mix.quartz\": 25,"
    "\"ore.mix.secondlanthanid\": 69,"
    "\"ore.mix.pitchblende\": 19,"
    "\"ore.mix.quartzspace\": 70,"
    "\"ore.mix.draconium\": 50,"
    "\"ore.mix.ross128ba.bart\": 86,"
    "\"ore.mix.redstone\": 15,"
    "\"ore.mix.sapphire\": 23,"
    "\"ore.mix.luvtantalite\": 73,"
    "\"ore.mix.ross128.CopperSulfits\": 80,"
    "\"ore.mix.mica\": 43,"
    "\"ore.mix.tfgalena\": 72,"
    "\"ore.mix.infusedgold\": 57,"
    "\"ore.mix.ross128.Thorianit\": 74,"
    "\"ore.mix.monazite\": 20,"
    "\"ore.mix.ross128.carbon\": 75,"
    "\"ore.mix.dolomite\": 44,"
    "\"ore.mix.blackplutonium\": 56,"
    "\"ore.mix.ross128ba.Amethyst\": 88,"
    "\"ore.mix.rareearth\": 64,"
    "\"ore.mix.nickel\": 17,"
    "\"ore.mix.titaniumchrome\": 39,"
    "\"ore.mix.platinumchrome\": 45,"
    "\"ore.mix.callistoice\": 52,"
    "\"ore.mix.uraniumgtnh\": 60,"
    "\"ore.mix.heavypentele\": 66,"
    "\"ore.mix.copper\": 12,"
    "\"ore.mix.ross128ba.CopperSulfits\": 89,"
    "\"ore.mix.vanadiumgold\": 61,"
    "\"ore.mix.bauxite\": 13,"
    "\"ore.mix.soapstone\": 16,"
    "\"ore.mix.neutronium\": 34,"
    "\"ore.mix.mytryl\": 53,"
    "\"ore.mix.ross128.Hedenbergit\": 82,"
    "\"ore.mix.perditioordo\": 37,"
    "\"ore.mix.gold\": 6,"
    "\"ore.mix.ross128ba.Fluorspar\": 91,"
    "\"ore.mix.kaolinitezeolite\": 42,"
    "\"ore.mix.ross128.RedZircon\": 83,"
    "\"ore.mix.netherstar\": 62,"
    "\"ore.mix.iron\": 7,"
    "\"ore.mix.ross128ba.TurmalinAlkali\": 87,"
    "\"ore.mix.beryllium\": 31,"
    "\"ore.mix.garnet\": 63,"
    "\"ore.mix.saltpeterelectrotine\": 48,"
    "\"ore.mix.lignite\": 3,"
    "\"ore.mix.platinum\": 18,"
    "\"ore.mix.tetrahedrite\": 9,"
    "\"ore.mix.galena\": 29,"
    "\"ore.mix.ross128.Tungstate\": 79,"
    "\"ore.mix.none\": 1,"
    "\"ore.mix.naquadah\": 2,"
    "\"ore.mix.oriharukon\": 55,"
    "\"ore.mix.niobium\": 58,"
    "\"ore.mix.apatite\": 28,"
    "\"ore.mix.rutile\": 71,"
    "\"ore.mix.ross128ba.RedZircon\": 90,"
    "\"ore.mix.mineralsand\": 40,"
    "\"ore.mix.quantium\": 51,"
    "\"ore.mix.ross128.TurmalinAlkali\": 77,"
    "\"ore.mix.europacore\": 68,"
    "\"ore.mix.ledox\": 54,"
    "\"ore.mix.ross128.bismuth\": 76,"
    "\"ore.mix.magnetite\": 5,"
    "\"ore.mix.garnettin\": 41,"
    "\"ore.mix.iridiummytryl\": 46,"
    "\"ore.mix.salts\": 14,"
    "\"ore.mix.cassiterite\": 8,"
    "\"ore.mix.aquaignis\": 35,"
    "\"ore.mix.netherquartz\": 10"
    "}";

#endif // MAINWINDOW_H
