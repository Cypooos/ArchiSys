# Exercice 1

a. J'utilliserai un groupe, contenant les 4990 utillisateurs, qui aurai les droits de lecture sur le fichier. Les autres n'aurai aucun droits.
b. Au lieu de stoquer 1 groupe contenant 4990 utillisateur, on pourrait fait un groupe avec les 10 utillisateur n'ayant pas le droit d'acceder au fichier, et tout le monde aurai le droit de le lire SAUF ce groupe. Malheuresement sur Unix on ne peux pas faire de conditionnel négatif.

# Exercice 2

=== ACL ===
**Avantages:**
 - On peut faire du cas par cas pour chaque fichier, et à l'aide des dossiers généralisé ça à une série de fichiers.
 - Un root peut savoir juste avec la référence d'un fichier qui à accès à quoi.
**Inconvégniants:**
 - Dans les cas ou on a beaucoup d'utillisateurs et des fichiers avec des droits différents de partout, il faut stoquer dans cahque fichier les droit d'accès pour tout les utillisateurs

=== UCL ===
**Avantages:**
 - On peut facilement, en ayant un utillisateur en tête, savoir quels sont ces droits, et on n'a pas à scanner tout le système pour s'assurer qu'il n'a pas accès à des choses dangereuses.
 - On peut faire du cas par cas pour chaque utillisateur.
**Inconvégniants:**
 - S'il y a beaucoup de fichiers, pour chaque utillisateur il faut stoquer la liste des droits d'accès.
 - Il est difficile de généraliser des accès partagé à des groupes d'utillisateurs: si on crée un fichier, il faut aller dans les paramètres de chaque utillisateurs pour indiquer ses droits.

# Exercice 3

Pour chaque opération, on fait les 3 cas: Cas contigue, cas concaténé, cas indexé

1. On ajoute un bloc au début    : 201, 1, 1
2. On ajoute un bloc au milieu   : 101, 51, 1
3. On ajoute un bloc à la fin    : 1, 1, 1
4. On supprime un bloc au début  : 0, 2, 0
5. On supprime un bloc au milieu : 98, 99, 0 
6. On supprime un bloc à la fin  : 0, 100, 0

# Exercice 4

Il y aura des problèmes de cohérence avec la donnée: une action aotimique d'écriture dans un fichier peut ne plus l'être car deux processus peuvent écrire sur ce qu'ils pensent être des disques différents.

# Exercice 5

On peut faire comme linux, mais dans une majorié des cas l'indexé est mieux.

# Exercice 6

Si on les lits souvent, le cout de la mise dans le cache swap sera compenser par le gain à la lecture. 

