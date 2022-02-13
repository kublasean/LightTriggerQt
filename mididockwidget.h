#ifndef MIDIDOCKWIDGET_H
#define MIDIDOCKWIDGET_H

#include <QDockWidget>
#include <QItemSelectionModel>

#include "midinotemodel.h"
#include "detectedmidinotemodel.h"

namespace Ui {
class MidiDockWidget;
}

class MidiDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit MidiDockWidget(MidiNoteModel *noteModel, QWidget *parent = nullptr);
    ~MidiDockWidget();

    QItemSelectionModel *selectionModel() const;
    DetectedMidiNoteModel *notesProxyModel();

public slots:
    void showNote(int note, int velocity);

private:
    Ui::MidiDockWidget *ui;
    DetectedMidiNoteModel detectedNotesModel;
};

#endif // MIDIDOCKWIDGET_H
