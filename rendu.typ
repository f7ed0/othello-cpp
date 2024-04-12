#import "@preview/lovelace:0.2.0": *
#show: setup-lovelace

#align(center + top, text("Compte Rendu de Traveaux Pratiques - Fondement de l'IA"))
#line(length: 100%)
\ \ \
#align(center, text("Stratégies d'IA pour Othello", weight: "bold", size: 24pt))
\
#figure(image("img/cover_image.png"))
\ \
#align(center, text("SAUSSE Sylvain - LAFONTAINE Robin\n4A ICy",size: 14pt))

#align(center + bottom, text("Fondement de l'IA\n R. Mandiau\n"+datetime.today().display("[day]-[month]-[year]"), size: 14pt))

#pagebreak()

#set page(numbering: "1")
#counter(page).update(1)

#outline(
  title: "Table des Matières",
  depth: 3,
  indent: true,
)

#pagebreak()
#outline(
  title: "Table des Figures",
  target: figure.where(kind: figure)
)

#pagebreak(weak: true)

#set heading(numbering:  "I.A.1.a.", supplement: [Partie])

#show heading: it => {
    if (it.level > 4){
        block(it.body)
    } else {
        block(counter(heading).display() + " " + it.body)
    }
}

= Développement du cadre de travail

Lors du developpement d'une IA il est important d'avoir à sa disposition des outils et un cadre de travail définit afin de pouvoir se concentrer sur le fonctionnement de notre IA et ses heuristiques.

Nous avons décidé d'utiliser le language C++ lors de ce TP pour sa rapiditée d'execution (via la compilation) et son archetype orienté objet qui permet d'encadrer les interactions avec les differentes structures de données.

Cette partie du rapport detaillera les differents algorithme et structures mis en place lors de ce projet pour faciliter l'interaction des IA avec le jeu.

== Logique du jeu 

// TODO : Ajouter regles du jeu / gameplay

Chaque joueur, noir et blanc, pose l'un après l'autre un pion de sa couleur sur le plateau de jeu dit « l'othellier » selon des règles précises. Le jeu s'arrête quand les deux joueurs ne peuvent plus poser de pion. On compte alors le nombre de pions. Le joueur ayant le plus grand nombre de pions de sa couleur sur l'othellier a gagné.

Othello etant un jeu simple, notre implémentation se base sur 3 structures detaillées ci dessous. Toutes ces stuctures et methodes font partie du namespace `othello`.

=== Enumeration `othello::pawn`

L'enumeration `pawn` est le type de base utilisé dans la représentation du plateau de jeu.



#figure(
  rect[
  ```cpp
  typedef enum : unsigned short {
      empty = 0,
      black = 1,
      white = 2,
  } pawn;
  ```
  ],
  supplement: "Figure",
  kind: figure,
  caption : [définition de `othello::pawn`],
)

Cette représentation est basée sur le type `unsigned short` qui est codé sur 16 bits. ce choix permet d'avoir une représentation compacte (comparé au type `int` ou `unsigned int`) tout en ayant la capacité d'etre utilisable en tant qu'index pour les tableaux par exemple.

=== Classe `othello::Board`

La classe `Board` est le coeur même du jeu. Elle contient la représentation du plateau de jeu : un tableau de 64 `othello::pawn` (8x8 cases). le tableau est alloué dynamiquement à la création de l'objet et liberée à sa destruction.

#figure(
  rect[
  ```cpp
  class Board {
    private :
      // 64 unsigned short allocated array : contains the state of the board
      pawn *cases;
  }
  ```
  ],
  supplement: "Figure",
  kind: figure,
  caption : [`othello::Board.cases` : représentation du plateau],
)

Cette classe gère la logique du jeu au niveau du placement des jetons, mais pas au niveau du déroulement. Par exemple, cette classe est capable de verifier qu'un coup est valide lorsqu'on demande de l'effectuer et donc de l'annuler mais pas de savoir si c'est bien au joueur en question de jouer.

`othello::Board.case` est un attribut privé, il n'est donc modifiable que par des méthodes définies dans la classe `ohtello::Board`. Cela permet de s'assurer que la structure est manipulé seulement de la manière prévue dans les méthodes d'interactions.

=== Methodes d'interactions

Les methodes d'interactions sont les methodes publiques de la classe `othello::Board`.

==== Systèmes de coordonnées

Deux système de coordonées sont utilisable pour intéragir avec le plateau :

1. L'index dans la stucture de donnée, avec lesquel on peut facilement retrouver les valeur lignes - colonnes :

