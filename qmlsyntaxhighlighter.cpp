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

#include "qmlsyntaxhighlighter.h"

QMLSyntaxHighlighter::QMLSyntaxHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
    setupRules();
}

QMLSyntaxHighlighter::QMLSyntaxHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    setupRules();
}

void QMLSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightRule &rule, m_rules) {
        QRegularExpressionMatch match = rule.pattern.match(text);
        while (match.hasMatch()) {
            int index = match.capturedStart();
            int length = match.capturedLength();
            setFormat(index, length, rule.format);
            match = rule.pattern.match(text, index + length);
        }
    }

    // Multiline comments
    /// TODO: Unify into HighlightRule
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor("#008000"));

    QRegularExpression startComment("/\\*");
    QRegularExpression endComment("\\*/");

    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = startComment.match(text).capturedStart();
    while (startIndex >= 0) {
        QRegularExpressionMatch m = endComment.match(text, startIndex);
        int endIndex = m.capturedStart();
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + m.capturedLength();
        }
        setFormat(startIndex, commentLength, commentFormat);
        startIndex = startComment.match(text, startIndex + commentLength).capturedStart();
    }
}

void QMLSyntaxHighlighter::setupRules()
{
    if (!m_rules.isEmpty())
        return;

    QTextCharFormat format;

    // Keyword
    //format.setForeground(QColor("#808000"));

    // Type
    format.setForeground(QColor("#800080"));
    m_rules << HighlightRule{
        QRegularExpression("\\b[A-Z][A-Za-z]+\\b"),
        format
    };

    // Number
    format.setForeground(QColor("#000080"));
    m_rules << HighlightRule{
        QRegularExpression("[0-9]+\\.?[0-9]*"),
        format
    };

    // String
    format.setForeground(QColor("#008000"));
    m_rules << HighlightRule{
        QRegularExpression("['\"].*['\"]"),
        format
    };

    // Single-line comment
    format.setForeground(QColor("#008000"));
    m_rules << HighlightRule{
        QRegularExpression("//[^\n]*"),
        format
    };
}
