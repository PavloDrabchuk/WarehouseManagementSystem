QT       += core gui sql
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    acceptance_goods.cpp \
    add_customers.cpp \
    add_good_category.cpp \
    add_goods.cpp \
    add_goods_group.cpp \
    add_new_users.cpp \
    add_provider.cpp \
    admin_page.cpp \
    connection.cpp \
    damage_goods_page.cpp \
    delivery_goods.cpp \
    main.cpp \
    mainwindow.cpp \
    reports.cpp \
    search_window.cpp \
    send_mails.cpp \
    update_users.cpp \
    write_off_goods.cpp

HEADERS += \
    acceptance_goods.h \
    add_customers.h \
    add_good_category.h \
    add_goods.h \
    add_goods_group.h \
    add_new_users.h \
    add_provider.h \
    admin_page.h \
    connection.h \
    damage_goods_page.h \
    delivery_goods.h \
    mainwindow.h \
    reports.h \
    search_window.h \
    send_mails.h \
    update_users.h \
    write_off_goods.h

FORMS += \
    acceptance_goods.ui \
    add_customers.ui \
    add_good_category.ui \
    add_goods.ui \
    add_goods_group.ui \
    add_new_users.ui \
    add_provider.ui \
    admin_page.ui \
    damage_goods_page.ui \
    delivery_goods.ui \
    mainwindow.ui \
    reports.ui \
    search_window.ui \
    send_mails.ui \
    update_users.ui \
    write_off_goods.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_ABIS = x86_64

RESOURCES +=
