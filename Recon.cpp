#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <sstream>

#define MAX_MEMORY  1000
#define SIZE        500
using namespace std;

/*
 * nodo base: 
 * 
 * 
 */

class   node {
    
    public:
        
        string  name;
        char    type;                           //  'L' 'S' 'D' 'X'
        int     father;
        int     firstChild, manyChildren;
        int     prevBrother, nextBrother;       //, manyBrothers; //brotherNumber no se ha usado
        int     map, comap, manyMaps;
        
        void    Make(string, char);
        void    Print();
        void    Children(int*, node*);
        void    Copy(node*);
    
};

void    node::Make(string Name, char Type){     //  inicializa nodos
    
    name=Name;
    type=Type;
    father=-1;
    firstChild=-1;
    comap=-1;
    map=-1;
    prevBrother=-1;
    nextBrother=-1;
    manyChildren=0;
    
}

void    node::Print(){                          //  reporta el nodo
    
    cout << "name = " << name<<endl;
    cout << "type = " << type<<endl;
    cout << "father = " << father<<endl;
    cout << "prev brother = " << prevBrother<<endl;
    cout << "nextBrother = " << nextBrother<<endl;
    cout << "firstChild = " << firstChild<<endl;
    cout << "manyChildren = " << manyChildren<<endl;
    cout << "map = " << map<<endl;
    cout << "comap = " << comap<<endl;
    cout << "many maps = " << manyMaps<<endl;
    cout<<endl;
}

void    node::Children(int *hijos, node *tree){ //  obtienes un array de hijos
    hijos[0]=firstChild;
    
    for (int i=1;i<manyChildren; i++ ){
        hijos[i]=tree[hijos[i-1]].nextBrother;
    }
}

void    node::Copy(node *copia){
    copia->father=father;
    copia->firstChild=firstChild;
    copia->prevBrother=prevBrother;
    copia->nextBrother=nextBrother;
    copia->manyChildren=manyChildren;
    
}

//  Definición de funciones

int     lca(int, int, node*);
int     lcan(int*, int , node* );
int     Chicken(int,node*, node*, map<string,string>&);
int     nwk2tree(const char*,node*);
int     contains (int*, int, int);
int     contains2 (int*, int, int);
string  num2str(int);
string  reverse(string);
void    gene_dict(char*,map<string,string>&);
void    hp(int, node*, node*, int*, int, int*);
int     herederos(int, int, node*, node*,int*, int );
string  node2nwk(node*, int, map<string,string>&);
void    printTree(node*, int);

//  Función principal

