#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QStringList>
#include <QHeaderView>
#include <QGridLayout>
#include <QFormLayout>
#include <QSplitter>
#include <QInputDialog>
#include <QDebug>
#include <QGroupBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("GUI for Lab-1");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    sequenceListWidget = new QListWidget(this);
    sequenceTableView = new QTableView(this);
    sequenceTableModel = new QStandardItemModel(this);
    sequenceTableView->setModel(sequenceTableModel);
    sequenceNameLineEdit = new QLineEdit(this);
    elementLineEdit = new QLineEdit(this);
    createButton = new QPushButton("Create Sequence", this);
    appendButton = new QPushButton("Append", this);
    prependButton = new QPushButton("Prepend", this);
    insertButton = new QPushButton("Insert", this);
    deleteButton = new QPushButton("Delete Sequence", this);
    removeButton = new QPushButton("Remove", this);

    contextMenu = new QMenu(this);
    renameAction = new QAction("Rename", this);
    contextMenu->addAction(renameAction);


    QGroupBox *sequenceGroupBox = new QGroupBox("Sequence Operations", this);
    QVBoxLayout *sequenceLayout = new QVBoxLayout(sequenceGroupBox);
    sequenceLayout->addWidget(createButton);
    sequenceLayout->addWidget(deleteButton);


    QGroupBox *elementGroupBox = new QGroupBox("Element Operations", this);
    QVBoxLayout *elementLayout = new QVBoxLayout(elementGroupBox);
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel("Element:", this));
    elementLineEdit->setPlaceholderText("Enter int, double or string");

    inputLayout->addWidget(elementLineEdit);


    elementLayout->addLayout(inputLayout);
    elementLayout->addWidget(appendButton);
    elementLayout->addWidget(prependButton);
    elementLayout->addWidget(insertButton);
    elementLayout->addWidget(removeButton);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(sequenceListWidget);
    splitter->addWidget(sequenceTableView);

    sequenceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    QGridLayout *mainLayout = new QGridLayout(centralWidget);
    QFormLayout *sequenceFormLayout = new QFormLayout();
    sequenceFormLayout->addRow("Sequence Name:", sequenceNameLineEdit);


    mainLayout->addLayout(sequenceFormLayout, 0, 0, 1, 2);
    mainLayout->addWidget(sequenceGroupBox, 1, 0, 1, 2);
    mainLayout->addWidget(elementGroupBox, 2, 0, 1, 2);
    mainLayout->addWidget(new QLabel("Sequences:"), 3, 0);
    mainLayout->addWidget(splitter, 4, 0, 1, 2);



    connect(createButton, &QPushButton::clicked, this, &MainWindow::createSequence);
    connect(appendButton, &QPushButton::clicked, this, &MainWindow::appendElement);
    connect(prependButton, &QPushButton::clicked, this, &MainWindow::prependElement);
    connect(insertButton, &QPushButton::clicked, this, &MainWindow::insertElement);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteSequence);
    connect(sequenceListWidget, &QListWidget::itemClicked, this, &MainWindow::sequenceSelected);
    connect(sequenceListWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(renameAction, &QAction::triggered, this, &MainWindow::renameSequence);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeElement);


    connect(sequenceListWidget, &QListWidget::currentItemChanged, this, [this](QListWidgetItem *current, QListWidgetItem *previous) {
        if (previous) {
            previous->setBackground(Qt::NoBrush);
        }
        if (current) {
            current->setBackground(QColor(Qt::lightGray));
        }
    });

    if (sequenceListWidget->count() > 0) {
        sequenceListWidget->setCurrentRow(0);
        if (QListWidgetItem *firstItem = sequenceListWidget->item(0)) {
            currentSequenceName = firstItem->text();
        }
    }
    updateButtonsState();
}

void MainWindow::createSequence() {
    QString sequenceName = sequenceNameLineEdit->text();
    if (sequenceName.isEmpty()) {
        displayErrorMessage("Sequence name cannot be empty.");
        return;
    }

    if (sequences.contains(sequenceName)) {
        displayErrorMessage("Sequence with this name already exists.");
        return;
    }

    sequences[sequenceName] = new MutableListSequence<QVariant>();
    currentSequenceName = sequenceName;
    updateSequenceList();
    updateButtonsState();

    sequenceTableModel->clear();
    sequenceTableModel->setHorizontalHeaderLabels({"Element"});

    sequenceNameLineEdit->clear();
}

void MainWindow::deleteSequence() {
    if (currentSequenceName.isEmpty()) {
        displayErrorMessage("No sequence selected.");
        return;
    }

    delete sequences[currentSequenceName];
    sequences.remove(currentSequenceName);
    currentSequenceName = "";
    updateSequenceList();
    updateButtonsState();

    sequenceTableModel->clear();
}

