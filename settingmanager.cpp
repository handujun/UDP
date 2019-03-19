#include "settingmanager.h"
#include <QtGui>
#include <QWidget>
SettingManager &SettingManager::instance() {
    static SettingManager ins; //  该对象在全局区，全程由系统管理，无需关心其析构
    return ins;
}

SettingManager::SettingManager() :
    iniFileName("PlcMonitorConfig.ini"),
    config(iniFileName, QSettings::IniFormat)
{
    propertyFilter = QStringList() << "checked" << "currentIndex" << "value" << "text" << "html";
}

void SettingManager::setValue(const QString &key, const QVariant &value)
{
    config.setValue(key, value);
}

QVariant SettingManager::value(const QString &key, const QVariant &defaultValue) const
{
    return config.value(key, defaultValue);
}

bool SettingManager::contains(const QString &key) const
{
    return config.contains(key);
}

bool SettingManager::fileExit()
{
    return QFile::exists(instance().iniFileName);
}

/**
* 概述 : 为一个类名注册一个控件列表，以便加载和保存配置时可使用该类名对应的控件列表
* 参数 :
 *     p : 其指向对象的真实类名将被作为注册的类名
*     wl : 控件列表
* 返回 : 是否保存成功
*/
void SettingManager::registerWidgetList(QObject *p, const QList<QWidget *> &wl)
{
    instance().mw[p->metaObject()->className()] = wl;
}

/**
 * 概述 : 保存一个已注册类名所对应的控件列表中，所有控件的特定属性
 * 参数 :
 *     p : 所指对象的真实类名应该是已注册的
 * 返回 : 是否保存成功
 */
bool SettingManager::saveWidgetConfig(QObject *p)
{
    /* 打包步骤：
     * QMap<属性名, 属性> --> 转为QVariant，放到QVariantList
     * 将QVariantList转为QVariant，写入到ini中
     */
    SettingManager &config = instance();
    QString className = p->metaObject()->className();
    QList<QWidget *> wl = config.mw[className];
    if ( !config.mw.contains(className) || !wl.count() )
        return false;
    QVariant vResult; // 最后结果
    QVariantList vl; // 列表：一个控件对应一个QVariant
    foreach (QWidget *w, wl) {
        QVariantMap m; // 存放一个控件的属性和属性值
        foreach (QString p, config.propertyFilter) {
            QVariant v = w->property(p.toStdString().data());
            if ( v.isValid() )
                m[p] = v;
        }
        vl << QVariant::fromValue(m);
    }
    vResult = QVariant::fromValue(vl);
    config.setValue( QString("/%1/WidgetConfig").arg(className), vResult );
    return true;
}

/**
 * 概述 : 加载一个已注册类名所对应的控件列表中，所有控件的特定属性
 * 参数 :
 *     p : 所指对象的真实类名应该是已注册的
 * 返回 : 是否加载成功
 */
bool SettingManager::loadWidgetConfig(QObject *p)
{
    SettingManager &config = instance();
    QString className = p->metaObject()->className();
    QList<QWidget *> wl = config.mw[className];
    if ( !fileExit() || !config.mw.contains(className) )
        return false;
    QVariant vResult = config.value( QString("/%1/WidgetConfig").arg(p->metaObject()->className()) );
    QVariantList vl = vResult.toList();
    int count = vl.count();
    if ( count != wl.count() )
        return false;
    for (int i=0; i<count; ++i) {
        if ( !vl[i].canConvert<QVariantMap>() )
            return false;
        QVariantMap m = vl[i].value<QVariantMap>();
        foreach (QString k, m.keys())
            wl[i]->setProperty(k.toStdString().data(), m[k]);
    }
    return true;
}


