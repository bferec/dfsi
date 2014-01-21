<?php

// ------------------------------------------------
// @(#) dfsi.admin.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_MODULE_NAME") ) { define( "_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_INCLUDE_EXTENSION") ) { define( "_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

module_load_include( _INCLUDE_EXTENSION , _MODULE_NAME , "dfsi.define");

// -----------------
// Hook_settings
// -----------------
/*
function dfsi_Export_settings()
{
    $dfsi_Preferred_ExportMode=variable_get("dfsi_Preferred_ExportMode", 1 );

        $form['modeExport'] = array
        (
            '#type' => 'radios',
            '#title' => t("Mode d'exportation"),
            '#options' => array
            (
                1 =>t('Csv'),
                2 =>t('Xml'),
            ),
            '#default_value' => $dfsi_Preferred_ExportMode,
            
        );
    $form['#submit'][] = 'dfsi_Export_settings_form_submit';
    return system_settings_form($form);
}

  */
 
function dfsi_settings() 
{ 

    $form['Jeux de test'] = array
    (
        '#type' => 'fieldset',
        '#title' => t('Jeux de test'),
        '#description' => t('Génération ou suppression de données de test.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE
    );
    
    if( variable_get('dfsi_generate_test_data#Disabled',FALSE) == FALSE )
    {        
        $form['Jeux de test']['dfsi_generate_test_data'] = array
        (
            '#type' => 'radios',
            '#title' => t('Eléments de test'),
            '#options' => array
            (
                t('Générer'),
                t('Ne rien faire'),
            ),
            '#default_value' => variable_get('dfsi_generate_test_data',2),
        );
    }
    else
    {
        $form['Jeux de test']['dfsi_generate_test_data'] = array
        (
            '#type' => 'radios',
            '#title' => t('Eléments de test'),
            '#options' => array
            (
                t('Supprimer'),
                t('Ne rien faire'),
            ),
            '#default_value' => variable_get('dfsi_generate_test_data',1),
        );    
    }
    
    $form['Jeux de test']['Executer'] = array
    (
        '#type' => 'submit',
        '#value' => 'Exécuter',
    );
              
    $form['Statuts'] = array
    (
        '#type' => 'fieldset',
        '#title' => t('Statuts'),
        '#description' => t('Paramètrage des états possible pour une élément.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE
    );
    
    $form['Statuts']['dfsi_ok_icon'] = array
    (
        '#type' => 'textfield',
        '#title' => t("Statut OK/Actif"),
        '#description' => t(""),
        '#default_value' => variable_get('dfsi_ok_icon', "green_button.png")
    );
    
    $form['Statuts']['dfsi_warning_icon'] = array(
        '#type' => 'textfield',
        '#title' => t("Statut Warning"),
        '#description' => t(""),
        '#default_value' => variable_get('dfsi_warning_icon', "orange_button.png")
    );
    $form['Statuts']['dfsi_error_icon'] = array
    (
        '#type' => 'textfield',
        '#title' => t("Statut Erreur/Inactif"),
        '#description' => t(""),
        '#default_value' => variable_get('dfsi_error_icon', "red_button.png")
    );
    
    $form['Données complémentaires'] = array
    (
        '#type' => 'fieldset',
        '#title' => t('Données complémentaires'),
        '#description' => t('Gestion des données spécifique à type de service'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE
    );
    
    $form['Données complémentaires']['dfsi_use_chunk'] = array
    (
        '#type' => 'radios',
        '#title' => t('Gestion des donnés complémentaires'),
        '#options' => array
        (
            1 => t('Activer'),
            0 => t('Désactiver'),
        ),
        '#default_value' => variable_get("dfsi_use_chunk",0),
    );    
    
    $form['#submit'][] = 'dfsi_settings_form_submit';
    
        
  return system_settings_form($form);
}

// -----------------
// Hook_validate
// -----------------
function dfsi_settings_validate($form, &$form_state) 
{
}


/*
function dfsi_Export_settings_form_submit($form, &$form_state) 
{    
    $dfsi_choosed_mode = $form_state['values']['modeExport'];
    //dpm($dfsi_choosed_mode);

    variable_set("dfsi_Preferred_ExportMode", $dfsi_choosed_mode );

}
*/
// -----------------
// Hook_submit
// -----------------
function dfsi_settings_form_submit($form, &$form_state) 
{    
    variable_set("dfsi_use_chunk",$form['Données complémentaires']['dfsi_use_chunk']['#value']);
    drupal_flush_all_caches();
    cache_clear_all();
    $msg = "Gestion des donnnées complémentaire ";
    $msg .= dfsi_ComplementaryDataEnabled() ? "activée" : "désactivée";
    dsm(t($msg) );
    
    module_load_include( _INCLUDE_EXTENSION , _MODULE_NAME , "dfsi.testData" );

    switch( $form_state['values']['dfsi_generate_test_data'] )
    {
        case "0":        

            if  ( $form_state['clicked_button']['#id'] == "edit-executer"  )
            {
                if( variable_get('dfsi_generate_test_data#Disabled',FALSE) == FALSE )
                {
                    variable_set('dfsi_sem_Update_Statut' , TRUE );
                    dfsi_create_Test_Data();
                    variable_set('dfsi_generate_test_data#Disabled', TRUE);
                    variable_set('dfsi_sem_Update_Statut' , FALSE );
                }
                else 
                {
                    variable_set('dfsi_sem_Update_Statut' , TRUE );                   
                    dfsi_Delete_Test_Data();
                    variable_set('dfsi_generate_test_data#Disabled', FALSE);
                    variable_set('dfsi_sem_Update_Statut' , FALSE );                    
                }
            }    
         break;
    
        case "1":   
        break;
    
        default:                
        break;
    }
    
}

