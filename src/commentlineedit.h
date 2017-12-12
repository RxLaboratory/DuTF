#ifndef COMMENTLINEEDIT_H
#define COMMENTLINEEDIT_H

#include <QLineEdit>

/**
 * @brief A Line edit that react to what you type in
 */
class CommentLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CommentLineEdit(QWidget *parent = nullptr);

signals:

public slots:

private slots:

    /**
     * @brief Update the line edit background color depending on the text prefix
     */
    void updateColor();
};

#endif // COMMENTLINEEDIT_H
