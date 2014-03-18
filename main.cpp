#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>
#include "applicationui.h"

#include <Qt/qdeclarativedebug.h>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
	Application app(argc, argv);

    // Create the Application UI object, this is where the main.qml file
	// is loaded and the application scene is set.
	ApplicationUI* pokeApp = new ApplicationUI(&app); // store applicationUI object
	pokeApp->init(); // Initialize the application
	app.setScene(pokeApp->getRoot()); // get the root to finish setting up the app

	// NOTE: no need to delete app1 (is done for us due to framework set up)

	// Enter the application main event loop.
	return Application::exec();
}
