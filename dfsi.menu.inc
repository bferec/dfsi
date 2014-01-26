<?php

// ------------------------------------------------
// @(#) dfsi.menu.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                      // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }           // extension pour les fichiers include php

module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.api");
module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.details");
// ----------------------------------------------------------
// Hook_menu
// ----------------------------------------------------------
// ajouter  menu de configuration dans rubrique developpement
// ajoute lien vers basculement statut
// ajoute lien vers page synthese de dependance des noeuds
// ----------------------------------------------------------
function dfsi_menu() 
{
    $items = array();

    if( dfsi_ComplementaryDataEnabled() )
    {
        $items['taxonomy/term/%taxonomy_term/Chunk'] = array
        (
            'title' => t("Données complémentaires") ,
            'type' => MENU_LOCAL_TASK,
            'page callback' => 'dfsi_DetailsDatasTypePage',    
            'access callback' => TRUE,    
            'page arguments' => array(2),
            'file' => 'dfsi.details.inc',
            'weight' => 100,         
        );
       $items['taxonomy/term/%taxonomy_term/Chunk/Edit'] = array
        (   
            'title' => t("Modifier"),
            'type' => MENU_LOCAL_ACTION,
            'page callback' => 'dfsi_EditDetailComplementType',   
            'access callback' => TRUE,    
            'access arguments' => array(_DFSI_PERM_UPDATE_CHUNK_DATA_TYPE),            
            'page arguments' => array(2),
            'file' => 'dfsi.details.inc',

        );
        $items['taxonomy/term/%taxonomy_term/Chunk/Delete'] = array
        (   
            'title' => t("Supprimer"),
            'type' => MENU_LOCAL_ACTION,
            'page callback' => 'dfsi_DeleteDetailComplementType',   
            'access callback' => TRUE,    
            'access arguments' => array(_DFSI_PERM_DELETE_CHUNK_DATA_TYPE),
            'page arguments' => array(2),
            'file' => 'dfsi.details.inc',

        );    
    }
    // ajout d'onglets sur page de visualisation du noeud
    // menu local  task
    $items['node/%node/Dependants'] = array
    (
        'title' => t("Elements dépendants..." ),
        'type' => MENU_LOCAL_TASK,
        'page callback' => 'dfsi_elementsDependantsPage',    
        'access callback' => TRUE,        
        'page arguments' => array(1),
        'file' => 'dfsi.dependances.inc',
        'weight' => 100,         
    );
    
    // ajout d'onglets sur page de visualisation du noeud
    // menu local  task    
    $items['node/%node/DependsDe'] = array
    (
      'title' => t('Dépends de...'),
      'type' => MENU_LOCAL_TASK,
      'page callback' => 'dfsi_dependsDePage',  
      'access callback' => TRUE,
      'page arguments' => array(1),
      'file' => 'dfsi.dependances.inc',
      'weight' => 110,  
    );
    // ajout d'onglets sur page de visualisation du noeud
    // menu local  task    
    $items['node/%node/Contient'] = array
    (
      'title' => t('Contient...'),
      'type' => MENU_LOCAL_TASK,
      'page callback' => 'dfsi_ContientPage',  
      'access callback' => TRUE,
      'page arguments' => array(1),
      'file' => 'dfsi.dependances.inc',
      'weight' => 120,  
    );    
    $items['admin/config/dfsi'] = array
    (
        'title' => _MODULE_COMMERCIAL_NAME,
        'description' => t('Configurer ')._MODULE_COMMERCIAL_NAME,
        'position' => 'right',
        'weight' => -15,
        'page callback' => 'system_admin_menu_block_page',
        'access arguments' => array(_DFSI_PERM_CONFIG),
        'file' => 'system.admin.inc',
        'file path' => drupal_get_path('module', 'system'),
    );

    // url de configuration de l'application
    $items['admin/config/dfsi/settings'] = array
    (
        'title' => t('Configuration ')._MODULE_COMMERCIAL_NAME,
        'description' => t("Jeu de test et paramètres de l'appplication"._MODULE_COMMERCIAL_NAME ),
        'access arguments' => array(_DFSI_PERM_CONFIG),
        'page callback' => 'drupal_get_form',
        'page arguments' => array('dfsi_settings'),
        'file' => 'dfsi.admin.inc'
    );    

 
    // lien permettant de desactiver un noeud
    $items['modifyStatut/%'] = array
    (
        'access arguments' => array(_DFSI_PERM_CHANGE_NODE_STATUT),
        'page callback' => 'dfsi_ModifyStatut',
        'page arguments' => array(1),
        'file' => 'dfsi.module'
    );
       
    $items['Noeuds_non_dépendants'] = array
    (
    'title' => t("Synthèse de dépendances"),
    'page callback' => 'dfsi_NoDependantList',
    'access callback' => TRUE,
    );
     
    // detail/noeud/type de contenu
    $items['detail/%/%'] = array
    (
        'access arguments' => array(_DFSI_PERM_CHANGE_NODE_STATUT),
        'page callback' => 'dfsi_ModifyStatut',
        'page arguments' => array(1),
        'file' => 'dfsi.module'
    );
    
    return $items;  
}


