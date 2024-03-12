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

#outline(
  title: "Sommaire",
  depth: 4,
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

== La logique du jeu 

Othello etant un jeu simple, notre implémentation se base sur 3 structures detaillées ci dessous. Toutes ces stuctures et methodes font partie du namespace `othello`.

=== L'enumeration `othello::pawn`

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

=== La classe `othello::Board`

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

==== Methodes d'interactions

Les methodes d'interactions sont les methodes publiques de la classe `othello::Board`.

===== Systèmes de coordonnées

Deux système de coordonées sont utilisable pour intéragir avec le plateau :

1. L'index dans la stucture de donnée, avec lesquel on peut facilement retrouver les valeur lignes - colonnes :
#figure(
  $"index" = "colonnes" + ("lignes" times 8) <==> ( "lignes" equiv "index" [8]  and "colonnes" = "index" div 8 )$
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
[$$gauche plus haut + diagonale haut-gauche = 4 + 1 + 16 = 21$$],
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

== L'affichage et le benchmarking


#pagebreak()

= Developpement des IA

== MinMax
Avant de procéder au développement de l'IA il était judicieux de bien comprendre l'algorithme de ce dernier.
Nous avons donc fait des recherches 

#figure(
  rect[
  ```py
  function minimax(node, depth, maximizingPlayer) is
    if depth = 0 or node is a terminal node then
        return the heuristic value of node
    if maximizingPlayer then
        value := −∞
        for each child of node do
            value := max(value, minimax(child, depth - 1, FALSE))
    else (* minimizing player *)
        value := +∞
        for each child of node do
            value := min(value, minimax(child, depth - 1, TRUE))
    return value
  ```
  ],
  supplement: "Figure",
  kind: figure,
  caption : [Algorithme MinMax],
)

#figure(rect[
```
313 match(s) gagné par les Noirs.
648 match(s) gagné par les Blanc.
39 match(s) nul(s).
IA1 (random) mean calculation time per move : 0.00389221 ms (29919 moves played)
IA2 (minmax3) mean calculation time per move : 0.856059 ms (29983 moves played)
```
], 
  supplement: "Figure",
  kind: figure,
  caption: "Résultats de Random v MinMax sur 1000 parties"
)

== Negamax

