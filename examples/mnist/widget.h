#ifndef WIDGET_H
#define WIDGET_H

#include "network.h"
#include "network_cb.h"
#include <QMainWindow>
#include <QTimer>
#include <QMutex>
#include <vector>
#include <memory>
#include <atomic>

namespace Ui
{
    class Widget;
}


struct NNSample
{
    Eigen::MatrixXd input;
    Eigen::MatrixXd normalizedinput;
    Eigen::MatrixXd output;
    uint8_t lable;
};

class Widget : public QMainWindow, public NetworkOperationCallback
{
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = 0);
    ~Widget();


    // NetworkOperationCallback interface
public:
    void networkOperationProgress( const NetworkOperationId &opId, const NetworkOperationStatus &opStatus,
                                   const double &progress );
    void networkTestResults(const double& successRateEuclidean, const double& successRateMaxIdx ,
                            const std::vector<size_t>& failedSamplesIdx);

private:
    bool loadMNISTSample( const std::vector<std::vector<double>>& imgSet, const std::vector<uint8_t>& lableSet,
                          const size_t& idx, Eigen::MatrixXd& img, uint8_t& lable);
    void displayTestMNISTImage(const size_t &idx);
    void learn();
    void sameImage();
    void prepareSamples();
    Eigen::MatrixXd lableToOutputVector( const uint8_t& lable );

public slots:
    void doNNTesting();
    void doNNLearning();
    void updateUi();

signals:
    void readyForTesting();
    void readyForLearning();

private:
    Ui::Widget* ui;
    QTimer* m_uiUpdaterTimer;
    std::vector<NNSample> m_trainingSet;
    std::vector<NNSample> m_testingSet;
    std::vector<Eigen::MatrixXd> m_batchin;
    std::vector<Eigen::MatrixXd> m_batchout;
    std::vector<Eigen::MatrixXd> m_testin;
    std::vector<Eigen::MatrixXd> m_testout;
    size_t m_currentIdx;
    std::shared_ptr<Network> m_net;

    // thread safe ui values
    std::atomic<double> m_sr_L2, m_sr_MAX;
    std::atomic<double> m_progress;
    QMutex m_listMutex;
    std::vector<std::size_t> m_failedSamples;

};

#endif // WIDGET_H
