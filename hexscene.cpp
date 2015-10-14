#include "hexscene.h"

#include "hexbrain.h"

#include <qDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

int HexScene::tile_width = 40;
int HexScene::tile_height = 40;

HexScene::HexScene(int size)
    : blackTurn(true)
    , game(size)
{
    this->setBackgroundBrush(QColor(25, 126, 254));

    for (int i = 0; i < game.size+1; i++)
    {
        this->addLine(tile_width*i, 0, tile_width*i, game.size*tile_height, QColor(0,0,0));
        this->addLine(0, tile_height*i, game.size*tile_width, tile_height*i, QColor(0,0,0));
    }
}

QPoint HexScene::screenToWorld(QPoint pt) const
{
    return QPoint(pt.x()/tile_width, pt.y()/tile_height);
}

QPoint HexScene::worldToScreen(QPoint pt) const
{
    return QPoint(pt.x()*tile_width, pt.y()*tile_width);
}

void HexScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QPoint worldPos = screenToWorld(event->scenePos().toPoint());
    QPoint tilePos = worldToScreen(worldPos);
    QRect tile = QRect(tilePos, QSize(tile_width, tile_height));

    if (event->buttons() | Qt::LeftButton)
    {
        if (blackTurn)
        {
            if (game.getPiece(worldPos) == HexGame::NonePiece)
            {
                game.setPiece(worldPos, HexGame::BlackPiece);
                pieces.append(this->addRect(tile, QColor(0,0,0), QColor(0,0,0)));
                blackTurn = false;

                worldPos = HexBrain::nextMove(game, HexGame::WhitePiece);
                if (worldPos != QPoint(-1, -1)) {
                    qDebug() << worldPos;
                    tilePos = worldToScreen(worldPos);
                    tile = QRect(tilePos, QSize(tile_width, tile_height));

                    game.setPiece(worldPos, HexGame::WhitePiece);
                    pieces.append(this->addRect(tile, QColor(0,0,0), QColor(255, 255, 255)));
                    blackTurn = true;
                } else {
                    qDebug() << "computer resigns!";
                    blackTurn = true;
                    emit playerWins();
                }
            }
        }
    }

    if (game.checkWin() == HexGame::BlackPiece)
    {
        emit playerWins();
    }
    else if (game.checkWin() == HexGame::WhitePiece)
    {
        emit computerWins();
    }
}

void HexScene::keyReleaseEvent(QKeyEvent* event)
{
    if (event->text() == "w") {
        emit playerWins();
    }
}
