<?php

// ------------------------------------------------
// @(#) dfsi.itservicetypes.taxonomy.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php


module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");
module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.api");



function dfsi_Create_itservicetypes_Taxonomy_term( $taxonomyName )
{
    $listeTermes = array
    (            
            "Adresse IP" , 
            "Serveur" , 
            "Application" ,
            "Routeur",
            "Switch",
            "Salle informatique",
            "Maitre d'opération active directory",
            "** Indéterminé **"
    );

    $listeServeurs = array
    (
        "Service Backup" , "Service de virtualisation" , "Service de fichiers" , "Service d'impression" , 
        "Service TSE" , "Contrôleur de domaine Active directory" ,"Service SMTP",
        "Service TFTP" , "Service FTP" , "Service de messagerie" , "Service Proxy","Service de passerelle", "Service DFS",
        "Service Antivirus","Service de supervision système et réseau", "Service SQL","Service d'applicaton","Service de log",
        "Service Web","Service intranet","Service San","Service Nas","Service de licences","Service de streaming",
        "Service de generation de documents",
    );

    $listeAdresses = array
    (
            "Adresse Mac", "Adresse IP V4" , "Adresse IP V6",
    );
    $termes_tid = array(0,0,0);
    $x = 0;
    
    foreach( $listeTermes as $name ) 
    {
        $terme = dfsi_AddTermToTaxonomy($taxonomyName , $name, null  );
        $termes_tid[ $x ] = $terme -> tid;
        $x ++;
    }

    foreach( $listeServeurs as $name ) 
    {
        $sousTerme = dfsi_AddTermToTaxonomy($taxonomyName , $name, $termes_tid[ 1 ] );
    }	

    foreach( $listeAdresses as $name ) 
    {
        $sousTerme = dfsi_AddTermToTaxonomy($taxonomyName , $name, $termes_tid[ 0 ] );
    }	
}

