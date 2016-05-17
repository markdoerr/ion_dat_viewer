

#ifndef WELL_VIEW_H
#define WELL_VIEW_H

#include <QFrame>
#include <QGraphicsView>

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QToolButton)

class WellView;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(WellView *v) : QGraphicsView(), view(v) { }

protected:
    void wheelEvent(QWheelEvent *);

private:
    WellView *view;
};

class WellView : public QFrame
{
    Q_OBJECT
public:
    WellView(const QString &name, QWidget *parent = 0);

    QGraphicsView *view() const;

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePointerMode();
    void toggleOpenGL();
    void toggleAntialiasing();
    void print();

private:
    GraphicsView *graphicsView;
    QLabel *label;
    QLabel *label2;
    QToolButton *selectModeButton;
    QToolButton *dragModeButton;
    QToolButton *openGlButton;
    QToolButton *antialiasButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QSlider *zoomSlider;
};

#endif
