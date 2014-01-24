<?php

if( !defined("_DFSIEXPORT_MODULE_NAME") ) { define( "_DFSIEXPORT_MODULE_NAME" , "dfsiexport" ); }                 // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }      		// extension pour les fichiers include php

module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSIEXPORT_MODULE_NAME , "dfsiexport.define");
module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");


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