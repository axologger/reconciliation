#!/bin/bash

mkdir genes
mv Gene_list_pars.pl .Gene_list_pars.pl
mv iced-md.jar .iced-md.jar
mv Recon.cpp .Recon.cpp
mv Rep_final.py .Rep_final.py
chmod +x Mod_Decom.sh
chmod +x Reconciliation.sh
c++ .Recon.cpp
mv a.out .a.out