#!/bin/sh
#file comandi ricorsivo

#ci spostiamo sulla directory giusta
cd $1
#variabile per vedere se si trova la directory richiesta
trovato=false

for F in *
do 
	if test -f $F -a -r $F
	then 
		if grep $2 $F > /dev/null 2>&1
		then 
			trovato=true
			echo `pwd`/$F >> $3
		fi
	fi
done

#controlliamo che sia stato trovato almeno una directory 
if test $trovato = true
then 
	echo trovato direttorio `pwd`
fi

#ricorsione
for i in *
do
	if test -d $i -a -x $i
	then 
		$0 `pwd`/$i $2 $3
	fi
done
