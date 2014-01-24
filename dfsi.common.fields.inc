<?php

// ------------------------------------------------
// @(#) dfsi.common.fields.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");


function dfsi_Create_Common_Fields( $typeName )
{
    	// -----------------------
	// Creer  tous les champs 
	// -----------------------
	foreach 
	(
		dfsi_Get_Common_Fields_Definitions( $typeName ) as $field
	) 
	{
		field_create_field( $field );
	}        
}
// --------------------------------------------
// Creation des instances de champ  : itservice
// --------------------------------------------
function dfsi_Create_Common_Fields_Instances( $typeName ) 
{ 
	foreach 
	(
		dfsi_Get_Common_Field_Instances( $typeName ) as $instance
	) 
	{
		$instance['entity_type'] = 'node';
		$instance['bundle'] = $typeName;
		field_create_instance( $instance );
	}
}


// ------------------------------
// creation des champs mutualisés
// ------------------------------
function dfsi_Get_Common_Fields_Definitions( $typeName )	
{
	$t = get_t();
	return	array
	(
		// ---------------------------
		// champ type de service
		// ---------------------------
		'field_service_type' => array
		(
			'translatable' => '0',
			'entity_types' => array(),
			'settings' => array
			(
				'allowed_values' => array
				(
					array
					(
						'vocabulary' => 'itservicetypes',
						'parent' => '0',
					),
				),
			),
			'storage' => array
			(
				'type' => 'field_sql_storage',
				'settings' => array(),
				'module' => 'field_sql_storage',
				'active' => '1',
				'details' => array		
				(
					'sql' => array
					(
						'FIELD_LOAD_CURRENT' => array
						(
							'field_data_field_service_type' => array			
							(
								'tid' => 'field_service_type_tid',
							),
						),
						'FIELD_LOAD_REVISION' => array
						(
							'field_service_type' => array
							(
								'tid' => 'field_service_type_tid',
							),
						),
					),
				),
			),
			'foreign keys' => array
			(
				'tid' => array
				(
					'table' => 'taxonomy_term_data',
					'columns' => array
					(
						'tid' => 'tid',
					),
				),
			),
			'indexes' => array
			(
				'tid' => array
				(
					'tid',
				),
			),
			'field_name' => 'field_service_type',
			'type' => 'taxonomy_term_reference',
			'module' => 'taxonomy',
			'active' => '1',
			'locked' => '0',
			'cardinality' => '1',
			'deleted' => '0',
			'columns' => array
			(
				'tid' => array
				(
					'type' => 'int',
					'unsigned' => TRUE,
					'not null' => FALSE,
				),
		 	),
			'bundle' => $typeName,
		),            
		// ---------------------------
		// champ statut
		// ---------------------------
		'field_statut' => array
		(
			'translatable' => '0',
			'entity_types' => array(),
			'settings' => array
			(
				'allowed_values' => array
				(
					'Ok' => 'Ok',
					'Warning' => 'Warning',
					'Error' => 'Error',
				),
				'allowed_values_function' => '',
			),
			'storage' => array
			(
				'type' => 'field_sql_storage',
				'settings' => array(),
				'module' => 'field_sql_storage',
				'active' => '1',
				'details' => array
				(
					'sql' => array
					(
						'FIELD_LOAD_CURRENT' => array
						(
							'field_data_field_statut' => array
							(
								'value' => 'field_statut_value',
							),
						),
						'FIELD_LOAD_REVISION' => array
						(
						 	'field_statut' => array
							(
								'value' => 'field_statut_value',
						 	),
						),
					),
				),
			),
			'foreign keys' => array(),
			'indexes' => array
			(
				'value' => array
				(
					'value',
				),
			),
			'field_name' => 'field_statut',
			'type' => 'list_text',
			'module' => 'list',
			'active' => '1',
			'locked' => '0',
			'cardinality' => '1',
			'deleted' => '0',
			'columns' => array
			(
				'value' => array
				(
					'type' => 'varchar',
					'length' => 10,
					'not null' => FALSE,
				),
			),
			'bundle' => $typeName,
		),
		// ---------------------------
		// champ Criticité
		// ---------------------------
		'field_criticite' => array
		(
			'translatable' => '0',
			'entity_types' => array(),
			'settings' => array
			(
				'allowed_values' => array
				(
					'Indispensable' => 'Indispensable',
					'Critique' => 'Critique',
					'Important' => 'Important',
					'Peu important' => 'Peu important',
				),
				'allowed_values_function' => '',
			),
			'storage' => array
			(
				'type' => 'field_sql_storage',
				'settings' => array(),
				'module' => 'field_sql_storage',
				'active' => '1',
				'details' => array
				(
					'sql' => array
					(
						'FIELD_LOAD_CURRENT' => array
						(
							'field_data_field_criticite' => array
							(
								'value' => 'field_criticite_value',
							),
						),
						'FIELD_LOAD_REVISION' => array
						(
						 	'field_criticite' => array
							(
								'value' => 'field_criticite_value',
						 	),
						),
					),
				),
			),
			'foreign keys' => array(),
			'indexes' => array
			(
				'value' => array
				(
					'value',
				),
			),
			'field_name' => 'field_criticite',
			'type' => 'list_text',
			'module' => 'list',
			'active' => '1',
			'locked' => '0',
			'cardinality' => '1',
			'deleted' => '0',
			'columns' => array
			(
				'value' => array
				(
					'type' => 'varchar',
					'length' => 255,
					'not null' => FALSE,
				),
			),
			'bundle' => $typeName,
		),            
		// ---------------------------
		// Champ groupe de dépendances
		// ---------------------------
		'field_groupe_de_dependances' => array
		(
			'translatable' => '0',
			'entity_types' => array(),
			'settings' => array
			(
				'referenceable_types' => array
				(
					'itcontainer' => 'itcontainer',
					'itservice' => 'itservice',
					'article' => 0,
					'page' => 0,
					'book' => 0,
				),
				'view' => array
				(
					'view_name' => '',
					'display_name' => '',
					'args' => array(),
				),
			),
			'storage' => array
			(
				'type' => 'field_sql_storage',
				'settings' => array(),
				'module' => 'field_sql_storage',
				'active' => '1',
				'details' => array
				(
					'sql' => array
					(
						'FIELD_LOAD_CURRENT' => array
						(
							'field_data_field_groupe_de_dependances' => array
							(
								'nid' => 'field_groupe_de_dependances_nid',
							),
						),
						'FIELD_LOAD_REVISION' => array
						(
							'field_groupe_de_dependances' => array
							(
								'nid' => 'field_groupe_de_dependances_nid',
							),
						),
					),
				),
			),
			'foreign keys' => array
			(
				'nid' => array
				(
					'table' => 'node',
					'columns' => array
					(
						'nid' => 'nid',
					),
				),
			),
			'indexes' => array
			(
				'nid' => array
				(
					'nid',
			 	),
			),
			'field_name' => 'field_groupe_de_dependances',
			'type' => 'node_reference',
			'module' => 'node_reference',
			'active' => '1',
			'locked' => '0',
			'cardinality' => '-1',
			'deleted' => '0',
			'columns' => array
			(
				'nid' => array
				(
					'type' => 'int',
					'unsigned' => TRUE,
					'not null' => FALSE,
				),
			),
                    'bundle' => $typeName,
		),
		// ---------------------------
		// champ dependances
		// ---------------------------
		'field_dependances' => array
		(
			'translatable' => '0',
			'entity_types' => array(),
			'settings' => array
			(
				'referenceable_types' => array
				(
					'itcontainer' => 'itcontainer',
					'itservice' => 'itservice',
					'article' => 0,
					'page' => 0,
					'book' => 0,
				),
				'view' => array
				(
					'view_name' => '',
					'display_name' => '',
					'args' => array(),
				),
			),
			'storage' => array
			(
				'type' => 'field_sql_storage',
				'settings' => array(),
				'module' => 'field_sql_storage',
				'active' => '1',
				'details' => array
				(
					'sql' => array
					(
						'FIELD_LOAD_CURRENT' => array
						(
							'field_data_field_dependances' => array
							(
								'nid' => 'field_dependances_nid',
							),
						),
						'FIELD_LOAD_REVISION' => array
						(
							'field_dependances' => array
							(
								'nid' => 'field_dependances_nid',
							),
						),
					),
				),
			),
			'foreign keys' => array
			(
				'nid' => array
				(
					'table' => 'node',
					'columns' => array
					(
						'nid' => 'nid',
					),
				),
			),
			'indexes' => array
			(
				'nid' => array
				(
					'nid',
				),
			),
			'field_name' => 'field_dependances',
			'type' => 'node_reference',
			'module' => 'node_reference',
			'active' => '1',
			'locked' => '0',
			'cardinality' => '-1',
			'deleted' => '0',
			'columns' => array
			(
				'nid' => array
				(
					'type' => 'int',
					'unsigned' => TRUE,
					'not null' => FALSE,
				),
			),
                    'bundle' => $typeName,
		),
		// ---------------------------
		// champ ancestor
		// ---------------------------
		'field_ancestor' => array
		(
			'translatable' => '0',
			'entity_types' => array(),
			'settings' => array
			(
				'referenceable_types' => array
				(
					'itcontainer' => 'itcontainer',
					'itservice' => 'itservice',
					'article' => 0,
					'page' => 0,
					'book' => 0,
				),
				'view' => array
				(
					'view_name' => '',
					'display_name' => '',
					'args' => array(),
				),
			),
			'storage' => array
			(
				'type' => 'field_sql_storage',
				'settings' => array(),
				'module' => 'field_sql_storage',
				'active' => '1',
				'details' => array
				(
					'sql' => array
					(
						'FIELD_LOAD_CURRENT' => array
						(
							'field_data_field_ancestor' => array
							(
								'nid' => 'field_ancestor_nid',
							),
						),
						'FIELD_LOAD_REVISION' => array
						(
							'field_ancestor' => array
							(
								'nid' => 'field_ancestor_nid',
							),
						),
					),
				),
			),
			'foreign keys' => array
			(
				'nid' => array
				(
					'table' => 'node',
					'columns' => array
					(
						'nid' => 'nid',
					),
				),
			),
			'indexes' => array
			(
				'nid' => array
				(
					'nid',
				),
			),
			'field_name' => 'field_ancestor',
			'type' => 'node_reference',
			'module' => 'node_reference',
			'active' => '1',
			'locked' => '0',
			'cardinality' => '-1',
			'deleted' => '0',
			'columns' => array
			(
				'nid' => array
				(
					'type' => 'int',
					'unsigned' => TRUE,
					'not null' => FALSE,
				),
			),
                    'bundle' => $typeName,
		),
	);
}

