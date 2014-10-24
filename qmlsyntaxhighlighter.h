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

#ifndef QMLSYNTAXHIGHLIGHTER_H
#define QMLSYNTAXHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>

class QMLSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit QMLSyntaxHighlighter(QObject *parent = 0);
    explicit QMLSyntaxHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private: // Methods
    void setupRules();

private: // Variables
    struct HighlightRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QList<HighlightRule> m_rules;
};

#endif // QMLSYNTAXHIGHLIGHTER_H
