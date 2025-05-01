#ifndef STATCARD_H
#define STATCARD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>

using namespace QtCharts;

    class StatCard : public QWidget {
    Q_OBJECT
public:
    enum class CardType {
        Total,
        Gender,
        Chronic,
        Region
    };

    explicit StatCard(CardType type, QWidget *parent = nullptr);
    void updateData(const QVariantMap &data);

private:
    void setupUI();
    void setupTotalCard();
    void setupDonutChartCard();
    void setupValueDisplay();

    CardType m_type;
    QVBoxLayout *m_layout;
    QLabel *m_titleLabel;
    QLabel *m_valueLabel;
    QChartView *m_chartView;
    QMap<QString, QColor> m_colorMap;
};

#endif // STATCARD_H
