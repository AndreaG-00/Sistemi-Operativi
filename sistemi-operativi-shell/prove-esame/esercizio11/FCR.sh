#!/bin/sh
#file comandi ricorsivo
#il file deve contare globalmente tutti i file leggibili 
#il contenuto del file deve essere tale per cui almeno 
#X linee terminino con il carattere 't'

#parametri
#primo parametro = gerarchia
#secondo parametro = X
#terzo parametro = /tmp/conta$$

cd $i
NG=

for i in *
do 
	if test -f $i -a -r $i
	then 
		NG=`grep 't$' $i | wc -l`
		if test $NG -ge $2
		then 
			echo `pwd`/$i >> $3 
		fi
	fi	
done

for i in *
do 
	if test -d $i -a -x $i
	then 
		FCR.sh `pwd`/$i $2 $3
	fi
done

