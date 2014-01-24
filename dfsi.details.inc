<?php

// ------------------------------------------------
// @(#) dfsi.details.inc
// @(#) 31/12/2013
// ------------------------------------------------

if( !defined("_MODULE_NAME") ) { define( "_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_INCLUDE_EXTENSION") ) { define( "_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

module_load_include( _INCLUDE_EXTENSION , _MODULE_NAME , "dfsi.define");
module_load_include( _INCLUDE_EXTENSION , _MODULE_NAME , "dfsi.api");


//function dfsi_GetAddDetailContentTypeLink($vid,$tid,$taxonomyTermName) 
//{
//
//        $arguments = json_encode($arg);
//        
//        popup_forms_parent_js();
//        $js = "
//            (function($) {
//            $(document).ready(function() {
//            popupFormsFormShow('dfsi_DetailAddForm',null,{$arguments});
//            });
//            })(jQuery);
//        ";
//        drupal_add_js($js, 'inline');
//}
//// -------------------------------------------------
// page d'association d'un type de contenu
// pourles données supplémentaires liées
// seulement à un terme de taxonomie
// -------------------------------------------------
function dfsi_ComplementDataTypePage($args)
{
    $output="";
    $did=-1;    
    
    //lecture du type de contenu spécifique à ce terme
    $type = dfsi_ReadTermContentTypeRelation($did,$type,$args->vid,$args->tid);
    if($type == NULL)
    {
        popup_forms_parent_js();
        
        if( user_access( _DFSI_PERM_ADD_CHUNK_DATA_TYPE))
        {
            // parametres à passer à popupFormShow() en flux json
            $PopupArgs = json_encode
            (
                array
                (
                    'vid'=> $args->vid,
                    'tid'=> $args->tid,            
                    'taxonomyTermName' => $args->name
                )
            );
            // simple quote au lieu de double
            $PopupArgs = str_replace ( '"' , "'" ,$PopupArgs );
            
            // appel javascript la fonction du module  popup_forms
            $url = "javascript:popupFormsFormShow('dfsi_DetailAddForm',null,{$PopupArgs});";   
            // creation du lien

            $link = '<a href="';
            $link .=  $url;
            $link .=  '">'.t("Ajouter").'</a>';
            
            $output .= t("Il n'existe pas de type de contenu de données complémentaires pour le terme ".$args->name);
            $output .= "<br/>";
            $output .= "<br/>";
            $output .= $link;
        }
    }
    else
    {
        $link = l($type,"admin/structure/types/manage/".$type);
        $output .= t("Type de contenu de données complémentaires existant pour ce terme : ").$link;
        
        $fields = field_info_instances("node",$type);
        $header = array( t('Nom du champ'), t('Etiquette'), t('Description'), t('Valeur par défaut'),t('Champ obligatoire') );
        $rows = array();        
        foreach( $fields as $fieldName => $field)
        {    
            // troncature du champ description si trop long pour affichage
            $description = substr( $fields[$fieldName]['description'] , 0 , _DFSI_TRUNCATE_COMMENT_LENGTH );
            $description .= strlen( $fields[$fieldName]['description'] ) > _DFSI_TRUNCATE_COMMENT_LENGTH ? '[...]' : '';
            $item = array
            (
                $fields[$fieldName]['field_name'],
                $fields[$fieldName]['label'],
                $description,
                $fields[$fieldName]['default_value'] == NULL ? t('-') : $fields[$fieldName]['default_value'][0]['value'],
                $fields[$fieldName]['required'] == 0 ? t("Non") : t("Oui"),                
            );
            $rows[] = $item;           
        }
        $output .= theme('table', array('header' => $header, 'rows' => $rows));
    }
    return $output;
}


// -------------------------------------------------
// formulaire d'ajout d'un type de contenu detail
// affiché en modal
// -------------------------------------------------
function dfsi_DetailAddForm($form, &$form_state, $vid , $tid , $taxonomyTermName ) 
{
    
    $newContentTypeName = $vid .$tid ."_detail";
    // nom lisible    
    $humanName  = strtolower($taxonomyTermName)."_detail";
    
    $form['arg'] = array
    (
        array
        (
            '#type' => 'item', '#markup' => t("Terme de taxonomie  : ").$taxonomyTermName,
        ),        
        array
        (
            '#type' => 'item', '#markup' => t("Nom machine : ").$vid.$tid."_".$taxonomyTermName,
        ),
        array
        (
         '#type' => 'item', '#markup' => t("Le type de contenu : ").$humanName,
        )
    );
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array
    (
        '#type' => 'submit',
        '#value' => t('Enregistrer'),
        '#suffix' => "&nbsp;"
    );    
    $form['actions']['cancel'] = array
    (
      '#type' => 'submit',
      '#value' => t('Annuler'),
    );    
    $form['#submit'][] = 'dfsi_DetailAddForm_submit';
    
    // champs cachés
    $form['vid']= array( '#type' => 'hidden','#value' => $vid );
    $form['tid']= array('#type' => 'hidden','#value' => $tid );
    $form['taxonomyTermName']= array('#type' => 'hidden','#value' => $taxonomyTermName); 
    $form['newContentTypeName']= array('#type' => 'hidden','#value' => $newContentTypeName); 
    return $form;
}

// ------------------------------------------
// callback function pour fsi_DetailAddForm()
// ------------------------------------------
function dfsi_DetailAddForm_submit($form, &$form_state) 
{ 
        dpm($form);
    // confirmation de creation du  type de contenu de detail
     if($form_state['values']['op'] == t('Enregistrer') )
     {
         dfsi_AddDetailRshp( $form['vid']['#value'] , $form['tid']['#value'] , $form['newContentTypeName']['#value'] );
         dfsi_createDetailContentType( $form['newContentTypeName']['#value'], $form['taxonomyTermName']['#value'] );
         //dsm("Le type de contenu ".$form['taxonomyTermName']['#value']."_detail à été créé...");
         //$path = drupal_get_path_alias(current_path());
         //drupal_goto($path);

     }
}

// -------------------------------
// modification du type de contenu 
// associé à  un terme de txonomie
// -------------------------------
function dfsi_EditDetailComplementType($args)
{
   $newContentTypeName = $args->vid .$args->tid ."_detail";
   drupal_goto("admin/structure/types/manage/".$newContentTypeName);    
}
// -------------------------------
// suppression du type de contenu 
// associé à  un terme de txonomie
// -------------------------------
function dfsi_DeleteDetailComplementType($args)
{
    $ContentName = $args->vid.$args->tid."_detail";
    dfsi_DeleteDetailRshp( $args->vid , $args->tid );
    node_type_delete($ContentName);    
    dsm("Type de contenu complémentaire [".$ContentName."] supprimé.");
    drupal_goto(drupal_get_path_alias("types-de-service/". $args->name ));
}
// -------------------------------
// creation du type de contenu 
// associé à  un terme de taxonomie
// -------------------------------
function dfsi_createDetailContentType($machineName,$taxonomyTermName)
{
    // nom lisible pour interface
    $humanName  = strtolower($taxonomyTermName."_detail");
    // definition du type
    $ct = array
    (
        'type' => $machineName,
        'name' => t($humanName),
        'base' => 'node_content',
        'title_label' => t("Titre ".$humanName),
        'sticky' => FALSE,
        'status' => TRUE,
        'description' => t("Type de contenu  de données supplémentaires pour [@name]",  array('@name' => $taxonomyTermName) ),
        'custom' => FALSE,
        'locked' => 1
    );
  // autres elements en valeur par defaut
  $content_type = node_type_set_defaults($ct);
  // ajout du champ nody
  //node_add_body_field($content_type, t("Informations supplémentaires pour [@name]",  array('@name' => $taxonomyTerm->name) );
  // sauvegarde
  node_type_save($content_type);

}
// -------------------------------------
// creation de la relation
// entre type de contenu et terme
// -------------------------------------
function dfsi_AddDetailRshp($vid,$tid,$termName)
{
    $sql= "INSERT INTO `"._DFSI_DETAIL_TABLE."` (`vid`,`tid`,`type`) VALUES (" . $vid . "," . $tid . ",'" .$termName."');";
    db_query($sql);   
}

function dfsi_DeleteDetailRshp($vid,$tid)
{
    $sql= "DELETE  FROM `"._DFSI_DETAIL_TABLE."` WHERE `vid`=". $vid . " AND `tid`=" . $tid .";";
    db_query($sql);   
}

// -------------------------------------
// lecture d'une relation entre
//  type de contenu de détail
// et un terme de taxonomie
// -------------------------------------
function dfsi_ReadTermContentTypeRelation(&$did,&$type,$vid,$tid)
{
    $Sql  = "SELECT did,TYPE FROM  `"._DFSI_DETAIL_TABLE."` WHERE `vid`=".$vid." AND "."`tid`=".$tid.";";
    $relations = db_query($Sql);
    
    while( $relation = $relations->fetchAssoc() ) 
    {
        // ajouter aux resultats
        $did = $relation['did'];
        $type = $relation['TYPE'];
    }  
    return $type;   
}
// --------------------------------------------
// type de contenu de detail créé pour ce noeud ?
// --------------------------------------------
function dfsi_IsComplementaryDataEnabled( $node )
{
    $language = field_language('node', $node, 'field_service_type', $langcode = NULL);
    $tid = $node->field_service_type[$language][0]['tid'];
    $Sql  = "SELECT TYPE FROM  `"._DFSI_DETAIL_TABLE."` WHERE `tid`=".$tid.";";
    $relations = db_query($Sql);  
    
    $relation = $relations->fetchAssoc() ; 

    return ($relation != NULL);
}

// --------------------------------------------
// leture du type de contenu de détail  pour un
// terme de taxonomie
// --------------------------------------------
function dfsi_GetTermRelationForContentType($typeMachineName)
{
    $result = array();
    $Sql  = "SELECT did,vid,tid,type FROM  `"._DFSI_DETAIL_TABLE."` WHERE `type`='".$typeMachineName."';";
    $relations = db_query($Sql);
    
    while( $relation = $relations->fetchAssoc() ) 
    {
        $result = $relation;
    }  
    return $result;   
}
