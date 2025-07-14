#!/bin/bash

ORIGEN="archivo_grande.bin"
DESTINO="destino.bin"
ITERACIONES=5

echo "Tipo,Iteracion,Tiempo" > resultados_CmayorT.csv

for tipo in single pingpong; do
  for i in $(seq 1 $ITERACIONES); do
    echo "Ejecutando $tipo - Iteración $i..."

    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"

    if [ "$tipo" == "single" ]; then
      RESULT=$(taskset -c 0 ./single_buffer $ORIGEN $DESTINO | grep 'Tiempo' | awk '{print $5}')
    else
      RESULT=$(taskset -c 0,1 ./double_buffer_pingpong $ORIGEN $DESTINO | grep 'Tiempo' | awk '{print $6}')
    fi

    echo "$tipo,$i,$RESULT" >> resultados_CmayorT.csv
  done
done

echo "Pruebas completas. Resultados guardados en resultados.csv"
