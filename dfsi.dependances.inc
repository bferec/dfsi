<?php

// ------------------------------------------------
// @(#) dfsi.dependances.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");
module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.api");

// recherche 
// -----------------------------------------
// test ref circulaire ou reflexive dans les 
// dependances ou groupe de dependance
// -----------------------------------------
function dfsi_IsDependanceError($node)
{

    $result = false;
  
    $allDependances = dfsi_GetAllDependanciesOfNode( $node );          
    // boucle dans les dependances detectées
    foreach ($allDependances as $dependance)
    {
        if( dfsi_GetNidOf($node) == dfsi_GetNidOf($dependance) && dfsi_GetNidOf($node) != NULL && dfsi_GetNidOf($dependance) != NULL )
        {
            $result = true;
            break;
        }
    }
      
    $allGroupDependances = dfsi_GetAllGroupDependanciesOfNode( $node );  
     // boucle dans les groupes de dependances detectées
    foreach ($allGroupDependances as $dependance)
    {
        if( dfsi_GetNidOf($node) == dfsi_GetNidOf($dependance) && dfsi_GetNidOf($node) != NULL && dfsi_GetNidOf($dependance) != NULL )
        {
            variable_set( _DFSI_ERROR_MSG , t("Erreur dans les dépendances de groupe, réflexivité ou circularité détectée...") );
            $result = true;
            break;
        }
    }   
    // un element dependant ne peut figurer dans les groupes de dependance
    foreach ($allDependances as $Dep)
    {
        foreach ($allGroupDependances as $GroupDep)
        {
            if( dfsi_GetNidOf($Dep) ==  dfsi_GetNidOf($GroupDep) && dfsi_GetNidOf($Dep) != NULL && dfsi_GetNidOf($GroupDep) != NULL )
            {
                variable_set( _DFSI_ERROR_MSG , t("Erreur entre les dépendances directes et dépendances de groupe, réflexivité ou circularité détectée...")  );
                $result = true;
                break;
            }
        }
    }   
    
    return $result;
}

// -------------------------------
// // test dependances incorrectes
// -------------------------------
function dfsi_validate_dependances($node , $form , $form_state  ) 
{
        $dfsi_Error = dfsi_IsDependanceError($node) ;
        // Si A depends de B qui depends de C  , C ne peut dependre de A       
        // A ne peut dependre de (A et n'importe quoi) ou de (A ET n'importe quoi)
        if( $dfsi_Error == true  )
        {
            form_set_error('field_dependances', variable_get( _DFSI_ERROR_MSG , t("Ok") ) );
        }               
}

