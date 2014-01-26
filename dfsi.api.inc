<?php

// ------------------------------------------------
// @(#) dfsi.api.inc
// @(#) entries point for DFSI
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");

// ------------------------------------------------------------------------------------------------------
// dfsi Get() function  obtain item from element or list
// ------------------------------------------------------------------------------------------------------
//
// ----------------------------------
// Acesseur du champ nid d'un noeud
// ----------------------------------
function dfsi_GetNidOf( $node )
{    
    $wrapper = entity_metadata_wrapper('node', $node);
    return $wrapper-> nid->value();
}
// ----------------------------------
// Acesseur du champ title d'un noeud
// ----------------------------------
function dfsi_GetTitleOf( $node )
{
    $wrapper = entity_metadata_wrapper('node', $node);
    return $wrapper-> title->value();
}
// ----------------------------
// Acesseur du type d'un noeud
// ---------------------------
function dfsi_GetTypeOf( $node )
{
    $wrapper = entity_metadata_wrapper('node', $node);
    return $wrapper-> type->value();    
}
// ----------------------------
// Acesseur du language d'un noeud
// ---------------------------
function dfsi_GetLanguageOf($node)
{
    $wrapper = entity_metadata_wrapper('node', $node);
    return $wrapper-> language->value();      
}
// ------------------------------------
// accesseur d'acces aux ancetres
// d'un noeud
// ------------------------------------
function dfsi_GetAncestorsOf( $node )
{
    $wrapper = entity_metadata_wrapper('node' , $node);
    return $wrapper-> field_ancestor->raw();   
}
// ------------------------------------------------
// recherche de toutes les ancetres de dependance directe
// recursivement d'un noeud
// DONC TOUS LES NOEUDS DONT
// il EST DEPENDANT
// ------------------------------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                             
function dfsi_GetAllAncestorsOf( $node )
{   
    $Result = array();
   
    $nids = dfsi_GetAncestorsOf( $node ); 
    
    $Result = dfsi_load($nids); 
    
    foreach ($Result as $ChildNode )
    {
       $Result += dfsi_GetAllAncestorsOf( $ChildNode );
    }

    return $Result;
}
// ------------------------------------------------
// Accesseur du champ contenu d'un noeud
// ------------------------------------------------
function dfsi_GetContentOf( $node )
{
    $wrapper = entity_metadata_wrapper('node' , $node);
    return $wrapper-> field_contenu->raw();
}
// ---------------------------------
// recherche de toutes les contenus
// recursivement d'un container
// ---------------------------------
function dfsi_GetAllContentOfNode( $node )
{   
    $nids = dfsi_GetContentOf( $node ); 
    $result = dfsi_load($nids); 

    foreach ($result as $oneNode )
    {
       if(dfsi_GetTypeOf($oneNode) == _DFSI_CONTAINER )
       {
        $result += dfsi_GetAllContentOfNode( $oneNode );
       }
    }

    return $result;
}
// ------------------------------------------------
// accesseur du champ dependance d'un noeud
// ------------------------------------------------
function dfsi_GetDependancesOf( $node )
{
    $wrapper = entity_metadata_wrapper('node' , $node);
    return  $wrapper-> field_dependances->raw();  
}
// ------------------------------------------------
// recherche de toutes les dependances directe
// recursivement d'un noeud
// donc tous les noeuds DEPENDANTS
// ------------------------------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                             
function dfsi_GetAllDependanciesOfNode( $node )
{   
    $nids = dfsi_GetDependancesOf( $node ); 
    $result = dfsi_load( $nids ); 

    foreach ($result as $ChildNode )
    {
       $result += dfsi_GetAllDependanciesOfNode( $ChildNode );
    }

    return $result;
}
// ------------------------------------------------
// Accesseur du champ groupe de dependance d'un noeud
// ------------------------------------------------
function dfsi_GetGroupeDeDependancesOf( $node )
{
    $wrapper = entity_metadata_wrapper('node' , $node);
    return $wrapper-> field_groupe_de_dependances->raw(); 
}
// ------------------------------------------------
// recherche de toutes les groupe de dependances
// recursivement d'un noeud
// ------------------------------------------------
function dfsi_GetAllGroupDependanciesOfNode( $node )
{
       
    $nids = dfsi_GetGroupeDeDependancesOf( $node ) ;     
    $result = dfsi_load($nids); 
    foreach ($result as $ChildNode )
    {
       $result +=  dfsi_GetAllGroupDependanciesOfNode( $ChildNode );
    }
    return $result;
}
// ------------------------------------------------------------------------------------------------------
// dfsi Isin() function  test existence of id in list
// ------------------------------------------------------------------------------------------------------
//
// -------------------------------
// true if nid is present 
// in dependancies list of a node
// -------------------------------
function dfsi_IsInAncestorsOf( $nid, $node )
{
    $result = false;
    $ancestors = dfsi_GetAncestorsOf( $node );
    if( count($ancestors >0) )
    {
        $result = in_array($nid,$ancestors);
    }
    return $result;
}
// -------------------------------
// true if nid is present 
// in content list of a node
// -------------------------------
function dfsi_IsInContentOf( $nid, $node )
{
    $result = false;
    
    $contenus = dfsi_GetContentOf( $node );
    if( count($contenus) > 0 )
    {    
        $result = in_array($nid,$contenus);
    }
    return $result;
}
// -------------------------------
// true if nid is present 
// in dependancies list of a node
// -------------------------------
function dfsi_IsInDependanciesOf( $nid, $node )
{
    $result = false;
    
    $dependances = dfsi_GetDependancesOf( $node );
    if( count($dependances) > 0 )
    {
        $result = in_array($nid,$dependances);
    }
    return $result;
}
// ------------------------------------------------------------------------------------------------------
// dfsi Find() function  retrieve one or multiple element with criterion and computing result(s)
// ------------------------------------------------------------------------------------------------------
//
// -------------------------------------------
// recherche de tous les elements
// ayant le $nid fourni dans le champ ancestor
// -------------------------------------------
function dfsi_FindDependantOf( $node )
{
    $resultList = array();
    
    if( ! isset($node->nid) )
    {
        return $resultList;
    }

    //SELECT `field_ancestor_nid` 'nid' FROM `field_data_field_ancestor` WHERE `entity_id` = 170
    $result = db_select(  "field_data_field_ancestor"  , "g" )
    ->   fields
         ( 
            "g",
            array
            ( 
                "entity_id", 
            ) 
         )
    -> condition( 'field_ancestor_nid', dfsi_GetNidOf($node) , "=" )
    -> execute();

    
    $impacted_Nids = array();
    while( $impactedNode = $result->fetchAssoc() ) 
    {
        // ajouter aux resultats
        $impacted_Nids[] = $impactedNode['entity_id'];
    }        
    
    $resultList = dfsi_load( $impacted_Nids );
    return $resultList;      
}
// -------------------------------------------
// // recherche tous les noeuds dont depends un élément
// directement ou par groupe
// -------------------------------------------
function dfsi_FindDependFromOf( $node )
{
    $direct = dfsi_GetAllAncestorsOf( $node );
    $group  = dfsi_GetAllGroupDependanciesOfNode( $node );
    $result = array_merge($direct,$group);
    return $result;    
}
// -------------------------------------------
// recherche de tous les elements
// ayant le $nid fourni dans le groupe de dependances
// -------------------------------------------
function dfsi_FindGroupDependantOf( $node )
{
    //SELECT `field_groupe_de_dependances_nid` 'nid' FROM `field_data_field_groupe_de_dependances` WHERE `entity_id` = 129;
    $result = db_select(  "field_data_field_groupe_de_dependances"  , "g" )
    ->   fields
         ( 
            "g",
            array
            ( 
                "entity_id", 
            ) 
         )
    -> condition( 'field_groupe_de_dependances_nid', dfsi_GetNidOf($node) , "=" )
    -> execute();
    
    $impacted_Nids = array();
    while( $impactedNode = $result->fetchAssoc() ) 
    {
        // ajouter aux resultats
        $impacted_Nids[] = $impactedNode['entity_id'];
    }

    $resultList = dfsi_load( $impacted_Nids );
    return $resultList;      
}
//
// -------------------------------------------
// recherche de tous les noeuds figurants
// dans les meme groupe de dependance
// que le noeud fourni en parametre
// -------------------------------------------
function dfsi_FindSameGroupMemberOf( $node )
{
    $resultList = array();
    $idOfSameGroup = array();
    
    $sql = "SELECT DISTINCT `field_groupe_de_dependances_nid` nid FROM `field_data_field_groupe_de_dependances`";
    $sql.= " WHERE ";
    $sql.= "`entity_id` IN ";
    $sql.= " (SELECT `Entity_id` FROM `field_data_field_groupe_de_dependances` WHERE `field_groupe_de_dependances_nid` = '".dfsi_GetNidOf($node)."') ";
    $sql.= " AND ";
    $sql.= "`field_groupe_de_dependances_nid` <> '".dfsi_GetNidOf($node)."';";
    $sameGroupMembers = db_query($sql);
        
    while( $sameGroupMember = $sameGroupMembers->fetchAssoc() )
    {
        $idOfSameGroup[] = $sameGroupMember['nid'];
    }
    $resultList = dfsi_load( $idOfSameGroup );
    return $resultList;          
}
// ----------------------------
// recherche par nid ou titre
// renvoie le noeud
// $mode = _DFSI_TITLE | _DFSI_NID
// $value = value to search
// ----------------------------
function dfsi_FindElementBy($mode, $value)
{
    $field=`title`;
    $result = null;
    
    switch( $mode )
    {
        case _DFSI_TITLE:
        default:
            $field="title";
            break;
        
        case _DFSI_NID:
            $field="nid";
            break;
    }
        
    $search = db_select( "node", 'n' )
            ->   fields
                 ( 
                         'n', 
                         array
                         ( 
                             'nid', 
                             'title' , 
                         ) 
                 )
            -> condition( $field , $value , "=" )                
            -> condition( 'type', array(_DFSI_SERVICE,_DFSI_CONTAINER) , "IN" )
            
            -> execute()
            -> fetchAssoc() ; 
        if( isset($search['nid']))
        {
            $result = $search['nid'];
        }
       return $result;
}
//
// ------------------------------------------------------------------------------------------------------
// dfsi Add() function  Add an item to element or list
// ------------------------------------------------------------------------------------------------------
//
// -------------------------------
// add an nid to list of ancestors of
// a node, (list of nodes that current node
// depend from
// -------------------------------
function dfsi_AddAncestors( $nid,  $node )
{
    $wrapper = entity_metadata_wrapper('node' , $node);
 
    // if not in, then add
    if( ! dfsi_IsInAncestorsOf( $nid, $node ) )
    {
        $oldAncestors = $wrapper-> field_ancestor->raw();     
        $oldAncestors = array_merge($oldAncestors, array($nid) );
        $wrapper ->field_ancestor->set( $oldAncestors );
        $wrapper->save();        
    }
}
// -------------------------------
// add an nid to list of dependancies of
// a node, (list of nodes who are
// dependant from current node
// ONLY if not exist in list
// -------------------------------
function dfsi_AddDependancies($nid,  $node )
{
    $wrapper = entity_metadata_wrapper('node', $node);
    //$language = field_language('node', $node, 'field_dependances', $langcode = NULL);

    if( ! dfsi_IsInDependanciesOf( $nid, $node ) )
    {
        //$node -> field_dependances[$language][]['nid'] = $nid;
        $OldListe =  $wrapper-> field_dependances->raw();
                
        if( count($OldListe) > 0 )
        {
            if( ! in_array($nid, $OldListe ) )
            {
                   $OldListe[] = $nid;
            } 
        }
        else
        {
            $OldListe[] = $nid;
        }
        $wrapper-> field_dependances->set($OldListe);
        $wrapper->save();
    }
}
//
// -------------------------------
// add an nid to list of content of
// a container if not present
// -------------------------------
function dfsi_AddContent( $nid,  $node )
{
    // is it a container ?
    if( dfsi_GetTypeOf($node) == _DFSI_CONTAINER )
    {
        $wrapper = entity_metadata_wrapper('node' , $node);
        if( ! dfsi_IsInContentOf( $nid, $node ) )
        {
            $oldContenus = $wrapper-> field_contenu->raw();     
            $oldContenus = array_merge($oldContenus, array($nid) );
            $wrapper->field_contenu->set( $oldContenus );
            $wrapper->save();
        }
    }
}
//
// -------------------------------
// Adding term to taxonomy
// -------------------------------
function dfsi_AddTermToTaxonomy($taxonomyName , $name, $parentTermTid = null  )
{
    $vocabulaire = taxonomy_vocabulary_machine_name_load( $taxonomyName );
    $terme = new stdClass();
    $terme -> vid = $vocabulaire -> vid;
    $terme -> name = $name;
    if( $parentTermTid != null )
    {
        $terme -> parent =  $parentTermTid;
    }
    taxonomy_term_save($terme);
    return $terme;
}
// -------------------------------
// ajout de container
// -------------------------------
function dfsi_AddContainer( $title , $body , $statut , $criticity  , $language = LANGUAGE_NONE)
{
    global $user;
    
    $values = array
    (
        'type' => _DFSI_CONTAINER,
        'uid' => $user->uid,
        'status' => TRUE,
        'promote' => FALSE,
        'sticky' => FALSE,   
    );
    $entity = entity_create('node', $values);
    $wrapper = entity_metadata_wrapper('node', $entity);
    $wrapper->title->set($title);
    $wrapper->body->set(array('value' => $body));
    $wrapper->field_statut->set($statut);
    $wrapper -> field_criticite->set($criticity);    
    $wrapper->save();
    entity_save('node', $entity);     
    return $entity;
}
//
// -------------------------------
// ajout de service
// -------------------------------
function dfsi_AddService( $title , $body , $statut , $criticity  , $language = LANGUAGE_NONE)
{
    
    global $user;
    
    $values = array
    (
        'type' => _DFSI_SERVICE,'uid' => $user->uid,'status' => TRUE,'promote' => FALSE,'sticky' => FALSE,
    );
    $entity = entity_create('node', $values);
    $wrapper = entity_metadata_wrapper('node', $entity);
    $wrapper->title->set($title);
    $wrapper->body->set(array('value' => $body));
    $wrapper->field_statut->set($statut);
    $wrapper->field_service_final->set(FALSE);
    $wrapper -> field_criticite->set($criticity);
    $wrapper->save();
    entity_save('node', $entity);        
    return $entity;
}
// ------------------------------------------------------------------------------------------------------
// dfsi remove() functions  remove an item to element or list
// ------------------------------------------------------------------------------------------------------
function dfsi_RemoveFromAncestors($nid, $node)
{
    $wrapper = entity_metadata_wrapper('node' , $node);
    $current = $wrapper -> field_ancestor->raw();     
    $toRemove = array(dfsi_GetNidOf($node));
    $current = $wrapper -> field_ancestor->set( array_diff($current, $toRemove) );
    $wrapper -> save();
}
function dfsi_RemoveFromContent($nid, $node)
{
    $wrapper = entity_metadata_wrapper('node' , $node);
    $current = $wrapper -> field_contenu->raw();     
    $toRemove = array(dfsi_GetNidOf($node));
    $current = $wrapper-> field_contenu->set( array_diff($current, $toRemove) );
    $wrapper -> save();
}
function dfsi_RemoveFromDependancies($nid, $node)
{
    $wrapper = entity_metadata_wrapper('node' , $node);
    $current = $wrapper -> field_dependances->raw();     
    $toRemove = array(dfsi_GetNidOf($node));
    $current = $wrapper-> field_dependances->set( array_diff($current, $toRemove) );
    $wrapper -> save();
}
function dfsi_RemoveFromGroupDependancies($nid, $node)
{
    $wrapper = entity_metadata_wrapper('node' , $node);
    $current = $wrapper ->field_groupe_de_dependances ->raw();     
    $toRemove = array(dfsi_GetNidOf($node));
    $current = $wrapper ->field_groupe_de_dependances ->set( array_diff($current, $toRemove) );
    $wrapper -> save();
}
// -------------------------------
// Affectation de type 
// de service ou container
// -------------------------------
function dfsi_SetTaxonomyServiceType( $node , $typeName )
{
     $termes = taxonomy_get_term_by_name($typeName);
     $wrapper = entity_metadata_wrapper('node', $node);
     $language = field_language('node', $node, 'field_service_type', $langcode = NULL);
     foreach( $termes as $terme )   
     {
          //$node -> field_service_type[$language][]['tid'] = $terme->tid;
          $wrapper-> field_service_type->set($terme->tid);
          $wrapper->save();
     }  
}
// ------------------------------------------------------------------------------------------------------
// dfsi divers () 
// ------------------------------------------------------------------------------------------------------
function dfsi_load($nids)
{
    $result= array();
    if( count($nids)> 0)
    {

        if($nids[0] != NULL )
        {
            $result = entity_load( 'node', $nids );       
        }
    }
    return $result;
}

