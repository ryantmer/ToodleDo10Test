#ifndef PROPERTIESMANAGER_HPP_
#define PROPERTIESMANAGER_HPP_

#include <QObject>

class PropertiesManager : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(bool showTaskTime READ showTaskTime WRITE setShowTaskTime NOTIFY showTaskTimeChanged);
    Q_PROPERTY(bool advancedMode READ advancedMode WRITE setAdvancedMode NOTIFY advancedModeChanged);
    Q_PROPERTY(uint lastUpdateTime READ lastUpdateTime WRITE setLastUpdateTime NOTIFY lastUpdateTimeChanged);

    static PropertiesManager *getInstance();
    PropertiesManager(QObject *parent = NULL);
    virtual ~PropertiesManager();

    bool showTaskTime();
    void setShowTaskTime(bool show);
    bool advancedMode();
    void setAdvancedMode(bool advanced);
    uint lastUpdateTime();
    void setLastUpdateTime(uint time);

signals:
    void showTaskTimeChanged(bool show);
    void advancedModeChanged(bool advanced);
    void lastUpdateTimeChanged(uint time);

private:
};

#endif /* PROPERTIESMANAGER_HPP_ */