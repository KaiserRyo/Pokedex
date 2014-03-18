/*
 * pokemon.cpp
 *
 *  Created on: 2013-11-20
 *      Author: Eric
 */

#include "pokemon.h"

/*
 * Was not necessary to initialise members in constructor since all values will just be empty (i.e. no
 * undefined behaviour)
 */
Pokemon::Pokemon(): m_local_language_id(0) {
}

Pokemon::~Pokemon() {
}

/*
 * The following getter and setter methods do not require additional comments.
 * Could have made member variables public and skipped this step, as there is no risk of them causing unwanted
 * Behaviour or changes
 */
const QString Pokemon::getHeight() const {
	return m_height;
}

void Pokemon::setHeight(const QString height) {
	m_height = height;
}

const QString Pokemon::getId() const {
	return m_id;
}

void Pokemon::setId(const QString id) {
	m_id = id;
}

const QString Pokemon::getIdentifier() const {
	return m_identifier;
}

void Pokemon::setIdentifier(const QString identifier) {
	m_identifier = identifier;
}

int Pokemon::getLocalLanguageId() const {
	return m_local_language_id;
}

void Pokemon::setLocalLanguageId(const int localLanguageId) {
	m_local_language_id = localLanguageId;
}

const QString Pokemon::getSpeciesId() const {
	return m_species_id;
}

void Pokemon::setSpeciesId(const QString speciesId) {
	m_species_id = speciesId;
}

const QString Pokemon::getWeight() const {
	return m_weight;
}

void Pokemon::setWeight(const QString weight) {
	m_weight = weight;
}

const QString Pokemon::getTypeId1() const {
	return m_type_id1;
}

void Pokemon::setTypeId1(const QString typeId1) {
	m_type_id1 = typeId1;
}

const QString Pokemon::getTypeId2() const {
	return m_type_id2;
}

void Pokemon::setTypeId2(const QString typeId2) {
	m_type_id2 = typeId2;
}

const QString Pokemon::getTypeId3() const {
	return m_type_id3;
}

void Pokemon::setTypeId3(const QString typeId3) {
	m_type_id3 = typeId3;
}
const QString Pokemon::getAttack() const {
	return m_attack;
}

void Pokemon::setAttack(const QString attack) {
	m_attack = attack;
}

const QString Pokemon::getDefence() const {
	return m_defence;
}

void Pokemon::setDefence(const QString defense) {
	m_defence = defense;
}

const QString Pokemon::gethp() const {
	return m_hp;
}

void Pokemon::sethp(const QString hp) {
	m_hp = hp;
}

const QString Pokemon::getSpecialAttack() const {
	return m_special_attack;
}

void Pokemon::setSpecialAttack(const QString specialAttack) {
	m_special_attack = specialAttack;
}

const QString Pokemon::getSpecialDefence() const {
	return m_special_defence;
}

void Pokemon::setSpecialDefence(const QString specialDefense) {
	m_special_defence = specialDefense;
}

const QString Pokemon::getSpeed() const {
	return m_speed;
}

void Pokemon::setSpeed(const QString speed) {
	m_speed = speed;
}

const QString Pokemon::getDescription() const {
	return m_description;
}

void Pokemon::setDescription(const QString description) {
	m_description = description;
}

const QString Pokemon::getVersionEntrance() const {
	return m_version_entrance;
}

void Pokemon::setVersionEntrance(const QString versionEntrance) {
	m_version_entrance = versionEntrance;
}

const QString Pokemon::getA1() const {
	return m_a_1;
}

void Pokemon::setA1(const QString a1) {
	m_a_1 = a1;
}

const QString Pokemon::getA2() const {
	return m_a_2;
}

void Pokemon::setA2(const QString a2) {
	m_a_2 = a2;
}

const QString Pokemon::getA3() const {
	return m_a_3;
}

void Pokemon::setA3(const QString a3) {
	m_a_3 = a3;
}

const QString Pokemon::getTotalStats() const {
	return QString::number(m_hp.toInt() + m_attack.toInt() + m_speed.toInt() +
			m_defence.toInt() +  m_special_attack.toInt() + m_special_defence.toInt());
}

/*
 * is type function checks is a pokemon is of a certain type and returns true or false
 */
bool Pokemon::isType(const QString type) {
	if (m_type_id1 == type)
		return true;
	else if(m_type_id2 == type)
		return true;
	else if( m_type_id3 == type)
		return true;
	else
		return false;
}

/*
 * numTypes returns the number of types a pokemon belongs to
 */
int Pokemon::numTypes() {
	if (m_type_id2 == "0")
		return 1;
	else if (m_type_id3 == "0")
		return 2;
	else
		return 3;
}

/*
 * numAbilities returns the number of abilities a pokemon has
 */
int Pokemon::numAbilities() {
	if (m_a_2 == "0")
		return 1;
	else if (m_a_3 == "0")
		return 2;
	else
		return 3;
}

/*
 * populateDataList populates a vector with the correct items depending on the current language of the pokemon
 */
void Pokemon::populateDataList(std::vector<QString> & datList, QFile & file) {
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		// Successfully opened
		QTextStream in(&file); // create text stream from the file
		in.readLine(); // skip first line
		while (!in.atEnd()) { // read until end of file
			/* reads each line into a list, Splits at comma locations			 */
			QStringList list = in.readLine().split(",", QString::KeepEmptyParts,Qt::CaseSensitive);
			bool found = false;
			while (!in.atEnd() && list[1].toInt() != 9) {
				if (list[1].toInt() == m_local_language_id) { // look for correct language
					found = true; // found the correct language
					datList.push_back(list[2]);
				}
				list = in.readLine().split(",", QString::KeepEmptyParts,Qt::CaseSensitive);
			}
			if (list[1].toInt() == 9 && !found) { // if correct language item not found, use English
				datList.push_back(list[2]);
			}
		}
		file.close(); // exit
	}
	else // gracefuly exit
		cerr << "Failed to open " << file.fileName().toStdString() << " Error code: " << file.error() << endl;
}

/*
 * returns a QString with all the correct types of a certain pokemon
 */
QString Pokemon::getTypeNames() {
	QFile types("app/native/assets/data/type_names.csv");
	populateDataList(m_pokemon_type_names, types); // populate list with correct types in correct language
	if(numTypes() == 1)
		return m_pokemon_type_names[m_type_id1.toInt()-1];
	else if (numTypes() == 2)
		return m_pokemon_type_names[m_type_id1.toInt()-1] + ", " + m_pokemon_type_names[m_type_id2.toInt()-1];
	else
		return m_pokemon_type_names[m_type_id1.toInt()-1] + ", " + m_pokemon_type_names[m_type_id2.toInt()-1] + ", " + m_pokemon_type_names[m_type_id3.toInt()-1];
}

/*
 * similar to getTypeNames, returns QString of abilities in the correct language
 */
QString Pokemon::getAbilityNames() {
	QFile abilities("app/native/assets/data/ability_names.csv");
	populateDataList(m_pokemon_ability_names, abilities);// stores abilities in vector member variable
	if(numAbilities() == 1)
		return m_pokemon_ability_names[m_a_1.toInt()-1];
	else if (numAbilities() == 2)
		return m_pokemon_ability_names[m_a_1.toInt()-1] + ", " + m_pokemon_ability_names[m_a_2.toInt()-1];
	else
		return m_pokemon_ability_names[m_a_1.toInt()-1] + ", " + m_pokemon_ability_names[m_a_2.toInt()-1] + ", " + m_pokemon_ability_names[m_a_3.toInt()-1];
}
