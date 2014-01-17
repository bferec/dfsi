<?php

// ------------------------------------------------
// @(#) dfsi.permissions.inc
// @(#) 11/12/2013
// ------------------------------------------------

if( !defined("_MODULE_NAME") ) { define( "_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_INCLUDE_EXTENSION") ) { define( "_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

module_load_include( _INCLUDE_EXTENSION , _MODULE_NAME , "dfsi.define");

// --------------------------------
// Hook_permission
// --------------------------------
// créee les permissions 
// specifiques au module DFSI
// --------------------------------
function dfsi_permission() 
{
    return array
    (
        'administer module dfsi' => array
        (
        'title' => t('Configurer le module DFSI'),
        'description' => t("permission de modifier les paramètres de l'application")
        ),
        "change node statut dfsi" => array
        (
        'title' => t("Modifier le statut d'un noeud  DFSI"),
        'description' => t("permission de modifier l'état d'un noeud, actif ou inactif")
        ),
        // ------------------------------
        // permissions liees aux services
        // ------------------------------
        "view service dfsi" => array
        (
        'title' => t("Consulter un service DFSI"),
        'description' => t("permission de consulter un service DFSI")
        ),
        "create service dfsi" => array
        (
        'title' => t("Ajouter un service DFSI"),
        'description' => t("permission d'ajouter un service DFSI")
        ),
        "update service dfsi" => array
        (
        'title' => t("Modifier un service DFSI"),
        'description' => t("permission de modifier un service DFSI")
        ),    
        "delete service dfsi" => array
        (
        'title' => t("supprimer un service DFSI"),
        'description' => t("permission de supprimer un service DFSI")
        ),        
        // --------------------------------
        // permissions liees aux containers
        // --------------------------------
        "view container dfsi" => array
        (
        'title' => t("Consulter un container DFSI"),
        'description' => t("permission de consulter un container DFSI")
        ),        
        "create container dfsi" => array
        (
        'title' => t("creer un container DFSI"),
        'description' => t("permission d'ajouter un container DFSI")
        ),
        "update container dfsi" => array
        (
        'title' => t("modifier un container DFSI"),
        'description' => t("permission de modifier un container DFSI")
        ),
        "delete container dfsi" => array
        (
        'title' => t("supprimer un container DFSI"),
        'description' => t("permission de supprimer un container DFSI")
        ),
        "Add detail content type to term" => array
        (
        'title' => t("Ajouter un type de contenu de détail spéfifique à un terme de taxonomie"),
        'description' => t("permission de créer un content type de détail DFSI")
        ),
    );
}
