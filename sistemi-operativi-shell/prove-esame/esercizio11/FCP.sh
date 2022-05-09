#!/bin/sh
#file comandi principale

#controllo numero di parametri
case $# in 
	0 | 1 | 2) echo error: numero errato di parametri
		exit 1;;
	*) echo OK;;
esac

#controllo primo parametro
#numero strettamente positivo
expr $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $1 -le 0
	then 
		echo error: non positivo
		exit 2
	fi
else 
	echo error: non numero
	exit 3
fi

#salvo il parametro e faccio lo shift
X=$1
shift

#controllo secondo parametro
for i in $*
do 
	case $i in 
		/*) if test ! -d $i -o ! -x $i
			then 
				echo error: non directory o non traversabile
				exit 4
			fi;;
		*) echo error: non nome assoluto
			exit 5;;
	esac
done

#fine controlli

#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH
> /tmp/conta$$

#invochiamo il file comandi ricorsivo per ogniuna delle fasi
for i 
do 
	echo FASE PER $i
	FCR.sh $i $X /tmp/conta$$
done

echo Il numero di file totali che soddisfano la specifica = `wc -l < /tmp/conta$$`

for i in `cat /tmp/conta$$`
do 
	 echo TROVATO IL FILE $i
	 echo -n "Dammi il numero K: "
	 read K

	 #controllo su K
	 expr $K + 0 > /dev/null 2>&1
	 N1=$?
	 if test $N1 -ne 2 -o $N1 -ne 3
	 then 
		if test $K -le 0
		then 
			echo error: negativo
			rm /tmp/conta$$
			exit 6
		fi
   	 else 
	 	echo error: non numero
		rm /tmp/conta$$
		exit 7	
	 fi

	 if test $K -ge $X
	 then 
		echo error: maggiore di X
		rm /tmp/conta$$
		exit 8
	 fi

	 #riporto in stdout la K-esima linea
	 head -$K $i | tail -1
done

rm /tmp/conta$$

#fine file comandi principale

