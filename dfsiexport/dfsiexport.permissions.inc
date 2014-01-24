<?php

if( !defined("_DFSIEXPORT_MODULE_NAME") ) { define( "_DFSIEXPORT_MODULE_NAME" , "dfsiexport" ); }                 // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }      		// extension pour les fichiers include php

module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSIEXPORT_MODULE_NAME , "dfsiexport.define");
module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");



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