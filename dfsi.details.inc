<?php

// ------------------------------------------------
// @(#) dfsi.details.inc
// @(#) 31/12/2013
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");
module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.api");

// -------------------------------------------------
// page d'association d'un type de contenu
// pour les données supplémentaires liées
// seulement à un terme de taxonomie
// -------------------------------------------------
function dfsi_DetailsDatasTypePage($args)
{
    $output="";
    $did=-1;    
    
    popup_forms_parent_js();
    
    //lecture du type de contenu spécifique à ce terme
    $type = dfsi_ReadTermContentTypeRelation($did,$type,$args->vid,$args->tid);
    if($type == NULL)
    {

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
            $url = "javascript:popupFormsFormShow('dfsi_DetailContentTypeAddForm',null,{$PopupArgs});";   
            
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
function dfsi_DetailContentTypeAddForm($form, &$form_state, $vid , $tid , $taxonomyTermName ) 
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
    $form['#submit'][] = 'dfsi_DetailContentTypeAddForm_submit';
    
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
function dfsi_DetailContentTypeAddForm_submit($form, &$form_state) 
{ 
    // confirmation de creation du  type de contenu de detail
     if($form_state['values']['op'] == t('Enregistrer') )
     {
         dfsi_AddDetailRshp( $form['vid']['#value'] , $form['tid']['#value'] , $form['newContentTypeName']['#value'] );
         dfsi_createDetailContentType( $form['newContentTypeName']['#value'], $form['taxonomyTermName']['#value'] );
         $path = "taxonomy/term/{$form['tid']['#value']}/Chunk";
         drupal_goto($path);
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
    //$relation = NULL;
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


// ------------------------------------------------------
// creation d'une section details de contenu 
// pour un noeud et un terme
// ------------------------------------------------------
function dfsi_BuildChunkViewContent($node,$key,$contentTypeName)
{
    // creation de la section repliable correspondant au type de contenu détail
    $fieldsetContent  = '<fieldset id="fieldset-'.$key.'" class="collapsible">';
    $fieldsetContent .= '<legend><span class="fieldset-legend">'.t($key." - (".$contentTypeName['label'].")").'</span></legend>';  
    $fieldsetContent .= '<div class="fieldset-wrapper">';
    $fieldsetContent .= '<div class="fieldset-description">'.''.'</div>';   

    // recheche d'eixtence de données saisies pour ce type de contenu de détail
    $detailNode = dfsi_ExistDetailContent(dfsi_GetNidOf($node),$contentTypeName['vid'],$contentTypeName['tid'] );
    // trouvées...
    if( $detailNode != null )
    { 
        $wrapper = entity_metadata_wrapper('node', $detailNode);

        $fields_infos = field_info_instances('node',$contentTypeName['ctname']);
        foreach( $fields_infos as $key => $field_info )
        {
            $fieldsetContent .= "<b>".$field_info['label'].":</b>&nbsp;".$wrapper->$key->value()."<br/>";
        }

    }
    // pas de données saisie
    else
    {
       $fieldsetContent .= t("Pas de données de détail définies pour le terme ").$key.".";
    }

    $fieldsetContent .= '</div>';
    $fieldsetContent .= '</fieldset>';  
    
    return $fieldsetContent;
}

// ------------------------------------------------------
// Creation des sections details de contenu pour un noeud
// ------------------------------------------------------
function dfsi_BuildChunkViewDataSection( $node )
{
                   
    $fieldsetTitle = t('Données complémentaires');
    $fieldsetDescription = t('');
    
    $contentTypeNames = FindDetailContentTypeNames( $node );
    // aucun type de contenu de détail existant pour cet élément
    if( count($contentTypeNames) == 0 )
    {
            $fieldsetContent = t('Aucune type de contenu défini pour les termes de taxonomie applicables à ce service');
    }
    // des types de contenu de données complémentaires sont définis
    else
    {
        // pour tous les types de contenus
        foreach ($contentTypeNames as $key => $contentTypeName)
        {    
            // créer la section repliable contenant les infos pour ces details de type de contenu
            $fieldsetContent = dfsi_BuildChunkViewContent($node,$key,$contentTypeName);
        }
    }
    // 
    $html = '<fieldset id="fieldset-id" class="collapsible">';
    $html .= '<legend><span class="fieldset-legend">'.$fieldsetTitle.'</span></legend>';
    $html .= '<div class="fieldset-wrapper">';
    $html .= '<div class="fieldset-description">'.$fieldsetDescription.'</div>';
    $html .= $fieldsetContent;
    $html .= '</div>';
    $html .= '</fieldset>';    
                
     return $html;           
}
