#ifndef SETTINGMANAGER_H
#define SETTINGMANAGER_H

#include <QVariant>
#include <QSettings>
#include <QStringList>

class SettingManager
{
public:
    // 便捷函数
    static void registerWidgetList(QObject *p, const QList<QWidget *> &wl);
    static bool saveWidgetConfig(QObject *p);
    static bool loadWidgetConfig(QObject *p);

    // 分步操作函数
    static SettingManager &instance(); // 外部访问对象的唯一方法
    static bool fileExit();
    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    bool contains(const QString &key) const;

private:
    explicit SettingManager(); // 禁止外部构造
     ~SettingManager() {} // 禁止外部delete
    SettingManager& operator =(const SettingManager &); // 禁用重载操作符=
    SettingManager(const SettingManager &); // 禁止拷贝构造

    const QString iniFileName;
    QSettings config;
    QStringList propertyFilter;
    QMap<QString, QList<QWidget *> > mw;
};

#endif // SETTINGMANAGER_H
