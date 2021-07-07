#ifndef OTHER_WINDOW_H
#define OTHER_WINDOW_H

#include <QWidget>

namespace Ui {
class Other_window;
}

class Other_window : public QWidget
{
    Q_OBJECT

public:
    explicit Other_window(QWidget *parent = nullptr);
    ~Other_window();

signals:
    void mainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Other_window *ui;
};

#endif // OTHER_WINDOW_H
