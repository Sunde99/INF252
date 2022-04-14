#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QColor>
#include <QVector2D>
#include <QMouseEvent>

class Node : public QWidget
{
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr);
    void setPos(QVector2D newPos);
    void moveNode(QVector2D moveVec){setPos(m_pos + moveVec);};
    void setColor(QColor newColor);

    QVector4D getInfo();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
signals:
public slots:
    void updateValues();
private:
    QVector2D m_pos;
    QColor m_color;
    bool m_isMoving = false;
};

#endif // NODE_H
