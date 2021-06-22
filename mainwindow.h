#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDate>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QString get_Criteria(const QString &selected_button);
    QString get_SelectedButton();
    bool validate_input(const QString &input, const QString &type);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_selectSourceButton_clicked();

    void on_exitButton_clicked();

    void on_ProcessDataButton_clicked();

    void on_sourceDirLabel_linkActivated(const QString &link);

    void on_selectDestinationButton_clicked();

    void on_destinationDirLabel_linkActivated(const QString &link);

    void on_actionNew_Operation_triggered();

    void on_radioButtonType_released();

    void on_radioButtonDate_released();

    void on_radioButtonName_released();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
