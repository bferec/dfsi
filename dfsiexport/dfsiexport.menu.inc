<?php


// ----------------------------------------------------------
// Hook_menu
// ----------------------------------------------------------
// 
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
    dpm($items);
    return $items;
}