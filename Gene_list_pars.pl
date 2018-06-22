#!/usr/local/bin/perl

#   PARCEO DE LISTA DE GENES
#   Este programa adquiere una lista de genes relacionados con especies en cabeceras (con especies y genes repetidos y en desorden).
#   <<  $input
#   Crea una lista que contiene las especies con su respectiva lista de genes
#   >>  $output


# Definición de variables

use Getopt::Long;
my $input;
my $output;
my $spec;
my %Genes;

#Extracción de argumetos (lista de entrada) -> (lista de salida)

GetOptions ('in=s' => \$input,
            'out=s' => \$output);
        
#   Programa principal
    #   Lee archivo de genes
open(my $inputH, '<:encoding(UTF-8)', $input)
  or die "Could not open file '$input' $!";
            
    #   Realiza un hash $Genes{'Nombre del gen'}= Especie
while (<$inputH>){
        if (/[#, ]*(.+)\.fa.*/){
            $spec=lc $1
        }
        elsif (/(^[^#].*)\n/){
            $Genes{$1}=$spec;
        }

    }
    close $inputH;
    
#   Guarda datos en el archvio de salida
open (my $outputH, '>', $output);
    print {$outputH} "$_\t$Genes{$_}\n" for (keys %Genes);
    close $outputH;