void MainWindow::appendElement() {
    if (!currentSequenceName.isEmpty()) {
        QString elementText = elementLineEdit->text();

        bool ok;
        int intValue = elementText.toInt(&ok);
        if (ok) {
             sequences[currentSequenceName]->append(intValue);
        } else {
             double doubleValue = elementText.toDouble(&ok);
             if (ok) {
                sequences[currentSequenceName]->append(doubleValue);
             } else {
                sequences[currentSequenceName]->append(elementText);
             }
         }
        elementLineEdit->clear();
        displaySequence();
    }
}

void MainWindow::prependElement() {
    if (!currentSequenceName.isEmpty()) {
        QString elementText = elementLineEdit->text();

        bool ok;
        int intValue = elementText.toInt(&ok);
        if (ok) {
            sequences[currentSequenceName]->prepend(intValue);
        } else {
            double doubleValue = elementText.toDouble(&ok);
            if (ok) {
               sequences[currentSequenceName]->prepend(doubleValue);
            } else {
               sequences[currentSequenceName]->prepend(elementText);
            }
        }
       elementLineEdit->clear();
       displaySequence();
    }
}

void MainWindow::insertElement() {
    if (!currentSequenceName.isEmpty()) {
        bool ok;
        int index = QInputDialog::getInt(this, "Insert Element", "Index:", 0, 0, sequences[currentSequenceName]->getLength(), 1, &ok);
        if (ok) {
            QString elementText = elementLineEdit->text();

            bool ok;
            int intValue = elementText.toInt(&ok);
            if (ok) {
                sequences[currentSequenceName]->insertAt(index, intValue);
            } else {
                double doubleValue = elementText.toDouble(&ok);
                if (ok) {
                   sequences[currentSequenceName]->insertAt(index, doubleValue);
                } else {
                   sequences[currentSequenceName]->insertAt(index, elementText);
                }
            }

            elementLineEdit->clear();
            displaySequence();
        }
    }
}

void MainWindow::removeElement() {
    if (currentSequenceName.isEmpty()) {
        displayErrorMessage("No sequence selected.");
        return;
    }

    bool ok;
    int index = QInputDialog::getInt(this, "Remove Element", "Index:", 0, 0, sequences[currentSequenceName]->getLength() - 1, 1, &ok);

    if (ok) {
        try {
            sequences[currentSequenceName]->removeAt(index);
            displaySequence(); // Обновляем таблицу
        } catch (const std::out_of_range& e) {
            displayErrorMessage("Error: " + QString::fromStdString(e.what()));
        }
    }
}

void MainWindow::displaySequence() {
    if (!currentSequenceName.isEmpty()) {
        sequenceTableModel->clear();
        sequenceTableModel->setHorizontalHeaderLabels({"Element"});

        for (int i = 0; i < sequences[currentSequenceName]->getLength(); ++i) {
            QStandardItem *item = new QStandardItem(sequences[currentSequenceName]->get(i).toString());
            sequenceTableModel->setItem(i, 0, item);
        }
    } else {
        displayErrorMessage("No sequence selected");
    }
}

void MainWindow::displayErrorMessage(const QString &message) {
    QMessageBox::critical(this, "Error", message);
}

void MainWindow::updateSequenceList() {
    sequenceListWidget->clear();
    for (const QString &key : sequences.keys()) {
        sequenceListWidget->addItem(key);
    }
}

void MainWindow::sequenceSelected(QListWidgetItem *item) {
    if (item) {
        currentSequenceName = item->text();
        updateButtonsState();
        displaySequence();
    }
}


void MainWindow::showContextMenu(const QPoint &pos) {
    QPoint globalPos = sequenceListWidget->mapToGlobal(pos);
    contextMenu->exec(globalPos);
}

void MainWindow::renameSequence() {
    QListWidgetItem *currentItem = sequenceListWidget->currentItem();
    if (!currentItem) {
        return;
    }

    QString oldName = currentItem->text();
    bool ok;
    QString newName = QInputDialog::getText(this, "Rename Sequence", "New name:", QLineEdit::Normal, oldName, &ok);

    if (ok && !newName.isEmpty()) {
        if (sequences.contains(newName)) {
            displayErrorMessage("Sequence with this name already exists");
            return;
        }

        MutableListSequence<QVariant> *sequence = sequences[oldName];
        sequences.remove(oldName);
        sequences[newName] = sequence;
        currentItem->setText(newName);

        if (currentSequenceName == oldName) {
            currentSequenceName = newName;
        }
    }
}


void MainWindow::updateButtonsState() {
    bool sequenceSelected = !currentSequenceName.isEmpty();
    appendButton->setEnabled(sequenceSelected);
    prependButton->setEnabled(sequenceSelected);
    insertButton->setEnabled(sequenceSelected);
    deleteButton->setEnabled(sequenceSelected);
}

MainWindow::~MainWindow() {
    for (MutableListSequence<QVariant> *sequence : sequences) {
        delete sequence;
    }
    delete sequenceTableModel;
    delete contextMenu;
    delete renameAction;
}