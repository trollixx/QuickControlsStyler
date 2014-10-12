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
