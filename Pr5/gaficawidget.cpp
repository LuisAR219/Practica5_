#include "gaficawidget.h"
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QStringList>

GraficaWidget::GraficaWidget(QWidget *parent) : QWidget(parent) {}

void GraficaWidget::cargarArchivo(const QString &archivo)
{
    m_tray.clear();
    m_cols.clear();
    m_pasoMax = 0;
    m_pasoAct = 0;

    QFile f(archivo);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&f);

    std::vector<QPointF> aux[3];          // 3 partículas
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.startsWith("Paso")) continue;

        if (line.startsWith("COLISION")) {
            QStringList p = line.split(' ');
            if (p.size() >= 5)
                m_cols.push_back({p[2].toDouble(), p[3].toDouble()}); // push_back
            continue;
        }

        QStringList xy = line.split(' ');
        if (xy.size() != 2) continue;
        double x = xy[0].toDouble();
        double y = xy[1].toDouble();

        // Reparte entre los 3 vectores auxiliares
        if (aux[0].size() == aux[1].size() && aux[0].size() == aux[2].size())
            aux[0].push_back({x, y});
        else if (aux[1].size() < aux[0].size())
            aux[1].push_back({x, y});
        else
            aux[2].push_back({x, y});
    }
    f.close();

    m_tray = {aux[0], aux[1], aux[2]};
    m_pasoMax = m_tray.empty() ? 0 : static_cast<int>(m_tray[0].size()) - 1;
    update();
}

void GraficaWidget::setPaso(int p)
{
    m_pasoAct = qBound(0, p, m_pasoMax);
    update();
}

void GraficaWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);


    // Proporción 800×600
    const double worldW = 800.0;
    const double worldH = 600.0;
    double scale = std::min(width() / worldW, height() / worldH);
    double offsetX = (width()  - worldW * scale) / 2.0;
    double offsetY = (height() - worldH * scale) / 2.0;

    auto toScreen = [&](const QPointF &l) -> QPointF {
        return QPointF(l.x() * scale + offsetX, l.y() * scale + offsetY);
    };

    // Trayectorias
    QColor colores[3] = {Qt::yellow, Qt::cyan, Qt::magenta};
    for (int i = 0; i < 3 && i < static_cast<int>(m_tray.size()); ++i) {
        QPen pen(colores[i]);
        painter.setPen(pen);
        for (int k = 1; k <= m_pasoAct && k < static_cast<int>(m_tray[i].size()); ++k)
            painter.drawLine(toScreen(m_tray[i][k - 1]),
                             toScreen(m_tray[i][k]));
    }

    // Colisiones (mismo sistema)
    painter.setPen(QPen(Qt::red, 0));
    painter.setBrush(Qt::red);
    for (const QPointF &c : m_cols)
        painter.drawEllipse(toScreen(c), 4, 4);
}