map <string,string> Dict;
int main(void){

    //  No tengo idea de por que no funciona el programa sin las 2 siguientes lineas
    node Gene_tree[1];
    Gene_tree[0].Make("0",'L' );
    
    //      PROGRAMA PRINCIPAL
    
    //  lectura de especies
    
    cout<<"-----LECTURA DE ESPECIES----"<<endl;
    node tree[SIZE*2];
    char file[]="Spec-Tree.nwk";
    int  len;
    len=nwk2tree(file,tree);
    
    ofstream newickFile_spec;
    newickFile_spec.open("Recon_spec_tree.nwk");
    cout<<"Recon_spec_tree.nwk"<<endl;
    string output_spec;
    output_spec=node2nwk(tree,0,Dict);
    newickFile_spec << output_spec << endl;
    newickFile_spec.close();
    
//     printTree(tree,len);
    
    cout<<"-----NEWICK-GENE-----"<<endl;
            
    //  parceo de genes Gene-Spec.txt
            
    cout<<"-----GENES - SPECIES----"<<endl;
    char dict_file[]="Gene-Spec.txt";
    gene_dict(dict_file,Dict);
            
    
    char        LIST[]="Gene_tree_list.txt";
    string      READ;
    ifstream    in_file (LIST);
    
     if(in_file.is_open()){
        
        while(in_file.eof()==0){                    //lee todo el archivo

            getline(in_file,READ);                  //linea por linea
            
            cout<<"-----LECTURA DE GENES----"<<endl;
            node tree1[SIZE*2];
            int len1;
            len1=nwk2tree(READ.data(), tree1);
            
//             printTree(tree1,len1);
            
            //  mapeo CHICKEN
            
            cout<<"-----CHICKEN----"<<endl;
            int     chick;
            chick=Chicken(0,tree,tree1,Dict);
            
            if (chick<0){
                cout<<"No congruente"<<endl;                //revisa congruencia
            }else{
//                  printTree(tree1,len1);
            }
            
            //  inferir eliminaciones por BFS
            
//             cout<<chick<<endl;

            // en caso de ser congruente
            if (chick>0){
                cout<<"-----ELIMINACIONES----"<<endl;
                int     tamano=0, i=0;
                int     Stack[SIZE*2];
                Stack[0]=0;
                
                while (i<=tamano){
                    int temp[tree[i].manyChildren+1];
                    hp(i, tree, tree1, temp, len, &len1);
                    int j=0;
//                     cout<<temp[0]<<endl;
                    while(temp[j]!=-1){
//                         cout<<temp[j]<<endl;
                        tamano++;
                        Stack[tamano]=temp[j];
                        j++;
                    }
                    i++;
                    
                }
                
//                   cout<<"----Tree1----"<<endl;
//                   printTree(tree1,len1);
                
                // reportar datos de reconciliación
                cout<<"----REPORTANDO----"<<endl;
                ofstream reporte;
                string temp_name (READ.substr(0,READ.size()-4));
                temp_name.append("_report.txt");
                cout<<temp_name<<endl;
                reporte.open(temp_name.data());
                
                // por cada nodo del arbol de especies
                for(int count=0; count<len; count++){
                    int     dup=0, elim=0, spec=0,flag=0, gen=0;
                    int     prev=tree[count].comap;
                    
                    reporte<<"Nodo:\t"<<tree[count].name<<endl;
//                     reporte<<"Tipo:\t"<<tree[count].type<<endl;
                    
                    reporte<<"Mapeos:"<<endl;
                    // revisa cada comap
                    while(prev!=-1&&flag==0){
                        switch (tree1[prev].type){
                            case 'L':
                                gen++;
                                break;
                            case 'X':
                                elim++;
                                break;
                            case 'S':
                                spec++;
                                break;
                            case 'P':
                                dup++;
                                break;
                            default:
                                flag=1;
                                break;
                        }
                        
                        // reporte      maps
                        reporte<<"\t Nodo:\t"<<tree1[prev].name<<endl;
                        reporte<<"\t Tipo:\t"<<tree1[prev].type<<endl;
//                         reporte<<endl;
                        
                        prev=tree1[prev].comap;
                    }
                    
                    // reporte nodo
                    reporte<<"Genes:\t"<<gen<<endl;
                    reporte<<"Especiaciones:\t"<<spec<<endl;
                    reporte<<"Eliminaciones:\t"<<elim<<endl;
                    reporte<<"Duplicaciones:\t"<<dup<<endl;
                    reporte<<endl;
                    reporte<<endl;
                    
                }
                reporte.close();

            // en caso de no ser congruente -> reportar
            }else{
                // reportar datos de reconciliación
                cout<<"----REPORTANDO----"<<endl;
                ofstream reporte;
                string temp_name (READ.substr(0,READ.size()-4));
                temp_name.append("_report.txt");
                cout<<temp_name<<endl;
                reporte.open(temp_name.data());
                reporte<<"No conruente"<<endl;
                reporte<<"Error en: \t"<<-1*chick <<endl;
                reporte.close();
            }
            
            //  guardar en newick
            
            cout<<"-----NEWICK-GENE-----"<<endl;
            
            ofstream newickFile;
            READ.insert(READ.size()-4,"_recon");
            cout<<READ<<endl;
            newickFile.open(READ.data());
            
            string output;
//              cout<<"root ="<<tree1[0].name<<"\""<<endl;
            output=node2nwk(tree1,0,Dict);
//               cout<<output<<endl;
            newickFile << output << endl;
            newickFile.close();
        
        //  limpiar mapeos del arbol de especies
        
            for (int count2=0; count2<=len ; count2++){
                tree[count2].map=-1;
                tree[count2].comap=-1;
                tree[count2].manyMaps=0;
            }
        
        }
        
    }else{
        
        cout<<"Main > No se puede abrir "<< file<<endl;
        exit(8);
    }

    //Cierra el archivo
    in_file.close();
    
    return 0;
}