// --------------------------------------------
// 1 si utilisation des données complémentaires
// 0 sinon
// --------------------------------------------
function dfsi_ComplementaryDataEnabled()
{
    return variable_get("dfsi_use_chunk",_DFSI_CHUNK_DATA_DISABLED) != _DFSI_CHUNK_DATA_DISABLED ;
}


// ------------------------------------------------
// recherche des noms de types de contenus existant
// pour un detail associé à un terme de contenu
// ------------------------------------------------
function FindDetailContentTypeNames( $node )
{
   $result = array();
   $existingTypeNames = array();
   $detail_terme_names = array();
   //recherche des contenus de detail existants
   $sqlRequest = db_select(_DFSI_DETAIL_TABLE, 'dt' )
    ->fields
         ( 
                 'dt', 
                 array
                 ( 
                     'did',
                     'type'
                 ) 
         )
    -> execute();
   while( $typeName = $sqlRequest-> fetchAssoc() )
   {
       $existingTypeNames[] = $typeName['type'];
   }   
   if( count($existingTypeNames) > 0  )
   {  
        // recuperer tous les champs du noeud
        $wrapper = entity_metadata_wrapper('node', $node);
        $fields = field_info_instances("node",dfsi_GetTypeOf($node) );

        // pour chaque champ
        foreach( $fields as $fieldName => $field)
        {      
            $label = $field['label'];
            $fieldInfo = field_info_field($field['field_name']);
            $fieldType = $fieldInfo['type'];
            // si terme de taxonomie
            if( $fieldType == 'taxonomy_term_reference')
            {
                $terms = $wrapper->$fieldName->value();
                // terme de taxonomie monovalué
                if( isset($terms -> tid) )
                {

                    $tid = $terms -> tid;
                    $vid = $terms -> vid;
                    $ctname = $vid.$tid."_detail";
                    // in existe un contenu de détail pour ce terme
                    if( in_array($ctname,$existingTypeNames))
                    {
                             $detail_terme_names[$terms->name]['ctname'] =  $ctname;
                             $detail_terme_names[$terms->name]['termName'] =  $terms->name;
                             $detail_terme_names[$terms->name]['vid'] =  $vid;
                             $detail_terme_names[$terms->name]['tid'] =  $tid;
                             $detail_terme_names[$terms->name]['label'] =  $label;
                    }
                }
                // terme de taxonomie multivalué
                else if ( $terms != NULL )
                {
                    foreach($terms as $term )
                    {
                        if($term != NULL )
                        {
                             $tid = $term -> tid;
                             $vid = $term -> vid;                   
                             $ctname = $vid.$tid."_detail";
                             // in existe un contenu de détail pour ce terme
                             if( in_array($ctname,$existingTypeNames))
                             {
                                 $detail_terme_names[$term->name]['ctname'] =  $ctname;
                                 $detail_terme_names[$term->name]['termName'] =  $terms->name;                                 
                                 $detail_terme_names[$term->name]['vid'] =  $vid;
                                 $detail_terme_names[$term->name]['tid'] =  $tid;                                 
                                 $detail_terme_names[$term->name]['label'] =  $label;                        
                             }
                        }
                    }
                }               
            }       
        }    
    }
   $result = $detail_terme_names;  
   return $result;
}
