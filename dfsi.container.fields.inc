<?php

// ------------------------------------------------
// @(#) dfsi.container.fields.inc
// @(#) 11/12/2013
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php


module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");

// -----------------------------------
// suppression des champs  : itcontainer
// -----------------------------------
function dfsi_Delete_itcontainer_Fields( $typeName )
{
	foreach( array_keys( dfsi_itcontainer_Get_Fields_Definitions($typeName) ) as $field ) 
	{
		field_delete_field( $field );
	}
}
// ------------------------------------------------------------------
// creation des champs pour le content type : itcontainer
// ------------------------------------------------------------------
function dfsi_Create_itcontainer_Fields( $typeName )
{
	// ----------------------------------------------
	// Creer  tous les champs pour le type de contenu
	// ----------------------------------------------
	foreach ( dfsi_itcontainer_Get_Fields_Definitions($typeName) as $field ) 
	{
		field_create_field( $field );
	}
}

// --------------------------------------------
// Creation des instances de champ  : itcontainer
// --------------------------------------------
function dfsi_Create_itcontainer_Fields_Instances( $typeName) 
{ 
	// ------------------------------------------
	// Creer toutes les instances pour les champs
	// ------------------------------------------
	foreach ( dfsi_itcontainer_Get_Fields_Instances($typeName) as $instance ) 
	{
		$instance['entity_type'] = 'node';
		$instance['bundle'] = $typeName;
		field_create_instance( $instance );
	}
}

// -------------------------------------------------------
// liste des champs pour le content type : itcontainer
// -------------------------------------------------------
function dfsi_itcontainer_Get_Fields_Definitions( $typeName )	
{
    return	array
    (
        'field_contenu' => array(
            'translatable' => '0',
            'entity_types' => array(),
            'settings' => array(
              'referenceable_types' => array(
                'itcontainer' => 'itcontainer',
                'itservice' => 'itservice',
                'article' => 0,
                'page' => 0,
                'book' => 0,
              ),
              'view' => array(
                'view_name' => '',
                'display_name' => '',
                'args' => array(),
              ),
            ),
            'storage' => array(
              'type' => 'field_sql_storage',
              'settings' => array(),
              'module' => 'field_sql_storage',
              'active' => '1',
              'details' => array(
                'sql' => array(
                  'FIELD_LOAD_CURRENT' => array(
                    'field_data_field_contenu' => array(
                      'nid' => 'field_contenu_nid',
                    ),
                  ),
                  'FIELD_LOAD_REVISION' => array(
                    'field_contenu' => array(
                      'nid' => 'field_contenu_nid',
                    ),
                  ),
                ),
              ),
            ),
            'foreign keys' => array(
              'nid' => array(
                'table' => 'node',
                'columns' => array(
                  'nid' => 'nid',
                ),
              ),
            ),
            'indexes' => array(
              'nid' => array(
                'nid',
              ),
            ),
            'field_name' => 'field_contenu',
            'type' => 'node_reference',
            'module' => 'node_reference',
            'active' => '1',
            'locked' => '0',
            'cardinality' => '-1',
            'deleted' => '0',
            'columns' => array(
              'nid' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                'not null' => FALSE,
              ),
            ),
            'bundles' => array(
              'node' => array(
                'itcontainer',
              ),
            ),
          ),
    );
}

// ------------------------------------------------------------------
// creation des instances de champ pour le content type : itcontainer
// ------------------------------------------------------------------
function dfsi_itcontainer_Get_Fields_Instances( $typeName )	
{

    return array
    (
        'field_contenu' => array(
            'label' => 'Eléments contenus',
            'widget' => array(
              'weight' => '2',
              'type' => 'options_buttons',
              'module' => 'options',
              'active' => 1,
              'settings' => array(),
            ),
            'settings' => array(
              'user_register_form' => FALSE,
            ),
            'display' => array(
              'default' => array(
                'label' => 'above',
                'type' => 'node_reference_default',
                'settings' => array(),
                'module' => 'node_reference',
                'weight' => 11,
              ),
              'teaser' => array(
                'type' => 'hidden',
                'label' => 'above',
                'settings' => array(),
                'weight' => 0,
              ),
              'dfsi_node_list' => array(
                'type' => 'hidden',
                'label' => 'above',
                'settings' => array(),
                'weight' => 0,
              ),
            ),
            'required' => 0,
            'description' => '',
            'default_value' => NULL,
            'field_name' => 'field_contenu',
            'entity_type' => 'node',
            'bundle' => 'itcontainer',
            'deleted' => '0',
          ),      
    );
}

