<?php
// ------------------------------------------------
// @(#) dfsi.form.inc
// @(#) Bruno Ferec
// @(#) 11/12/2013
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");

// ------------------------------------------
// modification formulaire edition service
// ------------------------------------------
function dfsi_form_itservice_node_form_alter(&$form, &$form_state, $form_id) 
{
    dfsi_form_ElementAlter($form);
}
// ------------------------------------------
// modification formulaire edition container
// ------------------------------------------
function dfsi_form_itcontainer_node_form_alter(&$form, &$form_state, $form_id) 
{
    dfsi_form_ElementAlter($form);
}

// ------------------------------------------
// Hook_form_alter
// concerne tous les formulaires
// ------------------------------------------
function dfsi_form_alter(&$form, &$form_state, $form_id) 
{   
          //dpm($form);
}
// -------------------------------------------
// true si formulaire de modification
// -------------------------------------------
function dfsi_IsEditForm($form)
{
    return $form['#node_edit_form'] == TRUE;
}
function dfsi_IsAddForm($form)
{
    return $form['nid']['#value'] == NULL;
}

// ------------------------------------------------
//  submit sous formulaire
// ------------------------------------------------
function dfsi_Edit_Detail_Callback($form, &$form_state) 
{
        dpm($form);
}
// -------------------------------------------
// mise en place du formulaire d'ajout de detail
// -------------------------------------------
function dfsi_CreateDetailForm($form,$node,$contentTypeName)   
{
    $formName = $contentTypeName['ctname'].'_node_form';
    $subform = drupal_get_form($formName, $node);  
    $subform['#title'] = "Détail ".$contentTypeName['label']; 
    unset($subform['actions']['preview']);    
    // TODO etiquette sur même ligne + boutton submit..
    hide($subform['additional_settings']); 
    $subform['#submit'][] = 'dfsi_Edit_Detail_Callback';
    return $subform;
}
// -------------------------------------------
// retirer le noeud des choix possible de 
// dependances pour lui meme
// -------------------------------------------
function dfsi_RemoveSelfFromDependanciesChoice(&$form)
{
    $node =  $form['#node'];
    
    $language = $form['field_dependances']['#language'];
    unset($form['field_dependances'][$language]['#options'][dfsi_GetNidOf($node)]);  

    if( dfsi_GetTypeOf($node) == _DFSI_CONTAINER )
    {
        $language = $form['field_contenu']['#language'];
        unset($form['field_contenu'][$language]['#options'][dfsi_GetNidOf($node)]);
    }

    $language = $form['field_groupe_de_dependances']['#language'];
    unset($form['field_groupe_de_dependances'][$language]['#options'][dfsi_GetNidOf($node)]);    
}

// -------------------------------------------
// retirer les noeuds dont il depends (ancestors)
// des choix possible de 
// dependances pour lui meme
// -------------------------------------------
function dfsi_RemoveAncestorsFromDependanciesChoice(&$form)
{
    $node =  $form['#node'];
    $language = $form['field_dependances']['#language'];
    foreach( $form['field_dependances'][$language]['#options'] as $key => $candidateDependance )
    {
        $candidateDependantNode = entity_load( 'node' , array($key) );
        $dependancesNodes = dfsi_GetAllDependanciesOfNode($candidateDependantNode[$key] ) ;

        foreach($dependancesNodes as $dependancesNode)
        {                
            if( dfsi_GetNidOf($dependancesNode) == dfsi_GetNidOf($node)  )
            {
                //$language = $form['field_dependances']['#language'];
                unset($form['field_dependances'][$language]['#options'][$key]);

                if( dfsi_GetTypeOf($node) == _DFSI_CONTAINER )
                {                
                    $language = $form['field_contenu']['#language'];
                    unset($form['field_contenu'][$language]['#options'][$key]);
                }

                $language = $form['field_groupe_de_dependances']['#language'];
                unset($form['field_groupe_de_dependances'][$language]['#options'][$key]);
            }
        }
    }    
}

