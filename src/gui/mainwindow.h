#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QVariant>
#include <QMap>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QMenu>
#include <QListWidget>

#include "MutableListSequence.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
        void createSequence();
        void appendElement();
        void prependElement();
        void insertElement();
        void displaySequence();
        void deleteSequence();
        void sequenceSelected(QListWidgetItem*);
        void showContextMenu(const QPoint&);
        void renameSequence();
        void removeElement();


private:
    QListWidget *sequenceListWidget;
    QTableView *sequenceTableView;
    QStandardItemModel *sequenceTableModel;
    QLineEdit *sequenceNameLineEdit;
    QLineEdit *elementLineEdit;
    QPushButton *createButton;
    QPushButton *appendButton;
    QPushButton *prependButton;
    QPushButton *insertButton;
    QPushButton *deleteButton;
    QPushButton *removeButton;
    QMenu *contextMenu;
    QAction *renameAction;

    QMap<QString, MutableListSequence<QVariant>*> sequences;
    QString currentSequenceName;

    void displayErrorMessage(const QString& message);
    void updateSequenceList();
    void updateButtonsState();
};

#endif // MAINWINDOW_H