// ---------------------------------
// creation d'un sous menu localtask
// ---------------------------------
function dfsi_CreateMenuElement($title,$href)
{
    $result = array();
    $result = array 
    (
        '#theme' => 'menu_local_task',
        '#link' => array
        (
            'title' => t($title),
            'href' => $href,
            'localized_options' => array
            (
                'attributes' => array
                (
                    'title' => t($title),
                ),
            ),
        ),
    );
    return $result;
}

// -----------------------------------------
// sous menu lié à l'affichage d'un element
// -----------------------------------------
function dfsi_AddNodeSubMenuElement(&$data, $router_item, $root_path)
{
    
    // noeud en cours de visualisation
    $node = $router_item['page_arguments'][0];
    
    // sous menu d'un noeud : ajouter Container
    $data['actions']['output'][] = dfsi_CreateMenuElement( t('Ajouter un container') , 'node/add/itcontainer' );
    // sous menu d'un noeud : ajouter service
    $data['actions']['output'][] = dfsi_CreateMenuElement( t('Ajouter un service') , 'node/add/itservice' );
    // sous menu d'un noeud : Modifier élément
    $label = t("Modifier [@title]",  array('@title' => dfsi_GetTitleOf($node)) );   
    $data['actions']['output'][] = dfsi_CreateMenuElement( $label , $data['tabs'][0]['output'][0]['#link']['tab_root_href'] .'/edit' );
    // sous menu d'un noeud : supprimer élément
    $label = t("Supprimer [@title]",  array('@title' => dfsi_GetTitleOf($node)) );
    $data['actions']['output'][] = dfsi_CreateMenuElement($label,$data['tabs'][0]['output'][0]['#link']['tab_root_href'] .'/delete');    
    // sous menu d'un noeud : retour à la liste des services
    $data['actions']['output'][] = dfsi_CreateMenuElement( t('Retour à la liste'),'Liste-des-services');    
}