// ---------------------------------------------------------
// mise à jour des elements diretement dependants d'un noeud
// ajouter/retirer ce noeud dans leur liste field_ancestor
// ---------------------------------------------------------
 function dfsi_UpdateElementsHavingAncestor($node)
{  
   // leture de tous les noeuds ayant le noeud 
   // en cours mis à jour comme ancetre 
   $OldDependantNodes = dfsi_FindDependantOf( $node );
   
   // pour tous les noeuds dependants
   foreach($OldDependantNodes as $OldDependantNode )
   {
       $nidOfOldDependantNode = dfsi_GetNidOf($OldDependantNode) ;
       $dependances = dfsi_GetDependancesOf( $node );
       
       if( count($dependances) > 0)
       {
           // il n'est plus dans les dependances du noeud mis à jour
            if( ! in_array( $nidOfOldDependantNode , $dependances ) )
            {                
                // supprimer le noeud mis à jour du champ ancestor
                dfsi_RemoveFromAncestors( dfsi_GetNidOf($node) , $OldDependantNode);
            }
        }
        // plus aucune dependances pour le noeud maj.
        else 
        {
            dfsi_RemoveFromAncestors( dfsi_GetNidOf($node) , $OldDependantNode );
        }
   }
    // ajout du noeud a la liste des ancetres 
    // de tous les noeuds dependants si non dejà present      
    $dependantNodes = entity_load( 'node', dfsi_GetDependancesOf( $node ) );
    foreach ($dependantNodes as $dependantNode)
    {
        $ancestors = dfsi_GetAncestorsOf( $dependantNode );
        if( count($ancestors) > 0 )
        {            
            if( ! in_array( dfsi_GetNidOf($node) , $ancestors ) )
            {
                dfsi_AddAncestors(dfsi_GetNidOf($node),$dependantNode);
            }
        }
        else
        {
            dfsi_AddAncestors(dfsi_GetNidOf($node),$dependantNode);
        }
        node_save($dependantNode);
    }
}
//
// ---------------------------------------------------------
// recherche d'éléments sans dependance
// $mode : type de recherche
//  _DFSI__DIRECTDEP_SEARCH | _DFSI__GROUPDEP_SEARCH | _DFSI__NODEP_SEARCH
// ---------------------------------------------------------
function dfsi_FindElementsWithoutDependances($mode)
{
    // 
    // SELECT  `nid` ,  `title` FROM  `node` n WHERE n.`type` IN ('itservice',  'itcontainer') 
    // AND  `nid` NOT IN (SELECT  `entity_id` FROM  `field_data_field_ancestor`)
    // AND  `nid` NOT IN (SELECT  `entity_id` FROM  `field_data_field_groupe_de_dependances`)
    switch( $mode  )
    {
        
        case _DFSI_NODIRECTDEP_SEARCH:
            $table = "field_data_field_ancestor";
            $havingDependances = db_select(  $table  , "d" )
            ->   fields
                 ( 
                    "d",
                    array
                    ( 
                        "entity_id", 
                    ) 
                 )            
            -> execute();
        break;
        case _DFSI_NOGROUPDEP_SEARCH:
            $table = "field_data_field_groupe_de_dependances" ;
            $havingDependances = db_select(  $table  , "d" )
            ->   fields
                 ( 
                    "d",
                    array
                    ( 
                        "entity_id", 
                    ) 
                 )            
            -> execute();            
        break;
    
        case _DFSI__NODEP_SEARCH:
            $sql  = "SELECT  `nid` as 'entity_id' FROM  `node` n WHERE n.`type` IN ('itservice',  'itcontainer') ";
            $sql .="AND ( n.`nid` IN (SELECT  `entity_id` FROM  `field_data_field_ancestor`) ";
            $sql .="OR  n.`nid` IN (SELECT  `entity_id` FROM  `field_data_field_groupe_de_dependances`) );";
            $havingDependances = db_query($sql);
        default:
            
        break;
    }  
            
    // recuperation des identifiants
    $havingDependancesNids = array();
    
    while( $havingDependance = $havingDependances->fetchAssoc() ) 
    {
        // ajouter aux resultats
        $havingDependancesNids[] = $havingDependance['entity_id'];
    }  
    
    
    // tous les noeuds sauf ceux
    // ayant la dependance indiquée dans $mode
    if( count($havingDependancesNids)> 0 )
    {
        $withoutDepNodes = db_select(  "node"  , "n" )
        ->   fields
             ( 
                "n",
                array
                ( 
                    "nid", 
                ) 
             )
        -> condition( 'type', array(_DFSI_SERVICE,_DFSI_CONTAINER) , "IN" )
        -> condition ( 'nid' , $havingDependancesNids  , "NOT IN")
        -> execute();
    }
    else
    {
        $withoutDepNodes = db_select(  "node"  , "n" )
        ->   fields
             ( 
                "n",
                array
                ( 
                    "nid", 
                ) 
             )
        -> condition( 'type', array(_DFSI_SERVICE,_DFSI_CONTAINER) , "IN" )
        -> execute();        
    }
    
    $NoDepNodeNids = array();
    while( $withoutDepNode = $withoutDepNodes->fetchAssoc() ) 
    {
        $NoDepNodeNids[] = $withoutDepNode['nid'];
    }        

    $resultList = entity_load( 'node' ,  $NoDepNodeNids  );
    return $resultList;          
}


// -------------------------------------------------------------
// // cree une table à partir de l'entete et des données de ligne
// -------------------------------------------------------------
function dfsi_RenderTableFromList($tableTitle, $header,$entries) 
{
    $output = '';
    $rows = array();
    $output = $tableTitle;
    if (!empty($entries)) 
    {
        foreach ($entries as $entry) 
        {
            $alias = drupal_get_path_alias("node/" . dfsi_GetNidOf($entry) );
            $link = l( dfsi_GetTitleOf($entry), $alias );
            $item = array
            (
                dfsi_GetNidOf($entry),
                $link,
                dfsi_GetTypeOf($entry) == _DFSI_SERVICE ? t("Service") : t("Container"),
                dfsi_GetImgOfStatut( dfsi_GetStatutOf( $entry ) )
            );
            $rows[] = $item;
        }
        $output .= theme('table', array('header' => $header, 'rows' => $rows));
    }
    /*
    else
    {
        $output .=t("Aucun résultat.");
    }
    */
    return $output;
}

