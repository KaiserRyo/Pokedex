#include "applicationui.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/DropDown>
#include <bb/cascades/RadioGroup>
#include <bb/cascades/Label>
#include <bb/cascades/ListView>
#include <bb/cascades/ScrollView>

#include <iostream>

#include "pokemonlist.h"
#include "pokemon.h"

using namespace bb::cascades;

using std::cerr;
using std::endl;

const QString DEFAULT_LANGUAGE = "9";
const QString ENGLISH = "English";
const QString ALL_TYPES_JAPENESE = QString::fromLocal8Bit("すべてのタイプ");
const int ENGLISH_INT_VALUE = 9;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        																		QObject(app), m_pokemonList(0)
{
	// Create scene document from main.qml asset, the parent is set
	// to ensure the document gets destroyed properly at shut down.
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	// Create root object for the UI
	m_root = qml->createRootObject<AbstractPane>();

	// Set the handle for the "pokedex" object from QML
	qml->setContextProperty("pokedex", this);

	// Create the "model" to store data about pokemon
	m_pokemonList = new PokemonList(this);
	qml->setContextProperty("pokemon_list", m_pokemonList);
}


void ApplicationUI::init() {
	// on startup, language and type variables of the pokemon list need to be set so that the right data is obtained
	// This was done instead of calling setType and setLanguage to decrease startup time
	if (m_pokemonList->getInitCount() == 0) {
		m_pokemonList->m_local_language = DEFAULT_LANGUAGE.toInt();
		m_pokemonList->m_local_type = 0;
		m_current_language = DEFAULT_LANGUAGE;
	}
	// Populate radio buttons for language settings
	RadioGroup *radio(0);	// A pointer to hold the RadioGroup UI object
	// Search the root QML object for a control named "pokemon_types"
	radio = m_root->findChild<RadioGroup *>("pokedex_languages");

	if (radio) { // did we succeed in getting a pointer to the radio button UI control?
		QFile file_languages("app/native/assets/data/language_names.csv");
		if (file_languages.open(QIODevice::ReadOnly | QIODevice::Text)) {
			// Successfully opened
			QTextStream in(&file_languages); // create text stream from the file
			in.readLine(); // skip first line
			while (!in.atEnd()) { // read until end of file
				/* reads each line into a list, Splits at comma locations			 */
				QStringList list = in.readLine().split(",", QString::KeepEmptyParts,Qt::CaseSensitive);
				if (list[1] == DEFAULT_LANGUAGE) {
					if (list[0] == DEFAULT_LANGUAGE) { // select the default language
						radio->add(Option::create().text(list[2]).value(list[0].toInt()).selected(true));
					}
					else { // add other languages
						radio->add(Option::create().text(list[2]).value(list[0].toInt()));
					}
				}
			}
			file_languages.close();
		}
		else { // gracefully exit
			cerr << "Failed to open language_names.csv. Error code: " << file_languages.error() << endl;
		}

	}
	// Now initialise all data with the correct languages
	if (m_pokemonList->getInitCount() == 0) {
		m_pokemonList->initPokemonList();
	}
	// Set status text
	Label *status(0);	// A pointer to hold the Label UI object
	// Search the root QML object for a control named "status"
	status = m_root->findChild<Label *>("pokedex_status");
	if (status) { // did we succeed in getting a pointer to the Label UI control?
		// Yes. Now set the text as appropriate
		// Had to change default encoding to UTF 8 to get all types in Japenese to work
		// This is a work around to get the e in pokemon to print the accent
		status->setText( QString("Found %1 Pok").arg(m_pokemonList->childCount(QVariantList())) +QChar(0x0E9)+QString("mon") );
	}
	else {
		cerr << "failed to find status " << endl;
	}
}

//return root object
AbstractPane* ApplicationUI::getRoot() {
	return m_root; // return root object
}

void ApplicationUI::typeSelected(int type) {
	cerr << "In typeSelected() with " << "type=" << type << endl;
	m_pokemonList->setType(type); // set the type of the pokemonList class
	// Set status text
	Label *status(0);	// A pointer to hold the Label UI object
	// Search the root QML object for a control named "status"
	status = m_root->findChild<Label *>("pokedex_status");
	if (status) { // did we succeed in getting a pointer to the Label UI control?
		// Yes. Now set the text as appropriate
		// again, workaround for encoding problems as in init function
		status->setText( QString("Found %1 Pok").arg(m_pokemonList->childCount(QVariantList())) +QChar(0x0E9)+QString("mon") );
	}
	else {
		cerr << "failed to find status " << endl;
	}
}

void ApplicationUI::languageSelected(int language) {
	cerr << "In languageSelected() with " << "language=" << language << endl;
	m_pokemonList->setLanguage(language); // set the correct language in pokemonList
	m_current_language = QString::number(language); // set the local language properly
	//refreshDropDown(language);
	DropDown *dropDown(0);	// pointer to hold the DropDown UI object
	// Search the root QML object for a control named "pokemon_types"
	dropDown = m_root->findChild<DropDown *>("pokemon_types");
	dropDown->removeAll(); // clear the current dropdown
	// Populate the dropdown list of types
	std::vector<QString> datList;
	QFile file_types("app/native/assets/data/type_names.csv");
	if (file_types.open(QIODevice::ReadOnly | QIODevice::Text)) {
		// Successfully opened
		QTextStream in(&file_types); // create text stream from the file
		in.readLine(); // skip first line
		while (!in.atEnd()) { // read until end of file
			/* reads each line into a list, Splits at comma locations */
			QStringList list = in.readLine().split(",", QString::KeepEmptyParts,Qt::CaseSensitive);
			bool found = false;
			while (!in.atEnd() && list[1].toInt() != ENGLISH_INT_VALUE) {
				if (list[1] == m_current_language) { // look for correct types in the current language
					found = true;
					datList.push_back(list[2]);
				}
				list = in.readLine().split(",", QString::KeepEmptyParts,Qt::CaseSensitive);
			}
			if (list[1].toInt() == ENGLISH_INT_VALUE && !found && m_current_language.toInt() != ENGLISH_INT_VALUE) {
				datList.push_back(list[2] + " [" + ENGLISH + "]"); // if not found, get English name and add "English"
			}
			else if (list[1].toInt() == ENGLISH_INT_VALUE && !found) {
				datList.push_back(list[2]); // Will add if english is the current language
			}
		}
		file_types.close();
	}
	else { // gracefully exit
		cerr << "Failed to open type_names.csv. Error code:  " << file_types.error() << endl;
	}

	if (dropDown) { // did we succeed in getting a pointer to the drop down UI control?
		//dropDown->add(Option::create().text("All Types").value(0).selected(true));
		if (m_pokemonList->m_local_language == 1) {
			// add the Japense translation of "All Types"
			dropDown->add(Option::create().text(ALL_TYPES_JAPENESE).value(0).selected(true));
		}
		else { // English all types option
			dropDown->add(Option::create().text("All Types").value(0).selected(true));
		}
		for (unsigned i = 0; i < datList.size()-2; i++) { // exclude values over 718
			dropDown->add(Option::create().text(datList[i]).value(i+1)); // Yes. Add a new option
		}
	}
}

