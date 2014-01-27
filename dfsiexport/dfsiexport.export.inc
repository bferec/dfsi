<?php

// ------------------------------------------------
// @(#) dfsiexport.export.inc
// @(#) export des donnees
// @(#) (c) Amael Hervoche
// @(#) 24/01/2014
// ------------------------------------------------

function dfsiexport_Export()
{
    $path = current_path();

    $form['dfsiexport_choixExport'] = array
    (
        '#type' => 'checkboxes',
        '#title' => t("Eléments à exporter"),
        '#title_display'  => 'before',
        '#options' => array
        (
                1 => t('Containers et services'),
                2 => t('Dépendances (directes et indirectes)'),
                3 => t('Taxonomies (types de service et construteurs)')	
        ),
        '#default_value'  => variable_get('dfsiexport_choixExport', array(true,false,false)),
        '#value' => array(),
        '#attributes' => array()
    );	
    $form['dfsiexport_choixExport'] = form_process_checkboxes($form['dfsiexport_choixExport']);
        
    $form['actions']['Exporter'] = array
    (
    '#type' => 'submit',
    '#value' => t('Exporter'),
    '#weight' => 255,
    '#submit' => array('dfsiexport_export_form_submit'),

   );    
   return  $form;
}

function dfsiexport_form_submit($form, &$form_state) 
{
    dpm($form);
    
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