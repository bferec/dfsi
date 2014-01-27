<?php

// ------------------------------------------------
// @(#) dfsi.define.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_MODULE_COMMERCIAL_NAME") ) { define( "_MODULE_COMMERCIAL_NAME" , "D.f.s.i." ); }                         // nom commercial du module

if( !defined("_DFSI_SERVICE") ) { define( "_DFSI_SERVICE" , "itservice" ); }                                                  // nom de type d'element service
if( !defined("_DFSI_CONTAINER") ) { define( "_DFSI_CONTAINER" , "itcontainer" ); }                                            // nom de type d'element container

if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

if( !defined("_DFSI_STATE_TABLE") ) { define( "_DFSI_STATE_TABLE" , "dfsi_states" ); }                              // en prevision V2 etats multiples
if( !defined("_DFSI_STATE_GROUP_TABLE") ) { define( "_DFSI_STATE_GROUP_TABLE" , "dfsi_states_groups" ); }           // en prevision V2 etats multiples
if( !defined("_DFSI_DETAIL_TABLE") ) { define( "_DFSI_DETAIL_TABLE" , "dfsi_detail_rshp" ); }                       // table des associations terme/type de contenu detail
if( !defined("_DFSI_NODE_DETAIL_TABLE") ) { define( "_DFSI_NODE_DETAIL_TABLE" , "dfsi_node_detail_rshp" ); }        // table des associations node/terme/node_detail
//

if( !defined("_DFSI_OK_STATE") ) { define( "_DFSI_OK_STATE" , "Ok" ); }                                             // textes du statut d'un élément
if( !defined("_DFSI_WARNING_STATE") ) { define( "_DFSI_WARNING_STATE" , "Warning" ); }
if( !defined("_DFSI_ERROR_STATE") ) { define( "_DFSI_ERROR_STATE" , "Error" ); }

if( !defined("_DFSI_ERROR_MSG") ) { define( "_DFSI_ERROR_MSG" , "dfs_error_msg" ); }                                // texte message erreur dfs
if( !defined("_DFSI_NODIRECTDEP_SEARCH") ) { define( "_DFSI_NODIRECTDEP_SEARCH" , 0 ); }                          // recherche noeud sans dependances directes
if( !defined("_DFSI_NOGROUPDEP_SEARCH") ) { define( "_DFSI_NOGROUPDEP_SEARCH" , 1 ); }                            // recherche noeud sans dependances de groupe
if( !defined("_DFSI__NODEP_SEARCH") ) { define( "_DFSI__NODEP_SEARCH" , 2 ); }                                      // recherche noeud sans aucune dependances 
if( !defined("_DFSI_TITLE") ) { define( "_DFSI_TITLE" , "title" ); }                                                // nom du champ title
if( !defined("_DFSI_NID") ) { define( "_DFSI_NID" , "nid" ); }                                                      // nom du champ node id (nid)
if( !defined("_DFSI_UNKNOWN_SERVICE_TYPE") ) { define( "_DFSI_UNKNOWN_SERVICE_TYPE" , "** Indéterminé **" ); }      // type de service inconnu
if( !defined("_DFSI_TRUNCATE_COMMENT_LENGTH") ) { define( "_DFSI_TRUNCATE_COMMENT_LENGTH" , 80 ); }                 // longueur maxi description type de contenu avant troncatue
// donneed de detail
if( !defined("_DFSI_CHUNK_DATA_ENABLED") ) { define( "_DFSI_CHUNK_DATA_ENABLED" , 1 ); }                            // donnees complementaires activees
if( !defined("_DFSI_CHUNK_DATA_DISABLED") ) { define( "_DFSI_CHUNK_DATA_DISABLED" , 2 ); }                          // donnees complementaires activees

