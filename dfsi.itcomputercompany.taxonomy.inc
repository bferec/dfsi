<?php

// ------------------------------------------------
// @(#) dfsi.itcomputercompany.taxonomy.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_MODULE_NAME") ) { define( "_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_INCLUDE_EXTENSION") ) { define( "_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php


module_load_include( _INCLUDE_EXTENSION , _MODULE_NAME , "dfsi.define");
module_load_include( _INCLUDE_EXTENSION , _MODULE_NAME , "dfsi.api");


function  dfsi_Create_itcomputercompany_Taxonomy_term( $taxonomyName )
{
    $listeTermes = array
    (
        "--interne--" , "Microsoft", "Dell" , "Hewlett-Packard" , "IBM" , 
        "Oracle" , "RedHat" , "Paessler" , "VEEAM" , "VmWare" , "Citrix" ,
        "Wyse",_DFSI_UNKNOWN_SERVICE_TYPE
    );
    foreach( $listeTermes as $name  ) 
    {
        dfsi_AddTermToTaxonomy($taxonomyName , $name, null );
    }
}

