<?php

// ------------------------------------------------
// @(#) dfsi.service.fields.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php


module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");

// -----------------------------------
// suppression des champs  : itservice
// -----------------------------------
function dfsi_Delete_itservice_Fields( $typeName )
{
	foreach( array_keys( dfsi_itservice_Get_Fields_Definitions($typeName) ) as $field ) 
	{
		field_delete_field( $field );
	}
}

//
// --------------------------------
// Creation des champs  : itservice
// --------------------------------
function dfsi_Create_itservice_Fields( $typeName )
{
    	// ----------------------------------------------
	// Creer  tous les champs pour le type de contenu
	// ----------------------------------------------
	foreach 
	(
		dfsi_itservice_Get_Fields_Definitions( $typeName ) as $field
	) 
	{
		field_create_field( $field );
	}        
}
//
//
// --------------------------------------------
// Creation des instances de champ  : itservice
// --------------------------------------------
function dfsi_Create_itservice_Fields_Instances( $typeName ) 
{ 
	foreach 
	(
		dfsi_itservice_Get_Fields_Instances( $typeName ) as $instance
	) 
	{
		$instance['entity_type'] = 'node';
		$instance['bundle'] = $typeName;
		field_create_instance( $instance );
	}
}

// -----------------------------------------------------
// liste des champs pour le content type : itservice
// -----------------------------------------------------
function dfsi_itservice_Get_Fields_Definitions( $typeName )	
{
	return	array
	(

		// --------------------------------
		// champ date de mise en production
		// --------------------------------
		'field_date_mep' => array
		(
			'translatable' => '0',
			'entity_types' => array(),
			'settings' => array
			(
				'granularity' => array
				(
					'month' => 'month',
					'day' => 'day',
					'year' => 'year',
					'hour' => 0,
					'minute' => 0,
					'second' => 0,
				),
				'tz_handling' => 'none',
				'timezone_db' => '',
				'cache_enabled' => 0,
				'cache_count' => '4',
				'todate' => '',
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
				 			'field_data_field_date_mep' => array
							(
								'value' => 'field_date_mep_value',
				 			),
						),
						'FIELD_LOAD_REVISION' => array
						(
				 			'field_date_mep' => array
							(
								'value' => 'field_date_mep_value',
							 ),
						),
					),
				),
			),
			'foreign keys' => array(),
			'indexes' => array(),
			'field_name' => 'field_date_mep',
			'type' => 'datetime',
			'module' => 'date',
			'active' => '1',
			'locked' => '0',
			'cardinality' => '1',
			'deleted' => '0',
			'columns' => array
			(
				'value' => array
				(
					'type' => 'datetime',
					'mysql_type' => 'datetime',
					'pgsql_type' => 'timestamp without time zone',
					'sqlite_type' => 'varchar',
					'sqlsrv_type' => 'smalldatetime',
					'not null' => FALSE,
					'sortable' => TRUE,
					'views' => TRUE,
				),
			),
			'bundle' => $typeName,
		),
		// -------------------------------
		// champ service final utilisateur
		// -------------------------------
		'field_service_final' => array
		(
			'translatable' => '0',
			'entity_types' => array(),
			'settings' => array
			(
				'allowed_values' => array
				(
					'Non',
					'Oui',
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
							'field_data_field_service_final' => array
							(
								'value' => 'field_service_final_value',
							),
						),
						'FIELD_LOAD_REVISION' => array
						(
							'field_service_final' => array
							(
								'value' => 'field_service_final_value',
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
			'field_name' => 'field_service_final',
			'type' => 'list_boolean',
			'module' => 'list',
			'active' => '1',
			'locked' => '0',
			'cardinality' => '1',
			'deleted' => '0',
			'columns' => array
			(
				'value' => array
				(
					'type' => 'int',
					'not null' => FALSE,
				),
			),
			'bundle' => $typeName,
		),
		// ---------------------------
		// champ constructeur/editeur
		// ---------------------------
	 	'field_constructeur' => array
		(
			'translatable' => '0',
			'entity_types' => array(),
			'settings' => array
			(
				'allowed_values' => array
				(
					array
					(
						'vocabulary' => 'itcomputercompany',
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
							'field_data_field_constructeur' => array
							(
								'tid' => 'field_constructeur_tid',
							),
						),
						'FIELD_LOAD_REVISION' => array
						(
							'field_constructeur' => array
							(
								'tid' => 'field_constructeur_tid',
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
			'field_name' => 'field_constructeur',
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
	);
}

// ------------------------------------------------------------------
// listes des instances de champ : itservice
// ------------------------------------------------------------------
function dfsi_itservice_Get_Fields_Instances($typeName)	
{
	return	array
	(
		// --------------------------------
		// champ date de mise en production
		// --------------------------------
		'field_date_mep' => array
		(
			'label' => 'Date de mise en production',
			'widget' => array
			(
				'weight' => '5',
				'type' => 'date_popup',
				'module' => 'date',
				'active' => 1,
				'settings' => array
				(
					'input_format' => 'm/d/Y - H:i:s',
					'input_format_custom' => '',
					'year_range' => '-3:+3',
					'increment' => '15',
					'label_position' => 'above',
					'text_parts' => array(),
				),
			),
			'settings' => array
			(
				'default_value' => 'now',
				'default_value_code' => '',
				'default_value2' => 'same',
				'default_value_code2' => '',
				'user_register_form' => FALSE,
			),
			'display' => array
			(
				'default' => array
				(
					'label' => 'inline',
					'type' => 'date_default',
					'settings' => array
					(
						'format_type' => 'long',
						'multiple_number' => '',
						'multiple_from' => '',
						'multiple_to' => '',
						'fromto' => 'both',
					),
					'module' => 'date',
					'weight' => 4,
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
			'description' => '',
			'field_name' => 'field_date_mep',
			'entity_type' => 'node',
			'bundle' => $typeName,
			'deleted' => '0',
		),
		// -------------------------------
		// champ service final utilisateur
		// -------------------------------
		'field_service_final' => array
		(
			'label' => 'Service final',
			'widget' => array
			(
				'weight' => '2',
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
					'type' => 'list_default',
					'settings' => array(),
					'module' => 'list',
					'weight' => 5,
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
			'description' => "Indique si le service est destiné aux utilisateurs, ou s\'il s'agit d\'un service intermédiaire",
			'default_value' => array
			(
				array
				(
					'value' => '0',
				),
			),
			'field_name' => 'field_service_final',
			'entity_type' => 'node',
			'bundle' => $typeName,
			'deleted' => '0',
		),
		// ---------------------------
		// champ constructeur/editeur
		// ---------------------------
		'field_constructeur' => array
		(
			'label' => 'Constructeur',
			'widget' => array
			(
				'weight' => '32',
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
					'type' => 'taxonomy_term_reference_link',
					'settings' => array(),
					'module' => 'taxonomy',
					'weight' => 6,
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
			'description' => '',
			'default_value' => NULL,
			'field_name' => 'field_constructeur',
			'entity_type' => 'node',
			'bundle' => $typeName,
			'deleted' => '0',
		),
	);
}