/*
 *                  **********+FUNCTIONS************
 * 
 * contains     busca elemnetos en arrays
 * lca          last common ancester
 * lcan         last common ancester for n nodes
 * Chicken      mapeo de reconciliación
 * nwk2tree     lee archivo newic a un árbol de nodos
 * reverse      invierte el orden de un string
 * num2str      convierte entero a string
 * gene_dict    pasa lista de "genes \t especie" a un map
 * hp           busca secciones para inferir eliminacion
 * herederos    incerta una especiación con eliminaciones
 * node2nwk     pasa de nodes a un string de newick
 * printTree    imprime los datos de un arreglo de nodes
 * 
 */



/*      --------CONTAINS n IN ARRAY?----------
 * 
 * Regresa la ubicación donde se encuentra "n"
 * en el "ARRAY"
 * regresa -1 si no lo encuentra
 */

int contains (int *ARRAY, int n, int end){
    
    int     i=0;
    
    while((ARRAY[i]!=end) && (i<=MAX_MEMORY)&&(ARRAY[i]!=n)){
        i++;
    }
    
    if(ARRAY[i]==n){
            return i;
    }
    
    return -1;
}


/*      --------CONTAINS2 n IN ARRAY?----------
 * 
 * Regresa la ubicación donde se encuentra "n"
 * en el "ARRAY"
 * regresa -1 si no lo encuentra
 */

int contains2 (int *ARRAY, int n, int end){
    
    int     i=0;
    
    while((i<end) && (i<=MAX_MEMORY)&&(ARRAY[i]!=n)){
        i++;
    }
    
    if(ARRAY[i]==n){
            return i;
    }
    
    
    return -1;
}

/*      ----------------------LAST COMON ANCESTER---------------------------
 * busca el ancestro común entre "leaf1" y "leaf2"
 * en el arreglo de árbol "tree"
 * 
 * regresa el lugar del nodo en tree
 * regresa -1 si no es posible
 *-------------------------------------------------------------------------*/

int     lca(int leaf1, int leaf2, node *tree){
    
    int     Stack1[SIZE];
    int     Stack2[SIZE];
    Stack1[0]=leaf1;
    Stack1[1]=-1;
    Stack2[0]=leaf2;
    Stack2[1]=-1;
    
    // LLenado del Stack1 con fathers
    
    int     i=0;
    int     current=leaf1;
    
    while (tree[current].father!=-1){
        i++;
        current=tree[current].father;
        Stack1[i]=current;
        
    }
    Stack1[i+1]=-1;
    
    // LLenado del Stack2 con fathers
   
         i=0;
         current=leaf2;
    while (tree[current].father!=-1){
        
        i++;
        current=tree[current].father;
        Stack2[i]=current;
    }
    Stack2[i+1]=-1;
    
    // Comparación de Stacks
    
    int     flag=-1;
            i=0;
    while((flag==-1)&&(i<=MAX_MEMORY)){
        
        flag=contains(Stack1, Stack2[i],-1);
        i++;
        
    }
    if (flag==-1)
        return  -1;
    return  Stack1[flag];
}

/*    --------------------LAST COMON ANCESTER FOR "N" NODES-------------------
 * busqueda recursiva de ancestros en "tree"
 * divide ente 2 los elementos de arreglo de nodos "descendencia"
 * de tamaño "size"
 * 
 * regresa el lca de todos en común
 * regresa-1 si no es posible
 *---------------------------------------------------------------------------*/

int lcan(int *descendencia, int size, node *tree ){
    
    if (size==1)
        return descendencia[0];
    if (size==2)
        return lca(descendencia[0],descendencia[1],tree);
    
    int     prueba=size/2;
    
    if ((size-2*prueba)==0){
        
            int temp1[prueba], temp2[prueba];
            
            for(int contador=0;contador<prueba;contador++){
                temp1[contador]=descendencia[contador];
                temp2[contador]=descendencia[contador+prueba];
            }
            
            return lca(lcan(temp1,prueba,tree),lcan(temp2,prueba, tree), tree);
            
    }else{
            int temp1[prueba], temp2[prueba+1];
            
            for(int contador=0;contador<prueba;contador++){
                temp1[contador]=descendencia[contador];
                temp2[contador]=descendencia[contador+prueba];
            }
            
            temp2[prueba]=descendencia[size-1];
            return lca(lcan(temp1,prueba, tree),lcan(temp2,prueba+1,tree),tree);
            
        }
}

