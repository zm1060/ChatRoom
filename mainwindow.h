#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:;
    void slotConnected();
    void slotDisconnected();
    void dataReceived();


private slots:
    void on_connect_button_clicked();

    void on_send_button_clicked();

    void on_clear_button_clicked();

    void on_textBrowser_sourceChanged(const QUrl &arg1);

    void on_enter_button_clicked();

    void on_connect_button_pressed();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
