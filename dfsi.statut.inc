<?php

// ------------------------------------------------
// @(#) dfsi.statut.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                 // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }      // extension pour les fichiers include php

module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.api");

// -----------------------------------
// Acesseur du champ statut d'un noeud
// -----------------------------------
function dfsi_GetStatutOf( $node )
{
    $language = field_language('node', $node, 'field_statut', $langcode = NULL);
    return $node-> field_statut[$language][0]['value'];
}

// -------------------------------------------
// modifie le statut d'un element
// -------------------------------------------
function dfsi_SetStatut( $node , $statut )
{
    $language = field_language('node', $node, 'field_statut', $langcode = NULL);
    $node-> field_statut[$language][0]['value'] = $statut;
}

// -------------------------------------------------------------------
// renvoie le statut en fonction de l'état du groupe de dépendances
// -------------------------------------------------------------------
function dfsi_ComputeStatutFromGroup( $GroupNidsList, $modifiedNode )
{
    $result = _DFSI_WARNING_STATE;
    $AllDown = TRUE;
    $AllUp = TRUE;
    $GroupDependancesNodes = array();
    $GroupDependancesNodes = entity_load('node', $GroupNidsList  );   

    foreach($GroupDependancesNodes as $currentNode )
    {    
            // le noeud modifié doit être testé en mémoire
            // pas depuis le rechargement disque
            $nodeToTest = dfsi_GetNidOf($currentNode) == dfsi_GetNidOf($modifiedNode) ? $modifiedNode : $currentNode;
            //  pas tous ok
            if(  dfsi_GetStatutOf( $nodeToTest ) == _DFSI_ERROR_STATE )
            {
                    $AllUp = False;
            }
            //  pas tous erreur
            if( dfsi_GetStatutOf( $nodeToTest ) == _DFSI_OK_STATE )
            {
                    $AllDown = False;
            } 
    }
    // tous ok
    if( $AllUp == true )
    {
        $result = _DFSI_OK_STATE;
    }
    // tous en erreur
    else
    {
        if( $AllDown == true )
        {
           $result = _DFSI_ERROR_STATE;
        }
    }
    return $result;
    
}
// -----------------------------------------
// MAJ de tous les noeuds ayant 
// le noeud modifie dans leur groupe de dep.
// -----------------------------------------
function dfsi_UpdateStatutForGroupDependantOf( $node )
{      
    // trouver tous les noeuds ayant le noeud modifié à l'interieur de leur groupe de dependances
    $ImpactedNodes = dfsi_FindGroupDependantOf( $node );
    foreach( $ImpactedNodes as $ImpactedNode )
    {   
        $groupMembers = dfsi_GetGroupeDeDependancesOf($ImpactedNode );

        $statut = dfsi_ComputeStatutFromGroup
        (  
            $groupMembers,
            $node 
         );     
        dfsi_SetStatut($ImpactedNode , $statut );
        node_save($ImpactedNode);
    }
}

// ------------------------------------------------
// // mise à jour du statut des noeuds impactés
// // lors de la modification d'un noeud
// ------------------------------------------------
function dfsi_UpdateRecursiveStatut( $node ) 
{   
    $dependantNodesIdList = dfsi_GetDependancesOf( $node );
    dfsi_UpdateStatutForGroupDependantOf( $node );
    $dependantNodesList = entity_load( 'node', $dependantNodesIdList );
    // traitement des dependances directes
    foreach($dependantNodesList as $impactedNode)
    {
        dfsi_SetStatut($impactedNode , dfsi_GetStatutOf( $node ) );
        node_save($impactedNode);
        dfsi_UpdateRecursiveStatut( $impactedNode ) ;
    }
}
// ----------------------------------------------------
// mise à jour arborescente des statut ok/warning/error
// des service ou containers dependants
// ----------------------------------------------------
function dfsi_UpdateStatut( $node ) 
{       
    dfsi_UpdateRecursiveStatut( $node ) ;  
}

