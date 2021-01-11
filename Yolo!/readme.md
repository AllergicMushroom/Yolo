# Yolo! pour "Youpi! Outils Logiciels pour l'Optimisation!"

Ce projet est une implémentation des concepts présentés dans l'UE "Outils logiciels pour l'Optimisation", donné à l'Université de Bordeaux.

## Comment générer le projet

Le projet peut être généré de deux manières différentes

### Le bon vieux CMake

```bash
mkdir build
cd build
cmake ..
```

Vous avez besoin de CMake 3.8 ou ultérieur pour que la génération fonctionne. Ceci est dans le but d'utiliser C++17.

### Premake

Pour générer avec Premake, nous fournissons le script Lua ainsi que des scripts pour Windows et Linux qui génèrent respectivement des projets Visual Studio 2019 et Makefile.

Si vous voulez un autre générateur:

```bash
premake5 [générateur]
```

En se référant à la documentation de Premake pour voir la liste des générateurs.