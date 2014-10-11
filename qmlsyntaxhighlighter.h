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