#figure(
  [_index_ $=$ _colonnes_ $+$ (_lignes_ $times 8$) $<==>$ (_lignes_ $equiv$ _index_ [8]  $and$ _colonnes_ $=$ _index_ $div 8$)]
)
Cette notation correspondant à l'index de la case dans le tableau de pion, elle est rapide mais difficile à comprendre pour une personne.
  
2. Les coordonées plus classiques composée d'une lettre et d'un chiffre (ex. "d7") qui sont utilisé pour l'entrée utilisateur.

===== Verifier si un coup est valide

La verification d'un coup valide se fait via la fonction `canPlaceHere` de la classe `Board`

#figure(
  rect[
    ```cpp
    direction canPlaceHere(int index, pawn team) const
    ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Fonction canPlaceHere]
)

La fonction prend en paramètre la coordonée où l'on souhaite placer le pion sous forme d'index, et l'équipe qui souhaite le placer. Après avoir verifié pour des raison évidente d'impossibilité (une pièce est deja présente sur la case par exemple) la fonction va ensuite verifier pour chaque direction si il est possible de le placer et ajoute au masque binaire `direction` la valeur correspondant à cette dernière.

#figure(
  rect[
    ```cpp
    typedef enum : direction {
        NODIR = 0,
        TOP = 1,
        BOTTOM = 2,
        LEFT = 4,
        RIGHT = 8,
        DTL = 16,
        DTR = 32,
        DBL = 64,
        DBR = 128,
    } dirs;
    ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Enumération dirs]
)

Par exemple pour le coup suivant (les noirs essaient de placer un pion sur le point rouge), le retour de la fonction canPlaceHere sera de :

#align(
[_gauche_ $+$ _haut_ $+$ _diagonale haut-gauche_ = $4 + 1 + 16 = 21$],
center
)



#figure(
  rect(image("img/move.png"),height: 20%),
  supplement: "Figure",
  kind: figure,
  caption: [Exemple de coup joué]
)

===== Lister les coups jouables

Le listage des coups jouables d'un joueur se fait à l'aide de la methode `listAllPlay` de la classe `Board`

#figure(
  rect[
    ```cpp
    std::vector<int> listAllPlay(pawn team) const;
    ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Fonction canPlaceHere]
)

Le listage des coups jouable est assez simple, on itère sur toute les cases du plateau et on teste la possibilitée de placement sur toute les cases du plateau avec la fonction `canPlaceHere`. La fonction retourne ensuite la liste des coups jouables sous forme de `vecteur` de coordonée sous forme d'index.

Durant no sessions de benchmarking nous avons noté un temps d'execution de $3 plus.minus 1\µs$ ce qui va être important par la suite puisque cette fonction va etre appelée frequement par les IAs

== Affichage et benchmarking

Notre jeu d'othello propose 2 interface pour jouer ou faire jouer des IA. Ces interface sont selectionnable et paramétrable via des arguments lors du lancement de l'application.

=== Affichage avec interface graphique

L'affichage avec iterface graphique utilise SDL2 pour permettre aux utilisateurs humain d'interagir plus naturellement avec le jeu. Pour jouer, il suffit de cliquer sur la case, les coups possibles sont affiché directement sur le board via un carré au centre des cases.

#figure(
  rect[
    #image("img/gui.png")
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Interface utilisateur du jeu]
)

Cette interface a pour but de permettre a un humain de jouer au jeu dans des condition normale de jeu. Elle n'a pas grande valeur lors du benchmarking.

=== Affichage CLI

L'affichage CLI est l'affichage pour le benchmarking. Il admet des paramètre supplémentaire qui permettent de tester les IA de manière plus précise, nottament en permettant de jouer plusieurs partie à la suite afin de faire des analyse statistique.

#figure(
  rect[
    ```
                          othello by 0xf7ed0 & Kappaccino      
  ------------------------------------------------------------------------------------

    othello [--gui | --no-gui] [--IA1 <name>=<depth>] [--IA2 <name>=<depth>] [args] 

  ------------------------------------------------------------------------------------

    --gui                     enable the GUI to play
    --no-gui                  play in CLI (default) 
    --progress                show real-time game progres (CLI only) 
    --no-result               don't show game results (CLI only)
    --gamecount <count>       set the number of games to play
    --IA1 <name>=<depth>      set player1 (black) to AI with depth
    --IA2 <name>=<depth>      set player2 (white) to AI with depth
    ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [`--help` de l'application]
)

Une fois les parties jouées, un récapitulatif des parties et des temps de jeu est affiché afin de pouvoir facilement evaluer les performances des joueurs. 

#figure(
  rect[
    ```
    ========================== RÉCAPITULATIFS DES SCORES ==========================
    10 match(s) gagné par les Noirs.
    0 match(s) gagné par les Blanc.
    0 match(s) nul(s).
    ===================== RÉCAPITULATIFS DES AIRES PAR MATCH ======================
    80.7812 % du terrain occupé par les Noirs en moyenne
    19.0625 % du terrain occupé par les Blanc en moyenne
    0.15625 % du terrain non-occupé en moyenne
    ==================== RÉCAPITULATIFS DES TEMPS D'EXECUTION =====================
    IA1 (alphabeta_mixte=6) mean calculation time per move : 66.6095 ms (316 moves played)
    IA2 (random) mean calculation time per move : 0.00325442 ms (283 moves played)
    ===============================================================================
    ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Récapitulatif de fin de session]
)





#pagebreak()

= Développement des IA

== Intégration des IA dans le cadre de travail

=== Classe abstraite `IA::Interface` et son intégration

Pour permettre aux IA de jouer dans l'application, une classe abstraite nommée `IA::Interface` a été créée. Elle intègre toute les fonctions et attribut necessaire pour la communication avec l'interface ( CLI ou Graphique ) ainsi que des outils pour les IA (tel que la matrice de coup des cases et la fonction `switchTeam`). Il suffit alors de l'heriter pour créer son IA.

#figure(
  rect[
    ```cpp
    class IAInterface {
        protected :

            virtual int heuristics(const othello::Board& current_board, const othello::pawn team ) = 0;
        
        public :

            static const int payoff_matrix[64];
            
            virtual int makeAMove(const othello::Board& current_board,othello::pawn team) = 0;

            virtual void resetAI() {};

            static IAInterface* selectByName(std::string name);

            static othello::pawn switchTeam(othello::pawn p);
    };
    ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Récapitulatif de fin de session]
)