// ---------------------------------------------------------
// fonction  de callback d'affichage de liste de noeuds 
// non dependants directement d'un aute
// ou d'un groupe
// ---------------------------------------------------------
function dfsi_NoDependantList() 
{
    $entriesNoDep = dfsi_FindElementsWithoutDependances(_DFSI__NODEP_SEARCH);
    $entriesNoDirectDep=dfsi_FindElementsWithoutDependances(_DFSI_NODIRECTDEP_SEARCH);
    $entriesNoGroupDep=dfsi_FindElementsWithoutDependances(_DFSI_NOGROUPDEP_SEARCH);          
    
    $header = array( t('Identifiant'), t('Titre'), t('Type'), t('Statut') );
    $output = array
    (
        'fieldset1' => array
        (
            '#type' => 'fieldset',
            '#title' => t("Noeuds totalement indépendants"),
            '#attributes' => array('class' => array('collapsible')),
            'content' => array
            (
                '#markup' => dfsi_RenderTableFromList('', $header,$entriesNoDep) ,
            ),
        ),
        'fieldset2' => array
        (
            '#type' => 'fieldset',
            '#title' => t("Noeuds non directement dépendants"),
            '#attributes' => array('class' => array('collapsible', 'collapsed')),
            'content' => array
            (
                '#markup' => dfsi_RenderTableFromList('', $header,$entriesNoDirectDep),
            ),
        ),
        'fieldset3' => array
        (
            '#type' => 'fieldset',
            '#title' => t("Noeuds non dépendants d'un groupe"),
            '#attributes' => array('class' => array('collapsible', 'collapsed')),
            'content' => array
            (
                '#markup' =>  dfsi_RenderTableFromList('', $header,$entriesNoGroupDep) ,
            ),
        ),
        
        '#attached' => array('js' => array('misc/collapse.js', 'misc/form.js')),        
    );
    
    return $output;
}

// ----------------------------------
// onglet des elements dependants
// ----------------------------------
function dfsi_elementsDependantsPage($args)
{
    $entriesDirectDep = dfsi_GetAllDependanciesOfNode( $args );
    $entriesGroupDep = dfsi_FindGroupDependantOf( $args );
    
    $header = array( t('Identifiant'), t('Titre'), t('Type'), t('Statut') );
    $output = array
    (
        'fieldset1' => array
        (
            '#type' => 'fieldset',
            '#title' => t("Noeuds directements dépendants de [@title]",  array('@title' => $args->title)),
            '#attributes' => array('class' => array('collapsible')),
            'content' => array
            (
                '#markup' => dfsi_RenderTableFromList('', $header,$entriesDirectDep),
            ),
        ),
        'fieldset2' => array
        (
            '#type' => 'fieldset',
            '#title' => t("Noeuds dépendants d'un goupe ou est présent [@title]",  array('@title' => $args->title)),
            '#attributes' => array('class' => array('collapsible', 'collapsed')),
            'content' => array
            (
                '#markup' => dfsi_RenderTableFromList('', $header,$entriesGroupDep) ,
            ),
        ),
        '#attached' => array('js' => array('misc/collapse.js', 'misc/form.js')),        
    );

    return $output;
}

// ----------------------------------
// onglet des elements ancestor
// Elements dont est dependant le noeud
// selectionné
// ----------------------------------
function dfsi_dependsDePage($args)
{    
    $entriesDirectDep = dfsi_GetAllAncestorsOf( $args );
    $entriesGroupDep = dfsi_GetAllGroupDependanciesOfNode($args);
    $entriesContainer  = dfsi_FindContainerOf( $args );        
    
    $header = array( t('Identifiant'), t('Titre'), t('Type') , t('Statut') );
    $output = array
    (
        'fieldset1' => array
        (
            '#type' => 'fieldset',
            '#title' => t("Eléments dont [@title] est dépendant directement ou par transitivité",  array('@title' => $args->title) ),
            '#attributes' => array('class' => array('collapsible')),
            'content' => array
            (
                '#markup' => dfsi_RenderTableFromList('', $header,$entriesDirectDep), 
            ),
        ),
        'fieldset2' => array
        (
            '#type' => 'fieldset',
            '#title' => t("Noeuds d'un goupe dont est dépendant [@title]",  array('@title' => $args->title)),
            '#attributes' => array('class' => array('collapsible', 'collapsed')),
            'content' => array
            (
                '#markup' => dfsi_RenderTableFromList('', $header,$entriesGroupDep) ,
            ),
        ),
        'fieldset3' => array
        (
            '#type' => 'fieldset',
            '#title' => t("Containers de [@title]",  array('@title' => $args->title)),
            '#attributes' => array('class' => array('collapsible', 'collapsed')),
            'content' => array
            (
                '#markup' => dfsi_RenderTableFromList('', $header,$entriesContainer) ,
            ),
        ),
        
        '#attached' => array('js' => array('misc/collapse.js', 'misc/form.js')),        
    );

    return $output;
}

// ----------------------------------
// onglet des elements contenus
// ----------------------------------
function dfsi_ContientPage($args)
{
    
    $entriesContent = dfsi_GetAllContentOfNode($args);
    
    $header = array(t('Identifiant'), t('Titre'), t('Type'),t('Statut') );
    $output = array
    (        
        'fieldset1' => array
        (
            '#type' => 'fieldset',
            '#title' => t("Liste des services ou containers inclus dans [@title]",  array('@title' => $args->title)),
            '#attributes' => array('class' => array('collapsible')),
            'content' => array
            (
                '#markup' => dfsi_RenderTableFromList('', $header,$entriesContent),
            ),
        ),
        
        '#attached' => array('js' => array('misc/collapse.js', 'misc/form.js')),        
    );
    return $output;
}

