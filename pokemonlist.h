/*
 * PokemonList.h
 *
 *  Created on: 2013-11-02
 *      Author: Jagath
 */

#ifndef POKEMONLIST_H_
#define POKEMONLIST_H_

#include <bb/cascades/DataModel>
#include "pokemon.h"

class PokemonList: public bb::cascades::DataModel {
	Q_OBJECT
public:
	PokemonList(QObject *parent = 0);

	/* public functions */
	virtual int childCount(const QVariantList& indexPath);
	virtual bool hasChildren(const QVariantList& indexPath);
	virtual QVariant data(const QVariantList& indexPath);

	void initPokemonList(); // creates all necessary pokemon objects
	void populateDataList(std::vector<QString> & datList, QFile & file); // function used for parsing text files
	void populateDataListOfLists(std::vector<QStringList> & datList, QFile & file);
	void setLanguage(int lang); // enables application UI to change the language
	void setType(int type); // enables applicationUI to change the type
	unsigned getInitCount(); // returns number of times init has been call (reduces calls to init at app start

	/* public member variables */
	int m_local_language; //language to read data for
	int m_local_type; // types to read data for

	QString m_hp, m_specAttack, m_specDefense, m_attack, m_defense, m_speed; // variables to store stat names in different languages

	virtual ~PokemonList();
private:
    std::vector<Pokemon *> m_pokemon; // vector to hold pointers to pokemon objects
};
#endif /* POKEMONLIST_H_ */