// -------------------------------------------
// retirer les noeuds dont il depends (ancestors)
// des choix possible de groupe de
// dependances
// -------------------------------------------
function dfsi_RemoveAncestorsFromGroupDependanciesChoice(&$form)
{
    $node =  $form['#node'];
    $language = $form['field_dependances']['#language'];
    
    $dependanciesOfCurrentNode = dfsi_GetAllDependanciesOfNode($node);
    $language = $form['field_groupe_de_dependances']['#language'];
    foreach( $form['field_groupe_de_dependances'][$language]['#options'] as $key => $candidateGroupDependance )
    {
        foreach($dependanciesOfCurrentNode as $dep)
        if( dfsi_GetNidOf($dep) == $key )
        {
            unset($form['field_groupe_de_dependances'][$language]['#options'][$key]);
        }
    }    
}

// -------------------------------------------
// Retrait des membres d'un meme groupe 
// de dépendance
// des choix possible de groupe de
// dependances  
// -------------------------------------------
function dfsi_RemoveSameGroupMembersFromGroupDependanciesChoice(&$form)
{
    $node =  $form['#node'];
    $language = $form['field_dependances']['#language'];
        
    $SameGroupMembersNodes = dfsi_FindSameGroupMemberOf( $node );
    foreach( $form['field_groupe_de_dependances'][$language]['#options'] as $key => $candidateGroupDependance )
    {
        foreach($SameGroupMembersNodes as $dep)
        if( dfsi_GetNidOf($dep) == $key )
        {
           unset($form['field_groupe_de_dependances'][$language]['#options'][$key]);
        }
    }    
}
// -------------------------------------------
// modification de certains elements dans le formulaire
// de mise à jour 
// -------------------------------------------
function dfsi_AlterModifyForm( &$form )
{
    // suppression des elements de la liste des elements dependants 
    // qui ne peuvent dependre de ce service
    // pour éviter circularite et reflexivité
    // Etape 1
    // retirer le noeud lui meme
    // => interdit les references reflexives    
    dfsi_RemoveSelfFromDependanciesChoice($form);
    //
    // Etape 2
    // retirer des choix possible en dependance 
    // les noeuds ayant eux meme ( ou l'un de leur descendants) en dependance
    //  le noeud en cours de modification
    // à n'effectuer que si modification du noeud, et non pas ajout
    dfsi_RemoveAncestorsFromDependanciesChoice($form);
    // Etape 3
    // repérer et ôter les élements de dépendance des choix possibles comme groupe de dépendances                
    // à n'effectuer que si modification du noeud, et non pas ajout       
    dfsi_RemoveAncestorsFromGroupDependanciesChoice($form);
    // Etape 4
    // retirer tous les noeuds figurant dans un même groupe de dépendance 
    dfsi_RemoveSameGroupMembersFromGroupDependanciesChoice($form);
}

