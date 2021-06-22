#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_selectSourceButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);

    QDir dir_path = QFileInfo(dir).filePath();
    QString dir_path_string = dir_path.path();

    on_sourceDirLabel_linkActivated(dir_path.path());
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_actionExit_triggered()
{
   QApplication::exit();
}

void MainWindow::on_sourceDirLabel_linkActivated(const QString &link)
{
    ui->sourceDirLabel->setText("Source dir: " + link);
}

void MainWindow::on_selectDestinationButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open directory bitch"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    QDir dir_path = QFileInfo(dir).filePath();
    on_destinationDirLabel_linkActivated(dir_path.path());
}

void MainWindow::on_destinationDirLabel_linkActivated(const QString &link)
{
    ui->destinationDirLabel->setText("Destination dir: " + link);
}

void MainWindow::on_actionNew_Operation_triggered()
{
    ui->radioButtonName->toggle();
    ui->radioButtonDate->toggle();
    ui->radioButtonType->toggle();

    ui->inputTextEdit->clear();
    ui->sourceDirLabel->setText("Source dir: ");
    ui->destinationDirLabel->setText("Destination dir: ");

    ui->exampleInputLabel->setText("Example: ");
    ui->output->setText("Program output: ");
}


void MainWindow::on_ProcessDataButton_clicked()
{
    QString source = ui->sourceDirLabel->text();
    source = source.split(": ").last(); //Get the source directory name
    if(source.isEmpty()){
        ui->output->setText("Program output: Failure");
        return;
    }

    QString destination = ui->destinationDirLabel->text();
    destination = destination.split(": ").last(); //Get the destination directory name
    if(destination.isEmpty()){
        ui->output->setText("Program output: Failure");
        return;
    }

    //Get's the file selection argument(date, extension or name)
    QString criteria = get_Criteria(get_SelectedButton());

    //Get the directory
    QDir dir_path = QFileInfo(source).filePath();
    //Get the files in the source directory
    QStringList files = dir_path.entryList(QDir::Files);

    //temp variables to get info
    QString full_file_path;
    QString new_file_path;

    if(criteria=="extension"){
        QString extension = ui->inputTextEdit->toPlainText();

        foreach(QString file, files){
            if(extension==QFileInfo(file).suffix()){ //Compare file's suffix with the input
                full_file_path = QFileInfo(file).filePath(); //Get the file in the source directory
                new_file_path = destination+QDir::separator()+full_file_path;

                qDebug() << full_file_path << " " << new_file_path;
                dir_path.rename(full_file_path, new_file_path);
            }
        }

        ui->output->setText("Program output: Success");

    }else if(criteria=="name"){
        QString name_to_check = ui->inputTextEdit->toPlainText();
        foreach(QString file, files){
            if(QFileInfo(file).fileName().contains(name_to_check)){
                full_file_path = QFileInfo(file).filePath(); //Get the file in the source directory
                new_file_path = destination+QDir::separator()+full_file_path;
                dir_path.rename(QFileInfo(file).filePath(), new_file_path);
            }
        }
        ui->output->setText("Program output: Success");


    }else if(criteria=="date"){
        QString date_to_check = ui->inputTextEdit->toPlainText();
        QStringList date_string_list = date_to_check.split('/');

        QDate dt (date_string_list.at(2).toInt(),date_string_list.at(1).toInt(), date_string_list.at(0).toInt());
        qDebug() << dt;
        //TO DO
    }

    return;
}

QString MainWindow::get_Criteria(const QString &selected_button ){
    QString input = ui->inputTextEdit->toPlainText();

    QMessageBox warning;
    try {
        if(!validate_input(input, selected_button)){
            throw 0;
        }
    } catch (...) {
        warning.critical(this, "Error","An error occurred! Please check the input!");
    }

    return selected_button;//TO DO
}

bool MainWindow::validate_input(const QString &input, const QString &type){
    if(type == "date"){

        QStringList date_string_list = input.split('/');
        std::int8_t day = date_string_list.at(0).toInt();
        std::int8_t month = date_string_list.at(1).toInt();

        //Initialize a map with the total number of days in each month
        QMap<int8_t, int8_t> months_with_max_num_days;
        months_with_max_num_days[1] = 31;
        months_with_max_num_days[2] = 29; //29 for leap years
        months_with_max_num_days[3] = 31;
        months_with_max_num_days[4] = 30;
        months_with_max_num_days[5] = 31;
        months_with_max_num_days[6] = 30;
        months_with_max_num_days[7] = 31;
        months_with_max_num_days[8] = 31;
        months_with_max_num_days[9] = 30;
        months_with_max_num_days[10] = 31;
        months_with_max_num_days[11] = 30;
        months_with_max_num_days[12] = 31;

        bool flag = false;
        //Check for month input
        flag = (month >= 1 && month <= 12) ? true : false;
        if(!flag){
            return false;
        }
        //Check for day input
        flag = (day >= 1 && day <= months_with_max_num_days[month]) ? true:false;
        if(!flag){
            return false;
        }
        //Check for year input
        flag = (date_string_list.at(2).length()==4) ? true:false;
        if(!flag){
            return false;
        }

        return true;

    }else if(type=="name" || type=="extension"){

        QList<QChar> forbidden_symbols {'\n', '$', '#', '%', '&', '{', '}', '\\', '<', '>', '*', '?', '/', '!', '\'', '"', ':', '@', '+', '`', '|', '=', ' '};

        for(int counter = 0; counter < input.length(); counter++){
            if(forbidden_symbols.contains(input.at(counter))){
                return false;
            }
        }
        //Return true as the filename or the extension to search for does not contain any forbidden symbols
        return true;
    }

    return false;
}

QString MainWindow::get_SelectedButton(){
    if(ui->radioButtonDate->isChecked()){
        return "date";
    }else if(ui->radioButtonName->isChecked()){
        return "name";
    }else if(ui->radioButtonType->isChecked()){
        return "extension";
    }

    return "none";
}


void MainWindow::on_radioButtonType_released()
{
    if(ui->radioButtonType->isChecked()){
        ui->exampleInputLabel->setText("Example: txt / rar / zip");
    }
}

void MainWindow::on_radioButtonDate_released()
{
    if(ui->radioButtonDate->isChecked()){
        ui->exampleInputLabel->setText("Example: dd/mm/yyyy");
    }
}

void MainWindow::on_radioButtonName_released()
{
    if(ui->radioButtonName->isChecked()){
        ui->exampleInputLabel->setText("Example: Name should not contain any forbidden symbols");
    }
}
