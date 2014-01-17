<?php

// ------------------------------------------------
// @(#) dfsi.define.inc
// @(#) 11/12/2013
// ------------------------------------------------

if( !defined("_MODULE_NAME") ) { define( "_MODULE_NAME" , "dfsi" ); }                                               // nom du module

if( !defined("_SERVICE") ) { define( "_SERVICE" , "itservice" ); }                                                  // nom de type d'element service
if( !defined("_CONTAINER") ) { define( "_CONTAINER" , "itcontainer" ); }                                            // nom de type d'element container

if( !defined("_INCLUDE_EXTENSION") ) { define( "_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

if( !defined("_DFSI_STATE_TABLE") ) { define( "_DFSI_STATE_TABLE" , "dfsi_states" ); }                              // en prevision V2 etats multiples
if( !defined("_DFSI_STATE_GROUP_TABLE") ) { define( "_DFSI_STATE_GROUP_TABLE" , "dfsi_states_groups" ); }           // en prevision V2 etats multiples
if( !defined("_DFSI_DETAIL_TABLE") ) { define( "_DFSI_DETAIL_TABLE" , "dfsi_detail_rshp" ); }                       // table des associations terme/type de contenu detail
if( !defined("_DFSI_NODE_DETAIL_TABLE") ) { define( "_DFSI_NODE_DETAIL_TABLE" , "dfsi_node_detail_rshp" ); }        // table des associations node/terme/node_detail
//

if( !defined("_DFSI_OK_STATE") ) { define( "_DFSI_OK_STATE" , "Ok" ); }                                             // textes du statut d'un élément
if( !defined("_DFSI_WARNING_STATE") ) { define( "_DFSI_WARNING_STATE" , "Warning" ); }
if( !defined("_DFSI_ERROR_STATE") ) { define( "_DFSI_ERROR_STATE" , "Error" ); }

if( !defined("_DFSI_ERROR_MSG") ) { define( "_DFSI_ERROR_MSG" , "dfs_error_msg" ); }                                // texte message erreur dfs
if( !defined("_DFSI__NODIRECTDEP_SEARCH") ) { define( "_DFSI__NODIRECTDEP_SEARCH" , 0 ); }                          // recherche noeud sans dependances directes
if( !defined("_DFSI__NOGROUPDEP_SEARCH") ) { define( "_DFSI__NOGROUPDEP_SEARCH" , 1 ); }                            // recherche noeud sans dependances de groupe
if( !defined("_DFSI__NODEP_SEARCH") ) { define( "_DFSI__NODEP_SEARCH" , 2 ); }                                      // recherche noeud sans aucune dependances 
if( !defined("_DFSI_TITLE") ) { define( "_DFSI_TITLE" , "title" ); }                                                // nom du champ title
if( !defined("_DFSI_NID") ) { define( "_DFSI_NID" , "nid" ); }                                                      // nom du champ node id (nid)
if( !defined("_DFSI_UNKNOWN_SERVICE_TYPE") ) { define( "_DFSI_UNKNOWN_SERVICE_TYPE" , "** Indéterminé **" ); }      // type de service inconnu

if( !defined("_DFSI_TRUNCATE_COMMENT_LENGTH") ) { define( "_DFSI_TRUNCATE_COMMENT_LENGTH" , 80 ); }                 // longueur maxi description type de contenu avant troncatue
