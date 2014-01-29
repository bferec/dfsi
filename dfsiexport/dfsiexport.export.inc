<?php

// ------------------------------------------------
// @(#) dfsiexport.export.inc
// @(#) export des donnees
// @(#) (c) Amael Hervoche
// @(#) 24/01/2014
// ------------------------------------------------


function dfsiexport_ExportFormGetOptions()
{
    return array
        (
                1 => t('Containers et services'),
                2 => t('Dépendances (directes et indirectes)'),
                3 => t('Taxonomies (types de service et construteurs)')	
        );
}

// callback ajax 
function ajax_options_callback( $form, &$form_state ) 
{
    
    
    // TODO ne fonctionne pas
    if( $form['dfsiexport_choixExport'][2]['#checked'] == true )
    {
        $form['dfsiexport_choixExport'][1]['#checked'] = true;
        $form['dfsiexport_choixExport'][1]['#default_value'] = true;
    }
}

function dfsiexport_ExportForm( $form, &$form_state ) 
{
    $form['dfsiexport_choixExport'] = array
    (
        '#type' => 'checkboxes',
        '#title' => t("Eléments à exporter"),
        '#title_display'  => 'before',
        '#options' => dfsiexport_ExportFormGetOptions(),
        '#ajax' => array(
            'callback' => 'ajax_options_callback',
            'wrapper' => 'options-form',
            'event' => 'change',
            'method' => 'replace',
          ),        
        '#value' => array(),
        '#attributes' => array(),
        '#prefix' => '<div id="options-form">',
        '#suffix' => '</div>',        
    );	
       
    $form['dfsiexport_choixExport'] = form_process_checkboxes($form['dfsiexport_choixExport']);
            
    $form['submit'] = array
    (
    '#type' => 'submit',
    '#value' => t('Exporter'),
    '#weight' => 255,
    '#submit' => array('dfsiexport_ExportForm_form_submit'),
   );    
    
   dpm($form_state);
   return  $form;
}

function dfsiexport_CreateExportFileName()
{
    $result = date("Ymd-h-m-s")."-"."dfsi"."-export.text";
    return $result;
}


function dfsiexport_ExportForm_form_submit($form, &$form_state) 
{
       
   $choix_export = array(); 
   // mise à jour des types d'export preferes
    
    for ($i = 1; $i <= 3; $i++) 
    {
        if( $form['dfsiexport_choixExport'][$i]['#checked'] == true )
        {
            $choix_export[] = $i;
        }
    }
   variable_set('dfsiexport_choixExport', $choix_export );
   
   $dfsi_ExportData = "test de fichier";
   $dfsi_export_ModeExport = "csv";
   $dfsi_fileName = dfsiexport_CreateExportFileName();
   $dfsi_ExportDestination = "temporary://{$dfsi_fileName}";  
   $filePath = file_unmanaged_save_data($dfsi_ExportData, $dfsi_ExportDestination, $replace = FILE_EXISTS_REPLACE);
    file_transfer
    (
         $filePath, array
         (
               'Content-Type' => 'text/csv',
               'Content-Disposition' => 'attachment; fileName="'.$dfsi_fileName.'"'
         )      
    );
  
    // rien ne sera exécuté ici car appel à drupal_exit();
    //dpm(file_delete($filePath, $force = TRUE));
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