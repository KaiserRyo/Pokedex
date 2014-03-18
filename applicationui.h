#ifndef ApplicationUI_H_
#define ApplicationUI_H_

#include <QObject>
#include <bb/cascades/AbstractPane>
#include "pokemon.h"

namespace bb
{
    namespace cascades
    {
        class Application;
    }
}

/*!
 * @brief Application object
 *
 *
 */
class PokemonList; // forward declaration to avoid including header

class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    // callables from QML
    Q_INVOKABLE void typeSelected(int type);
    Q_INVOKABLE void languageSelected(int language);
    bb::cascades::AbstractPane* getRoot(); // returns root object
    void init(); // initialises GUI

    virtual ~ApplicationUI() { }
private:
    PokemonList *m_pokemonList; // pointer to pokemoneList object
    bb::cascades::AbstractPane *m_root;
    QString m_current_language; // current language to display info in
};

#endif /* ApplicationUI_H_ */