// ------------------------------------------------------------
// liste des instances de champ commune pour le content type
// ------------------------------------------------------------
function dfsi_Get_Common_Field_Instances($typeName)	
{
	return	array
	(
		// ---------------------------
		// champ type de service
		// ---------------------------
		'field_service_type' => array
		(
			'label' => 'Type de service',
			'widget' => array
			(
				'weight' => '-3',
				'type' => 'options_select',
				'module' => 'options',
				'active' => 0,
				'settings' => array
				(
					'size' => 60,
					'autocomplete_path' => 'taxonomy/autocomplete',
				),
			),
			'settings' => array
			(
				'user_register_form' => FALSE,
			),
			'display' => array
			(
				'default' => array
				(
					'label' => 'inline',
					'type' => 'taxonomy_term_reference_link',
					'settings' => array(),
					'module' => 'taxonomy',
					'weight' => 1,
				),
				'teaser' => array
				(
					'type' => 'hidden',
					'label' => 'inline',
					'settings' => array(),
					'weight' => 0,
				),
			),
			'required' => 1,
			'description' => '',
			'default_value' => NULL,
			'field_name' => 'field_service_type',
			'entity_type' => 'node',
			'bundle' => $typeName,
			'deleted' => '0',
		),            
		// ---------------------------
		// champ statut
		// ---------------------------
		'field_statut' => array
		(
			'label' => 'statut',
			'widget' => array
			(
				'weight' => '0',
				'type' => 'options_select',
				'module' => 'options',
				'active' => 1,
				'settings' => array(),
			),
			'settings' => array
			(
				'user_register_form' => FALSE,
			),
			'display' => array
			(
				'default' => array
				(
					'label' => 'inline',
					'type' => 'list_default',
					'settings' => array(),
					'module' => 'list',
					'weight' => 3,
				),
				'teaser' => array
				(
					'type' => 'hidden',
					'label' => 'inline',
					'settings' => array(),
					'weight' => 0,
				),
			),
			'required' => 1,
			'description' => "Ok si actif, Errror si inactif, Warning si un élément du groupe de dépendances est inactif",
			'default_value' => array
			(
				array
				(
					'value' => 'Ok',
				),
			 ),
			 'field_name' => 'field_statut',
			 'entity_type' => 'node',
			 'bundle' => $typeName,
			 'deleted' => '0',
		),
		// ---------------------------
		// champ Criticité
		// ---------------------------
		'field_criticite' => array
		(
			'label' => 'Criticité',
			'widget' => array
			(
				'weight' => '1',
				'type' => 'options_select',
				'module' => 'options',
				'active' => 1,
				'settings' => array(),
			),
			'settings' => array
			(
				'user_register_form' => FALSE,
			),
			'display' => array
			(
				'default' => array
				(
					'label' => 'inline',
					'type' => 'list_default',
					'settings' => array(),
					'module' => 'list',
					'weight' => 3,
				),
				'teaser' => array
				(
					'type' => 'hidden',
					'label' => 'inline',
					'settings' => array(),
					'weight' => 0,
				),
			),
			'required' => 1,
			'description' => "Indique l\'importance relative du service , en cas de non fonctionnement",
			'default_value' => array
			(
				array
				(
					'value' => 'Important',
				),
			 ),
			 'field_name' => 'field_criticite',
			 'entity_type' => 'node',
			 'bundle' => $typeName,
			 'deleted' => '0',
		),            
		// ---------------------------
		// champ dependances
		// ---------------------------
		'field_dependances' => array
		(
			'label' => 'Eléments qui dépendent de ce service',
			'widget' => array
			(
				'weight' => '3',
				'type' => 'options_buttons',
				'module' => 'options',
				'active' => 1,
				'settings' => array(),
			),
			'settings' => array	
			(
				'user_register_form' => FALSE,
			),
			'display' => array			
			(
				'default' => array
				(
					'label' => 'above',
					'type' => 'node_reference_default',
					'weight' => '7',
					'settings' => array(),
					'module' => 'node_reference',
				),
				'teaser' => array
				(
					'type' => 'hidden',
					'label' => 'inline',
					'settings' => array(),
					'weight' => 0,
				),
			),
			'required' => 0,
			'description' => 'Liste des services ou containers qui dépendent de ce service',
			'default_value' => NULL,
			'field_name' => 'field_dependances',
			'entity_type' => 'node',
			'bundles' => $typeName,
			'deleted' => '0',
		 ),
		// ---------------------------
		// champ groupe de dependances
		// ---------------------------
		'field_groupe_de_dependances' => array
		(
			'label' => 'Groupe de dépendances',
			'widget' => array
			(
				'weight' => '4',
				'type' => 'options_buttons',
				'module' => 'options',
				'active' => 1,
				'settings' => array(),
			),
			'settings' => array
			(
				'user_register_form' => FALSE,
			),
			'display' => array
			(
				'default' => array
				(
					'label' => 'inline',
					'type' => 'node_reference_default',
					'settings' => array(),
					'module' => 'node_reference',
					'weight' => 8,
				),
				'teaser' => array
				(
					'type' => 'hidden',
					'label' => 'inline',
					'settings' => array(),
					'weight' => 0,
				),
			),
			'required' => 0,
			'description' => "Groupe dont dépends ce container.\r\nUn au moins des éléments du groupe doit être actif, pour que le service soit actif. (dépendance agrégative)",
			'default_value' => NULL,
			'field_name' => 'field_groupe_de_dependances',
			'entity_type' => 'node',
			'bundles' => $typeName,
			'deleted' => '0',
		),
		// ---------------------------
		// champ ancestor
		// ---------------------------
		'field_ancestor' => array		
		(
			'label' => 'Elements dont ce service est dépendant',
			'widget' => array
			(
				'weight' => '7',
				'type' => 'options_buttons',
				'module' => 'options',
				'active' => 1,
				'settings' => array(),
			),
			'settings' => array
			(
				'user_register_form' => FALSE,
			),
			'display' => array
			(
				'default' => array
				(
					'label' => 'above',
					'type' => 'node_reference_default',
					'weight' => '10',
					'settings' => array(),
					'module' => 'node_reference',
				),
				'teaser' => array
				(
					'type' => 'hidden',
					'label' => 'above',
					'settings' => array(),
					'weight' => 0,
		  		),
			),
			'required' => 0,
			'description' => "Il s'agit de la liste des services ou container dont dépends cet élément. Pour que le service/container soit actif, il faut que TOUS les élements de cette liste soient actifs.\r\nElle est alimentée automatiquement lorsque le champ dépendance d'un service/container est affecté d'un élément.",
			'default_value' => NULL,
			'field_name' => 'field_ancestor',
			'entity_type' => 'node',
			'bundles' => $typeName,
			'deleted' => '0',
		  ),
	);
}
