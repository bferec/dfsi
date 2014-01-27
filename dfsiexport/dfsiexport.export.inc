<?php

// ------------------------------------------------
// @(#) dfsiexport.export.inc
// @(#) export des donnees
// @(#) (c) Amael Hervoche
// @(#) 24/01/2014
// ------------------------------------------------


function dfsiexport_Export()
{
	$form['exportation'] =array
    (
		'#markup' => 'Export de données',
	);
	
	$form['exportation']['dfsiexport_choixExport'] = array
    (
		'#type' => 'checkboxes',
		'#title' => t("Que voulez vous exporter ?"),
		'#options' => array
                (
                        1=> t('Containers et services'),
                        2 => t('Dependances directes et indirectes'),
                        3 => t('Taxonomies')	
                ),
                '#default_value'  => variable_get('dfsiexport_choixExport', array(1)),
//		'#attributes' => array
//		(
//			'name' => 'example-checkboxes[]'
//		)
	);	
	$form['exportation']['dfsiexport_choixExport'] = form_process_checkboxes($form['exportation']['dfsiexport_choixExport']);
        
	$form['exportation']['#submit'][] = 'dfsiexport_export_form_submit';
        
	return $form;
}

function dfsiexport_export_form_submit($form, &$form_state) 
{
    
}

function dfsiexport_Import()
{
	$output = "";
    $output = array
    (
			'contentImport' => array
            (
                '#markup' => 'Import de données',
            ),
	);
	return $output;
}