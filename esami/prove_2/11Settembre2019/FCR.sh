#!/bin/sh
#file comandi ricorsivo

cd $1

#file pari e file dispari trovati
file_pari=
file_dispari=

#lunghezza in caratteri
LC=

for F in *
do
	if test -f $F
	then 
		LC=`wc -c < $F`
		if test $LC -eq $3
		then 
			cont=`expr $cont + 1`
			if test `expr $cont % 2` -eq 0
			then 
				#file pari
				file_pari="$file_pari $F"
			else
				#file dispari
				file_dispari="$file_dispari $F"
			fi
		fi
	fi
done

if test $cont -eq $2
then 
	echo trovato direttorio `pwd`
	echo invochiamo parte c per file dispari
	main $file_dispari

	echo invochiamo la parte c per i file pari
	main $file_pari

for i in *
do 
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $2 $3
	fi
done
