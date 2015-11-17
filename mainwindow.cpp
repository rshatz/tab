#include <QDebug>

#include <QToolBar>
#include <QDockWidget>
#include <QHBoxLayout>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    noteLabel = new QLabel *[6];

    for(int i = 0; i < 6; i++)
    {
        noteLabel[i] = new QLabel[13];
        for(int u = 0; u < 13; u++)
        {
            noteLabel[i][u].setText("");
        }
    }
    //tabSheet = new QPlainTextEdit;
    //setCentralWidget(tabSheet);
    createToolBar();
    //createDockWindow();

    fretBoardLabel = new QLabel(this);
    fretBoardLabel->setPixmap(QPixmap(":/FretBoard.jpg"));

    //fretBoardLabel->setStyleSheet("QLabel{margin: 0 0 0 50px;}");
    notesDisplayLabel = new QLabel(this);

    topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(50, 50, 0, 50);
    topLayout->addWidget(fretBoardLabel);
    topLayout->addWidget(notesDisplayLabel);

    QWidget *mainLayout = new QWidget;
    mainLayout->setLayout(topLayout);

    setCentralWidget(mainLayout);
}

const QString MainWindow::notes[] = {"A", "A#", "B", "C", "C#", "D",
                                     "D#", "E", "F", "F#", "G", "G#"};

MainWindow::~MainWindow()
{

}

void MainWindow::createToolBar()
{
    selectionToolBar = addToolBar("Selection Tool Bar");

    keyComboBox = new QComboBox;
    keyComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    keyLabel = new QLabel("   &Key ");
    keyLabel->setBuddy(keyComboBox);

    for(int i = 0; i < 11; i++)
    {
        keyComboBox->addItem(notes[i]);
    }

    scaleComboBox = new QComboBox;
    scaleComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    scaleLabel = new QLabel("   S&cale ");
    scaleLabel->setBuddy(scaleComboBox);

    scaleComboBox->addItem("major");
    scaleComboBox->addItem("major pentatonic");
    scaleComboBox->addItem("natural minor");
    scaleComboBox->addItem("minor pentatonic");
    scaleComboBox->addItem("harmonic minor");
    scaleComboBox->addItem("melodic minor");
    scaleComboBox->insertSeparator(6);
    scaleComboBox->addItem("blues");
    scaleComboBox->addItem("whole tone");
    scaleComboBox->addItem("whole-half diminished ");
    scaleComboBox->addItem("half-whole diminished ");
    scaleComboBox->insertSeparator(11);
    scaleComboBox->addItem("All Notes");


    tuningComboBox = new QComboBox;
    tuningComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    tuningLabel = new QLabel("   &Tuning ");
    tuningLabel->setBuddy(tuningComboBox);

    tuningComboBox->addItem("Standard");
    tuningComboBox->addItem("Drop D");
    //tuningComboBox->insertSeparator(2);
    tuningComboBox->addItem("Open A");
    tuningComboBox->addItem("Open C");
    tuningComboBox->addItem("Open D");
    tuningComboBox->addItem("Open D Minor");
    tuningComboBox->addItem("Open G");

    submitButton = new QPushButton("Submit");

    selectionToolBar->addWidget(keyLabel);
    selectionToolBar->addWidget(keyComboBox);
    selectionToolBar->addSeparator();
    selectionToolBar->addWidget(scaleLabel);
    selectionToolBar->addWidget(scaleComboBox);
    selectionToolBar->addSeparator();
    selectionToolBar->addWidget(tuningLabel);
    selectionToolBar->addWidget(tuningComboBox);
    selectionToolBar->addSeparator();
    selectionToolBar->addWidget(submitButton);

    connect(submitButton, SIGNAL(clicked(bool)), this, SLOT(submitPressed()));
}

//void MainWindow::createDockWindow()
//{
//    fretBoardLabel = new QLabel;
//    fretBoardLabel->setPixmap(QPixmap(":/FretBoard.jpg"));
//    int h = fretBoardLabel->height();
//    int w = this->width();

//    qDebug() << h;
//    qDebug() << w;
//    fretBoardDock = new QDockWidget("fretbord");
//    fretBoardDock->setObjectName("fretBoardDock");
//    fretBoardDock->setWidget(fretBoardLabel);
//    //fretBoardDock->setFixedHeight(fretBoardLabel->minimumHeight());
//    //fretBoardDock->setFixedWidth(fretBoardLabel->minimumWidth());
//    fretBoardDock->setAllowedAreas(Qt::TopDockWidgetArea |
//                                   Qt::BottomDockWidgetArea);
//    addDockWidget(Qt::TopDockWidgetArea, fretBoardDock);

//    //noteLabel = new QLabel(fretBoardDock);
//    //noteLabel->setPixmap(QPixmap(":/noteDot.png"));
//    //noteLabel->setGeometry(190, 200, w, h);
//}

void MainWindow::submitPressed()
{
    for(int i = 0; i < 6; ++i) {
        delete [] noteLabel[i];
    }
    delete [] noteLabel;
    scaleDegrees.clear();
    changeScale();
    setTuning();
    drawScale();

    QString noteList;
    for(int i = 0; i < scaleDegrees.size(); i++)
    {
        noteList += notes[scaleDegrees[i]];
        noteList += " ";
    }
    notesDisplayLabel->setText(noteList);
}

