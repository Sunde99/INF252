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
    explicit Node(QWidget *parent = nullptr, QVector2D pos={0,0}, QColor color={1,1,1});
    void setPos(QVector2D newPos) {m_pos = newPos;};
    void setColor(QColor newColor) {m_color = newColor; repaint();};
    void moveNode(QVector2D moveVec){setPos(m_pos + moveVec);};
    QColor getColor() {return m_color;};

    QVector4D getInfo();
    QVector2D getPos() {return m_pos;};

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual QSize sizeHint() const;
signals:
    void nodeSelected(Node* = nullptr);
public slots:
    void updateValues();
private:
    QVector2D m_pos;
    QColor m_color;
    bool m_isSelected = false;
};

#endif // NODE_H
