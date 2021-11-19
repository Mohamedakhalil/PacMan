#ifndef GHOST_H
#define GHOST_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QGraphicsScene>
#include "Vector2.h"
enum class Direction
{
    Right, Left, Up, Down, None
};

class Ghost :public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Ghost(int initialRow, int initialColumn, int** data, Vector2 size, Vector2 offset, const QString& ghostName);
    void SetRow(int newRow);
    void SetColumn(int newColumn);
    int GetRow();
    int GetColumn();
    void GotEaten();
    void MakeEdiable(bool value);
    void RetunToHouse();
    void CanMove(bool value);

public slots:
    void GhostAdvance();
    void StartMovement();

private:
    //Attributes needed for the movement and changing position
    int m_row, m_column, m_startRow, m_startColumn;
    Vector2 m_size, m_offset, m_direction;

    Direction lastDir = Direction::None;
    int** m_boardData;
    int ticks = 0;
    //Timers
    QTimer* m_moveTimer;
    QTimer* m_deathTimer;

    //Used pixmaps
    QPixmap m_originalGhostPixmap;
    QPixmap m_eadibleGhostPixmap;

    //States
    bool m_eatable;
    bool m_canMove;
};


#endif // GHOST_H
