/****************************************************************************
**
**  Copyright (C) 2014 Oleg Shparber <trollixx+github@gmail.com>
**
**  This file is part of QuickControlsStyler (QCStyler).
**  Web site: https://github.com/trollixx/QuickControlsStyler
**
**  QCStyler is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  QCStyler is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with QCStyler. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef NEWSTYLEDIALOG_H
#define NEWSTYLEDIALOG_H

#include <QDialog>

namespace Ui {
class NewStyleDialog;
}

class NewStyleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewStyleDialog(const QStringList &baseStyles = QStringList(), QWidget *parent = 0);
    ~NewStyleDialog();

    int baseStyleIndex() const;
    QString name() const;
    QString location() const;

private slots:
    void browse();
    void checkLocation();

private:
    Ui::NewStyleDialog *ui;

    QString m_location;
};

#endif // NEWSTYLEDIALOG_H
