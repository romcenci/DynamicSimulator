#!/bin/bash

qtd=$(yad --title="ColorFileGen" \
	  --width=100 \
	  --text="Digite o número de cores:" \
	  --entry)

if [[ $qtd =~ ^-?[0-9]+$ ]]; then
    for (( i=1; i<=$qtd; i++ )); do
	fields+='--field=Color'$i':CLR ';
    done;

    yad --title="ColorFileGen" \
	--width=100 \
	--form \
	$fields \
	--button="Cancel:1" \
	--button="Done:0" \
	--separator="\n" \
	| sed -e 's/#/0x/g' > color.txt

    sed -i '/^$/d' color.txt
else
    echo "Digite um número de cores válido."
fi
