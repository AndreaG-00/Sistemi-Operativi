#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0 | 1 | 2) echo error: numero parametri errato
		exit 1;;
	*) echo OK;;
esac

#controllo primi N parametri (nomi assoluti)
num=1
params=

for i
do 
	if test $num -ne $#
	then 
		case $i in 
			/*) if test ! -d $i -o ! -x $i
				then 
					echo $i non directory o non traversabile
					exit 2
				fi;;
			*) echo error: non nome assoluto
				exit 3;;
		esac
		params="$params $i"
	else
		S=$i
		case $S in 
			*/*) echo error: non relativo semplice
				exit 4;;
			*) echo OK;;
		esac
	fi
	num=`expr $num + 1`
done

#settiamo lqa variabile PATH
PATH=`pwd`:PATH
export PATH
> /tmp/conta$$

for i in $params
do 
	echo fase per $i
	FCR.sh $i $S /tmp/conta$$
done

echo il numero dei file locali che sono stati trovati sono = `wc -l < /tmp/conta$$`
c=0
for i in `cat /tmp/conta$$`
do 
	c=`expr $c + 1`
	if test `expr $c % 2` -eq 1
	then 
		echo elemento dispari, lunghezza = $i
	else
		echo elemento pari, nome assoluto del file $i
		echo -n "ordinare file (si, yes)? "
		read RISPOSTA
		case $RISPOSTA in 
			S* | s* | y* | Y*) echo file ordinato
				sort -f $i
				echo file ordinato inverso
				sort -r $i;;
			*) echo non ordinato ;;
		esac
	fi
done

rm /tmp/conta$$


