#include <exception>
#include <QQmlContext>
#include <QUrl>
#include <QQmlComponent>
#include <QObject>
#include <utils/ui/qt/app.h>
#include <utils/ui/qt/widget_factory.h>
#include <utils/ui/qt/node.h>
#include <utils/Log.h>
#ifdef LOG_ON
#include <QDebug>
#endif
LOG_POSTFIX("\n");
LOG_PREFIX("[qt::node]: ");

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			qt::node::node(ui::node* parent)
				: base(parent)
			{
				set_factory(qt::widget_factory::instance());
			}
			
			// All the dynamic casts require a fully defined type.
			qt::app& qt::node::app()
			{
				return dynamic_cast<qt::app&>(base::app());
			}

			const qt::app& qt::node::get_app() const
			{
				return dynamic_cast<const qt::app&>(base::get_app());
			}

			int qt::node::init(const QUrl& componentUrl, const QVariantMap& initialProperties)
			{
				try
				{
					QVariantMap finalInitialProperties = initialProperties;
					auto parent_ptr = parent();
					assert(parent_ptr);
					if (!parent_ptr)
						return -2;
					QObject* parentObject = parent_ptr->content_qobject();
					if (parentObject)
						finalInitialProperties["parent"] = QVariant::fromValue(parentObject);
					else
						LOG_WARNING("Parent object is null!");
					QQmlComponent component(&app().engine(), componentUrl);
					QQmlContext* ctx = app().engine().rootContext();
					if (m_object = component.createWithInitialProperties(finalInitialProperties, ctx))
						return 0;
					else
					{
						for (const auto& error : component.errors())
							LOG("QQmlComponent error: " << error.toString().toStdString());
						LOG("There were errors during creating a dialog from qml");
						return -1;
					}
				}
				catch (std::exception& ex)
				{
					LOG("Loading qml exception: " << ex.what());
					return -10;
				}
				catch (...)
				{
					LOG("Loading qml exception!");
					return -11;
				}
				return 0;
			}
		}
	}
}