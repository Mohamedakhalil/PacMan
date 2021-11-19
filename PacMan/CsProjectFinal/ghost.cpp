#include "ghost.h"

Ghost::Ghost(int initialRow, int initialColumn, int** data, Vector2 size, Vector2 offset, const QString& ghostName)
{
    //Seeding the random Function
    qsrand(time(0));

    //I only need to equate the pointers, we don't really need to do anything else with it anyway
    //so I don't need to keep copying the data around and have different copies of it for each object
    m_boardData = data;

    //Initalizing the variables
    m_size = size;
    m_offset = offset;

    m_row = initialRow;
    m_column = initialColumn;
    m_startRow = m_row;
    m_startColumn = m_column;
    m_direction = Vector2(0, 0);

    //Loading Pixmaps
    m_originalGhostPixmap.load(ghostName);
    m_originalGhostPixmap = m_originalGhostPixmap.scaledToWidth(size.x);
    m_originalGhostPixmap = m_originalGhostPixmap.scaledToHeight(size.y);

    m_eadibleGhostPixmap.load("BlueGhost.png");
    m_eadibleGhostPixmap = m_eadibleGhostPixmap.scaledToWidth(size.x);
    m_eadibleGhostPixmap = m_eadibleGhostPixmap.scaledToHeight(size.y);

    setPixmap(m_originalGhostPixmap);

    //Setting the position according to the reversed map policy (Check the Map class)
    setPos(m_offset.x + m_size.x * m_column, m_offset.y + m_size.y * m_row);

    //initializing the timers
    m_moveTimer = new QTimer(this);
    m_deathTimer = new QTimer(this);

    //Connecting the timers to the slots
    connect(m_deathTimer, SIGNAL(timeout()),this, SLOT(StartMovement()));

    //We made a custom advance for the ghost only cause we wanted it to move SLOWER than the player.
    connect(m_moveTimer, SIGNAL(timeout()), this, SLOT(GhostAdvance()));
    m_moveTimer->start(200);


    //setting the states flags
    m_eatable = false;
    m_canMove = true;
}

void Ghost::GhostAdvance()
{
    if(m_canMove)
    {
        ticks++;
        if (m_direction.x == -1 && m_column - 1 < 0){
            //qDebug() << "Portaled Left";
            m_column = 21;
        }
        else if (m_direction.x == 1 && m_column + 1 > 22)
            m_column = 1;
        else if (m_direction.y == 1 && m_boardData[m_column][m_row + 1] != -1)
            m_row++;
        else if(m_direction.y == -1 && m_boardData[m_column][m_row - 1] != -1)
            m_row--;
        else if (m_direction.x == 1 && m_boardData[m_column + 1][m_row] != -1)
            m_column++;
        else if(m_direction.x == -1 && m_boardData[m_column - 1][m_row] != -1)
            m_column--;
        setPos(m_offset.x + m_size.x * m_column, m_offset.y + m_size.y * m_row);

        if(m_column > 0 && m_column < 22)
        {
            QVector<Direction> availableTiles;
            if(m_boardData[m_column][m_row + 1] != -1 && lastDir != Direction::Up)
            {
                //qDebug() << "Added Down";
                availableTiles.push_back(Direction::Down);
            }
            if(m_boardData[m_column][m_row - 1] != -1 && lastDir != Direction::Down)
            {
                //qDebug() << "Added Up";
                availableTiles.push_back(Direction::Up);
            }
            if(m_boardData[m_column + 1][m_row] != -1 && lastDir != Direction::Left)
            {
                //qDebug() << "Added Right";
                availableTiles.push_back(Direction::Right);
            }
            if(m_boardData[m_column - 1][m_row] != -1 && lastDir != Direction::Right)
            {
               // qDebug() << "Added Left";
                availableTiles.push_back(Direction::Left);
            }
            int nextDir;
            if(availableTiles.size() > 0)
            {
                 nextDir = qrand() % availableTiles.size();
            }
            else
            {
                nextDir = 0;
                if(lastDir == Direction::Left)
                    availableTiles.push_back(Direction::Right);
                else if(lastDir == Direction::Right)
                    availableTiles.push_back(Direction::Left);
                else if(lastDir == Direction::Up)
                    availableTiles.push_back(Direction::Down);
                else if(lastDir == Direction::Down)
                    availableTiles.push_back(Direction::Up);
            }

            if(availableTiles[nextDir] == Direction::Up)
            {
                m_direction = Vector2(0, -1);
            }
            else if(availableTiles[nextDir] == Direction::Down)
            {
                m_direction = Vector2(0, 1);
            }
            else if(availableTiles[nextDir] == Direction::Left)
            {
                m_direction = Vector2(-1, 0);
            }
            else if(availableTiles[nextDir] == Direction::Right)
            {
                m_direction = Vector2(1, 0);
            }

            lastDir = availableTiles[nextDir];
        }
    }
}



void Ghost::SetRow(int newRow)
{
    m_row = newRow;
    setPos(m_offset.x + m_size.x * m_column, m_offset.y + m_size.y * m_row);
}
void Ghost::SetColumn(int newColumn)
{
    m_column = newColumn;
    setPos(m_offset.x + m_size.x * m_column, m_offset.y + m_size.y * m_row);
}
int Ghost::GetRow()
{
    return m_row;
}
int Ghost::GetColumn()
{
    return m_column;
}

void Ghost::GotEaten()
{
    //If this function was called, the ghost will return to the house and start waiting 4.5 seconds
    //before he can get back to movement
    RetunToHouse();
    m_deathTimer->start(4500);
    m_canMove = false;
}

void Ghost::StartMovement()
{
    //This slot gets called after 4.5 seconds from calling Ghost::GotEaten()
    m_deathTimer->stop();
    m_canMove = true;
}

void Ghost::MakeEdiable(bool value)
{
    //Chceks if the ghost should be eadiable or not and setting the Pixmap to the correct one
    if (value)
    {
        m_eatable = true;
        setPixmap(m_eadibleGhostPixmap);
    }
    else
    {
        m_eatable = false;
        setPixmap(m_originalGhostPixmap);
    }
}

//Setting the values to the initial values for the row and the column
void Ghost::RetunToHouse(){
    m_row = m_startRow;
    m_column = m_startColumn;
    setPos(m_offset.x + m_size.x * m_column, m_offset.y + m_size.y * m_row);
}

void Ghost::CanMove(bool value)
{
    m_canMove = value;
}