//  ---------------------------------
//  retrait d'un onglet en fonction
//  du chemin d'execution
//  ---------------------------------
function dfsi_RemoveMenuItem($mode , &$data, $path)
{
    switch( $mode )
    {
         // menu actions
        case MENU_LOCAL_ACTION:
            if( isset($data['actions']['output'] ))
            {
                // nombre d'entrees de menu
                $nbEntry = count($data['actions']['output']);
                // pour tous les entrees de menu
                for( $x = 0; $x < $nbEntry ; $x ++ ) 
                {
                    if( $data['actions']['output'][$x]['#link']['path'] == $path)
                    {
                        array_splice($data['actions']['output'], $x, 1);
                        $data['actions']['count'] --;
                        break;
                   }
                } 
            }
            break;
            
        // menu tabs
        case MENU_LOCAL_TASK:
        default:
            // existe il des onglets ?
            if( isset($data['tabs'][0]['output'] ))
            {
                // nombre d'entrees de menu
                $nbEntry = count($data['tabs'][0]['output']);
                // pour tous les entrees de menu
                for( $x = 0; $x < $nbEntry ; $x ++ ) 
                {

                    if( $data['tabs'][0]['output'][$x]['#link']['path'] == $path)
                    {
                        array_splice($data['tabs'][0]['output'], $x, 1);
                        $data['tabs'][0]['count'] --;
                        break;
                   }
                } 
            }
        break;
    }
    
}

//  -------------------------------------
//  Hook_menu_local_task_alter
//  modification des onglets menu_local_task
//  affichés pour un noeud
//  -------------------------------------
function dfsi_menu_local_tasks_alter (&$data, $router_item, $root_path ) 
{         
    // sur l'affichage d'un element de type noeud, 
    $NodesPath = array("node/%","node/%/edit", "node/%/Dependants","node/%/DependsDe","node/%/Contient");
    
    if( in_array($root_path, $NodesPath ) )
    {   
        // un noeud es affiché, ou en cours d'ajout ou de modification
        $node = $router_item['page_arguments'][0];
         //
        // container ou service ? modifier les menus localtask
        if( dfsi_GetTypeOf( $node ) == _DFSI_CONTAINER || dfsi_GetTypeOf( $node ) == _DFSI_SERVICE )
        {
            // afficher le sous menu de gestion des noeuds 
            // seulement en visualisation  de container ou service         
            dfsi_AddNodeSubMenuElement( $data, $router_item, $root_path);  
            // retrait des options de livres pour les containers et services
            dfsi_RemoveMenuItem( MENU_LOCAL_TASK , $data, 'node/%/outline' );
            // retirer onglet edit (modifier)
            dfsi_RemoveMenuItem( MENU_LOCAL_TASK , $data, 'node/%/edit' );
        }
        // element n'est pas un service ou container dfsi, 
        // ôter onglets de dependances
        else
        {
            dfsi_RemoveMenuItem( MENU_LOCAL_TASK , $data, 'node/%/Dependants' );
            dfsi_RemoveMenuItem( MENU_LOCAL_TASK , $data, 'node/%/DependsDe' );
        }
        // retirer l'onglet de contenu pour tout 
        // ce qui n'est pas un container
        if(dfsi_GetTypeOf( $node ) != _DFSI_CONTAINER )
        {
            dfsi_RemoveMenuItem( MENU_LOCAL_TASK , $data, 'node/%/Contient' );
        }
    } 
    // on est dans l'onglet des types de données complémentaires
     else if(dfsi_ComplementaryDataEnabled() && $root_path == 'taxonomy/term/%/Chunk')     
    {
        $did = -1;
        $type= '';
        // chargement de la relation si existante.
        $relation = dfsi_ReadTermContentTypeRelation
                    (
                        $did,
                        $type,
                        $router_item['page_arguments'][0]->vid ,  
                        $router_item['page_arguments'][0]->tid 
                    );
        // pas de relation existante pour ce terme et type
        // de contenu, oter Edit et Delete
        if( $relation ==  NULL )
        {
            dfsi_RemoveMenuItem( MENU_LOCAL_ACTION , $data, 'taxonomy/term/%/Chunk/Edit' );
            dfsi_RemoveMenuItem( MENU_LOCAL_ACTION , $data, 'taxonomy/term/%/Chunk/Delete' );
        }
        // relation existante pour ce terme et type
        // de contenu, oter ajouter
        else
        {
            dfsi_RemoveMenuItem( MENU_LOCAL_ACTION , $data, 'taxonomy/term/%/Chunk/Add' );
        }
    } 
    else
    {
    }
}