La methode statique `selectByName` permet de generer une IA (classe fille de Interface donc) à partir de texte afin de pouvoir selectionner une IA en particulier facilement.

#figure(
  rect[
    ```cpp
    IAInterface* IAInterface::selectByName(std::string name) {
      std::vector <std::string> tokens;
      std::string intermediate; std::stringstream stream(name);
      while(getline(stream, intermediate, '=')) tokens.push_back(intermediate);
      if(tokens[0] == "random") return (new Random());
      else if(tokens[0] == "minmax"){
          return (new MinMax(std::stoi(tokens[1])));
      } else if(tokens[0] == "alphabeta"){
          return (new AlphaBeta(std::stoi(tokens[1])));
      } else if(tokens[0] == "alphabeta_absolute"){
          return (new AlphaBeta_Absolute(std::stoi(tokens[1])));
      } else if(tokens[0] == "alphabeta_mobility"){
          return (new AlphaBeta_Mobility(std::stoi(tokens[1])));
      } else if(tokens[0] == "alphabeta_mixte"){
          return (new AlphaBeta_Mixte(std::stoi(tokens[1])));
      } else {
          std::cout << "No IA named " << name << "." << std::endl;
          throw errors::OutOfBoundError();
      }
    }
    ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Selection de l'IA via `IAInterface::selectByName`]
)

Cette classe est alors utilisé pour stocker et appeler les IA lors du déroulement de la partie, qu'importe la classe fille d'IA utilisée.

Les IA sont alors appelée par la methode `makeAMove` par l'application pour recuperer leur coup.

#figure(
  rect[
    ```cpp
    // La fonction IA play est une fonction statique qui appelle makeAMove
    // Les methodes de classe ne peuvent pas être utilisé pour un thread tel quel
    if(!gameEnded && ((current_player == othello::pawn::black && isPlayer1AI) || (current_player == othello::pawn::white && isPlayer2AI))) {
        if(!IA_launched) {
            IA_thinking = true;
            IA_launched = true;
            if(this->current_player == othello::pawn::black){
                this->IAthread = new std::thread(Window::IAPlay,this->IA1,*(this->board),this->current_player,&IA_thinking,&IA_result);
            } else {
                this->IAthread = new std::thread(Window::IAPlay,this->IA2,*(this->board),this->current_player,&IA_thinking,&IA_result);
            }
        } else {
            if(!IA_thinking) {
                if(this->IAthread->joinable()) {
                    this->IAthread->join();
                    delete this->IAthread;
                }
                IA_launched = false;
                this->placePawn(IA_result);
            }
        }
    }
  ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Execution des coups d'une IA en mode GUI]
)

#pagebreak()

== Minmax
Avant de procéder à l'implémentation de l'algorithme de Minmax il était judicieux de bien comprendre ce dernier.
Nous avons donc commencé par mettre en place le pseudo-code #footnote[https://en.wikipedia.org/wiki/Minmax] de l'algorithme .