/*++++++++++++++++++++++++++++++ FUNCION Chicken ++++++++++++++++++++++++
 * función recursiva que realiza el mapeo de reconciliación entre:
 * un árbol de especies y uno de genes
 * 
 * raiz         nodo raíz del árbol de genes
 * spec_tree    arbol de especies
 * gene_tree    arbol de genes
 * Dict         map que permite relacionar los genes y especies
 * 
 *  
 * regresa -1 si no no es congruente el mapeo
 * regresa 1 si es congruente
 * 
 * nota: puede que cont sea redundante
 * 
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

int Chicken(int raiz,node *spec_tree, node *gene_tree, map<string,string> &Dict){
    int     i=0;
    int     ancester;
    cout<<"raiz= "<<raiz<<endl;
    //  caso sencillo, hoja mapea a hoja
    //  se busca la especie en el arbol de especies
//      gene_tree[raiz].Print();
    
    if (gene_tree[raiz].manyChildren==0){               //  en caso de ser hoja
        
        //  busqueda de especie correspondiente
//         cout<<"match\t"<<Dict[gene_tree[raiz].name]<<endl;
        while(Dict[gene_tree[raiz].name].compare(spec_tree[i].name)!=0){
            i++;
        }
//         cout<<"match\t"<<Dict[gene_tree[raiz].name]<<"-"<<spec_tree[i].name<<endl;
        spec_tree[i].manyMaps++;
        
        //  mapeo a especie correspondiente
        gene_tree[raiz].map=i;
        
        //  mapeo a gen correspondiente
        if (spec_tree[i].comap==-1){
            spec_tree[i].comap=raiz;
        }else{
            i=spec_tree[i].comap;
            while(gene_tree[i].comap!=-1){
                i=gene_tree[i].comap;
            }
            gene_tree[i].comap=raiz;
        }        
        return 1;
        
    }else{                                              //  nodo interno
        
        int     temp, bro, cont=0;
        int     hijos[gene_tree[raiz].manyChildren];
        bro=gene_tree[raiz].firstChild;
            
            //  mapea el resto de arbol
            while (cont<gene_tree[raiz].manyChildren){
                int test=Chicken(bro,spec_tree,gene_tree,Dict);
                if (test==1){
                    hijos[cont]=gene_tree[bro].map;
                    bro=gene_tree[bro].nextBrother;
                    cont++;
                }
                else return test;                          //  no se puede mapear
                
            }
        
        if (cont==gene_tree[raiz].manyChildren){        //  el mapeo es correcto
            
            ancester=lcan(hijos,cont,spec_tree);
            
            if (ancester!=-1){                          //  si existe el lcan
                
                //  revisa que no haya Especiaciones mapeando hojas
                if (gene_tree[raiz].type=='S'){
                    if (spec_tree[ancester].type=='L'){
                        return -1*raiz;
                    }
                    
                }
                
                gene_tree[raiz].map=ancester;
                spec_tree[ancester].manyMaps++;
                if (spec_tree[ancester].comap==-1){     //  si es el primer mapeo
                    
                    spec_tree[ancester].comap=raiz;
                    
                }else{                                  //  si no es el primer mapeo
                    
                    ancester=spec_tree[ancester].comap;
                    while(gene_tree[ancester].comap!=-1){
                        ancester=gene_tree[ancester].comap;
                    }
                    gene_tree[ancester].comap=raiz;
                }
                return 1;
            }else{return -1*raiz;}                            // si no existe lcan
            
        }else{return -1*raiz;}
        
        return 0;                                       //  en caso de un error inesperado
        
    }
    
}

/*++++++++++++++++++++++++++ FUNCION nwk2tree +++++++++++++++++++++++++++++
 * lee un archivo en formato newick para hacer un árbol
 * lee de derecha a izquierda
 * 
 * FILE_NAME        nombre del archivo
 * tree             arbol que se quiere llenar
 * 
 * regresa la cantidad de nodos en el árbol
 * 
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

int nwk2tree(const char *FILE_NAME,node *tree){

    string      READ, name;
    ifstream    in_file (FILE_NAME);
    char        cadena;
    int         i=0, father=-1, brother=-1, number=0;
    
    if(in_file.is_open()){
        getline(in_file,READ);
    
    }else{
        
        cout<<"nwk2tree > No se puede abrir  "<< FILE_NAME<<endl;
        exit(8);
    }

    //Cierra el archivo
    in_file.close();
    
    i=READ.size();
    i-=1;
    name.erase();
//     cout<<i<<endl;
    
    while(i>=0){        //  recorrido hasta el inicio del string
        
//         cout<<READ[i];
//         cout<<number<<endl;
        switch (READ[i]){
            
            //  cierra un sub arbol
            case '(':
//                 cout<<"("<<endl;
                
                if (name.size()>0){         //  ¿hay nombre?
                    
                    tree[number].Make(reverse(name),'L');
//                     tree[number].Print();
                    name.erase();
                    
                    if(brother!=-1){        //  ¿primer hijo?
                        
                        tree[brother].nextBrother=number;
                        tree[number].prevBrother=brother;
//                         cout<<"a"<<"\t number = "<<number<<endl;
//                         cout<<"a"<<"\t brother.nextBrother = "<<tree[brother].nextBrother<<endl;
                        
                    }else{                  //  no es primer hijo
                        
                        tree[father].firstChild=number;
                        tree[number].prevBrother=-1;
//                         cout<<"b"<<"\t number = "<<number<<endl;
//                         cout<<"b"<<"\t father.firstChild = "<<tree[father].firstChild<<endl;
                        
                    }
                    tree[number].nextBrother=-1;
                    tree[number].father=father;
                    tree[father].manyChildren++;
                    brother=tree[number].father;
                    father=tree[brother].father;
                    number++;

                }else{                      //  no hay nombre
                    
                    tree[brother].nextBrother=-1;
                    tree[father].manyChildren++;
                    brother=tree[brother].father;
                    father=tree[brother].father;
                }
                
                i--;
                
                break;
                
            //  separación entre nodos
            case ',':
                
//                 cout<<","<<endl;
                if (name.size()>0){         //  ¿hay nombre?
                    
                    tree[number].Make(reverse(name),'L');
//                     tree[number].Print();
                    name.erase();
                    
                    if(brother!=-1){        // es primer hijo
                        
                        tree[brother].nextBrother=number;
                        tree[number].prevBrother=brother;
//                         cout<<"a"<<"\t number = "<<number<<endl;
//                         cout<<"a"<<"\t brother.nextBrother = "<<tree[brother].nextBrother<<endl;
                        
                    }else{                  //  no es primer hijo
                        
                        tree[father].firstChild=number;
                        tree[number].prevBrother=-1;
//                         cout<<"b"<<"\t number = "<<number<<endl;
//                         cout<<"b"<<"\t father.firstChild = "<<tree[father].firstChild<<endl;
                        
                    }
                    tree[number].father=father;
                    brother=number;
                    number++;
                    
                }else{                      //  no hay nombre
                    
                    //en espera de "("
                }
                tree[father].manyChildren++;
                i--;
                
                break;
            
            //  crea un nodo raíz de un subarbol
            case ')':
//                 cout<<")"<<endl;
                
                if (name.size()>0){
                    if(name[0]==';'){
                        if(name.size()==1){
                            tree[number].Make(";",'S');
                        }else{
                            tree[number].Make(";",name[1]);
                        }
                    }else{
                        tree[number].Make(num2str(number),name[0]);
                    }
                    
                }else{
                    
                    tree[number].Make(num2str(number),'S');
                    
                }
                tree[number].prevBrother=brother;
                tree[number].father=father;
//                 tree[father].manyChildren++;
                
                if(brother==-1){
                    
                    tree[father].firstChild=number;
                    
                }else{
                    
                    tree[brother].nextBrother=number;
                    
                }
//                 tree[father].Print();
                father=number;
                number++;
                brother=-1;
                name.erase();
                i--;
                break;
                
            //  captura los nombres
            default:
                
                name.append(READ.substr(i,1));
                i--;
                
        }
      
    }
//     cout<<number-1<<endl;
    return number;
}

/*      --------------------------------REVERSE----------------------------
 * 
 * invierte el string "mess"
 * 
 * regresa un string
 *-----------------------------------------------------------------------*/

