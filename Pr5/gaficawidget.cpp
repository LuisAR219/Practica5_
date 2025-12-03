#include "gaficawidget.h"
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <map>
#include <QDebug>

GraficaWidget::GraficaWidget(QWidget *parent) : QWidget(parent) {}

void GraficaWidget::setObstaculos(const std::vector<Rectangulo>& obs)
{
    m_obstaculos = obs;
    update();
}

void GraficaWidget::cargarArchivo(const QString &archivo)
{
    // Reset data
    m_tray.clear();
    m_colsByStep.clear();
    m_obstaculos.clear();
    m_pasoMax = 0;
    m_pasoAct = 0;

    QFile f(archivo);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "No se pudo abrir" << archivo;
        return;
    }

    QTextStream in(&f);

    // frames[paso] = vector posiciones de partículas
    std::vector<std::vector<QPointF>> frames;
    std::map<int, std::vector<QPointF>> collisionsMap;

    int currentStep = -1;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        // PASO
        if (line.startsWith("Paso")) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);

            if (parts.size() >= 2) {
                bool ok = false;
                int p = parts[1].toInt(&ok);
                currentStep = ok ? p : (int)frames.size();
            } else {
                currentStep = (int)frames.size();
            }

            frames.emplace_back();
            continue;
        }

        // COLISIONES
        if (line.startsWith("COLISION")) {
            QStringList p = line.split(' ', Qt::SkipEmptyParts);

            if (p.size() >= 5) {
                bool okS=false, okX=false, okY=false;
                int step = p[2].toInt(&okS);
                double x = p[3].toDouble(&okX);
                double y = p[4].toDouble(&okY);

                if (okS && okX && okY)
                    collisionsMap[step].push_back(QPointF(x,y));
            }
            continue;
        }

        // POSICIONES "x y"
        QStringList xy = line.split(' ', Qt::SkipEmptyParts);
        if (xy.size() != 2) continue;

        bool okX=false, okY=false;
        double x = xy[0].toDouble(&okX);
        double y = xy[1].toDouble(&okY);
        if (!okX || !okY) continue;

        if (frames.empty()) {
            frames.emplace_back();
            currentStep = 0;
        }

        frames.back().push_back(QPointF(x,y));
    }

    f.close();

    // No hay datos
    if (frames.empty()) {
        update();
        return;
    }

    // Construir trayectorias
    size_t nPart = frames[0].size();
    for (auto &fr : frames)
        nPart = std::min(nPart, fr.size());

    m_tray.assign(nPart, std::vector<QPointF>());

    for (auto &fr : frames)
        for (size_t i = 0; i < nPart; ++i)
            m_tray[i].push_back(fr[i]);


    m_colsByStep.assign(frames.size(), {});
    for (auto &kv : collisionsMap)
        if (kv.first >= 0 && kv.first < (int)m_colsByStep.size())
            m_colsByStep[kv.first] = kv.second;

    m_pasoMax = frames.size() - 1;
    m_pasoAct = 0;

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

    // Mapa lógico
    const double worldW = 800.0;
    const double worldH = 600.0;

    double scale = std::min(width() / worldW, height() / worldH);
    double offsetX = (width()  - worldW * scale) / 2.0;
    double offsetY = (height() - worldH * scale) / 2.0;

    auto toScreen = [&](QPointF p) {
        return QPointF(p.x() * scale + offsetX,
                       p.y() * scale + offsetY);
    };


    painter.setPen(QPen(Qt::green, 2));
    painter.setBrush(Qt::NoBrush);

    for (const auto &r : m_obstaculos) {
        QRectF rect(
            r.x * scale + offsetX,
            r.y * scale + offsetY,
            r.ancho * scale,
            r.alto * scale
            );
        painter.drawRect(rect);
    }


    QColor colores[3] = { Qt::yellow, Qt::cyan, Qt::magenta };

    for (int i = 0; i < (int)m_tray.size(); ++i) {
        QPen pen = (i < 3 ? QPen(colores[i]) : QPen(Qt::white));
        painter.setPen(pen);

        for (int k = 1; k <= m_pasoAct && k < (int)m_tray[i].size(); ++k)
            painter.drawLine(toScreen(m_tray[i][k-1]),
                             toScreen(m_tray[i][k]));

        if (m_pasoAct < (int)m_tray[i].size()) {
            painter.setBrush(pen.color());
            painter.drawEllipse(toScreen(m_tray[i][m_pasoAct]), 4, 4);
        }
    }

    painter.setPen(QPen(Qt::red));
    painter.setBrush(Qt::red);

    if (m_pasoAct >= 0 && m_pasoAct < (int)m_colsByStep.size()) {
        for (auto &c : m_colsByStep[m_pasoAct])
            painter.drawEllipse(toScreen(c), 5, 5);
    }
}



