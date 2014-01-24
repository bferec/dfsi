<?php

// ------------------------------------------------
// @(#) dfsi.permissions.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php

module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");

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
        //  configuration permission
        _DFSI_PERM_CONFIG => array
        (
        'title' => t("Configurer le module ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission de modifier les paramètres de l'application"),
        ),
        // node statut permission
        _DFSI_PERM_CHANGE_NODE_STATUT => array
        (
        'title' => t("Modifier le statut d'un noeud  ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission de modifier l'état d'un noeud, actif ou inactif")
        ),
        _DFSI_PERM_CHUK_DATA_ACTIVATION  => array
        (
        'title' => t("Activer ou désactiver la gestion des données complémentaires ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission d'activer/désactiver la prise en charge des données complémentaires")
        ),        
        // ------------------------------
        // permissions liees aux services
        // ------------------------------
        "view service dfsi" => array
        (
        'title' => t("Consulter un service ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission de consulter un service")
        ),
        "create service dfsi" => array
        (
        'title' => t("Ajouter un service ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission d'ajouter un service")
        ),
        "update service dfsi" => array
        (
        'title' => t("Modifier un service ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission de modifier un service")
        ),    
        "delete service dfsi" => array
        (
        'title' => t("supprimer un service ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission de supprimer un service")
        ),        
        // --------------------------------
        // permissions liees aux containers
        // --------------------------------
        "view container dfsi" => array
        (
        'title' => t("Consulter un container ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission de consulter un container")
        ),        
        "create container dfsi" => array
        (
        'title' => t("creer un container ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission d'ajouter un container")
        ),
        "update container dfsi" => array
        (
        'title' => t("modifier un container ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission de modifier un container")
        ),
        "delete container dfsi" => array
        (
        'title' => t("supprimer un container ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission de supprimer un container")
        ),
        // permissions liees aux type de contenu detail
        "Add detail content type to dfsi term" => array
        (
        'title' => t("Ajouter un type de contenu de détail spécifique à un terme de taxonomie ")._MODULE_COMMERCIAL_NAME,
        'description' => t("permission de créer un content type de détail")
        ),
    );
}
