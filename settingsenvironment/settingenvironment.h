#ifndef SETTINGENVIRONMENT_H
#define SETTINGENVIRONMENT_H

#include <QString>
#include <QMap>
#include <QStringList>

struct setting {
    QString value;
    QStringList attrbs;
};

class SettingEnvironment : private QObject
{
    Q_OBJECT
public:
    explicit SettingEnvironment(QObject *parent = 0);
    ~SettingEnvironment();

    Q_INVOKABLE setting GrabSetting();
    Q_INVOKABLE virtual void LoadFileToEnv(QString file,);

private:
    QMap<QString,setting> settingsMap;

}Settings;

#endif // SETTINGENVIRONMENT_H