#figure(
  algorithm(
    caption: [Minmax],
    pseudocode-list([
    - *entrées:* _nœud_, _profondeur_, _joueurMax_
    - *sortie:* _entier_
    + *debut*
      + *si* _profondeur_ $= 0$ *ou* estTerminal(_nœud_) *alors*
        + *retourner* heuristique(_nœud_)
      + *si* _joueurMax_ *alors*
        + _valeur_ $ <- -infinity$
        + *pour chaque* _enfant_ de _nœud_ *faire*
          + _valeur_ $<- $ max(v, minmax(_enfant_, profondeur$-1$, Faux))
      + *sinon*
        + _valeur_ $<- +infinity$
        + *pour chaque* _enfant_ de _nœud_ *faire*
          + _valeur_ $<- $ min(v, minmax(_enfant_, _profondeur_ $-$ $1$, Vrai))
      + *retourner* _valeur_
    ], indentation-guide-stroke: .5pt
  )
),
  supplement: "Figure",
  kind: figure,
  caption : [Pseudocode Minmax]
)

En suite, nous avons implémenté l'algorithme en C++ en utilisant la classe `othello::Board` pour représenter le plateau de jeu et `othello:pawn` pour représenter le joueur qu'on veut maximiser.

De plus, le code differe légerement du pseudo-code dans l'implémentation du joueur a maximiser. En effet, dans le pseudo-code, le joueur a maximiser est le joueur qui maximise la valeur de la fonction heuristique, alors que dans notre implémentation, le joueur a maximiser est le joueur qui joue le coup. Cela permet de partager la fonction avec les deux joueur via la fonction `MinMax::switchTeam(current_player)`.

#figure(
  rect[
```cpp
if (player == team){
        value = INT32_MIN;
        for (int i=0 ; i < possibility ; i++) {
            othello::Board* next_move = new othello::Board(current_board);
            if(next_move->placePawn(choices.at(i),player) == 0) {
                throw -1;
            }
            value = std::max(value,this->minmax(*next_move,MinMax::switchTeam(player),depth-1,team));  
            delete next_move;     
        }
    } else {
        value = INT32_MAX;
        //...
    }
    return value;
```
  ],
  supplement: "Figure",
  kind: figure,
  caption : [`MinMax::minmax()` : Implémentation min/max du joueur],
)

#pagebreak()

== Negamax

L'algorithme de Negamax est une simplification de l'algorithme Minmax. En effet, Negamax est une version simplifiée de Minmax où les valeurs des nœuds sont toujours positives. Cela permet de simplifier l'implémentation de l'algorithme, montré par le pseudocode #footnote[https://en.wikipedia.org/wiki/Negamax].

#figure(
  algorithm(
    caption: [Negamax],
    pseudocode-list([
      - *Entrées:* _nœud_, _profondeur_, _couleur_
      - *Sortie:* _entier_
      + *debut*
        + *si* _profondeur_ $= 0$ *ou* estTerminal(_nœud_) *alors*
          + *retourner* _couleur_ $times$ heuristique(_nœud_)
        + _valeur_ $ <- -infinity$
        + *pour chaque* _enfant_ de _nœud_ *faire*
          + _valeur_ $<- $ max(v, $-$negamax(_enfant_, _profondeur_ $-$ $1$, $-$_couleur_))
        + *retourner* _valeur_
    ], indentation-guide-stroke: .5pt
  )
), 
  supplement: "Figure",
  kind: figure,
  caption : [Pseudocode Negamax]
)


Afin de simplifier l'algorithme la notion de couleur est introduite. La couleur est un entier qui vaut 1 si le joueur est le joueur maximisant et -1 si c'est le joueur minimisant.

La couleur est utilisé pour inverser la valeur de la fonction heuristique si le joueur est le joueur minimisant.

#figure(
  rect[
  ```cpp
  int color = (player == team) ? 1 : -1;
    
    if (depth == 0) {
        return color*this->heuristics(current_board,team);
    }
    //...
  ```
  ],
  supplement: "Figure",
  kind: figure,
  caption : [`NegaMax::negamax()` : Implémentation de la couleur],
)
=======
#pagebreak(weak: true)

== Alpha-Beta

L'algorithme Alpha-Beta ($alpha - beta$) est une amélioration de l'algorithme Minmax qui permet de réduire le nombre de nœuds explorés en éliminant les branches inutiles.

L'algorithme Alpha-Beta possède plusieurs variantes qui permettent d'améliorer les performances de l'algorithme. 