string reverse(string mess){
    string ret;
    for (int i =mess.size()-1;i>=0;i-- ){
        
        ret.append(mess.substr(i,1));
    }
    return ret;
}

/*   ------------------------------NUMBER TO STRING------------------------
 * 
 * convierte "Number" <int>
 * a un "String"
 * 
 * requiere <sstream>
 *-------------------------------------------------------------------------*/

string num2str(int Number){
    string String = static_cast<ostringstream*>( &(ostringstream() << Number) )->str();
    return String;
}


/*      -------------------DICCTIONARIO DE GENES------------------------------
 * 
 * obtiene los datos del dicctionario a partir de "file"
 * leyendo linea por lineas
 * 
 * las guarda en "Dict"
 * 
 *-------------------------------------------------------------------------*/

void     gene_dict(char *file,map<string,string> &Dict){

    string      READ;
    ifstream    in_file (file);
    char        cadena;
    int     i, fi=0;
    string  gen;
    string  spec;
    
    if(in_file.is_open()){
        
        while(in_file.eof()==0){                    //lee todo el archivo

            getline(in_file,READ);                  //linea por linea
            i=READ.size();
            
            for(int count1=0;count1<i;count1++){

                if (READ[count1]=='\t'){            //separa por un tabulador
                    fi=1;

                    
                }else{
                    
                    if(fi==0){
                        gen.append(READ.substr(count1,1));  //guarda el gen
                    
                    }else{
                        
                        spec.append(READ.substr(count1,1)); //guarda la especie
                    }
                }
            }
            
            Dict[gen]=spec;                         //dict
//             cout<<gen<<"\t"<<spec<<endl;
            gen.erase();
            spec.erase();
            fi=0;
        }
        
    }else{
        
        cout<<"Dict-Gene > No se puede abrir  "<< file<<endl;
        exit(8);
    }

    //Cierra el archivo
    in_file.close();
    
}

