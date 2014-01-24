<?php

// ------------------------------------------------
// @(#) dfsi.container.inc
// @(#) 11/12/2013
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php


module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");
module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.api");

// ---------------------------------------
// recherche le(s) container(s) d'un noeud
// return all entities that contain this node
// ---------------------------------------
function dfsi_FindContainerOf( $node )
{
    //SELECT entity_id FROM `field_data_field_contenu` WHERE `field_contenu_nid` = 216    
    $result = db_select(  "field_data_field_contenu"  , "c" )
    ->   fields
         ( 
            "c",
            array
            ( 
                "entity_id", 
            ) 
         )
    -> condition( 'field_contenu_nid', $node->nid , "=" )
    -> execute();

    
    $container_Nids = array();
    while( $containerNode = $result->fetchAssoc() ) 
    {
        // ajouter aux resultats
        $container_Nids[] = $containerNode['entity_id'];
    }

    $resultList = entity_load( 'node', $container_Nids );
    return $resultList;      
    
}


// ----------------------------------------------
// returne true si l'element se contient lui meme 
// ----------------------------------------------
function dfsi_IsContainerError($node)
{
    $result = false;
  
    $allContent = dfsi_GetAllContentOfNode( $node );          
    // boucle dans les dependances detectées
    foreach ($allContent as $content)
    {
        if( $node -> nid == $content-> nid && $node -> nid != NULL && $content->nid != NULL )
        {
            variable_set( _DFSI_ERROR_MSG , "Erreur détectée dans les élements contenus : réflexivité de contenu..." );
            $result = true;
            break;
        }
    }
    
    return $result;
}

// -----------------------------------
// // test container contenu incorrect
// -----------------------------------
function dfsi_validate_DFSI_CONTAINER($node , $form , $form_state  ) 
{   
    $dfsi_Error = dfsi_IsContainerError($node) ;
    
    if( $dfsi_Error == true  )
    {
        form_set_error('field_contenu', variable_get( _DFSI_ERROR_MSG , "Ok" ) );
    }       
}