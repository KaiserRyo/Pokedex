/*
 * PokemonList.cpp
 *
 *  Created on: 2013-11-02
 *      Author: Jagath
 */

#include "pokemonlist.h"
/*
 * PokemonList is derive from DataModel which provides the base class for the "model"
 * in the model-view-controller pattern used by the ListView UI control
 */
static unsigned init_count = 0; // counts times initPokemonList has been called
const QString ENGLISH = "English";

PokemonList::PokemonList(QObject* parent) : bb::cascades::DataModel(parent), m_local_language(9), m_local_type(0) {
}

// Return the number of child items contained in a given entry
// Since we only implement a flat list, we must return 0 for any sub item.
int PokemonList::childCount(const QVariantList& indexPath) {
	// For indexPath, See http://developer.blackberry.com/native/reference/cascades/bb__cascades__datamodel.html

	// Return the item count if asking for top level count
	if (indexPath.empty())
		return m_pokemon.size();

	// No sub levels
	return 0;
}

// Used for displaying multi-level lists
bool PokemonList::hasChildren(const QVariantList& indexPath) {
	if (indexPath.empty())
		return true; // The root node always has children
	else
		return false; //
}

/*
 * data function retrieves data about the current indexpath item so that it can be displayed in the app
 */
QVariant PokemonList::data(const QVariantList& indexPath) {
	int i = indexPath[0].toInt(); 		// Get the menu index
	QVariantMap data;

	/* format pokemon name */
	Pokemon * current = m_pokemon[i];
	QString name_with_id_types =  current->getId() + " "; //add ids to names
	if (name_with_id_types.length() < 3)
		name_with_id_types.prepend("#00");
	else if (name_with_id_types.length() < 4)
		name_with_id_types.prepend("#0");
	else
		name_with_id_types.prepend("#");

	// name to be used on the item page
	QString name_with_id = name_with_id_types + current->getIdentifier();
	name_with_id_types += (current->getIdentifier() + " [" + current->getTypeNames() + "]");

	// fetch all the information about the current pokemon using get methods
	data["name"] = name_with_id_types;	// Get the name of pokemon formatted for the list
	data["itemName"] = name_with_id; // name of pokemon on the item page
	data["type"] = current->getTypeNames();

	// stats for current pokemon, call get functions of current pokemon
	data["weight"] = current->getWeight();
	data["height"] = current->getHeight();
	data["attack"] = current->getAttack();
	data["defence"] = current->getDefence();
	data["specialAttack"] = current->getSpecialAttack();
	data["specialDefence"] = current->getSpecialDefence();
	data["hitPoints"] = current->gethp();
	data["speed"] = current->getSpeed();
	data["totalStatPoints"] = current->getTotalStats();
	data["description"] = current->getDescription();
	data["introductoryVersion"] = current->getVersionEntrance();
	data["ability"] = current->getAbilityNames();

	// set stats in correct language (wasn't sure if required)
	data["hp"] = m_hp;
	data["attackName"] = m_attack;
	data["defenseName"] = m_defense;
	data["specialAttackName"] = m_specAttack;
	data["specialDefenseName"] = m_specDefense;
	data["speedName"] = m_speed;

	return data;			// Return the QVariant object
}

PokemonList::~PokemonList() {
	// delete all pokemon objects that exist in the vector
	// Since items were dynamically created as a list of pointers, vector class cannot handle deletion
	// so it must be done manually. Alternatively, my best best probably would have been to use the
	// boost library shared_ptrs as there is really no need for me to handle memory in this app
	while (!m_pokemon.empty()) {
		delete m_pokemon.back();
		m_pokemon.pop_back();
	}
}

/*
 * populates a vector with QStringLists, separates and stores all lines in a file and stores each line as
 * QStringList with each index of the list contatined a part of the line separated by a comma (,)
 */
void PokemonList::populateDataListOfLists(std::vector<QStringList>& datList,
		QFile& file) {
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		// Successfully opened
		QTextStream in(&file); // create text stream from the file
		in.readLine(); // skip first line
		while (!in.atEnd()) { // read until end of file
			/* reads each line into a list, Splits at comma locations			 */
			QStringList myList = in.readLine().split(",", QString::KeepEmptyParts,Qt::CaseSensitive);
			datList.push_back(myList);
		}
		file.close();
	}
	else
		cerr << "Failed to open " << file.fileName().toStdString() << " Error code: " << file.error() << endl;
}

/*
 * populates a vector with QStrings, separates and stores all QStrings separated by a comma (,)
 */
void PokemonList::populateDataList(std::vector<QString> & datList, QFile & file) {
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		// Successfully opened
		QTextStream in(&file); // create text stream from the file
		in.readLine(); // skip first line
		while (!in.atEnd()) { // read until end of file
			/* reads each line into a list, Splits at comma locations			 */
			QStringList list = in.readLine().split(",", QString::KeepEmptyParts,Qt::CaseSensitive);
			while (!list.empty())
				datList.push_back(list.takeFirst()); // adds all elements of line to vector
		}
		file.close();
	}
	else {
		cerr << "Failed to open " << file.fileName().toStdString() << " Error code: " << file.error() << endl;
	}
}