- *Alpha-Beta classique :* L'algorithme Alpha-Beta classique est l'algorithme de base. 

- *Alpha-Beta absolue :* L'algorithme Alpha-Beta absolue est une variante de l'algorithme Alpha-Beta qui utilise une heuristique absolue. L'heuristique absolue est une heuristique qui prend en compte le nombre de pions de chaque joueur sur le plateau.

- *Alpha-Beta de mobilité :* L'algorithme Alpha-Beta de mobilité est une variante de l'algorithme Alpha-Beta qui utilise une heuristique de mobilité. L'heuristique de mobilité est une heuristique qui prend en compte le nombre de coups possibles pour chaque joueur.

- *Alpha-Beta mixte :* L'algorithme Alpha-Beta mixte est une variante de l'algorithme Alpha-Beta qui utilise une heuristique mixte. L'heuristique mixte est une combinaison de l'heuristique absolue et de l'heuristique de mobilité.

Nous avons implémenté ces variantes de l'algorithme Alpha-Beta pour comparer leurs performances.

= Analyse des heuristiques

== Heritage et changement d'heuristiques


= Analyse des Résultats

#figure(
  rect[
    ```
    =========================== RÉCAPITULATIFS DES SCORES ===========================
    20 match(s) gagné par les Noirs.
    0 match(s) gagné par les Blanc.
    0 match(s) nul(s).
    ====================== RÉCAPITULATIFS DES AIRES PAR MATCH =======================
    62.9588 % du terrain occupé par les Noirs en moyenne
    36.1719 % du terrain occupé par les Blanc en moyenne
    0.859375 % du terrain non-occupé en moyenne
    ===================== RÉCAPITULATIFS DES TEMPS D'EXECUTION ======================
    IA1 (alphabeta=10) mean calculation time per move : 16573.5 ms (606 moves played)
    IA2 (random) mean calculation time per move : 0.00394168 ms (583 moves played)
    =================================================================================
    ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Résultats de Alpha-Beta]
)

#figure(
  rect[
    ```
    ============================ RÉCAPITULATIFS DES SCORES ===========================
    19 match(s) gagné par les Noirs.
    1 match(s) gagné par les Blanc.
    0 match(s) nul(s).
    ======================= RÉCAPITULATIFS DES AIRES PAR MATCH =======================
    82.3438 % du terrain occupé par les Noirs en moyenne
    15.625 % du terrain occupé par les Blanc en moyenne
    2.03125 % du terrain non-occupé en moyenne
    ====================== RÉCAPITULATIFS DES TEMPS D'EXECUTION ======================
    IA1 (alphabeta_absolute=10) mean calculation time per move : 1205.58 ms (629 moves played)
    IA2 (random) mean calculation time per move : 0.00413578 ms (545 moves played)
    ==================================================================================
    ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Résultats de Alpha-Beta absolue]
)

#figure(
  rect[
    ```
    ============================ RÉCAPITULATIFS DES SCORES ===========================
    20 match(s) gagné par les Noirs.
    0 match(s) gagné par les Blanc.
    0 match(s) nul(s).
    ======================= RÉCAPITULATIFS DES AIRES PAR MATCH =======================
    83.3594 % du terrain occupé par les Noirs en moyenne
    11.1719 % du terrain occupé par les Blanc en moyenne
    5.46875 % du terrain non-occupé en moyenne
    ====================== RÉCAPITULATIFS DES TEMPS D'EXECUTION ======================
    IA1 (alphabeta_mobility=10) mean calculation time per move : 8838.38 ms (638 moves played)
    IA2 (random) mean calculation time per move : 0.0035 ms (492 moves played)
    ==================================================================================
    ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Résultats de Alpha-Beta mobilité]
)

#figure(
  rect[
    ```
    ========================== RÉCAPITULATIFS DES SCORES ==========================
    20 match(s) gagné par les Noirs.
    0 match(s) gagné par les Blanc.
    0 match(s) nul(s).
    ===================== RÉCAPITULATIFS DES AIRES PAR MATCH ======================
    96.7188 % du terrain occupé par les Noirs en moyenne
    2.42188 % du terrain occupé par les Blanc en moyenne
    0.859375 % du terrain non-occupé en moyenne
    ==================== RÉCAPITULATIFS DES TEMPS D'EXECUTION =====================
    IA1 (alphabeta_mixte=10) mean calculation time per move : 30848.2 ms (680 moves played)
    IA2 (random) mean calculation time per move : 0.00397053 ms (509 moves played)
    ===============================================================================
    ```
  ],
  supplement: "Figure",
  kind: figure,
  caption: [Résultats de Alpha-Beta mixte]
)