void MainWindow::changeScale()
{
    switch(scaleComboBox->currentIndex())
    {
    case MAJOR:
        major();
        break;
    case MAJOR_PENTATONIC:
        majorPentatonic();
        break;
    case NATURAL_MINOR:
        naturalMinor();
        break;
    case MINOR_PENTATONIC:
        minorPentatonic();
        break;
    case HARMONIC_MINOR:
        harmonicMinor();
        break;
    case MELODIC_MINOR:
        melodicMinor();
        break;
    case BLUES:
        blues();
        break;
    case WHOLE_TONE:
        wholeTone();
        break;
    case WHOLE_HALF_DIM:
        wholeHalfDim();
        break;
    case HALF_WHOLE_DIM:
        halfWholeDim();
        break;
    case PHRYGIAN:
        phrygian();
        break;
    case LYDIAN:
        lydian();
        break;
    case MIXOLYDIAN:
        mixolydian();
        break;
    case AEOLIAN:
        aeolian();
        break;
    case LOCRIAN:
        locraian();
        break;
    }
}

void MainWindow::setTuning()
{
    switch(tuningComboBox->currentIndex())
    {
    case STANDARD:
        tuningPositions = new int[6]{7, 2, 10, 5, 0, 7};
        break;
    case DROP_D:
        tuningPositions = new int[6]{7, 2, 10, 5, 0, 5};
        break;
    case OPEN_A:
        tuningPositions = new int[6]{7, 0, 7, 4, 0, 7};
        break;
    case OPEN_C:
        tuningPositions = new int[6]{7, 3, 10, 3, 10, 3 };
    }
}

void MainWindow::major()
{
    const int formula[] = {2, 2, 1, 2, 2, 2, 1};
    const int SIZE = 7;
    buildScale(formula, SIZE);
}

void MainWindow::majorPentatonic()
{
    const int formula[] = {2, 2, 3, 2, 3};
    const int SIZE = 5;
    buildScale(formula, SIZE);
}

void MainWindow::naturalMinor()
{
    const int formula[] = {2, 1, 2, 2, 1, 2, 2};
    const int SIZE = 7;
    buildScale(formula, SIZE);
}

void MainWindow::minorPentatonic()
{
    const int formula[] = {3, 2, 2, 3, 2};
    const int SIZE = 5;
    buildScale(formula, SIZE);
}

void MainWindow::harmonicMinor()
{
    const int formula[] = {2, 1, 2, 2, 1, 3, 2};
    const int SIZE = 7;
    buildScale(formula, SIZE);
}

void MainWindow::melodicMinor()
{
    const int formula[] = {2, 1, 2, 2, 2, 2, 1};
    const int SIZE = 7;
    buildScale(formula, SIZE);
}

void MainWindow::blues()
{
    const int formula[] = {3, 2, 1, 1, 3, 2};
    const int SIZE = 6;
    buildScale(formula, SIZE);
}

void MainWindow::wholeTone()
{
    const int formula[] = {2, 2, 2, 2, 2, 2};
    const int SIZE = 6;
    buildScale(formula, SIZE);
}

void MainWindow::wholeHalfDim()
{
    const int formula[] = {2, 1, 2, 1, 2, 1, 2, 1};
    const int SIZE = 8;
    buildScale(formula, SIZE);
}

void MainWindow::halfWholeDim()
{
    const int formula[] = {1, 2, 1, 2, 1, 2, 1, 2};
    const int SIZE = 8;
    buildScale(formula, SIZE);
}

void MainWindow::phrygian()
{

}

void MainWindow::lydian()
{

}

void MainWindow::mixolydian()
{

}

void MainWindow::aeolian()
{

}

void MainWindow::locraian()
{

}

void MainWindow::buildScale(const int scaleFormula[], const int size)
{
    int position = keyComboBox->currentIndex();
    for(int i = 0; i < size; i++)
    {
        scaleDegrees.append(position % 12);
        position += scaleFormula[i];
    }
}

void MainWindow::drawScale()
{
    int x = 30;//65
    int y = 75;//0

    noteLabel = new QLabel *[6];

    for(int i = 0; i < 6; i++)
    {
        noteLabel[i] = new QLabel[13];
        for(int u = 0; u < 13; u++)
        {
            noteLabel[i][u].setText("");
        }
    }

    QVectorIterator<int> i(scaleDegrees);
    for(int string = 0; string < 6; string++)
    {
        int notePos = tuningPositions[string];
        //noteLabel[string] = new QLabel[13];

        for(int fret = 0; fret < 13; fret++)
        {
            if(notePos >= 12)
            {
                notePos = 0;
            }

            noteLabel[string][fret].setParent(this/*fretBoardLabel*/);
            noteLabel[string][fret].setText(notes[notePos]);
            noteLabel[string][fret].setGeometry(x, y, 32, 32);

            while(i.hasNext())
            {
                if(noteLabel[string][fret].text() == notes[i.next()])
                {
                    noteLabel[string][fret].show();
                }
            }
            i.toFront();
            x += 95;//increment pixel length to next fret
            notePos++;
        }
        x = 30;
        y += 40;//increment pixel length to next string
    }
}
