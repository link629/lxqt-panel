/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012-2013 Razor team
 * Authors:
 *   Kuzma Shapran <kuzma.shapran@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#ifndef LXQTPANELWORLDCLOCK_H
#define LXQTPANELWORLDCLOCK_H

#include <unicode/unistr.h>

#include <QLabel>

#include <LXQt/RotatedWidget>

#include "../panel/ilxqtpanelplugin.h"
#include "lxqtworldclockconfiguration.h"


class ActiveLabel;
class QTimer;
class QDialog;
class LxQtWorldClockEventFilter;
namespace U_ICU_NAMESPACE {
    class Locale;
    class Calendar;
    class DateFormat;
}

class LxQtWorldClock : public QObject, public ILxQtPanelPlugin
{
    Q_OBJECT
public:
    LxQtWorldClock(const ILxQtPanelPluginStartupInfo &startupInfo);
    ~LxQtWorldClock();

    virtual QWidget *widget() { return mMainWidget; }
    virtual QString themeId() const { return "WorldClock"; }
    virtual ILxQtPanelPlugin::Flags flags() const { return PreferRightAlignment | HaveConfigDialog ; }
    bool isSeparate() const { return true; }
    void activated(ActivationReason reason);

    virtual void settingsChanged();
    virtual void realign();
    QDialog *configureDialog();

private slots:
    void synchroTimeout();
    void timeout();
    void wheelScrolled(int);

private:
    static size_t instanceCounter;

    QWidget *mMainWidget;
    LxQt::RotatedWidget* mRotatedWidget;
    ActiveLabel *mContent;
    QDialog* mPopup;

    typedef enum FormatType
    {
        FORMAT__INVALID = -1,
        FORMAT_SHORT = 0,
        FORMAT_MEDIUM,
        FORMAT_LONG,
        FORMAT_FULL,
        FORMAT_CUSTOM
    } FormatType;

    QTimer *mTimer;

    QStringList mTimeZones;
    QString mDefaultTimeZone;
    QString mActiveTimeZone;
    QString mCustomFormat;
    FormatType mFormatType;

    QString mDefaultLanguage;
    bool mAutoRotate;

    icu::Locale *mLocale;
    icu::Calendar *mCalendar;
    icu::DateFormat *mFormat;
    icu::UnicodeString mLastShownText;

    void updateFormat();
    void restartTimer(int);
    void updateTimezone();
};


class ActiveLabel : public QLabel
{
Q_OBJECT

public:
    explicit ActiveLabel(QWidget * = NULL);

signals:
    void wheelScrolled(int);
    void leftMouseButtonClicked();
    void middleMouseButtonClicked();

protected:
    void wheelEvent(QWheelEvent *);
    void mouseReleaseEvent(QMouseEvent* event);
};

class LxQtWorldClockLibrary: public QObject, public ILxQtPanelPluginLibrary
{
    Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_PLUGIN_METADATA(IID "lxde-qt.org/Panel/PluginInterface/3.0")
#endif
    Q_INTERFACES(ILxQtPanelPluginLibrary)
public:
    ILxQtPanelPlugin *instance(const ILxQtPanelPluginStartupInfo &startupInfo)
    {
        return new LxQtWorldClock(startupInfo);
    }
};


#endif // LXQTPANELWORLDCLOCK_H
