#include <chrono>
#include <QQuickWindow>
#include <QList>
#include <QObject>
#include <QQuickItem>
#include <utils/ui/qt/app.h>
#include <utils/file_utils.h>
#include <utils/ui/qt/app_environment.h>
#include <utils/ui/qt/import_qml_components_plugins.h>
#include <utils/ui/qt/import_qml_plugins.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			app::app(int argc, char* argv[])
				: ui::node(nullptr)
				, base::app(argc, argv)
				, QGuiApplication(argc, argv)
				, m_timer(this)
			{
				connect(&m_timer, &QTimer::timeout, this, &app::update_qt);
				m_timer.start();
			}
			
			int app::init()
			{
				QList<QObject*> rootObjects = engine().rootObjects();
				for (QObject* obj : rootObjects)
					if (m_main_window = qobject_cast<QQuickWindow*>(obj))
						break;
				if (!m_main_window) {
					qWarning() << "Could not find main window";
					return -1;
				}
				qt::node::init(m_main_window);
				return 0;
			}

            QObject* app::content_qobject()
			{
				return m_main_window->contentItem();
			}

			int app::on_run()
			{
				// Setup Qt
				set_qt_environment();

				engine().addImportPath("qrc:/imports");
				const QUrl url(u"qrc:Main/main.qml"_qs);
				QObject::connect(
					&engine(), &QQmlApplicationEngine::objectCreated, this,
					[url](QObject *obj, const QUrl &objUrl) {
						if (!obj && url == objUrl)
							QCoreApplication::exit(-1);
					},
					Qt::QueuedConnection);

				 engine().addImportPath(QCoreApplication::applicationDirPath() + "/qml");
				 engine().addImportPath(":/");

				engine().load(url);

				if (engine().rootObjects().isEmpty()) {
					return -1;
				}

				auto ret = init();
				if (ret != 0)
					return ret;
				return exec();
			}

			void app::update_qt()
			{
				using namespace std::chrono;
				long long ns = 0;
				static auto start = steady_clock::now();
				auto end = steady_clock::now();
				ns = duration_cast<nanoseconds>(end - start).count();
				if (!app::update((float(ns) * float(1e-9))))
					quit();
				start = end;
			}
			
			bool app::update(float dt)
			{
				bool active = true;
				base::update(dt);
				processEvents(QEventLoop::AllEvents);
				active = on_update(dt);
				return active;
			}
		}
	}
}