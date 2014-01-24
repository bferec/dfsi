<?php

if( !defined("DFSIEXPORT_MODULE_NAME") ) { define( "DFSIEXPORT_MODULE_NAME" , "dfsiexport" ); }                 // nom du module
if( !defined("DFSIEXPORT_INCLUDE_EXTENSION") ) { define( "DFSIEXPORT_INCLUDE_EXTENSION" , "inc" ); }      		// extension pour les fichiers include php

module_load_include( DFSIEXPORT_INCLUDE_EXTENSION , DFSIEXPORT_MODULE_NAME , "dfsiexport.define");

// ----------------------------------------------------------
// Hook_menu
// ----------------------------------------------------------
// ----------------------------------------------------------
// Fonction de creation du menu de "dfsiexport"
// ----------------------------------------------------------
function dfsiexport_menu() 
{
    $items = array();
	
    // url de configuration de l'application
    $items['admin/config/dfsi/export'] = array
    (
        'title' => t("Parametres d'exportation"),
        'description' => t("Définir le type d'exportation"),
        'access arguments' => array('export dfsi data'),
        'page callback' => 'drupal_get_form',
        'page arguments' => array('dfsiexport_admin_settings'),
        'file' => 'dfsiexport.admin.inc'
    );
    return $items;
}