// données de test
if( !defined("_DFSI_GENERATE_TESTDATA") ) { define( "_DFSI_GENERATE_TESTDATA" , 1 ); }                              // generer donnees de test
if( !defined("_DFSI_DELETE_TESTDATA") ) { define( "_DFSI_DELETE_TESTDATA" , 1 ); }                                  // generer donnees de test
if( !defined("_DFSI_GENERATE_DO_NOTHING") ) { define( "_DFSI_GENERATE_DO_NOTHING" , 2 ); }                          // ne pas generer

// permissions
if( !defined("_DFSI_PERM_CONFIG") ) { define( "_DFSI_PERM_CONFIG" , "dfsi: admin config" ); }                          // dfsi config permission
if( !defined("_DFSI_PERM_CHANGE_NODE_STATUT") ) { define( "_DFSI_PERM_CHANGE_NODE_STATUT" , "dfsi: change node statut" ); }                          // dfsi change node statut permission

if( !defined("_DFSI_PERM_CHUK_DATA_ACTIVATION") ) { define( "_DFSI_PERM_CHUK_DATA_ACTIVATION" , "dfsi: chunk data activation" ); }

if( !defined("_DFSI_PERM_VIEW_DFSI_SERVICE") ) { define( "_DFSI_PERM_VIEW_DFSI_SERVICE" , "dfsi: view service" ); }
if( !defined("_DFSI_PERM_CREATE_DFSI_SERVICE") ) { define( "_DFSI_PERM_CREATE_DFSI_SERVICE" , "dfsi: create service" ); }
if( !defined("_DFSI_PERM_UPDATE_DFSI_SERVICE") ) { define( "_DFSI_PERM_UPDATE_DFSI_SERVICE" , "dfsi: update service" ); }
if( !defined("_DFSI_PERM_DELETE_DFSI_SERVICE") ) { define( "_DFSI_PERM_DELETE_DFSI_SERVICE" , "dfsi: delete service" ); }

if( !defined("_DFSI_PERM_VIEW_DFSI_CONTAINER") ) { define( "_DFSI_PERM_VIEW_DFSI_CONTAINER" , "dfsi: view container" ); }
if( !defined("_DFSI_PERM_CREATE_DFSI_CONTAINER") ) { define( "_DFSI_PERM_CREATE_DFSI_CONTAINER" , "dfsi: create container" ); }
if( !defined("_DFSI_PERM_UPDATE_DFSI_CONTAINER") ) { define( "_DFSI_PERM_UPDATE_DFSI_CONTAINER" , "dfsi: update container" ); }
if( !defined("_DFSI_PERM_DELETE_DFSI_CONTAINER") ) { define( "_DFSI_PERM_DELETE_DFSI_CONTAINER" , "dfsi: delete container" ); }

if( !defined("_DFSI_PERM_ADD_CHUNK_DATA_TYPE") ) { define( "_DFSI_PERM_ADD_CHUNK_DATA_TYPE" , "dfsi: add chunk data type" ); }
if( !defined("_DFSI_PERM_UPDATE_CHUNK_DATA_TYPE") ) { define( "_DFSI_PERM_UPDATE_CHUNK_DATA_TYPE" , "dfsi: update chunk data type" ); }
if( !defined("_DFSI_PERM_DELETE_CHUNK_DATA_TYPE") ) { define( "_DFSI_PERM_DELETE_CHUNK_DATA_TYPE" , "dfsi: delete chunk data type" ); }

if( !defined("_DFSI_PERM_ADD_CHUNK_DATA") ) { define( "_DFSI_PERM_ADD_CHUNK_DATA" , "dfsi: add chunk data" ); }
if( !defined("_DFSI_PERM_UPDATE_CHUNK_DATA") ) { define( "_DFSI_PERM_UPDATE_CHUNK_DATA" , "dfsi: update chunk data" ); }
if( !defined("_DFSI_PERM_DELETE_CHUNK_DATA") ) { define( "_DFSI_PERM_DELETE_CHUNK_DATA" , "dfsi: delete chunk data" ); }


