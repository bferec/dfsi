<?php

// ------------------------------------------------
// @(#) dfsi.views.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");


// ------------------------------------------
// Hook_views_api
// ------------------------------------------
function dfsi_views_api() 
{
    return array
    (
        'api' => '3.0',
        'path' => drupal_get_path('module', 'view_actions') . '/views',
    );
}

// ------------------------------------------
// // Hook_views_pre_view
// ------------------------------------------
function dfsi_views_pre_view(&$view, &$display_id, &$args)
{
}

// ------------------------------------------
// Hook_views_post_render
// ------------------------------------------
function dfsi_views_post_render(&$view, &$output, &$cache)
{
    
}
// ------------------------------------------
// Hook_views_pre_render
// ------------------------------------------
function dfsi_views_pre_render(&$view)
{
    
    if ($view->name == 'services') 
    {
        // pour toutes les lignes du resultat de la vue
        $viewResult = $view->result;
        foreach($viewResult as $resultRow )
        {

        }
    }
}

// ------------------------------------------
// Hook_views_data_alter
// modifie les données d'une vue:
// ajoute un champ action dans la vue
// ------------------------------------------
function dfsi_views_data_alter(&$data) 
{
   
    $data['node']['actions'] = array
    (
        'title' => t('actions'),
        'help' => t("Lien clickable vers les actions qu'un utilisateur peur accomplir sur un noeud" ),
        'field' => array
            (
                'handler' => 'dfsi_views_handler_field_actions',
                'group' => 'Content',
                'click sortable' => FALSE,
            ),
    );
}

// ------------------------------------------
// Handler opur le champ actions
// Ajoute les icones edit/delete
// ------------------------------------------
class dfsi_views_handler_field_actions extends views_handler_field 
{
function render($values) 
{
        
    $actions = array();
    if (user_access('edit any content')) 
    {
        $alt = t('Modifier element');
        $actions[] = array
        (
            'name' => t('Edit'),
            'url' => 'node/' . $values->nid . '/edit',
            'icon' => theme('image', array('path' => drupal_get_path('module',_DFSI_MODULE_NAME) . '/pic/edit-icon.png', 'alt' => $alt)),
            'title' => $alt,
        );
    }
    if (user_access('delete any content')) 
    {
      $alt = t('Supprimer élément');
      $actions[] = array(
        'name' => t('Delete'),
        'url' => 'node/'  . $values->nid . '/delete',
        'icon' => theme('image', array('path' => drupal_get_path('module',  _DFSI_MODULE_NAME ) . '/pic/delete-icon.png', 'alt' => $alt)),
        'title' => $alt,
      );
    }
    $output = '';
    foreach ($actions as $action) 
    {
      $output .= l($action['icon'], $action['url'], array('attributes' => array('title' => $action['title']), 'html' => TRUE));
    }
    return $output;
  }
  /**
   *
   */
  function query() 
  {
  }
}