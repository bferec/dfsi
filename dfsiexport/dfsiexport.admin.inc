<?php
// ----------------------------------------------------------
// ----------------------------------------------------------
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
		'#default_value' => $dfsiexport_preferred_exportMode,
	);      
    return system_settings_form($form);
}



function dfsiexport_settings_form_submit($form, &$form_state) 
{    
    $dfsiexport_choosed_mode = $form_state['values']['modeExport'];
    //dpm($dfsiexport_choosed_mode);

    variable_set("dfsiexport_preferred_exportMode", $dfsiexport_choosed_mode );

}