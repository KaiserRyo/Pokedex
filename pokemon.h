/*
 * pokemon.h
 *
 *  Created on: 2013-11-20
 *      Author: Eric
 */

#ifndef POKEMON_H_
#define POKEMON_H_

#include <QString>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <iostream>
using std::cerr;
using std::endl;

class Pokemon {
public:
	Pokemon();

	virtual ~Pokemon();

	const QString getHeight() const;

	void setHeight(const QString height);

	const QString getId() const;

	void setId(const QString id);

	const QString getIdentifier() const;

	void setIdentifier(const QString identifier);

	const QString getIsDefault() const;

	void setIsDefault(const QString isDefault);

	int getLocalLanguageId() const;

	void setLocalLanguageId(const int localLanguageId);

	const QString getSpeciesId() const;

	void setSpeciesId(const QString speciesId);

	const QString getWeight() const;

	void setWeight(const QString weight);

	const QString getTypeId1() const;

	void setTypeId1(const QString typeId1);

	const QString getTypeId2() const;

	void setTypeId2(const QString typeId2);

	const QString getTypeId3() const;

	void setTypeId3(const QString typeId3);

	bool isType(const QString type);

	int numTypes();

	QString getTypeNames();

	const QString getAccuracy() const;

	void setAccuracy(const QString accuracy);

	const QString getAttack() const;

	void setAttack(const QString attack);

	const QString getDefence() const;

	void setDefence(const QString defense);

	const QString gethp() const;

	void sethp(const QString hp);

	const QString getSpecialAttack() const;

	void setSpecialAttack(const QString specialAttack);

	const QString getSpecialDefence() const;

	void setSpecialDefence(const QString specialDefence);

	const QString getSpeed() const;

	void setSpeed(const QString speed);

	const QString getDescription() const;

	void setDescription(const QString description);

	const QString getTotalStats() const;

	QString getAbilityNames();

	const QString getVersionEntrance() const;

	void setVersionEntrance(const QString versionEntrance);

	const QString getA1() const;

	void setA1(const QString a1);

	const QString getA2() const;

	void setA2(const QString a2);

	const QString getA3() const;

	void setA3(const QString a3);

	int numAbilities();


private:
	int m_local_language_id; // holds language so that type and ability names are added correctly

	// types
	QString m_type_id1;
	QString m_type_id2;
	QString m_type_id3;

	// abilities (can have 1-3)
	QString m_a_1;
	QString m_a_2;
	QString m_a_3;

	//from pokemon.csv
	QString m_id;
	QString m_identifier;
	QString m_species_id;
	QString m_height;
	QString m_weight;

	// stats
	QString m_hp;
	QString m_attack;
	QString m_defence;
	QString m_special_attack;
	QString m_special_defence;
	QString m_speed;
	QString m_total_stats;
	QString m_description;
	QString m_version_entrance;

	/* Name Lists */
	void populateDataList(std::vector<QString> & datList, QFile & file);
	std::vector<QString> m_pokemon_type_names; // vector to hold possible pokemon type names (depending on language)
	std::vector<QString> m_pokemon_ability_names; // holds correct ability names depending on language
};

#endif /* POKEMON_H_ */