// -------------------------------------------------------------------
// renvoie le statut en fonction de l'état de TOUTES ses dependances
// -------------------------------------------------------------------
function dfsi_ComputeStatutFromAlldependances( $node )
{
     $result = _DFSI_OK_STATE;
     
     // dependances de groupe dont depends ce noeud
     $result = dfsi_ComputeStatutFromGroup( dfsi_GetGroupeDeDependancesOf( $node ), $node );
     
     // dependances directes dont depends ce noeud
     $nids = dfsi_GetAncestorsOf( $node ); 
     $ancestors = entity_load('node', $nids); 
     
    foreach ($ancestors as $ancestor )
    {
        // un seul en erreur suffit à mettre le moeud en erreur
       if(dfsi_GetStatutOf($ancestor) ==  _DFSI_ERROR_STATE )
       {
            $result = _DFSI_ERROR_STATE;
            break;
       }
       // warning prends le pas sur OK
       if(dfsi_GetStatutOf($ancestor) == _DFSI_WARNING_STATE &&  $result == _DFSI_OK_STATE)
       {
            $result = _DFSI_WARNING_STATE;
       }       
    }
   
    return $result;
}

// -----------------------------------
// doit on verrouiller le champ statut
// -----------------------------------
function dfsi_MustLockStatutOf($node)
{
    $result = false;    
    
    $AttendedStatut  = dfsi_ComputeStatutFromAlldependances( $node );
    // si ascendants sont en warning ou error, interdire de passer au statut OK
    // sinon on peut decider de passer au statut Error
    if( $AttendedStatut == _DFSI_WARNING_STATE || $AttendedStatut == _DFSI_ERROR_STATE )
    {
         $result = true;
    }
    return $result;
}

// -----------------------------------------
// renvoie l'icone correspondant à un statut
// -----------------------------------------
function dfsi_GetImgOfStatut($statut)
{
switch($statut)
    {
        case 'Ok':
        default:
            $img_name = variable_get('dfsi_ok_icon', "green_button.png");
        break;

        case 'Warning':
            $img_name = variable_get('dfsi_warning_icon', "orange_button.png");
        break;

        case 'Error':
            $img_name = variable_get('dfsi_error_icon', "red_button.png");
        break;    
    }    
    
    $img = '<img src="/' . drupal_get_path('module', 'dfsi') . '/pic/' . $img_name . '">';    
    
    return $img;
}

// --------------------------------
// modification directe du statut 
// par le lien dans la vue des elements
// --------------------------------
function dfsi_ModifyStatut($args)
{
    $node = node_load( $args );    
     if( ! dfsi_MustLockStatutOf($node) )
     {   
        $statut = dfsi_GetStatutOf( $node );
        switch( $statut )
        {
            case _DFSI_OK_STATE:
                dfsi_SetStatut( $node , _DFSI_ERROR_STATE );
                node_save( $node );
                break;

            case _DFSI_WARNING_STATE:
                break;

            case _DFSI_ERROR_STATE:
                dfsi_SetStatut( $node , _DFSI_OK_STATE );
                node_save( $node );
                break;
        }
        
     }
     // retour à la page appelante => liste service
     drupal_goto( $_SERVER['HTTP_REFERER'] );
     //drupal_goto( current_path() );
    return;
}

/*
 * 
 * 
 * --------------------------------
 * EN ATTENTE V2 - MULTIPLE STATUTS
 * --------------------------------
// ---------------------
// selection d'un statut
// ---------------------
function dfsi_Select_State( $libelle )
{
	$result = db_select(_DFSI_STATE_TABLE, 'st' )
            ->   fields
                 ( 
                         'st', 
                         array
                         ( 
                             'pid', 
                             'libelle' , 
                             'picture'
                         ) 
                 )
            -> condition( 'libelle', $libelle , "=" )
            -> execute()
            -> fetchAssoc() ; 
        return $result;
}
// ---------------------
// insertion d'un statut
// ---------------------
function dfsi_Insert_State($libelle,$picture )
{
    db_insert(_DFSI_STATE_TABLE)
      ->fields
        (
            array
            (
                'libelle' => $libelle, 
                'picture' => $picture
            )
        )
        ->execute();
}

// ---------------------
// maj d'un statut
// ---------------------
function dfsi_Update_State($pid , $libelle,$picture )
{
    db_update(_DFSI_STATE_TABLE)
      ->fields
        (
            array
            (
                'libelle' => $libelle, 
                'picture' => $picture
            )
        )
      ->condition('pid', $pid )
      ->execute();
}
// ---------------------
// suppression d'un statut
// ---------------------
function _dfsi_Delete_State($pid )
{
    db_delete(_DFSI_STATE_TABLE)     
      ->condition('pid', $pid )
      ->execute();
}

*/