/*      -------------------ELIMINACION DE GENES ----------------------------
 * 
 * infiere las eliminaciones dado un nodo "i"
 * 
 * 
 * 
 * 
 *-------------------------------------------------------------------------*/

void    hp(int i,node *spec_tree, node *gene_tree,int *temp, int len_spec, int *len_gen){
    int     C2=0;   //  contador
    
    gene_tree[i].Children(temp, gene_tree);                        //obtienes todos los hijos
//     cout<<"hp temp[0]= "<<temp[0]<<endl;
//     cout<<gene_tree[i].type<<endl;
//       gene_tree[i].Print();
    if(gene_tree[i].type=='P'){                         //si es una duplicación
        
        for(C2=0;C2<gene_tree[i].manyChildren;C2++){    //para cada hijo
            if(gene_tree[i].map!=gene_tree[temp[C2]].map){//si ambos no mapean al mismo
                
                //  inferir una nueva especiación y perdidad de genes
                temp[C2]=herederos(temp[C2],gene_tree[i].map, gene_tree, spec_tree, len_gen, len_spec);
            }
        }
        
    }else if(gene_tree[i].type=='S'){                   //si es una especiación
    
        //array de los hijos del mapeo en el arbol de especies
        int Hijos[spec_tree[gene_tree[i].map].manyChildren];
        spec_tree[gene_tree[i].map].Children(Hijos, spec_tree);
        
        for(C2=0;C2<gene_tree[i].manyChildren;C2++){    //para cada hijo de genes
            
            if(1!=contains(Hijos,gene_tree[temp[C2]].map,-1)){//si no se mapean a un hijo en genes
                
                temp[C2]=herederos(temp[C2],gene_tree[i].map, gene_tree, spec_tree, len_gen, len_spec);
            }
        }
    }else{                                              //en caso de una hoja, no hacer nada
//          cout<<"LOL"<<endl;
    }
    temp[C2]=-1;
}

/*      --------------------------HEREDEROS----------------------------
 * 
 * cambia el nodo señalado, por un una especiación
 * agrega hijos de genes eliminados
 * 
 * regresa la posición del nodo correspondiente a la nueva especiación
 * 
 *-------------------------------------------------------------------------*/

