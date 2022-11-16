# XWorker library
Worker utility to create classes supporting multithreading operations

## This project will not be maintenance anymore
I am not working for Qt since a while and updates started to become hard after so much changes in Qt 6

### Features
- Worker template creator
- Basic XWorker class to inherit
- Start, pause and cancel


### Example

#### Base interface to provide functions that will run in a thread
```
#ifndef STORAGEDATAINTERFACE_HPP
#define STORAGEDATAINTERFACE_HPP

#include <QObject>

#include "storagedevice.hpp"


class StorageDataInterface : public QObject
{
    Q_OBJECT
public:
    explicit StorageDataInterface(QObject *parent = nullptr);

    virtual StorageDevice getPrimaryDevice() = 0;
    virtual QList<StorageDevice> getAllStoragesData() = 0;
    virtual StorageDevice getStorageDeviceByFolder(const QString &folderPath) = 0;
};

#endif // STORAGEDATAINTERFACE_HPP
```

#### Worker definition (this will create a class with the name StorageFactoryWorker that inherit from StorageDataInterface and will contain all the threading functionalities)
```
#ifndef STORAGEFACTORYWORKER_HPP
#define STORAGEFACTORYWORKER_HPP

#include <QObject>

#include "xworkertemplate.hpp"
#include "storagedatainterface.hpp"


X_WORKER_DECLARATION_TEMPLATE(StorageFactoryWorker, StorageDataInterface, Q_OBJECT)

#endif //STORAGEFACTORYWORKER_HPP
```

```
#include "storagefactoryworker.hpp"


X_WORKER_DEFINITION_TEMPLATE(StorageFactoryWorker, StorageDataInterface)

```

#### X Storage Library [Link](https://github.com/CamiloDelReal/x_storage)
- [storagefactory.hpp](https://github.com/CamiloDelReal/x_storage/blob/develop/src/storagefactory.hpp)
- [storagefactory.cpp](https://github.com/CamiloDelReal/x_storage/blob/develop/src/storagefactory.cpp)