<?php

// ------------------------------------------------
// @(#) dfsi.itcomputercompany.taxonomy.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php


module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");
module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.api");


function  dfsi_Create_itcomputercompany_Taxonomy_term( $taxonomyName )
{
    $listeTermes = array
    (
        "Cisco" , "Symantec" , "Apple" , "Oracle" , "RedHat" , 
        "Paessler" , "VEEAM" , "VmWare" , "Citrix" ,
        "Wyse","Hewlet Packard" , _DFSI_UNKNOWN_SERVICE_TYPE
    );
    foreach( $listeTermes as $name  ) 
    {
        dfsi_AddTermToTaxonomy($taxonomyName , $name, null );
    }
}

