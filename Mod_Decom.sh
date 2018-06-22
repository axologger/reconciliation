#   descompocición modular          $Dirección > $name.nwk , Gene_tree_list.txt

    #   obetner direcciones             $Dirección > temp_dirs.txt
    
    find genes -name \*.txt -print > temp_dirs.txt
    
    #   guardar descompocición          temp.txt    > $name.nwk
    #   lista de arboles de genes                   > Gene_tree_list_full_temp.txt
    
    while read input
    do
        #   parceo
        grep -v "#" $input > temp.txt
        
        #   descompocición modular
        name=$(echo $input | rev| cut -d/ -f1 |rev| cut -d. -f1)
        java -jar .iced-md.jar temp.txt $name.nwk
        echo $name.nwk >> Gene_tree_list_full_temp.txt
        
    done < temp_dirs.txt
    
    # Ordenar                       Gene_tree_list_full_temp.txt > Gene_tree_list.txt
    sort  Gene_tree_list_full_temp.txt > Gene_tree_list.txt
    truncate -s -1 Gene_tree_list.txt
    
    rm temp_dirs.txt
    rm temp.txt
    rm Gene_tree_list_full_temp.txt
