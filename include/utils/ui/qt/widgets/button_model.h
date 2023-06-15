#pragma once

#include <QObject>
#include <QString>

namespace utils
{
    namespace ui
    {
        namespace qt
        {
            class button_model : public QObject
            {
                Q_OBJECT
            public:
                explicit button_model(QObject* parent = 0) : QObject(parent) {}

            public slots:
                void buttonClicked();
                void buttonClicked(QString &in);
            };
        }
    }
}
Q_DECLARE_METATYPE(utils::ui::qt::button_model)
