#################################################################################

        !!!!!!!!!!!!        RECONCILIACION v 3.3               !!!!!!!!!!!!!!


       This is a reconciliation software for genes and species
       Authors: Alitzel López Sanchez, Chávez Aparicio Edgar Iván
       Ref:    Hernandez-Rosales et al.: From event-labeled gene
               trees to species trees. BMC Bioinformatics 2012 13(Suppl 19):S6.

-------------------------------INTRODUCTION--------------------------------------

Hello, thanks for using our software.

We hope you find it usefull.

Te purpose of the app is to help you find data regarding gene families in
    species trees. You can use orthology relations given by other tools like
    ProteinOtho. And then you can use this software in order to obtain the
    reconciliation tree between this data and a proposed species tree.

-------------------------------HOW TO USE???--------------------------------------

--      INSTALLING


1)  You need to create a folder where you want to run the software
2)  You have to copy the content inside Recon_App to the folder from step 1
3)  Get inside your folder
4)  You will have to tell your system that the file "Install.sh" is executable:

        Options:

            a)  right_click on the folder
                click properties
                select the box "Is executable"

            b)  get to your folder from the console
                run "chmod +x Install.sh" whithout quotations

5)  Now you can install


            a)  duble_click "Install.sh"

            b)  get to your folder from the console
                run "./Install.sh" whithout quotations

Now you are ready
You will notice that some of the files you copy are gone, but don't worry.
An other thing you will see is a new folder called "genes"


--      PLACING YOUR DATA


You will need:
    A newick "species.nwk" file
    gene families adjacency lists "name.txt" whith any "name" you want
    the original graph from ProteinOtho "GraphPO"

You need to use the names given in the previous lines.

6)  copy (recommended) or move the files "species.nwk" and "GraphPO" to the folder
    created on step 1
7)  copy (recommended) or move all the adjacency lists files to the folder "genes"

Now you are ready to GO 


--      RUNNING THE PROGRAM

Note:   We recommend to use the console in order to see any warnings or errors

8)  The first part is to run the Modular decomposition.
    Options:
        
    a)  get to your folder from the console
        run "./Mod_Decom.sh" whithout quotations
        (recommended)

    b)  Dubble-click the file "Mod_Decom.sh"

You will see how new text files are created
    The "*.nwk" files are phylogenetic trees obtained from Modular decomposition

9)  Now you can run the reconciliation.
    Options:

    a)  run "./Reconciliation.sh" whithout quotations
        (recommended)

    b)  Dubble-click the file "Reconciliation.sh"

Now you will see the results in text files

--      RESULTS

*_recon.nwk             They are the reconciliation phylogenetic tree
*_report.txt            Contains the information from the philogenetic tree
                        adressed to the species tree
Recon_spec_tree.nwk     Is the species tree whith tags
report_conjunto.txt     Contains the information from all the _report.txt files
resumen.txt             Is a resume from report_conjunto.txt
verbose.txt             Is a file that saves what the algorithm is going