int     herederos(int i, int map, node *gene_tree, node *spec_tree,int *len_gen, int len_spec ){
    
    
    // nueva especiación
    int     nueva=*len_gen;
    (*len_gen)++;
    gene_tree[nueva].Make(num2str(nueva),'S');
    gene_tree[i].Copy(&gene_tree[nueva]);
    
    // el procedimeinto es diferente si es el primer hijo
    if (gene_tree[i].prevBrother==-1){
        gene_tree[gene_tree[nueva].father].firstChild=nueva;
    }else{
        gene_tree[gene_tree[nueva].prevBrother].nextBrother=nueva;
    }
    
    if (gene_tree[nueva].nextBrother!=-1){
        gene_tree[gene_tree[nueva].nextBrother].prevBrother=nueva;
    }
    
    gene_tree[nueva].firstChild=i;
    gene_tree[nueva].manyChildren=1;
    gene_tree[i].father=nueva;
    gene_tree[i].prevBrother=-1;
    gene_tree[i].nextBrother=-1;
    
    // adaptar el mapeo
    gene_tree[nueva].map=map;
    if (spec_tree[map].comap==-1){
        spec_tree[map].comap=nueva;
    }else{
        int next=spec_tree[map].comap;
        while(next!=-1){
            next=gene_tree[next].comap;
        }
        gene_tree[next].comap=nueva;
    }
    
//     gene_tree[nueva].Print();
    
    // eliminaciones añadir eliminaciones para los demás hijos y mapear
    
    int     father=nueva;   // el padre ahora es el incertado
    int     brother=i;      // nodo desplazado ahora es hijo de incertado
    int     ender_son;      // specie mapeada para eliminación
    
    // encontrando ender_son
    
    ender_son=spec_tree[gene_tree[father].map].firstChild;
    
    for (int count=1; count < spec_tree[map].manyChildren; count++){
        
        // añade eliminacion
        gene_tree[*len_gen].Make(num2str(*len_gen),'X');
        gene_tree[*len_gen].prevBrother=brother;
        gene_tree[*len_gen].father=father;
        gene_tree[father].manyChildren++;
        gene_tree[brother].nextBrother=*len_gen;
        brother=*len_gen;
        
        // añade mapeo
        
        if ( ender_son == lca(ender_son,gene_tree[i].map,spec_tree) ){
            ender_son=spec_tree[ender_son].nextBrother;
        }
        
        gene_tree[*len_gen].map=ender_son;      // mapeo gene -> spec
        spec_tree[ender_son].manyMaps++;        // mapeo spec -> gene
        
        // buscqueda de comaps
        int     spec_comap=spec_tree[ender_son].comap;
        if (spec_comap==-1){
            spec_tree[ender_son].comap=*len_gen;    // en caso de ser primer mapeo
        }else{
            while(gene_tree[spec_comap].comap!=-1){            // en caso de no ser el primero
                spec_comap=gene_tree[spec_comap].comap;
            }
            gene_tree[spec_comap].comap=*len_gen;
        }
        
        ender_son=spec_tree[ender_son].nextBrother;
        (*len_gen)++;
        
    }
    
    
    
    return  nueva;
}

/*      --------------------------node2nwk----------------------------
 * 
 * contruye el newick de un árbol
 * 
 * ingresas un arreglo de "nodes"
 * regresa el string "temp" del newick
 * 
 *-------------------------------------------------------------------------*/

string  node2nwk(node *tree, int i, map<string,string> &Dict){
    string temp;
    
    // caso de las hojas
    if(tree[i].manyChildren==0){
        if (tree[i].type=='X'){
            temp="X";
        }else{
            temp=tree[i].name+"_"+Dict[tree[i].name];      // regresa "name,"
        }
        
    // caso del los nodos internos
    }else{                          // regresa (hijos)type:map
        
        int hijo=tree[i].firstChild;
        temp="(";
        temp.append(node2nwk(tree, hijo, Dict));
        hijo=tree[hijo].nextBrother;
        for(int count=1;count<tree[i].manyChildren;count++){
            temp.append(","+node2nwk(tree, hijo,Dict));
            hijo=tree[hijo].nextBrother;
        }
        
        string letter;
        letter=tree[i].name;
        // caso especial del final del árbol ");"
        if (letter==";"){
            temp.append(");");
        }else{
            if (tree[i].map==-1){
                temp.append(")"+letter);
            }else{
                temp.append(")"+letter+"_"+tree[i].type);
            }
        }
    }
    
    return temp;
    
}

/*      --------------------------printTree----------------------------
 * 
 * imprime los datos del arbol
 * 
 * ingresas un arreglo de "nodes"
 * y la longitud del arreglo "len"
 *-------------------------------------------------------------------------*/

void printTree(node *tree, int len){
for(int cont1=0; cont1<len; cont1++){
         tree[cont1].Print();
         cout << endl;
     }
}
