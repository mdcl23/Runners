#include "hexutils.h"

QVector<QPoint> neighbours4(QPoint pt)
{
    QVector<QPoint> ns;
    for (int i = -1; i < 2; i += 2)
    {
        ns.append(pt + QPoint(i,0));
        ns.append(pt + QPoint(0,i));
    }
    return ns;
}
