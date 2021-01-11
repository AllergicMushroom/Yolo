# Yolo! pour "Youpi! Outils Logiciels pour l'Optimisation!"

Ce projet est une impl�mentation des concepts pr�sent�s dans l'UE "Outils logiciels pour l'Optimisation", donn� � l'Universit� de Bordeaux.

## Comment g�n�rer le projet

Le projet peut �tre g�n�r� de deux mani�res diff�rentes

### Le bon vieux CMake

```bash
mkdir build
cd build
cmake ..
```

Vous avez besoin de CMake 3.8 ou ult�rieur pour que la g�n�ration fonctionne. Ceci est dans le but d'utiliser C++17.

### Premake

Pour g�n�rer avec Premake, nous fournissons le script Lua ainsi que des scripts pour Windows et Linux qui g�n�rent respectivement des projets Visual Studio 2019 et Makefile.

Si vous voulez un autre g�n�rateur:

```bash
premake5 [g�n�rateur]
```

En se r�f�rant � la documentation de Premake pour voir la liste des g�n�rateurs.