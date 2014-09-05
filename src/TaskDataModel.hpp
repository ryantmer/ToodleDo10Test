#ifndef TASKDATAMODEL_HPP_
#define TASKDATAMODEL_HPP_

#include <QObject>
#include <bb/cascades/DataModel>
#include <bb/cascades/ArrayDataModel>

class TaskDataModel : public bb::cascades::DataModel {
    Q_OBJECT
    static const QString databasePath;

public:
    TaskDataModel(QObject *parent = 0);
    virtual ~TaskDataModel();

    //Required by bb::cascades::DataModel
    Q_INVOKABLE virtual int childCount(const QVariantList &indexPath);
    Q_INVOKABLE virtual bool hasChildren(const QVariantList &indexPath);
    Q_INVOKABLE virtual QString itemType(const QVariantList &indexPath);
    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);

    Q_INVOKABLE virtual void removeItems(const QVariantList &indexPaths);

    QList<int> tasks();

private:
    QVariantList internalDB;
    void initDatabase(const QString &filename);

public slots:
    void onTaskAdded(QVariant data);
    void onTaskEdited(QVariant data);
};

#endif /* TASKDATAMODEL_HPP_ */
