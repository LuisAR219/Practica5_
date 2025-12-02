#pragma once
#include <QWidget>
#include <vector>
#include <QPointF>

class GraficaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraficaWidget(QWidget *parent = nullptr);

    void cargarArchivo(const QString &archivo);
    void setPaso(int p);
    int pasoMax() const { return m_pasoMax; }
    int pasoAct() const { return m_pasoAct; }

protected:
    void paintEvent(QPaintEvent *) override;

private:
    std::vector<std::vector<QPointF>> m_tray; // [part][paso]
    std::vector<QPointF> m_cols;              // colisiones
    int m_pasoMax = 0;
    int m_pasoAct = 0;
};
