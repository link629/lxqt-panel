/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
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


#ifndef PLUGINMOVEPROCESSOR_H
#define PLUGINMOVEPROCESSOR_H

#include <QWidget>
#include <QVariantAnimation>
#include <QEvent>
#include "plugin.h"
#include "lxqtpanelglobals.h"

class LxQtPanelLayout;
class QLayoutItem;


class LXQT_PANEL_API PluginMoveProcessor : public QWidget
{
    Q_OBJECT
public:
    explicit PluginMoveProcessor(LxQtPanelLayout *layout, Plugin *plugin);
    ~PluginMoveProcessor();

    Plugin *plugin() const { return mPlugin; }

signals:
    void finished();

public slots:
    void start();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void doStart();
    void doFinish();

private:
    enum MarkType
    {
        TopMark,
        BottomMark,
        LeftMark,
        RightMark
    };

    struct MousePosInfo
    {
        int index;
        QLayoutItem *item;
        bool after;
    };

    LxQtPanelLayout *mLayout;
    Plugin *mPlugin;
    int mDestIndex;

    MousePosInfo itemByMousePos(const QPoint mouse) const;
    void drawMark(QLayoutItem *item, MarkType markType);
};


class LXQT_PANEL_API CursorAnimation: public QVariantAnimation
{
    Q_OBJECT
public:
    void updateCurrentValue(const QVariant &value) { QCursor::setPos(value.toPoint()); }
};

#endif // PLUGINMOVEPROCESSOR_H
