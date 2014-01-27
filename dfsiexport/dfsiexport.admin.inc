<?php
// ----------------------------------------------------------
// Configuration du mode d'exportation par defaut
// ----------------------------------------------------------

// if( !defined("_DFSIEXPORT_MODULE_NAME") ) { define( "_DFSIEXPORT_MODULE_NAME" , "dfsiexport" ); }                 // nom du module
// if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }      		// extension pour les fichiers include php

// module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSIEXPORT_MODULE_NAME , "dfsiexport.define");
// module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");



// ------------------------------------------------
// Hook_settings
// ------------------------------------------------
function dfsiexport_admin_settings()
{
    $dfsiexport_preferred_exportMode=variable_get("dfsiexport_preferred_exportMode", 1 );
    
	$form['modeExport'] = array
	(
		'#type' => 'radios',
		'#title' => t("Mode d'exportation"),
		'#options' => array
		(
			1 =>t('Csv'),
			2 =>t('Xml'),
		),
		'#default_value' => variable_get('modeExport','#options'),
	);      
	$form['#submit'][] = 'dfsiexport_settings_form_submit';
	return system_settings_form($form);
	
}



function dfsiexport_settings_form_submit($form, &$form_state) 
{    
    $dfsiexport_choosed_mode = $form_state['values']['modeExport'];
}