#ifndef TOODLEDOTEN_CPP_
#define TOODLEDOTEN_CPP_

#include "ToodleDoTen.hpp"

#include <bb/cascades/Container>
#include <bb/cascades/SceneCover>
#include <bb/cascades/Label>
#include <bb/system/SystemToast>
#include <bb/system/SystemUiPosition>
#include <bb/PackageInfo>

using namespace bb::cascades;
using namespace bb::system;

ToodledoTen::ToodledoTen() : QObject() {
    qmlRegisterType<CustomDataModel>("DataModelUtil", 1, 0, "CustomDataModel");

    _propertiesManager = PropertiesManager::getInstance();
    _loginManager = LoginManager::getInstance();
    _networkManager = NetworkManager::getInstance();
    _taskSenderReceiver = new SenderReceiver(this);
    _taskSenderReceiver->setDataType(SenderReceiver::Task);
    _folderSenderReceiver = new SenderReceiver(this);
    _folderSenderReceiver->setDataType(SenderReceiver::Folder);
    _taskDataModel = new CustomDataModel(this);
    _taskDataModel->setDataType(CustomDataModel::Task);
    _folderDataModel = new CustomDataModel(this);
    _folderDataModel->setDataType(CustomDataModel::Folder);

    //Create root QML document from main.qml and expose certain variables to QML
    QmlDocument *qml = QmlDocument::create("asset:///TaskMain.qml").parent(this);
    qml->setContextProperty("app", this);
    qml->setContextProperty("propertyManager", _propertiesManager);
    qml->setContextProperty("taskDataModel", _taskDataModel);
    qml->setContextProperty("folderDataModel", _folderDataModel);
    _root = qml->createRootObject<NavigationPane>();
    Application::instance()->setScene(_root);

    //Expose app to the main listview
    QDeclarativeEngine *engine = QmlDocument::defaultDeclarativeEngine();
    QDeclarativeContext *rootContext = engine->rootContext();
    rootContext->setContextProperty("app", this);

    //Login page, shown if required
    QmlDocument *loginQml = QmlDocument::create("asset:///Login.qml").parent(this);
    _loginPage = loginQml->createRootObject<Page>();
    _loginWebView = _loginPage->findChild<WebView*>("loginWebView");

    //Add cover QML for app
    QmlDocument *qmlCover = QmlDocument::create("asset:///Cover.qml").parent(this);
    qmlCover->setContextProperty("app", this);
    _coverRoot = qmlCover->createRootObject<Container>();
    SceneCover *cover = SceneCover::create().content(_coverRoot);
    Application::instance()->setCover(cover);

    bool isOk;

    //Toodledo10 listeners
    isOk = connect(_loginWebView, SIGNAL(urlChanged(QUrl)),
            this, SLOT(onWebViewUrlChanged(QUrl)));
    Q_ASSERT(isOk);
    isOk = connect(_loginManager, SIGNAL(accessTokenRefreshed()),
            this, SLOT(onAccessTokenRefreshed()));
    Q_ASSERT(isOk);
    isOk = connect(_loginManager, SIGNAL(refreshTokenExpired()),
            this, SLOT(onRefreshTokenExpired()));
    Q_ASSERT(isOk);
    isOk = connect(_networkManager, SIGNAL(networkStateChanged(bool)),
            this, SLOT(onNetworkStateChanged(bool)));
    Q_ASSERT(isOk);
    //YEAH TOAST
    isOk = connect(_propertiesManager, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_loginManager, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_networkManager, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_taskSenderReceiver, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_folderSenderReceiver, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_taskDataModel, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);
    isOk = connect(_folderDataModel, SIGNAL(toast(QString)),
            this, SLOT(onToast(QString)));
    Q_ASSERT(isOk);

    //LoginManager listeners
    isOk = connect(this, SIGNAL(loggedOut()),
            _loginManager, SLOT(onLogOut()));
    Q_ASSERT(isOk);

    //TaskDataModel listeners
    //UI->sender/receiver->datamodel
    isOk = connect(this, SIGNAL(loggedOut()),
            _taskDataModel, SLOT(onLogOut()));
    Q_ASSERT(isOk);
    isOk = connect(_taskSenderReceiver, SIGNAL(taskGetReply(QVariantMap)),
            _taskDataModel, SLOT(onEdit(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_taskSenderReceiver, SIGNAL(taskEditReply(QVariantMap)),
            _taskDataModel, SLOT(onEdit(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_taskSenderReceiver, SIGNAL(taskAddReply(QVariantMap)),
            _taskDataModel, SLOT(onAdd(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_taskSenderReceiver, SIGNAL(taskRemoveReply(QVariantMap)),
            _taskDataModel, SLOT(onRemove(QVariantMap)));
    Q_ASSERT(isOk);

    //FolderDataModel listeners
    //UI->sender/receiver->datamodel
    isOk = connect(this, SIGNAL(loggedOut()),
            _folderDataModel, SLOT(onLogOut()));
    Q_ASSERT(isOk);
    isOk = connect(_folderSenderReceiver, SIGNAL(folderGetReply(QVariantMap)),
            _folderDataModel, SLOT(onEdit(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_folderSenderReceiver, SIGNAL(folderEditReply(QVariantMap)),
            _folderDataModel, SLOT(onEdit(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_folderSenderReceiver, SIGNAL(folderAddReply(QVariantMap)),
            _folderDataModel, SLOT(onEdit(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(_folderSenderReceiver, SIGNAL(folderRemoveReply(QVariantMap)),
            _folderDataModel, SLOT(onRemove(QVariantMap)));
    Q_ASSERT(isOk);

    //TSR listeners
    isOk = connect(this, SIGNAL(taskAdded(QVariantMap)),
            _taskSenderReceiver, SLOT(onAdd(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(taskEdited(QVariantMap, QVariantMap)),
            _taskSenderReceiver, SLOT(onEdit(QVariantMap, QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(taskRemoved(QVariantMap)),
            _taskSenderReceiver, SLOT(onRemove(QVariantMap)));
    Q_ASSERT(isOk);

    //FSR listeners
    isOk = connect(this, SIGNAL(folderAdded(QVariantMap)),
            _folderSenderReceiver, SLOT(onAdd(QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(folderEdited(QVariantMap, QVariantMap)),
            _folderSenderReceiver, SLOT(onEdit(QVariantMap, QVariantMap)));
    Q_ASSERT(isOk);
    isOk = connect(this, SIGNAL(folderRemoved(QVariantMap)),
            _folderSenderReceiver, SLOT(onRemove(QVariantMap)));
    Q_ASSERT(isOk);

    Q_UNUSED(isOk);
}
ToodledoTen::~ToodledoTen() {};

CustomDataModel *ToodledoTen::taskDataModel() {
    return _taskDataModel;
}

CustomDataModel *ToodledoTen::folderDataModel() {
    return _folderDataModel;
}

void ToodledoTen::showToast(QString message) {
    SystemToast *toast = new SystemToast(this);
    toast->setBody(message);
    toast->setPosition(SystemUiPosition::MiddleCenter);
    toast->show();
}

/*
 * Q_INVOKABLE functions begin
 */
QDateTime ToodledoTen::unixTimeToDateTime(uint unixTime) {
    return QDateTime::fromTime_t(unixTime);
}

uint ToodledoTen::dateTimeToUnixTime(QDateTime dateTime) {
    return dateTime.toTime_t();
}

QDateTime ToodledoTen::unixTimeToDateTimeNoOffset(uint unixTime) {
    //Calculate offset in timestamp, and remove it
    QDateTime dt1 = QDateTime::currentDateTime();
    QDateTime dt2 = dt1.toUTC();
    dt1.setTimeSpec(Qt::UTC);
    int offset = dt2.secsTo(dt1);

    return QDateTime::fromTime_t(unixTime - offset);
}

uint ToodledoTen::dateTimeToUnixTimeNoOffset(QDateTime dateTime) {
    QDateTime dt1 = QDateTime::currentDateTime();
    QDateTime dt2 = dt1.toUTC();
    dt1.setTimeSpec(Qt::UTC);
    int offset = dt2.secsTo(dt1);

    return dateTime.toTime_t() + offset;
}

uint ToodledoTen::getLengthValue(QDateTime dateTime) {
    return dateTime.time().hour() * 60 + dateTime.time().minute();
}

QString ToodledoTen::getVersionNumber() {
    bb::PackageInfo pi;
    return pi.version();
}

void ToodledoTen::refreshTasks() {
    if (_networkManager->isConnected()) {
        if (_loginManager->isLoggedIn()) {
            _taskDataModel->clear();
            _taskSenderReceiver->fetchAll();
        } else {
            qWarning() << Q_FUNC_INFO << "LoginManager indicated not logged in";
        }
    } else {
        qWarning() << Q_FUNC_INFO << "NetworkManager indicated no network connection";
        showToast("No network connection!");
    }
}

void ToodledoTen::refreshFolders() {
    if (_networkManager->isConnected()) {
        if (_loginManager->isLoggedIn()) {
            _folderDataModel->clear();
            _folderSenderReceiver->fetchAll();
        } else {
            qWarning() << Q_FUNC_INFO << "LoginManager indicated not logged in";
        }
    } else {
        qWarning() << Q_FUNC_INFO << "NetworkManager indicated no network connection";
    }
}

void ToodledoTen::addTask(QVariantMap data) {
    emit taskAdded(data);
}

void ToodledoTen::editTask(QVariantMap oldData, QVariantMap newData) {
    if (oldData != newData) {
        emit taskEdited(oldData, newData);
    }
}

void ToodledoTen::removeTask(QVariantMap data) {
    emit taskRemoved(data);
}

void ToodledoTen::addFolder(QVariantMap data) {
    qDebug() << Q_FUNC_INFO << "Folder added";
    emit folderAdded(data);
}

void ToodledoTen::editFolder(QVariantMap oldData, QVariantMap newData) {
    qDebug() << Q_FUNC_INFO << "Folder edited";
    if (oldData != newData) {
        emit folderEdited(oldData, newData);
    }
}

void ToodledoTen::removeFolder(QVariantMap data) {
    qDebug() << Q_FUNC_INFO << "Folder removed";
    emit folderRemoved(data);
}

QVariantList ToodledoTen::getFolderList() {
    return _folderDataModel->getInternalList();
}

void ToodledoTen::logout() {
    emit loggedOut();
}
/*
 * Q_INVOKABLE functions end
 */

/*
 * Slots begin
 */
void ToodledoTen::onNetworkStateChanged(bool connected) {
    if (connected) {
        refreshFolders();
        refreshTasks();
    } else {
        qWarning() << Q_FUNC_INFO << "Network connection lost";
        showToast("Network connection lost");
    }
}

void ToodledoTen::onWebViewUrlChanged(QUrl url) {
    if (url.hasQueryItem("code") && url.hasQueryItem("state")) {
        if (url.queryItemValue("state") == _loginManager->getState()) {
            //Get authCode from webview's URL
            QString authCode = url.queryItemValue("code");
            //Start access token request
            _loginManager->refreshRefreshToken(authCode);
            //Remove webview from nav pane
            _root->pop();
        } else {
            qWarning() << Q_FUNC_INFO << "State didn't match";
        }
    }
}

void ToodledoTen::onAccessTokenRefreshed() {
    //access token is automatically refreshed when it expires using refresh token
    //When a new access token is received, refresh
    refreshTasks();
    refreshFolders();
}

void ToodledoTen::onRefreshTokenExpired() {
    //emitted by LoginManager when refresh token is no longer valid (30-day expiry)
    //when this occurs, user has to log in again
    showToast("Please log in!");
    _root->push(_loginPage);
    _loginWebView->setUrl(_loginManager->getAuthorizeUrl().toString());
}

void ToodledoTen::onToast(QString message) {
    showToast(message);
}
/*
 * Slots end
 */

#endif /* TOODLEDOTEN_CPP_ */