// --------------------------------------------------------
// Ajout des sections accordeon pour chaque detail de terme  
// --------------------------------------------------------
function dfsi_AddDetailSections(&$form,$contentTypeNames)
{
    // aucun type de contenu de détail défini pour les termes
    // de taxonomie affectés à ce noeud
    if( count($contentTypeNames) == 0 )
    {
        // en ajout de neoud, pas encore de NID, donc
        // donnée complémentaires interdites.      
        if( dfsi_IsAddForm($form) )
        {
            $msg = t("Element non enregistré, impossible pour l'instant de créer des données complémentaires...");
        }
        else
        {
            $msg = t('Aucun type de données complémentaires défini.');
        }
        $form['DetailDataSection']['msg']= array
        (
            '#markup' => $msg,
            '#weight' => 1, 
        ); 
    }
    // des type de contenus existent, créer sections accordéon pour chaque terme
    else
    {
        foreach ($contentTypeNames as $key => $contentTypeName)
        {    
            $form['DetailDataSection'][$key] = array
            (
                '#type' => 'fieldset',
                '#title' => t($key." - (".$contentTypeName['label'].")"),
                '#description' => t( '' ),
                '#collapsible' => TRUE,
                '#collapsed' => TRUE,
            );        
     
        }
    }
}
// -------------------------------------------
// recherche d'eixstence pour cet élément
// d'un détail pour le terme
// -------------------------------------------
function dfsi_ExistDetailContent($nid,$vid,$tid)
{
    global $user;
    $result = NULL;
   $foundedDetails = array();
   //recherche des contenus de detail existants
   $sqlRequest = db_select(_DFSI_NODE_DETAIL_TABLE, 'ndt' )
    ->fields
         ( 
                 'ndt', 
                 array
                 ( 
                     'nid',
                     'did',
                     'vid',
                     'tid',
                 ) 
         )
    -> condition( 'nid',$nid , "=" )
    -> condition( 'vid',$vid , "=" )
    -> condition( 'tid',$tid , "=" )           
    -> execute();
   while( $detail = $sqlRequest-> fetchAssoc() )
   {
       $foundedDetails[] = $detail['did'];
   }   
   if(count($foundedDetails) > 0  )
   {
        $detailNodeId = $foundedDetails[0];
        
        $detailNodes = entity_load( 'node' , array($detailNodeId) );
        if( count($detailNodes > 0) )
        {
            $result = $detailNodes[$detailNodeId];
            $result->promote = FALSE;
            $result->status = TRUE;
            $result->uid = $user->uid;
            $result->sticky = FALSE;            
            
        }
   }
    return $result;
}
// -------------------------------------------
// modification de certains elements dans le formulaire
//  de mise à jour OU ajout
// -------------------------------------------
function dfsi_AlterEditForm( &$form )
{
    
    $node =  $form['#node'];
    $language = $form['field_statut']['#language'];
    // verrouillage statut en fonction du statut des dependances
    if( dfsi_MustLockStatutOf($node) )
    {
        // non modifiable
        $form['field_statut']['#disabled'] = true;
        // seule la valeur actuelle est afichee dans la liste

        $form['field_statut'][$language]['#options'] = array( dfsi_GetStatutOf($node) => dfsi_GetStatutOf($node) );
        // valeur par defaur = valeur actuelle
        $form['field_statut']['#default_value'] = dfsi_GetStatutOf($node);
        //message d'aide indiquant le verrouillage
        $form['field_statut'][$language]['#description']="(verrouillé de part le statut des services/containers parents)";
    }
    else
    {
        // cacher warning meme si le validate l'autorise
        $form['field_statut'][$language]['#options'] = array('Ok' => 'Ok','Error' => 'Error');
    }
    // recherche des types de contenu de détail definis pour ce noeud
    $contentTypeNames = FindDetailContentTypeNames( $node );
    
    // si utilisation de données complémentaires ?
    if( dfsi_ComplementaryDataEnabled() )
    {
        // creation de la section accordeon detail 
        dfsi_AddDetailSections($form, $contentTypeNames);
    }    
    // pas en ajout ? modifier
    if( ! dfsi_IsAddForm($form) )
    {    
        dfsi_AlterModifyForm( $form );
        // si utilisation de données complémentaires ?
        if( dfsi_ComplementaryDataEnabled() )
        {
            dfsi_BuildChunkEditDataSection( $form , $contentTypeNames);
        }
       
    }  
}

// -----------------------------
// // ajout des sous formulaires
// -----------------------------
function dfsi_BuildChunkEditDataSection(&$form, $contentTypeNames)
{    
     
    $node =  $form['#node'];
    // Ajout des sous formulaires de detail                   
    foreach ($contentTypeNames as $key => $contentTypeName)
    {    
        $form['DetailDataSection'][$key]['links'] = array();
        
        $detailNode = dfsi_ExistDetailContent(dfsi_GetNidOf($node),$contentTypeName['vid'],$contentTypeName['tid'] );
        if( $detailNode != null )
        {       
            $form['DetailDataSection'][$key]['links'][]= array
            (
                '#type' => 'link',
                '#title'=> t('Modifier '.$key),
                '#href'=> "",
                '#description' => t(""),                 
            );             
            /*
            node_object_prepare($detailNode);
            $subform = dfsi_CreateDetailForm($form,$detailNode,$contentTypeName);  

            // ajout des champs cachés
            $subform['detail_nid'] = array('#type' => 'hidden', '#value' => dfsi_GetNidOf($node));
            $subform['detail_vid'] = array('#type' => 'hidden', '#value' => $contentTypeName['vid']);
            $subform['detail_tid'] = array('#type' => 'hidden', '#value' => $contentTypeName['tid']);
            $form['DetailDataSection'][$key]['formulaire'] = $subform;           
            */     
        }
        else
        {
            $label = $contentTypeName['label'];
            $form['DetailDataSection'][$key]['links'][]= array
            (
                '#type' => 'link',
                '#title'=> t('Ajouter un détail pour '.$key),
                '#href'=> "http://www.google.fr",
                '#description' => t("création d'un élément de détail"),                 
            ); 
        }
    }    
}
// -----------------------
// validation champ statut
// -----------------------
function dfsi_field_statut_validate($element, &$form_state ) 
{
   // rien à valider liste limitéee 
   // mais necessaire pour cacher element de liste
   // car la fonction de validation standard interdit d'affecter un
   // element non present (warning) dans la liste 
}


