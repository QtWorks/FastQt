#include "perbasecontentanalysis.h"

PerBaseContentAnalysis::PerBaseContentAnalysis()
    :Analysis()
{

    setName("Per Base content");
    setTooltip("Per base content");
}

void PerBaseContentAnalysis::processSequence(const Sequence &sequence)
{


    if (mGCounts.size() < sequence.size())
    {
        mGCounts.resize(sequence.size());
        mACounts.resize(sequence.size());
        mTCounts.resize(sequence.size());
        mCCounts.resize(sequence.size());
    }


    for (int i=0; i<sequence.size(); ++i)
    {
        if (sequence.sequence().at(i) == 'G')
            mGCounts[i]++;

        if (sequence.sequence().at(i) == 'A')
            mACounts[i]++;

        if (sequence.sequence().at(i)== 'T')
            mTCounts[i]++;

        if (sequence.sequence().at(i)== 'C')
            mCCounts[i]++;
    }



}

void PerBaseContentAnalysis::reset()
{
    mGCounts.clear();
    mACounts.clear();
    mTCounts.clear();
    mCCounts.clear();

    gPercent.clear();
    aPercent.clear();
    tPercent.clear();
    cPercent.clear();
}

QWidget *PerBaseContentAnalysis::createResultWidget()
{

    computePercentages();

    QSplineSeries  * ASerie = new QSplineSeries ();
    QSplineSeries  * GSerie = new QSplineSeries ();
    QSplineSeries  * CSerie = new QSplineSeries ();
    QSplineSeries  * TSerie = new QSplineSeries ();


    for (int i=0; i<gPercent.size(); ++i)
    {
        ASerie->append(i, aPercent[i]);
        GSerie->append(i, gPercent[i]);
        CSerie->append(i, cPercent[i]);
        TSerie->append(i, tPercent[i]);
    }


    QChart * chart = new QChart();
    chart->addSeries(ASerie);
    chart->addSeries(GSerie);
    chart->addSeries(CSerie);
    chart->addSeries(TSerie);
    chart->createDefaultAxes();

    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::green);
    ASerie->setPen(pen);

    pen.setColor(Qt::blue);
    CSerie->setPen(pen);

    pen.setColor(Qt::red);
    TSerie->setPen(pen);


    pen.setColor(Qt::black);
    GSerie->setPen(pen);



    chart->setTitle("Quality per base");
    chart->setAnimationOptions(QChart::NoAnimation);



    QChartView * view = new QChartView;
    view->setChart(chart);

    return view;
}

void PerBaseContentAnalysis::computePercentages()
{


    QVector<BaseGroup> groups = BaseGroup::makeBaseGroups(mGCounts.size());
    mXCategories.resize(groups.length());

    gPercent.resize(groups.length());
    aPercent.resize(groups.length());
    tPercent.resize(groups.length());
    cPercent.resize(groups.length());


    quint64 total;
    quint64 gCount;
    quint64 aCount;
    quint64 tCount;
    quint64 cCount;


    for (int i=0;i<groups.length();i++) {

        mXCategories[i] = groups[i].toString();

        gCount = 0;
        aCount = 0;
        tCount = 0;
        cCount = 0;
        total  = 0;

        for (int bp=groups[i].lowerCount()-1;bp<groups[i].upperCount();bp++) {

            total += mGCounts[bp];
            total += mCCounts[bp];
            total += mACounts[bp];
            total += mTCounts[bp];

            aCount += mACounts[bp];
            tCount += mTCounts[bp];
            cCount += mCCounts[bp];
            gCount += mGCounts[bp];
        }



        gPercent[i] = (gCount/(double)total)*100;
        aPercent[i] = (aCount/(double)total)*100;
        tPercent[i] = (tCount/(double)total)*100;
        cPercent[i] = (cCount/(double)total)*100;

    }
}