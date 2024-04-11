# Exercice 4

## Que pouvez-vous voir dans la deuxième colonne de la sortie ?
Le résultat de la commande est 
```
-rw-r--r--. 1 cypooos cypooos    4 26 févr. 18:59 answers.md
-rw-r--r--. 1 cypooos cypooos 5202 26 févr. 13:05 td3in.copy
-rw-r--r--. 2 cypooos cypooos 5202 26 févr. 00:34 td3in.hl
lrwxrwxrwx. 1 cypooos cypooos    9 26 févr. 18:58 td3in.sl -> td3in.txt
-rw-r--r--. 2 cypooos cypooos 5202 26 févr. 00:34 td3in.txt
```
On peut voir que le RC (Reference counter) du fichier `td3in.txt` = `td3in.hl` est de 2.

## Pourquoi les fichiers n'ont-ils pas tous la même taille ?
Le soft link n'a que comme taille la taille nécéssaire pour indiquer le lien vers le fichier, il n'a pas en réalité le contenu du fichier dedans.

## Pourquoi, bien que le fichier td3in.sl"ait une taille différente des autres, il a la même taille que le fichier original ? différente des autres, il a le même contenu ?
La question ne fait pas vraiment de sens je crois, mais comme c'est un soft link, quand `diff` l'ouvre, il a accès au même contenu interne, et donc ne voit pas de différence.

## Comment s'attend-on à ce que les informations taille du fichier et la date de dernière modification ? Pourquoi ?
On s'attend à ce que seul les deux références `td3in.txt` et `td3in.hl` changent de taille et de date de modification. Cela est du au fait que ces informations sont stoqué dans le inode du fichier, donc que le softlink ne se met pas à jour (il a son propre inode).

## Quelles sont les modifications attendues du contenu des fichiers ? Pourquoi ?
Seul le `td3in.copy` n'est pas modifié en contenu car il est un copy qui n'a rien à voir. Le contenu du softlink `td3in.sl` est modifié car c'est en rélité derrière le même contenu.

## Que pouvez-vous voir maintenant à partir de la sortie de la commande ls -l ?
Le softlink n'est plus valide (car le path du fichier à changer), mais le hardlink l'est toujours.

## Pourquoi est-ce que j'obtiens un message d'erreur dans le deuxième cas ?
Car le softlink n'est plus à jour, le path vers le vrai fichier est incorrect.

## Ce qui est maintenant attendu comme étant le contenu du fichier ltd3in.sl ? Pourquoi ?
Maintenant, car le soft link pointe vers le fichier `./td3in.txt`, le contenu c'est mis à jour pour être le même que ce nouveau fichier.