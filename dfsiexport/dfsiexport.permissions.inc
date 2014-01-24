<?php

if( !defined("DFSIEXPORT_MODULE_NAME") ) { define( "DFSIEXPORT_MODULE_NAME" , "dfsiexport" ); }                 // nom du module
if( !defined("DFSIEXPORT_INCLUDE_EXTENSION") ) { define( "DFSIEXPORT_INCLUDE_EXTENSION" , "inc" ); }      		// extension pour les fichiers include php

module_load_include( _INCLUDE_EXTENSION , _MODULE_NAME , "dfsi.define");
module_load_include( _INCLUDE_EXTENSION , DFSIEXPORT_MODULE_NAME , "dfsiexport.define");


// --------------------------------
// Hook_permission
// --------------------------------
// créee les permissions 
// specifiques au module dfsiexport
// --------------------------------
function dfsiexport_permission() 
{
    return array
    (
        'export dfsi data' => array
        (
        'title' => t('exporter les données de ')._MODULE_COMMERCIAL_NAME,
        'description' => t("permission d'exportation")
        ),
    );
}