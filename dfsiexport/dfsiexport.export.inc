<?php

// ------------------------------------------------
// @(#) dfsiexport.export.inc
// @(#) export des donnees
// @(#) (c) Amael Hervoche
// @(#) 24/01/2014
// ------------------------------------------------




// ---------------------------------------
// callback ajax 
// appelé sur changement d'etat des cases
// à cocher
// export dependances implique export noeuds
// ---------------------------------------
function dfsiexport_Ajax_CheckBoxesOptionsCallback( $form, &$form_state ) 
{
    if( $form['dfsiexport_choixExport'][2]['#checked'] == true )
    {
        $form['dfsiexport_choixExport'][1]['#checked'] = true;
    }
    return $form['dfsiexport_choixExport'];
}

// ---------------------------------------
// renvoie le tableau associatif des options
// possibles d'éléments à exporter
// ---------------------------------------
function dfsiexport_ExportFormGetOptions()
{
    return array
        (
                1 => t('Containers et services'),
                2 => t('Dépendances (directes et indirectes)'),
                3 => t('Taxonomies (types de service et construteurs)')	
        );
}

// ---------------------------------------
// // formulaire exportation
// ---------------------------------------
function dfsiexport_ExportForm( $form, &$form_state ) 
{
    $form['dfsiexport_choixExport'] = array
    (
        '#type' => 'checkboxes',
        '#title' => t("Eléments à exporter"),
        '#title_display'  => 'before',
        '#options' => dfsiexport_ExportFormGetOptions(),
        '#ajax' => array
        (
            'callback' => 'dfsiexport_Ajax_CheckBoxesOptionsCallback',
            'wrapper' => 'CheckBoxesOptions',       // nom de la div à reafficher en Ajax
            'event' => 'change',                    // event declencheur
            'method' => 'replace',                  // que faire de la div
            //'effect' => 'slide',                  // effet visuel
            'progress' => array('type' => 'none'),  // pas de barre de progression
        ),        
        '#value' => array(),
        '#attributes' => array(),
        '#prefix' => '<div id="CheckBoxesOptions">',
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
    
   return  $form;
}

// ---------------------------------------
// Génère le nom de fichier pour
// l'exportation
// ---------------------------------------
function dfsiexport_CreateExportFileName()
{
    $result = date("Ymd-h-m-s")."-"."dfsi"."-export.txt";
    return $result;
}

// ---------------------------------------
// sauvegarer les type de données à exporter
// cochées
// ---------------------------------------
function dfsiexport_SaveExportOptions($form)
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
   
   return $choix_export;
}


// ---------------------------------------
// lecture des données à exporter
// ---------------------------------------
function dfsiexport_GetDataToExport($DataChoiceToExport = array() )
{
    $result = array();
    
    $query = new EntityFieldQuery();
    $query-> entityCondition('entity_type', 'node')
    ->entityCondition('bundle', array('itservice','itcontainer') )            
    ->propertyCondition('status', 1);
    
    $result = $query->execute();
    if (isset($result['node'])) 
    {
        $items_nids = array_keys($result['node']);
        $result = entity_load('node', $items_nids);
    }
    
    return $result;
}

// ---------------------------------------
// transformation des données brutes
// en fonction du type d'exportation
// ---------------------------------------
function dfsiexport_GetStructuredDataToExport($ModeExport, $ExportData)
{
    $result = "";
    
    $itServiceFields = field_info_instances('node',_DFSI_SERVICE);
    $itContainerFields = field_info_instances('node',_DFSI_CONTAINER);
   
    
    
    foreach ( $itServiceFields as $key => $itServiceField ) 
    {
        //dpm( );
    }
    // TODO    
    switch( $ModeExport )
    {
        // construire csv
        case _DFSIEXPORT_MODE_EXPORT_CSV:
            foreach($ExportData as $key =>  $oneNode )
            {
                //dpm($oneNode);
                
                $result .=  "{$key};";
                if( $oneNode-> type == _DFSI_SERVICE)
                {
                    foreach( $itServiceFields as $key => $itServiceField )
                    {
                        
                        $fieldName = $itServiceField['field_name'];
                        $wrapper = entity_metadata_wrapper('node', $oneNode);
                        $fieldValue = $wrapper->$fieldName->value();
                        
                        if(  gettype ($fieldValue) == 'object')
                        {
                            dpm($itServiceField);
                        }
                        else if(  gettype ($fieldValue) == 'array')
                        {
                            dpm($itServiceField);
                        }
                        else if( $fieldValue == NULL )
                        {
                            
                        }
                        else 
                        {
                           $result .=  '"'.$fieldValue.'"'.',';
                        }
                        
                    }
                    $result .="\r\n";
                }                
            }

        break;

        case _DFSIEXPORT_MODE_EXPORT_XML:
            // construire Xml
        break;
    }
    
    return $result ;
}


// ---------------------------------------
// traitement du submit export
// ---------------------------------------
function dfsiexport_ExportForm_form_submit($form, &$form_state) 
{
       
   $dfsiexport_DataChoiceToExport = dfsiexport_SaveExportOptions($form);
   
   $dfsiexport_ExportData = dfsiexport_GetDataToExport($dfsiexport_DataChoiceToExport);     

   // recuperation du mode exportation
   $dfsiexport_ModeExport = variable_get('dfsiexport_Preferred_ExportMode', _DFSIEXPORT_MODE_EXPORT_CSV );
   
   // transformation de données brutes lues en données csv ou Xml
   $dfsiexport_StructuredExportData = dfsiexport_GetStructuredDataToExport($dfsiexport_ModeExport, $dfsiexport_ExportData);
   
   // construction du nom et chemin de ficheir d'export
   $dfsiexport_fileName = dfsiexport_CreateExportFileName();
   
    $dfsexport_ExportDestination = "temporary://{$dfsiexport_fileName}";  
   
   // sauvegarde du fichier d'export
   $filePath = file_unmanaged_save_data($dfsiexport_StructuredExportData, $dfsexport_ExportDestination, $replace = FILE_EXISTS_REPLACE);
   
   // download navigateur
   // a tester sous IE qui bogue peut être...   

    file_transfer
    (
         $filePath, array
         (
               'Content-Type' => 'text/csv',
               'Content-Disposition' => 'attachment; fileName="'.$dfsiexport_fileName.'"'
         )      
    );
    // rien ne sera exécuté ici car appel à drupal_exit(),  pas moyen d'effecer le fichier temporaire ici !!!!
}
// ---------------------------------------
// validation du formulaire d'export
// rien à faire dans l'immédiat...
// ---------------------------------------
function dfsiexport_ExportForm_form_validate($form, &$form_state) 
{
    
}
    
// ---------------------------------------
// ---------------------------------------
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