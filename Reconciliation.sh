#!/bin/bash

#   Recon Alemania
#   Edgar Chávez Aparicio
#   Alitzel López Sánchez
#   Maribel Hernández Rosalez
#

#   IN:
#       GraphPO
#       primates_species_tree.nwk
#       $name.nwk
#       Gene_tree_list_full.txt

#   OUT:
#       $name_recon.nwk
#       $name_report.txt
#       Recon_spec_tree.nwk
#       verbose.txt
#       resumen.txt
#       report_conjunto.txt
    
    #   Obtener columnas de genes y especies
    cut -f1 GraphPO > temp_list.txt
    cut -f2 GraphPO >> temp_list.txt
    
    #   parceo de lista en bruto    temp_list.txt > Gene-Spec.txt
    perl -w .Gene_list_pars.pl --in temp_list.txt --out Gene-Spec.txt 
    
    rm temp_list.txt
    
    #   Cambiar el arbol de genes a minpusculas         $Especies > Spec-Tree.nwk
    cat species.nwk | tr '[:upper:]' '[:lower:]' > Spec-Tree.nwk

#   reconcliacion
#       Input:
#           $name.nwk
#           Gene-Spec.txt
#           Gene_tree_list_full.txt
#           Spec-Tree.nwk
#       Output:
#           $name_recon.nwk
#           $name_report.txt
#           Recon_spec_tree.nwk
#           verbose.txt

    #   Gene_tree_list_full.txt > Gene_tree_list.txt , Output, verbose
    echo Recon
    ./.a.out > verbose.txt
    
    #   ------------------      Reporte final      -----------------
    
    #   Reporte     *report.txt     >       NoCong.txt , report_conjunto.txt , resumen.txt
    echo Resumen
    #   $name_report.txt > NoCong.txt
    grep "No " *_report.txt > NoCong.txt
    echo No Congruentes $(wc -l NoCong.txt)
    
    rm  Gene-Spec.txt
    rm  Spec-Tree.nwk
    
    #   Reprte general      $name_report.txt > report_conjunto.txt
    #   Resumen                              > resumen.txt
    
        #   listado de reportes         > report_list.txt
        ls -1 | grep report > report_list.txt
        sed -i '$ d' report_list.txt
    
    python3 .Rep_final.py

    rm report_list.txt