/**
 * initPokemonList function initialises the pokemon vector with all data from the csv files
 * It receives info from applicationUI to ensure the correct data is collected from files
 */
void PokemonList:: initPokemonList() {

	//if list is not empty, delete items so that correct information is display in application
	while(!m_pokemon.empty()) {
		delete m_pokemon.back();
		m_pokemon.pop_back();
	}

	//create file objects for all files
	QFile file_pokemon_species_flavor_text("app/native/assets/data/pokemon_species_flavor_text.csv");
	QFile file_pokemon_species_names("app/native/assets/data/pokemon_species_names.csv");
	QFile file_pokemon_stats("app/native/assets/data/pokemon_stats.csv");
	QFile file_pokemon_types("app/native/assets/data/pokemon_types.csv");
	QFile file_pokemon("app/native/assets/data/pokemon.csv");
	QFile file_stat_names("app/native/assets/data/stat_names.csv");
	QFile file_version_names("app/native/assets/data/version_names.csv");
	QFile file_pokemon_abilities("app/native/assets/data/pokemon_abilities.csv");

	/* Data Lists */
	std::vector<QString> list_stat_names;
	std::vector<QString> list_version_names;
	std::vector<QString> list_pokemon_types;
	std::vector<QString> list_pokemon_stats;
	std::vector<QString> list_pokemon_abilities;
	std::vector<QStringList> list_id_correct_languages;
	std::vector<QStringList> list_descriptions;

	/*populate lists of attributes from files */
	populateDataList(list_stat_names, file_stat_names);
	populateDataList(list_pokemon_types, file_pokemon_types);
	populateDataList(list_pokemon_stats, file_pokemon_stats);
	populateDataList(list_pokemon_abilities, file_pokemon_abilities);
	populateDataListOfLists(list_descriptions, file_pokemon_species_flavor_text);
	populateDataListOfLists(list_id_correct_languages , file_pokemon_species_names);


	// set stat names in the correct languages
	// each stat is the third work of each line, hence incremented stat_counter by 9.
	// proably would have been more eloquent to hard code in this case
	int stat_counter;
	if (m_local_language == 1)
		stat_counter = 2;
	else if (m_local_language == 8)
		stat_counter = 5;
	else
		stat_counter = 8;
	m_hp = list_stat_names[stat_counter];
	stat_counter = stat_counter + 9;
	m_attack = list_stat_names[stat_counter];
	stat_counter = stat_counter + 9;
	m_defense = list_stat_names[stat_counter];
	stat_counter = stat_counter + 9;
	m_specAttack = list_stat_names[stat_counter];
	stat_counter = stat_counter + 9;
	m_specDefense = list_stat_names[stat_counter];
	stat_counter = stat_counter + 9;
	m_speed = list_stat_names[stat_counter];


	// find out how many objects we need to retrieve of the type selected
	unsigned current_count_type = 0;
	if (m_local_type == 0 || init_count == 0) { // if init hasn't been called yet, we know there will be 718 (all types)
		current_count_type = 718;
	}
	else { // if not, we need to count the types
		for (unsigned i = 1; i < list_pokemon_types.size() && list_pokemon_types[i-1].toInt() < 1000; i = i + 3) {
			if (list_pokemon_types[i].toInt() == m_local_type) {
				current_count_type++;
			}
		}
	}

	// couldn't use a populate function since only one part of each line was needed
	if (file_version_names.open(QIODevice::ReadOnly | QIODevice::Text)) {
		// Successfully opened
		QTextStream in(&file_version_names); // create text stream from the file
		in.readLine(); // skip first line
		while (!in.atEnd()) { // read until end of file
			/* reads each line into a list, Splits at comma locations			 */
			QStringList myList = in.readLine().split(",", QString::KeepEmptyParts,Qt::CaseSensitive);
			list_version_names.push_back(myList[2]);
		}
		file_version_names.close();
	}
	else
		cerr << "Failed to open " << file_version_names.fileName().toStdString() << " Error code: " << file_version_names.error() << endl;

	/* open pokemon file to populate the list to size 718 */
	if (file_pokemon.open(QIODevice::ReadOnly | QIODevice::Text)) {
		// Successfully opened
		QTextStream in(&file_pokemon); // create text stream from the file
		in.readLine(); // skip first line

		// The following are counter variables used to keep track of files
		unsigned j = 0;
		unsigned f = 0;
		unsigned statCounter = 0;
		unsigned m = 0;
		unsigned n = 0;
		bool keepGoing = true;


		while (m_pokemon.size() < current_count_type) { // continue until we've got info for all pokemon of selected type
			/* reads each line into a list, Splits at comma locations			 */
			QStringList list = in.readLine().split(",", QString::KeepEmptyParts,Qt::CaseSensitive);
			Pokemon * tmp = new Pokemon; // create new pokemon object
			m_pokemon.push_back(tmp); // store it in our vector
			// Initialize member variables for pokemon objects
			tmp->setLocalLanguageId(m_local_language);
			tmp->setId(list[0]);
			tmp->setSpeciesId(list[2]);
			tmp->setHeight(QString::number(list[3].toDouble()/10)); //divide by 10 to get correct units
			tmp->setWeight(QString::number(list[4].toDouble()/10)); //divide by 10 to get correct units
			// Loop through description list to find earliest occurence of each pokemon
			keepGoing = true;
			if (n <list_descriptions.size()) {
				QString fullDescription = "";
				for (int i = 3; i < list_descriptions[n].size(); i++) {
					// add all parts of description (because some descriptions contain commas)
					fullDescription = fullDescription + list_descriptions[n][i];
				}
				tmp->setDescription(fullDescription);
				tmp->setVersionEntrance(list_version_names[list_descriptions[n][1].toInt() - 1]); // store earliest occurence
			}
			while(keepGoing) { // loop ahead in list until we've reached the next pokemon
				if (!(n < list_descriptions.size()))
					keepGoing = false;
				else if(list_descriptions[n][0].toInt() == tmp->getId().toInt())
					n++;
				else
					keepGoing = false;
			}
			// get abilities for current pokemon
			// fairly tedious method of getting this information
			if (list_pokemon_abilities.size() > f && list_pokemon_abilities[f] == tmp->getId()) {
				tmp->setA1(list_pokemon_abilities[++f]); // set first ability
				f = f + 3;// jump to next line
			}
			if (list_pokemon_abilities.size() > f && list_pokemon_abilities[f] == tmp->getId()) {
				tmp->setA2(list_pokemon_abilities[++f]);
				f = f + 3;
			}
			else
				tmp->setA2("0");  // doesn't have a second ability
			if (list_pokemon_abilities.size() > f && list_pokemon_abilities[f] == tmp->getId()) {
				tmp->setA3(list_pokemon_abilities[++f]);
				f = f + 3;
			}
			else {
				tmp->setA3("0"); // doesn't have a third ability
			}

			// using same method as getting abilities, get types of each pokemon
			if (list_pokemon_types.size() > j && list_pokemon_types[j] == tmp->getId()) {
				tmp->setTypeId1(list_pokemon_types[++j]);
				j = j + 2; // jump to next line
			}
			if (list_pokemon_types.size() > j && list_pokemon_types[j] == tmp->getId()) {
				tmp->setTypeId2(list_pokemon_types[++j]);
				j = j + 2;
			}
			else
				tmp->setTypeId2("0");
			if (list_pokemon_types.size() > j && list_pokemon_types[j] == tmp->getId()) {
				tmp->setTypeId3(list_pokemon_types[++j]);
				j = j + 2;
			}
			else {
				tmp->setTypeId3("0");
			}

			bool found = false;
			// loop to current pokemon
			while (m < list_id_correct_languages.size() && list_id_correct_languages[m][0] != tmp->getId()) {
				m++;
			}
			// search for correct identifier
			while (m < list_id_correct_languages.size() && list_id_correct_languages[m][0] == tmp->getId()) {
				if (list_id_correct_languages[m][1].toInt() == m_local_language){
					found = true;
					tmp->setIdentifier(list_id_correct_languages[m][2]); // set identifier in correct language
				}
				m++;
			}
			if (!found) { // not found, use english identifier and let user know by adding [English]
				tmp->setIdentifier((list[1].replace(0, 1, list[1][0].toUpper()))+ " [" + ENGLISH + "]");
			}

			// get pokemon stats
			if (statCounter <= list_pokemon_stats.size() && list_pokemon_stats[statCounter] == tmp->getId()) {
				tmp->sethp(list_pokemon_stats[statCounter + 2]);
				statCounter= statCounter+ 4;
				tmp->setAttack(list_pokemon_stats[statCounter + 2]);
				statCounter= statCounter+ 4;
				tmp->setDefence(list_pokemon_stats[statCounter + 2]);
				statCounter= statCounter+ 4;
				tmp->setSpecialAttack(list_pokemon_stats[statCounter + 2]);
				statCounter= statCounter+ 4;
				tmp->setSpecialDefence(list_pokemon_stats[statCounter + 2]);
				statCounter= statCounter+ 4;
				tmp->setSpeed(list_pokemon_stats[statCounter + 2]);
				statCounter= statCounter+ 4;
			}
			// filter results
			if (!tmp->isType(QString::number(m_local_type)) && m_local_type != 0) {
				delete m_pokemon.back();
				m_pokemon.pop_back();
			}

		}
		file_pokemon.close();
	}
	else
		cerr << "Failed to open pokemon.csv. Error code: " << file_pokemon.error() << endl;

	init_count++;
	emit itemsChanged( bb::cascades::DataModelChangeType::Init); // let GUI know to refresh
}

/*
 * setLanguage sets language and reloads data
 */
void PokemonList::setLanguage(int lang) {
	// set the new language if init has already been called
	m_local_language = lang;
	if (init_count != 0) {
		initPokemonList();
	}
}

/*
 * set type sets the type and reloads data
 */
void PokemonList::setType(int type) {
	// set the new type if init has been called
	m_local_type = type;
	if (init_count != 0) {
		initPokemonList();
	}
}

/*
 * return init_count unsigned
 */
unsigned PokemonList::getInitCount() {
	return init_count;
}