// -------------------------------------------
// modification du formulaire de modification 
// d'édition de service ou container
// -------------------------------------------
function dfsi_form_ElementAlter(&$form)
{      
    // masque le champ ancestor
    hide($form['field_ancestor']);
    
    // masque les elements additional settings: livre/revision/auteur/options de publication
    hide($form['additional_settings']);
   
    // definit la fonction de validation du champ statut
    $language = $form['field_statut']['#language'];
    $form['field_statut'][$language]['0']['#element_validate']   = array('dfsi_field_statut_validate');        
    
    if( dfsi_ComplementaryDataEnabled() )
    {
        // ---------------------------------------------------    
        // ajout d'un regroupement accordéon 
        // pour données complémentaires
        // ---------------------------------------------------    
        $form['DetailDataSection'] = array
        (
            '#type' => 'fieldset',
            '#title' => t('Données complémentaires'),
            '#description' => t(''),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#weight' => '3',
        );   
    }
    // ---------------------------------------------------
    // regroupement accordéon pour gestion des dépendances
    // ---------------------------------------------------    
    $form['Dependances']= array
    (
        '#type' => 'fieldset',
        '#title' => t('Dépendances'),
        '#description' =>  'modifier ou créer les dépendances fonctionnelles de noeud',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => '2',
    );    
    
    // tabulations verticales pour dep/groupe et contenu
    $form['Dependances']['DependancesManagement'] = array
    (
        '#type' => 'vertical_tabs',
    );
    // ------------------------------------------        
    // dependances directes - premiere tabulation
    // ------------------------------------------
    $form['Dépendances directes'] = array
    (
        '#type' => 'fieldset',
        '#title' => t('Dépendances directes'),
        '#description' =>  '',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#group' => 'DependancesManagement',
        '#weight' => '1',
    );            
    // déplacement des champs de dépendance et contenu vers cet accordéon
    // et masquer le champs d'origine
    $form['Dépendances directes']['field_dependances'] = $form['field_dependances'];
    hide( $form['field_dependances'] );
    // ------------------------------------------    
    // dependances de groupe - seconde tabulation
    // ------------------------------------------    
    $form['groupe_de_dependances'] = array
    (
        '#type' => 'fieldset',
        '#title' => t('Groupe de dépendances'),
        '#description' => '',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#group' => 'DependancesManagement',
        '#weight' => '2',
    );        
    // déplacement des champs de groupe de dépendance vers cet accordéon
    // et masquer le champs d'origine    
    $form['groupe_de_dependances']['field_groupe_de_dependances'] = $form['field_groupe_de_dependances'];    
    hide( $form['field_groupe_de_dependances'] );
    // -------------------------------
    // contenus - troisieme tabulation
    // uniquement sur formulaire  pour container
    // -------------------------------
    if( $form['#form_id'] === 'itcontainer_node_form' )
    {
        $form['Contenus'] = array
        (
            '#type' => 'fieldset',
            '#title' => t('Contenus'),
            '#description' => '',
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#group' => 'DependancesManagement',
            '#weight' => '3',
        ); 
        // déplacement des champs  contenu vers cet accordéon
        // et masquer le champs d'origine        
        $form['Contenus']['field_contenu']= $form['field_contenu'];
        hide( $form['field_contenu'] );
    }
    // modification de service ?
    // masquer certains elements    
    if( dfsi_IsEditForm($form) == true ) 
    {
        dfsi_AlterEditForm($form);  
    }
}