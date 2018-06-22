"""
Reporte de Reconciliación 
Por CHávez Aparicio Edgar Iván

"""

lista_f = open('report_list.txt', 'r')

reporte={}
resumen={}
flag=0


#   Junta información de los archivos
for name in lista_f:
    
    #abre archivos
    temp_f= open(name[:-1], 'r')
    for line in temp_f:
        
        #print(line)
        #Guarda el nombre delos nodos
        if line[:4]=="Nodo":
            nodo=line
            flag=1
        
        #Añade la información al nodo correspondiente a mapeos
        elif line[0]=='\t':
            
            #En caso de que no exista el nodo en el reporte, lo crea
            if not reporte.__contains__(nodo):
                reporte[nodo]=""
            
            #sie es el primer dato del archivo añadido al nodo, añade una cabecera
            if flag==1:
                reporte[nodo]+="\t"+name
                flag=0
            reporte[nodo]+=line
            
        #  Añade información de conteo de genes mapeados
        elif line[:5]=="Genes":
            if not resumen.__contains__(nodo):
                resumen[nodo]=[0,0,0,0]
            resumen[nodo][0]+=int(line[7])
        
        elif line[:13]=="Especiaciones":
            resumen[nodo][1]+=int(line[15])
        
        elif line[:13]=="Eliminaciones":
            resumen[nodo][2]+=int(line[15])
        
        elif line[:13]=="Duplicaciones":
            resumen[nodo][3]+=int(line[15])
        
    temp_f.close()
    
lista_f.close()

#   Imprime reporte
reporte_f= open('report_conjunto.txt', 'w')
resumen_f= open('resumen.txt', 'w')

for keys,values in reporte.items():
    reporte_f.write(keys)
    reporte_f.write(values)
    reporte_f.write("Genes:\t"+str(resumen[keys][0])+"\n")
    reporte_f.write("Especiaciones:\t"+str(resumen[keys][1])+"\n")
    reporte_f.write("Eliminaciones:\t"+str(resumen[keys][2])+"\n")
    reporte_f.write("Duplicaciones:\t"+str(resumen[keys][3])+"\n")
    reporte_f.write("\n")
    
    resumen_f.write(keys)
    resumen_f.write("Genes:\t"+str(resumen[keys][0])+"\n")
    resumen_f.write("Especiaciones:\t"+str(resumen[keys][1])+"\n")
    resumen_f.write("Eliminaciones:\t"+str(resumen[keys][2])+"\n")
    resumen_f.write("Duplicaciones:\t"+str(resumen[keys][3])+"\n")
    resumen_f.write("\n")

reporte_f.close()
resumen_f.close()
