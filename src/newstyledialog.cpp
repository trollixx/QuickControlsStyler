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

#include "newstyledialog.h"
#include "ui_newstyledialog.h"

#include <QFileDialog>

NewStyleDialog::NewStyleDialog(const QStringList &baseStyles, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewStyleDialog),
    m_location(QDir::homePath())
{
    ui->setupUi(this);
    ui->locationEdit->setText(m_location);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    foreach (const QString &style, baseStyles)
        ui->baseStyleComboBox->addItem(style);

    connect(ui->browseButton, &QPushButton::clicked, this, &NewStyleDialog::browse);
    connect(ui->nameEdit, &QLineEdit::textChanged, this, &NewStyleDialog::checkLocation);
    connect(ui->locationEdit, &QLineEdit::textChanged, this, &NewStyleDialog::checkLocation);
}

NewStyleDialog::~NewStyleDialog()
{
    delete ui;
}

int NewStyleDialog::baseStyleIndex() const
{
    return ui->baseStyleComboBox->currentIndex();
}

QString NewStyleDialog::name() const
{
    return ui->nameEdit->text();
}

QString NewStyleDialog::location() const
{
    return m_location;
}

void NewStyleDialog::browse()
{
    m_location = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), m_location,
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);
    ui->locationEdit->setText(m_location);
}

void NewStyleDialog::checkLocation()
{
    const QString location = ui->locationEdit->text();
    const QString name = ui->nameEdit->text();

    if (location.isEmpty() || name.isEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->warningLabel->clear();
        return;
    }

    bool ok = true;
    QString warning;
    QFileInfo fi(location);
    if (!fi.exists()) {
        warning = tr("Target directory does not exist");
        ok = false;
    } else if (!fi.isWritable()) {
        warning = tr("Target directory is read only");
        ok = false;
    } else {
        fi.setFile(location + QStringLiteral("/") + name);
        if (fi.exists())
            warning = tr("Target directory already exists. Contents will be overwritten.");
    }

    m_location = location;
    ui->warningLabel->setText(warning);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ok);
}
