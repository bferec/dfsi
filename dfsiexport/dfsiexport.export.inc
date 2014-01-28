<?php

// ------------------------------------------------
// @(#) dfsiexport.export.inc
// @(#) export des donnees
// @(#) (c) Amael Hervoche
// @(#) 24/01/2014
// ------------------------------------------------


function dfsiexport_ExportForm( $form, &$form_state ) 
{
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
        '#default_value'  => array(1,3), //variable_get('dfsiexport_choixExport', array(1,2,3)),
        '#value' => array(),
        '#attributes' => array()
    );	
       
    $form['dfsiexport_choixExport'] = form_process_checkboxes($form['dfsiexport_choixExport']);
        
    
    
    $form['submit'] = array
    (
    '#type' => 'submit',
    '#value' => t('Exporter'),
    '#weight' => 255,
    '#submit' => array('dfsiexport_ExportForm_form_submit'),
   );    
   return  $form;
}

function dfsiexport_CreateExportFileName()
{
    $result = date("Ymd-h:m:s")."-"."dfsi"."-export.text";
    return $result;
}


function dfsiexport_ExportForm_form_submit($form, &$form_state) 
{
   $dfsi_ExportData = "test de fichier";
   $dfsi_export_ModeExport = "csv";
   $dfsi_fileName = dfsiexport_CreateExportFileName();
   $dfsi_ExportDestination = "temporary://{$fileName}";  
    $filePath = file_unmanaged_save_data($dfsi_ExportData, $dfsi_ExportDestination, $replace = FILE_EXISTS_REPLACE);
    
    file_transfer
    (
         $filePath, array
         (
               'Content-Type' => 'text/csv',
               'Content-Disposition' => 'attachment; $dfsi_fileName="'.$fileName.'"'
         )      
    );
    
    dsm("exportation achevée");
}

function dfsiexport_ExportForm_form_validate($form, &$form_state) 
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